#include "plot.h"
#include "mainwindow.h"

#if QT_VERSION < 0x040601
#define qExp(x) ::exp(x)
#define qAtan2(y, x) ::atan2(y, x)
#endif

double max = 0;
QString fileName;

const int ArraySize = 16385;
extern int *shared_memory_cmd, *shared_memory;
double channels[ArraySize] = { 0 }, counts[ArraySize] = { 0 };

extern bool autoscaleOn, liveOn, logchecked;

bool EnergyOn = false;
double cal_gradient = 1, cal_offset = 0;
double channel1_cal, channel2_cal, energy1_cal, energy2_cal;
double channel_upperbound = static_cast<double>(ArraySize), channel_lowerbound = 0;

Plot::Plot( QWidget *parent ): QwtPlot( parent ) {


  QwtPlotCanvas *canvas = new QwtPlotCanvas();
  canvas->setBorderRadius( 0 );

  setCanvas( canvas );
  setCanvasBackground( QColor( "White" ) );


  // Grid
  // Instantiating the class QwtPlotGrid through a pointer
  QwtPlotGrid *grid = new QwtPlotGrid;
  grid->enableXMin( true );
  grid->setMajorPen( Qt::black, 0, Qt::DashDotLine );
  grid->setMinorPen( Qt::gray, 0 , Qt::DashDotLine );
  grid->attach( this );

  // Axes
  setAxisTitle( QwtPlot::xBottom, "Instrumental Channel" );
  setAxisTitle( QwtPlot::yLeft, "Event Counts" );
  setAxisScaleEngine( QwtPlot::xBottom, new QwtLinearScaleEngine );
  setAxisScale(QwtPlot::yLeft, 0, max);
  setAxisScale(QwtPlot::xBottom, 0, ArraySize);

  // Plot
  // Instantiating the class QwtPlotCurve through a pointer
  d_curve1 = new QwtPlotCurve( "AdcCounts" );

  d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased );
  d_curve1->setPen( Qt::black );
  d_curve1->setYAxis( QwtPlot::yLeft );
  d_curve1->attach( this );
  d_curve1->setStyle(d_curve1->Steps);

  // Marker
  d_marker1 = new QwtPlotMarker();
  d_marker1->setValue( 0.0, 0.0 );
  d_marker1->setLineStyle( QwtPlotMarker::VLine );
  d_marker1->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
  d_marker1->setLinePen( Qt::red, 0, Qt::DashDotLine );
  d_marker1->attach( this );

  setAutoReplot( true );
  connect(this, SIGNAL(statusChanged(QString)), parent, SLOT(showInfo(QString)));

}

void Plot::loadCalibration() {
  std::string local_folder { PROJECT_LOCAL_FOLDER };
  local_folder.append("/etc/maxrf/Calibration.txt");


  QFile file(local_folder.c_str());

  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      printf("[!] Couldn't open binary file storing calibration parameters");
      return;
    }
    else {
      QString line = file.readLine();
      cal_gradient = line.toDouble();

      line = file.readLine();
      cal_offset= line.toDouble();

      file.close();
      Calibration(cal_gradient, cal_offset);
    }
  }
  else return;
}

void Plot::Calibration(double gradient = 1, double offset = 0) {
  std::string local_folder { PROJECT_LOCAL_FOLDER };
  local_folder.append("/etc/maxrf/Calibration.txt");

  cal_offset = offset;
  cal_gradient = gradient;

  QFile file( local_folder.c_str() );
  file.remove();
  file.open(QIODevice::WriteOnly);
  QTextStream out(&file);
  out<<cal_gradient<<'\n';
  out<<cal_offset<<'\n';
  file.close();

  if (cal_gradient == (double)(1) && cal_offset == (double)(0)) {
    setAxisTitle(QwtPlot::xBottom, "Channel" );
  }
  else {
    setAxisTitle( QwtPlot::xBottom, "Energy (keV)" );
  }

  channel_upperbound = ArraySize * cal_gradient + cal_offset;
  channel_lowerbound = cal_offset;

  const bool doReplot = autoReplot();
  setAutoReplot( true );

  for (int h = 0; h < ArraySize; h++) {
    if (EnergyOn) channels[h] = h * cal_gradient + cal_offset;
    else channels[h] = h;
  }

  if (autoscaleOn) setAxisScale(QwtPlot::yLeft, 0, max);
  setAxisScale(QwtPlot::xBottom, channel_lowerbound, channel_upperbound);

  d_curve1->setSamples(channels, counts, ArraySize);
  setAutoReplot( doReplot );
  replot();
  //EnergyOn=true;
}

void Plot::Check_SHM() { // Requests to show the spectrum of pixels selected in the map
  if (*(shared_memory+99) == 1) { showPixelHisto(); *(shared_memory+99) = 0; }
}

void Plot::Open() {
  QString dataDir = "/home/frao/Desktop/XRFData";
  fileName = QFileDialog::getOpenFileName(this, tr("Open histogram file"), dataDir);

  if (!fileName.isEmpty()) {
    autoReplot();
    setAutoReplot(true);
    for (int k = 0; k < 16385; k++) {
      channels[k] = k; counts[k] = 0; k++; }


    //    if (*shared_memory_cmd+70 == 0) for (int h = 0; h < 16385; h++) {
    //      *(shared_memory+100+h) = 0;
    //      *(shared_memory+20000+h) = 0;
    //      *(shared_memory+40000+h) = 0;
    //    }
    std::ifstream file_opened { fileName.toStdString() };

    //    QFile fileOpened(fileName);
    if (!file_opened.is_open()) {
      emit statusChanged("[!] File cannot be opened");
      return;
    }

    //    if (!fileOpened.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //      emit statusChanged("[!] File cannot be opened");
    //      return;
    //    }
    else {
      pugi::xml_document doc {};
      auto parse_result = doc.load(file_opened);

      if (parse_result.status != pugi::xml_parse_status::status_ok) {
        emit statusChanged("[!] File is corrupt");
        return;
      }

      auto node = doc.child("XRFAnalysis").child("Analysis_Data");
      if (node.type() == pugi::xml_node_type::node_null) {
        emit statusChanged("[!] This file has no data!");
        return;
      }
      auto data = node.child_value();
      if (data == nullptr) {
        emit statusChanged("[!] This file has no data!");
        return;
      }
      std::string formatted { data };
      std::strstream sdata { };
      sdata << formatted;

      int  counts {};
      char separator {};
      std::vector<int> vec;
      vec.reserve(16384);

      sdata >> std::noskipws;
      sdata >> separator; // First \n character
      // If the ignore white space flag is not set, the strstream skips
      // any leading white space including \n
      while (sdata >> counts >> separator) {
        vec.push_back(counts);

      }

      uint16_t offset { 0};
      auto it = vec.begin();
      while (it != vec.end() && offset < 16384) {
        shared_memory[100 + offset] = *(it + offset);
        ++offset;
      }

//      for (auto it = vec.begin(); it != vec.begin() + 16384; ++it) {
//        shared_memory[100] = *it;
//      }

      //      int iLinedet1=0;
      //      while (!fileOpened.atEnd()) { // Parses the values and writes them to shared memory
      //        QString line = fileOpened.readLine();
      //        QStringList list1 = line.split('\t');
      //        *(shared_memory+100+iLinedet1) = list1[0].toInt();
      //        *(shared_memory+20000+iLinedet1) = list1[1].toInt();
      //        *(shared_memory+40000+iLinedet1) = list1[2].toInt();
      //        iLinedet1++;


      //        // 0\t0\t0
      //        // 0 1 0
      //        // 2 0 1
      //      }
      //
    }
  }

  showPixelHisto();
  emit statusChanged(fileName);
}

void Plot::showPixelHisto() {
  if (!EnergyOn) setAxisTitle(QwtPlot::xBottom, "Channel");
  else setAxisTitle(QwtPlot::xBottom, "Energy (keV)");

  max = 0; // Reset the scale to zero first
  const bool doReplot = autoReplot();
  for (int h = 0; h < 16385; h++) {
    if (EnergyOn) channels[h] = h * cal_gradient + cal_offset;
    else channels[h] = h;

    counts[h] = 0;
    if ( *(shared_memory_cmd+100) == 0 )        counts[h] = static_cast<double>(*(shared_memory+100+h));
    else if ( *(shared_memory_cmd+100) == 1 )   counts[h] = static_cast<double>(*(shared_memory+20000+h));
    else if ( *(shared_memory_cmd+100) == 2 )   counts[h] = static_cast<double>(*(shared_memory+40000+h));

    if (logchecked == true && counts[h] > 0) counts[h] = (log10(counts[h]));
    if (h != 0 && counts[h] > max) max = counts[h]; //A bit of a work-around the autoscale issue
  }

  if (autoscaleOn)    setAxisScale(QwtPlot::yLeft, 0, max);
  if (!liveOn)      setAxisScale(QwtPlot::yLeft, 0, max);

  d_curve1->setSamples(channels, counts, 16385);
  setAutoReplot(doReplot);
  replot();
}



