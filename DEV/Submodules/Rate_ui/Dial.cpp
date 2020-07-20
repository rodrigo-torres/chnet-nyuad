#include "Dial.h"

int main ( int argc, char **argv ) {
  QApplication app(argc, argv);

  maxrf::rate::DialBox dialbox {};

  return app.exec();
}

namespace maxrf::rate {

template <typename T>
T * assignSHM(key_t key, size_t size) {
  T * ptr {nullptr};

  // Attempt to access the segment as if it existed first
  int shmID = shmget(key, size, 0);
  if (shmID == - 1 && errno != ENOENT) {
    std::cout << "[!] Couldn't get SHM segment with key: " << key << std::endl;
    std::cout << strerror(errno) << std::endl;
  }
  // If the segment doesn't exist, create it and tag it for removal
  else {
    shmID = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
    if (shmID != -1) {
      ptr   = static_cast<T *>(shmat(shmID, nullptr, 0));
      shmctl(shmID, IPC_RMID, nullptr);
    }
    else {
      std::cout << "[!] Couldn't get SHM segment with key: " << key << std::endl;
      std::cout << strerror(errno) << std::endl;
    }
  }

  return ptr;
}

DialBox::DialBox(QWidget * parent): QWidget {parent} {
  key_t key_cmd = 6900;
  shared_memory_cmd = assignSHM<int>(key_cmd, SHMSZ_CMD_STATUS);

  *(shared_memory_cmd+83) = getpid();


  createDial();
  auto update_event_enable_ = new QCheckBox("Enable", this);

  rate_label_ = new QLabel( this );
  rate_label_->setAlignment( Qt::AlignCenter );

  max_rate_label_ = new QLabel( this );
  max_rate_label_->setAlignment( Qt::AlignRight );

  QVBoxLayout *layout = new QVBoxLayout {this};
  layout->setSpacing( 0 );
  layout->addWidget( update_event_enable_ );
  layout->addWidget( rate_dial_, 10 );
  layout->addWidget( rate_label_ );
  layout->addWidget( max_rate_label_ );


  connect(rate_dial_, &QwtDial::valueChanged, this, &DialBox::setNum);
  connect(update_event_enable_, &QCheckBox::stateChanged,
          this, &DialBox::EventEnable);

  rate_dial_->setValue(0);
  setNum( rate_dial_->value() );

  update_event_timer_ = new QTimer {this};
  connect(update_event_timer_, SIGNAL(timeout()), this, SLOT(TimerEvent()));

  this->setAutoFillBackground(true);
  this->setPalette(Qt::darkGray);

  this->resize(this->sizeHint());
  this->show();
}

void DialBox::createDial() {
  rate_dial_ = new QwtDial();
  rate_dial_->setTracking( true );
  rate_dial_->setFocusPolicy( Qt::StrongFocus );

  QColor needleColor( Qt::red );

  rate_dial_->setOrigin( 135.0 );
  rate_dial_->setScaleArc( 0.0, 270.0 );
  rate_dial_->setScaleMaxMinor( 9 );
  rate_dial_->setScaleEngine( new QwtLogScaleEngine );
  rate_dial_->setScale( 1.0, 1.0e5 );

  QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, needleColor,
        QColor( Qt::gray ).light( 130 ) ); //130
  rate_dial_->setNeedle( needle );

  const QColor base( QColor( Qt::darkGray ).dark( 150 ) );

  QPalette palette;
  palette.setColor( QPalette::Base, base );
  palette.setColor( QPalette::Window, base.dark( 150 ) );
  palette.setColor( QPalette::Mid, base.dark( 110 ) );
  palette.setColor( QPalette::Light, base.light( 170 ) );
  palette.setColor( QPalette::Dark, base.dark( 170 ) );
  palette.setColor( QPalette::Text, base.dark( 200 ).light( 800 ) );
  palette.setColor( QPalette::WindowText, base.dark( 200 ) );

  rate_dial_->setPalette( palette );
  rate_dial_->setLineWidth( 4 );
  rate_dial_->setFrameShadow( QwtDial::Sunken );

}

void DialBox::setNum(double rate) {
  if (rate >  max_rate_) {
    max_rate_ = rate;
    max_rate_label_->setText( "Max. rate: " + QString::number(max_rate_, 'f', 2));
  }
  rate_label_->setText(QString::number(rate, 'f', 2));
}

void DialBox::TimerEvent() {

  auto rate = shared_memory_cmd[600];
  if (rate <= 0) {
    rate = 1;
  }

  rate_dial_->setValue(rate);
}



void DialBox::EventEnable(int state) {
  constexpr int kUpdateInterval = 1000;

  switch (state) {
  case Qt::PartiallyChecked :
    [[fallthrough]];
  case Qt::Checked :
    update_event_timer_->start(kUpdateInterval);
    break;
  case Qt::Unchecked :
    [[fallthrough]];
  default:
    update_event_timer_->stop();
    break;
  }

}


DialBox:: ~DialBox() {
  *(shared_memory_cmd+73) = 0;
  shmdt(shared_memory_cmd);
}

}  //namespace maxrf::rate







