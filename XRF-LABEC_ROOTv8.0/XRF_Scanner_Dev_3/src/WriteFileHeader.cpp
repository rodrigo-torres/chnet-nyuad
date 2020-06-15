#include "include/MAXRF/mainwindow.h"
#include <../Shm.h>
#include <QtGui>
using namespace std;

extern QString percorso_Data_file;
extern int *shared_memory_cmd;
int obj_n=2;
int dev_n=2;
int ses_n=3;
int ana_n=7;
int map_n=7;

int MainWindow::WriteFileHeader(){

int Xmin, Xmax, Ymin, Ymax, Px, Py, V;
int Info[map_n];
QString info_message,n;

////build the gui and fileds///////////////////////////////// 
QFont *font1 = new QFont("Courier New");
font1->setPixelSize(14);

QString object_elements[obj_n]={"objectName","objectIdentifier"};
QString device_elements[dev_n]={"deviceIdentifier","deviceMake"};
QString session_elements[ses_n]={"sessionIdentifier","investigationType","investigationCalibration"};
QString analysis_elements[ana_n]={"analysisIdentifier","measurementUnit","sampleAreaFile","samplingNote","analysisOperator","analysisDateAndTime","analysisNote"};
QString info_elements[map_n]={"Xmin","Xmax","Ymin","Ymax","Xstep","Ystep","Velocity"};

QString defaults[12] ={"Unknown","object00","INFN-CHnet-123456","XRF_Scanner","OPD-123456","XRF surface","cal.txt","2017-XX","keV","foto.jpg","area of interest","CHnet #XX No name"};
QDialog dialog(this);
QFormLayout form(&dialog);
QLabel *label1= new QLabel("Fill in info for XML file header");
//form.addRow(new QLabel("Fill in info for XML file header"));
form.addRow(label1);

QList<QLineEdit *> fields;//list of all fields to be filled manually

///object////////
for(int i = 0; i < obj_n; ++i) {
    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = object_elements[i]; //QString("Value %1").arg(i + 1);
    form.addRow(label, lineEdit);

    fields << lineEdit;
}

////device//////
QComboBox *Dev_id_combo = new QComboBox(&dialog);
Dev_id_combo->addItem(tr("INFN-CHnet-123456"));
Dev_id_combo->addItem(tr("INFN-CHnet-??????"));
Dev_id_combo->addItem(tr("INFN-CHnet-??????"));
QString labeld = device_elements[0]; 
form.addRow(labeld, Dev_id_combo);

QLineEdit *lineEditd = new QLineEdit(&dialog);
lineEditd->setText("Philips PW1606");
QString labeld1 = device_elements[1]; 
form.addRow(labeld1, lineEditd);
fields << lineEditd;

///session///////
for(int i = 0; i < (ses_n-1); ++i) {
    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = session_elements[i]; //QString("Value %1").arg(i + 1);
    form.addRow(label, lineEdit);

    fields << lineEdit;
}

QComboBox *calib_combo = new QComboBox(&dialog);
calib_combo->addItem(tr("cal.txt"));
calib_combo->addItem(tr("cal1.txt"));
calib_combo->addItem(tr("cal2.txt"));
QString labelc = session_elements[2]; 
form.addRow(labelc, calib_combo);

////analysis//////
QLineEdit *lineEditx = new QLineEdit(&dialog);
QString labelx = analysis_elements[0]; //QString("Value %1").arg(i + 1);
form.addRow(labelx, lineEditx);
fields << lineEditx;

QComboBox *unit_combo = new QComboBox(&dialog);
unit_combo->addItem(tr("eV"));
unit_combo->addItem(tr("keV"));
unit_combo->addItem(tr("mV"));
QString labelu = analysis_elements[1]; 
form.addRow(labelu, unit_combo);

for(int i = 2; i < (ana_n-2); ++i) { 

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = analysis_elements[i]; //QString("Value %1").arg(i + 1);
    form.addRow(label, lineEdit);
    fields << lineEdit;
	
}

/*    QTextEdit *note_txt = new QTextEdit(&dialog);
	note_txt->setText("write notes here...(optional)");
	QString labeln = analysis_elements[6]; 
    form.addRow(labeln, note_txt);
*/




    QLineEdit *note_txt = new QLineEdit(&dialog);
	note_txt->setText("write notes here...(optional)");
	QString labeln = analysis_elements[6]; 
    form.addRow(labeln, note_txt);






	QDateTime date = QDateTime::currentDateTime(); 
	QString dateString = date.toString("yyyy.MM.dd_hh:mm:ss");
        QLineEdit *falselineEdit = new QLineEdit();
        falselineEdit->setText(dateString);
        fields << falselineEdit;


		
//////////////////////	
	
// Buttons (Cancel/Ok) at the bottom of the dialog
QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
form.addRow(&buttonBox);
QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


     /////////////////////////////////////////////////
/////read info already loaded in memory at start scan//////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////
Xmin =*(shared_memory_cmd+50);
Xmax =*(shared_memory_cmd+51);
Ymin =*(shared_memory_cmd+52);
Ymax =*(shared_memory_cmd+53);
Px =*(shared_memory_cmd+60);
Py =*(shared_memory_cmd+61);
V =*(shared_memory_cmd+67);

Info[0]=Xmin;
Info[1]=Xmax;
Info[2]=Ymin;
Info[3]=Ymax;
Info[4]=Px;
Info[5]=Py;
Info[6]=V;

n.setNum(Xmin);
info_message = "Map parameters loaded from memory:\n\nXmin = " + n;
n.setNum(Xmax);
info_message.append(", Xmax = ");
info_message.append(n);
info_message.append('\n');
n.setNum(Ymin);
info_message.append("Ymin = ");
info_message.append(n);
info_message.append(", Ymax = ");
n.setNum(Ymax);
info_message.append(n);
info_message.append('\n');
n.setNum(Px);
info_message.append("Px = ");
info_message.append(n);
info_message.append(", Py = ");
n.setNum(Py);
info_message.append(n);
info_message.append('\n');
info_message.append("Scan velocity = ");
n.setNum(V);
info_message.append(n);
info_message.append(" um/s \n");


QMessageBox::information(this,tr("Map Info"),info_message);

/////////////////////////////////////////////////////////////////////////////////
	
//If OK has been clicked////
if (dialog.exec() == QDialog::Accepted) {

printf("ok clicked\n");	
///create file and write header///
    percorso_Data_file = QFileDialog::getSaveFileName(this,tr("Output file will be saved as: ... "), QDir::currentPath());
	QFile file2(percorso_Data_file); 
	file2.open(QIODevice::ReadWrite | QIODevice::Truncate);
	QTextStream out2(&file2);
	
	out2 << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	out2 << "<XRFanalysis>\n";
        out2 << "\t<analysisFileLocation>\"http://server.web/your/folder/\"</analysisFileLocation>\n";

        out2 << "\t<object>\n";
	for(int j=0;j<obj_n;j++)
		out2 << "\t\t<" << object_elements[j] << ">\"" << fields[j]->text() << "\"" << "</" << object_elements[j] << ">\n";		
        out2 << "\t</object>\n";	

	out2 << "\t<device>\n";
	out2 << "\t\t<" << device_elements[0] << ">\"" << Dev_id_combo->currentText() << "\"" << "</" << device_elements[0] << ">\n";
	out2 << "\t\t<" << device_elements[1] << ">\"" << fields[2]->text() << "\"" << "</" << device_elements[1] << ">\n";
        out2 << "\t</device>\n";
	
	out2 << "\t<session>\n";
	for(int j=0;j<(ses_n-1);j++)
		out2 << "\t\t<" << session_elements[j] << ">\"" << fields[j+3]->text() << "\"" << "</" << session_elements[j] << ">\n";
	out2 << "\t\t<" << session_elements[2] << ">\"" << calib_combo->currentText() << "\"" << "</" << session_elements[2] << ">\n";	
	out2 << "\t</session>\n";
	
	out2 << "\t<analysisMetadata>\n";	
	out2 << "\t\t<" << analysis_elements[0] << ">\"" << fields[5]->text() << "\"" << "</" << analysis_elements[0] << ">\n";
	out2 << "\t\t<" << analysis_elements[1] << ">\"" << unit_combo->currentText() << "\"" << "</" << analysis_elements[1] << ">\n";
	for(int j=0;j<(ana_n-3);j++)
		out2 << "\t\t<" << analysis_elements[j+2] << ">\"" << fields[j+6]->text() << "\"" << "</" << analysis_elements[j+2] << ">\n";
	out2 << "\t\t<" << analysis_elements[6] << ">\"" << note_txt->text()/*->toPlainText()*/ << "\"" << "</" << analysis_elements[6] << ">\n";	///note
	out2 << "\t</analysisMetadata>\n";		
	out2 << "\t<analysisInfo>\n";
		for(int j=0;j<(map_n);j++)
		out2 << "\t\t<" << info_elements[j] << ">\"" << Info[j] << "\"" << "</" << info_elements[j] << ">\n";
	out2 << "\t</analysisInfo>\n";	
	out2 << "\t<analysisData>\n";	
	
file2.close();
return 1;
}
else{ ///if Cancel has been clicked ---> Default header

///create file and write header///
        percorso_Data_file = "../Data/data_"+dateString+".txt";
	QFile file2(percorso_Data_file); 
	file2.open(QIODevice::ReadWrite);
	QTextStream out2(&file2);
	
	out2 << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	out2 << "<XRFanalysis>\n";
        out2 << "\t<analysisFileLocation>\"http://server.web/your/folder/\"</analysisFileLocation>\n";
        out2 << "\t<object>\n";

	for(int j=0;j<obj_n;j++)
		out2 << "\t\t<" << object_elements[j] << ">\"" << defaults[j] << "\"" << "</" << defaults[j] << ">\n";		
        out2 << "\t</object>\n";
	
	out2 << "\t<device>\n";	
		for(int j=0;j<dev_n;j++)
		out2 << "\t\t<" << device_elements[j] << ">\"" << defaults[j+2] << "\"" << "</" << device_elements[j] << ">\n";	
    out2 << "\t</device>\n";

	out2 << "\t<session>\n";
		for(int j=0;j<ses_n;j++)
		out2 << "\t\t<" << session_elements[j] << ">\"" << defaults[j+4] << "\"" << "</" << session_elements[j] << ">\n";
	out2 << "\t</session>\n";

	out2 << "\t<analysisMetadata>\n";	
		for(int j=0;j<(ana_n-2);j++)
		out2 << "\t\t<" << analysis_elements[j] << ">\"" << defaults[j+7] << "\"" << "</" << analysis_elements[j] << ">\n";
	out2 << "\t\t<" << analysis_elements[5] << ">\"" << dateString << "\"" << "</" << analysis_elements[5] << ">\n";
	out2 << "\t</analysisMetadata>\n";	
	out2 << "\t<analysisInfo>\n";
		for(int j=0;j<(map_n);j++)
		out2 << "\t\t<" << info_elements[j] << ">\"" << Info[j] << "\"" << "</" << info_elements[j] << ">\n";
	out2 << "\t</analysisInfo>\n";	

	out2 << "\t<analysisData>\n";


file2.close();
return 0;
}


}
