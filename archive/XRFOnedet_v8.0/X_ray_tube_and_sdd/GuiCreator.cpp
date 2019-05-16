#include <QtGui>
#include <QSpinBox>
#include "mainwindow.h"

extern int Resolution_mode;

void MainWindow::GuiCreator()
{

    QWidget *mywidget= new QWidget;
    setCentralWidget(mywidget);

    tabWidget_sx = new QTabWidget(mywidget);
    tabWidget_dx = new QTabWidget(mywidget);
    tab1 = new QWidget(tabWidget_sx);
    tab2 = new QWidget(tabWidget_sx);
    tab3 = new QWidget(tabWidget_dx);
//////////////// INPUT SIGNAL/////////////////////////////
//////////////// TAB_WIDGET_SX 
//////////////// TAB1 -> TTY

    QComboBox *comboBox = new QComboBox(tab1); ////// INPUT RANGE
    comboBox->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "-select port-", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "TTYACM 0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "TTYACM 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "TTYACM 2", 0, QApplication::UnicodeUTF8)
        );


    QPushButton *TTY_connect_button = new QPushButton(tab1);
    TTY_connect_button->setText("Connect");
    linedit_TTY = new QLineEdit(tab1);
    linedit_TTY->setEnabled(false);

////////////// TAB2 -> SDD

    QPushButton *SDD_ON_OFF_button = new QPushButton(tab2);
    SDD_ON_OFF_button->setText("ON/OFF");    
    linedit_SDD_HV_status = new QLineEdit(tab2);
    linedit_SDD_HV_status->setEnabled(false);
    linedit_SDD_HV_status->setText("OFF");

//////////////// TAB_WIDGET_DX 

    QLabel *label_X_Voltage = new QLabel(tab3);
    label_X_Voltage->setText("Voltage (kV)");
    QDoubleSpinBox *X_voltage_SpinBox = new QDoubleSpinBox(tab3);
    X_voltage_SpinBox->setDecimals(1);
    X_voltage_SpinBox->setMaximum(40);
    X_voltage_SpinBox->setSingleStep(0.5);
    X_voltage_SpinBox->setValue(4);
    QLabel *label_X_Current = new QLabel(tab3);
    label_X_Current->setText("Current (uA)");
    QDoubleSpinBox *X_current_SpinBox = new QDoubleSpinBox(tab3);
    X_current_SpinBox->setDecimals(1);
    X_current_SpinBox->setMaximum(100);
    X_current_SpinBox->setSingleStep(0.5);
    X_current_SpinBox->setValue(20);
    QPushButton *XRAY_ON_OFF_button = new QPushButton(tab3);
    XRAY_ON_OFF_button->setText("ON/OFF");    
    linedit_XRAY_HV_status = new QLineEdit(tab3);
    linedit_XRAY_HV_status->setEnabled(false);
    linedit_XRAY_HV_status->setText("OFF");



//////////////// WIDGET (BOTTOM)

    QLabel *label_SDD_monitor = new QLabel(mywidget);
    label_SDD_monitor->setText("SDD monitor");

    QLabel *label_XRAY_monitor = new QLabel(mywidget);
    label_XRAY_monitor->setText("X-ray monitor");

    QLabel *label_SDD_volt_mon = new QLabel(mywidget);
    label_SDD_volt_mon->setText("Voltage");

    QLabel *label_XRAY_volt_mon = new QLabel(mywidget);
    label_XRAY_volt_mon->setText("Voltage");

    QLabel *label_XRAY_curr_mon = new QLabel(mywidget);
    label_XRAY_curr_mon->setText("Current");




    linedit_Xray_V = new QLineEdit(mywidget);
    linedit_Xray_V->setEnabled(false);

    linedit_Xray_I = new QLineEdit(mywidget);
    linedit_Xray_I->setEnabled(false);

    linedit_SDD_V = new QLineEdit(mywidget);
    linedit_SDD_V->setEnabled(false);



//    linedit_Energy = new QLineEdit(mywidget);
//    linedit_Energy->setEnabled(false);


    QPushButton *button2 = new QPushButton(mywidget);
    button2->setText("quit");




// GEOMETRY ///////////////////////////////////////////////////////////////////

//qDebug()<<Resolution_mode;

   switch (Resolution_mode)
   {
    case(0):
    {
/////TAB1

    tabWidget_sx->setGeometry(QRect(20, 20, 300, 270));
    tabWidget_dx->setGeometry(QRect(340, 20, 400, 270));
    comboBox->setGeometry(QRect(50, 30, 200, 44));
    TTY_connect_button->setGeometry(QRect(50, 90, 200, 44));
    linedit_TTY->setGeometry(QRect(50, 150, 200, 40));
/////TAB2
    SDD_ON_OFF_button->setGeometry(QRect(20, 150, 150, 40));
    linedit_SDD_HV_status->setGeometry(QRect(200, 150, 80, 40));
/////TAB3
    label_X_Voltage->setGeometry(QRect(20, 30, 160, 40));
    X_voltage_SpinBox->setGeometry(QRect(210, 30, 150, 44));
    label_X_Current->setGeometry(QRect(20, 90, 160, 40));
    X_current_SpinBox->setGeometry(QRect(210, 90, 150, 44));
    XRAY_ON_OFF_button->setGeometry(QRect(20, 150, 150, 40));    
    linedit_XRAY_HV_status->setGeometry(QRect(210, 150, 150, 40));


    tabWidget_sx->addTab(tab1, QString());
    tabWidget_sx->addTab(tab2, QString());
    tabWidget_sx->setTabText(tabWidget_sx->indexOf(tab1), QApplication::translate("MyWidget", "TTY", 0, QApplication::UnicodeUTF8));
    tabWidget_sx->setTabText(tabWidget_sx->indexOf(tab2), QApplication::translate("MyWidget", "SDD_HV", 0, QApplication::UnicodeUTF8));


    tabWidget_dx->addTab(tab3, QString());
    tabWidget_dx->setTabText(tabWidget_dx->indexOf(tab3), QApplication::translate("MyWidget", "X-RAY", 0, QApplication::UnicodeUTF8));

    
//    labelSpin->setGeometry(QRect(400, 40, 280, 24));

    label_SDD_monitor->setGeometry(QRect(90, 320, 180, 40));
    label_XRAY_monitor->setGeometry(QRect(460, 320, 180, 40));

    label_SDD_volt_mon->setGeometry(QRect(120, 390, 180, 40));
    label_XRAY_volt_mon->setGeometry(QRect(400, 390, 180, 40));
    label_XRAY_curr_mon->setGeometry(QRect(600, 390, 180, 40));

    linedit_SDD_V->setGeometry(QRect(80, 440, 180, 40));
    linedit_Xray_V->setGeometry(QRect(360, 440, 180, 40));
    linedit_Xray_I->setGeometry(QRect(560, 440, 180, 40));




    button2->setGeometry(QRect(20, 500, 720, 56));
    break;
    }

    case(1): ////////// LOW RESOLUTION CASES
    {
    comboBox->setGeometry(QRect(52, 67, 150, 33));
    linedit_Xray_V->setGeometry(QRect(30, 120, 187, 30));
    linedit_Xray_I->setGeometry(QRect(30, 157, 187, 30));
    linedit_SDD_V->setGeometry(QRect(30, 195, 187, 30));

    button2->setGeometry(QRect(15, 360, 540, 42));
    break;
    }

    case(2):
    {
    comboBox->setGeometry(QRect(35, 45, 100, 22));
    linedit_Xray_V->setGeometry(QRect(20, 80, 125, 20));
    linedit_Xray_I->setGeometry(QRect(20, 105, 125, 20));
    linedit_SDD_V->setGeometry(QRect(20, 130, 125, 20));

    button2->setGeometry(QRect(10, 240, 360, 28));
    break;
    }
   }


// COONNECTIONS

     connect(comboBox, SIGNAL(activated(int)), 
          this, SLOT(NameACM(int)));


     connect(TTY_connect_button, SIGNAL(released()), 
          this, SLOT(Arduino_serial_init()));

     connect(SDD_ON_OFF_button, SIGNAL(released()), 
          this, SLOT(Sdd_power()));

     connect(X_voltage_SpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Xray_voltage(double)));

     connect(X_current_SpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Xray_current(double)));

     connect(XRAY_ON_OFF_button, SIGNAL(released()), 
          this, SLOT(Xray_power()));


     connect(button2, SIGNAL(clicked()), this, SLOT(Exit())); ///aggiungere check se è fermo qApp, SLOT(quit())




}



