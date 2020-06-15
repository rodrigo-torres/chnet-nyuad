#include <mainwindow.h>
#include "../Header.h"
#include <../Shm.h>

extern int Resolution_mode;

void MainWindow::GUI_CREATOR()
{
   centralWidget = new QWidget;
   setCentralWidget(centralWidget);
        
   CurrentActionX = new QLineEdit(centralWidget);
   label_tab_2_bX = new QLabel(centralWidget);

   spinBox_assignX = new QSpinBox(centralWidget);
   spinBox_SPA_CH = new QSpinBox(centralWidget);
   spinBox_SPA_value = new QDoubleSpinBox(centralWidget);

   pushButton_assignX_port = new QPushButton(centralWidget);
   label_tab_2_2 = new QLabel(centralWidget);
   pushButton_tab_2_2X = new QPushButton(centralWidget);
   pushButton_tab_2_2ABORT = new QPushButton(centralWidget);

   label_send = new QLabel(centralWidget);
   label_SPA_CH = new QLabel(centralWidget);
   label_SPA_VALUE = new QLabel(centralWidget);

   pushButton_send = new QPushButton(centralWidget);
   pushButton_PI  = new QPushButton(centralWidget);

   MOVE_TO_X_pushButton = new QPushButton(centralWidget);
   XY_SPEED_label = new QLabel(centralWidget);
   XY_SPEED_doubleSpinBox = new QDoubleSpinBox(centralWidget);
   X_POSITION_label = new QLabel(centralWidget);
   X_POSITION_lineEdit = new QLineEdit(centralWidget);                 ///// linedit 
   ANSWER_lineEdit = new QLineEdit(centralWidget);                 ///// linedit 

   QComboBox *comboBox = new QComboBox(centralWidget); ////// Motor Selection
   comboBox->insertItems(0, QStringList()
    << QApplication::translate("mywidget", "-- select motor --", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "M404-2PD", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "M404-4PD", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "M404-8PD", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "VT80-100", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "VT80-150", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "VT80-200", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "VT75-600", 0, QApplication::UnicodeUTF8)
    );

   QComboBox *comboBox_CMD = new QComboBox(centralWidget); ////// Motor Selection
   comboBox_CMD->insertItems(0, QStringList()
    << QApplication::translate("mywidget", "------", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "CST?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "CSV?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "DFH", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "DFH?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "ERR?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "FNL", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "FPL", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "FRF", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "FRF?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "GOH", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "*IDN?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "LIM?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "ONT?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "RBT", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "SPA", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "SPA?", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("mywidget", "VER?", 0, QApplication::UnicodeUTF8)
    );


//    qDebug()<<"GuiMode: "<<Resolution_mode;

   switch (Resolution_mode)
   {
    case(0):
    {

        CurrentActionX->setObjectName(QString::fromUtf8("CurrentActionX"));
        CurrentActionX->setEnabled(false);
        CurrentActionX->setGeometry(QRect(30, 600, 1040, 60));
        CurrentActionX->setText(".... waiting ....");

        ANSWER_lineEdit->setObjectName(QString::fromUtf8("AnswerX"));
        ANSWER_lineEdit->setEnabled(false);
        ANSWER_lineEdit->setGeometry(QRect(30, 680, 1040, 60));
        ANSWER_lineEdit->setText(".... waiting for connection....");


        label_tab_2_bX->setObjectName(QString::fromUtf8("label_10"));
        label_tab_2_bX->setGeometry(QRect(100, 40, 340, 40));
        label_tab_2_bX->setText(QApplication::translate("MyWidget", "Motor initialisation", 0, QApplication::UnicodeUTF8));

        spinBox_assignX->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignX->setGeometry(QRect(30, 100, 100, 60));
        spinBox_assignX->setMaximum(3);

        pushButton_assignX_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignX_port->setGeometry(QRect(160, 100, 240, 60));
        pushButton_assignX_port->setText(QApplication::translate("MyWidget", "Assign port", 0, QApplication::UnicodeUTF8));


        label_tab_2_2->setObjectName(QString::fromUtf8("label_10"));
        label_tab_2_2->setGeometry(QRect(640, 40, 360, 40));
        label_tab_2_2->setText(QApplication::translate("MyWidget", "... play with the motor ...", 0, QApplication::UnicodeUTF8));

        comboBox->setGeometry(QRect(60, 200, 320, 60));

        pushButton_tab_2_2X->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2X->setGeometry(QRect(60, 300, 320, 60));
        pushButton_tab_2_2X->setText(QApplication::translate("MyWidget", "Init motor", 0, QApplication::UnicodeUTF8));

        label_send->setObjectName(QString::fromUtf8("label_10"));
        label_send->setGeometry(QRect(160, 460, 70, 60));
        label_send->setText(QApplication::translate("MyWidget", "CMD", 0, QApplication::UnicodeUTF8));

        comboBox_CMD->setGeometry(QRect(120, 520, 160, 60));

        label_SPA_CH->setObjectName(QString::fromUtf8("label_10"));
        label_SPA_CH->setGeometry(QRect(360, 460, 100, 60));
        label_SPA_CH->setText(QApplication::translate("MyWidget", "SPA ch", 0, QApplication::UnicodeUTF8));

        spinBox_SPA_CH->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_SPA_CH->setGeometry(QRect(350, 520, 100, 60));
        spinBox_SPA_CH->setMaximum(60);

        label_SPA_VALUE->setObjectName(QString::fromUtf8("label_10"));
        label_SPA_VALUE->setGeometry(QRect(550, 460, 150, 60));
        label_SPA_VALUE->setText(QApplication::translate("MyWidget", "SPA value", 0, QApplication::UnicodeUTF8));

        spinBox_SPA_value->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_SPA_value->setGeometry(QRect(520, 520, 200, 60));
        spinBox_SPA_value->setDecimals(4);
        spinBox_SPA_value->setMaximum(10000);

        pushButton_PI->setObjectName(QString::fromUtf8("pushButton_8")); 
        pushButton_PI->setGeometry(QRect(780, 400, 300, 60));
        pushButton_PI->setText(QApplication::translate("MyWidget", "PI settings", 0, QApplication::UnicodeUTF8));

        pushButton_send->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_send->setGeometry(QRect(800, 520, 260, 60));
        pushButton_send->setText(QApplication::translate("MyWidget", "SEND", 0, QApplication::UnicodeUTF8));

        pushButton_tab_2_2ABORT->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2ABORT->setGeometry(QRect(30, 760, 1040, 100));
        pushButton_tab_2_2ABORT->setText(QApplication::translate("MyWidget", "STOP", 0, QApplication::UnicodeUTF8));


        MOVE_TO_X_pushButton->setObjectName(QString::fromUtf8("MOVE_TO_X_pushButton"));
        MOVE_TO_X_pushButton->setGeometry(QRect(500, 100, 240, 60));
        MOVE_TO_X_pushButton->setText(QApplication::translate("MyWidget", "... move to ...", 0, QApplication::UnicodeUTF8));

        MOVE_TO_X_doubleSpinBox = new QDoubleSpinBox(centralWidget);
        MOVE_TO_X_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_X_doubleSpinBox"));
        MOVE_TO_X_doubleSpinBox->setGeometry(QRect(780, 100, 300, 60));
        MOVE_TO_X_doubleSpinBox->setDecimals(3);
        MOVE_TO_X_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        MOVE_TO_X_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_X_doubleSpinBox->setValue(25);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////// AREA BELOW TABWIDGET2


        XY_SPEED_label->setObjectName(QString::fromUtf8("XY_SPEED_label"));
        XY_SPEED_label->setGeometry(QRect(500, 200, 240, 60));
        XY_SPEED_label->setText(QApplication::translate("MyWidget", "... speed (mm/s) ...", 0, QApplication::UnicodeUTF8));

        XY_SPEED_doubleSpinBox->setObjectName(QString::fromUtf8("XY_SPEED_doubleSpinBox"));
        XY_SPEED_doubleSpinBox->setGeometry(QRect(780, 200, 300, 60));
        XY_SPEED_doubleSpinBox->setDecimals(3);
        XY_SPEED_doubleSpinBox->setMaximum(50);
        XY_SPEED_doubleSpinBox->setSingleStep(0.001);
        XY_SPEED_doubleSpinBox->setValue(5);

        X_POSITION_label->setObjectName(QString::fromUtf8("X_POSITION_label"));
        X_POSITION_label->setGeometry(QRect(500, 300, 240, 60));
        X_POSITION_label->setText(QApplication::translate("MyWidget", "... stage position: ", 0, QApplication::UnicodeUTF8));

        X_POSITION_lineEdit->setObjectName(QString::fromUtf8("X_POSITION_lineEdit"));
        X_POSITION_lineEdit->setEnabled(false);
        X_POSITION_lineEdit->setGeometry(QRect(780, 300, 300, 60));


     break;
     }

    case(1):
     {
        ;
        break;

     }

    case(2):
     {
        ;
        break;

     }

    } //swich closed
     connect(spinBox_assignX, SIGNAL(valueChanged(int)), 
          this, SLOT(NameX(int)));
     connect(pushButton_assignX_port, SIGNAL(released()),
            this, SLOT(AssignX()));
     connect(pushButton_tab_2_2X, SIGNAL(released()),
            this, SLOT(StartX()));
     connect(MOVE_TO_X_pushButton, SIGNAL(released()),
            this, SLOT(MoveX_To()));
     connect(pushButton_tab_2_2ABORT, SIGNAL(clicked()), this, SLOT(Stop()));
     connect(XY_SPEED_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Velocity(double)));
     connect(pushButton_send, SIGNAL(clicked()), this, SLOT(Send_CMD()));
     connect(comboBox, SIGNAL(activated(int)), this, SLOT(Motor_selection(int)));
     connect(MOVE_TO_X_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(X_To(double)));


     connect(pushButton_PI, SIGNAL(clicked()), this, SLOT(Show_PI_Parameters()));

     connect(comboBox_CMD, SIGNAL(activated(int)), this, SLOT(Motor_CMD(int)));
     connect(spinBox_SPA_CH, SIGNAL(valueChanged(int)), 
          this, SLOT(SPA_Parameter(int)));
     connect(spinBox_SPA_value, SIGNAL(valueChanged(double)), 
          this, SLOT(SPA_Parameter_value(double)));


}












