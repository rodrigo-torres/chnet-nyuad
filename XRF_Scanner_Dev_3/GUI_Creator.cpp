#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

extern int Resolution_mode;

void MainWindow::GUI_CREATOR()
{
        // Putting this here for future use.
        QFont font7;
        font7.setFamily(QString::fromUtf8("Helvetica"));
        font7.setPointSize(18);

        QFont font1;
        font1.setFamily(QString::fromUtf8("Cantarell"));
        font1.setPointSize(9);
        font1.setBold(true);

        QFont font2;
        font2.setFamily(QString::fromUtf8("Cantarell"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);

        centralWidget = new QWidget();
        setCentralWidget(centralWidget);
        QGroupBox *groupBox = new QGroupBox(centralWidget); //QGroupBox(tr("DAQ Type"));
        QRadioButton *radioUSB = new QRadioButton(groupBox);
        QRadioButton *radioOptical = new QRadioButton(groupBox);
        FRAME_line1 = new QFrame(centralWidget);
        FRAME_line2 = new QFrame(centralWidget);
        FRAME_line3 = new QFrame(centralWidget);
        FRAME_line4 = new QFrame(centralWidget);
        CAMERA_label = new QLabel(centralWidget);
        VLC_Button = new QPushButton(centralWidget);
        LASER_label = new QLabel(centralWidget);
        LASER_Button = new QPushButton(centralWidget);
        //HELIUM_label = new QLabel(centralWidget);
        //HELIUM_Button = new QPushButton(centralWidget);
        //lcdNumber = new QLCDNumber(centralWidget);
        //horizontalSlider = new QSlider(centralWidget);
        DAQ_label = new QLabel(centralWidget);
        Digitizer_Button = new QPushButton(centralWidget);
        Detector_Button = new QPushButton(centralWidget);
        SW_treshold_Button = new QPushButton(centralWidget);
        TTY_label= new QLabel(centralWidget);
        CurrentActionX = new QLineEdit(centralWidget);
        CurrentActionY = new QLineEdit(centralWidget);
        CurrentActionZ = new QLineEdit(centralWidget);
        Export_Button = new QPushButton(centralWidget);
        CurrentAction = new QLineEdit(centralWidget);
        line_1 = new QFrame(centralWidget);
        line_2 = new QFrame(centralWidget);
        line_3 = new QFrame(centralWidget);
        line_4 = new QFrame(centralWidget);
        //GAS_label = new QLabel(centralWidget);
        XRAY_TABLE_Button = new QPushButton(centralWidget);
        tabWidget = new QTabWidget(centralWidget);
        tab = new QWidget(tabWidget);
        //TTY_Z_label = new QLabel(tab);
        spinBox_assignZ = new QSpinBox(tab);
        AssignZ_pushButton = new QPushButton(tab);
        //linedit_Z_Assignement = new QLineEdit(tab);
        //line_6_tab = new QFrame(tab);
        //label_6_tab = new QLabel(tab);
        INIT_Z_pushButton = new QPushButton(tab);
        STOP_Z_INIT_pushButton = new QPushButton(tab);
        tab_2 = new QWidget(tabWidget);
        MOVE_Z_MOTOR_label_1 = new QLabel(tab_2);
        MOVE_Z_To_pushButton = new QPushButton(tab_2);
        MOVE_Z_To_doubleSpinBox = new QDoubleSpinBox(tab_2);
        MOVE_Z_FORWARD_pushButton = new QPushButton(tab_2);
        MOVE_Z_MOTOR_label_2 = new QLabel(tab_2);
        MOVE_Z_BACKWARD_pushButton = new QPushButton(tab_2);
        STOP_Z_MOVE_pushButton = new QPushButton(tab_2);
        tab_3 = new QWidget(tabWidget);
        //ACM_label = new QLabel(tab_3);
        spinBox_assignACM = new QSpinBox(tab_3);
        AssignACM_pushButton = new QPushButton(tab_3);
        CurrentActionACM = new QLineEdit(centralWidget);
        //tab_4 = new QWidget(tabWidget);
        AUTOFOCUS_ON_pushButton = new QPushButton(tab_3);
        //AUTOFOCUS_STATUS_checkBox = new QCheckBox(tab_4);
        label_23_tab_4 = new QLabel(tab_3);
        lineEdit_2_tab_4 = new QLineEdit(tab_3);
        ENABLE_TRACKING_checkBox = new QCheckBox(tab_3);
        //label_7_below_tab = new QLabel(centralWidget);
        //Z_VELOCITY_doubleSpinBox = new QDoubleSpinBox(centralWidget);
        //line_11 = new QFrame(centralWidget);
        //label_8_below_tab = new QLabel(centralWidget);
        //Z_STEP_doubleSpinBox = new QDoubleSpinBox(centralWidget);
        //line_14 = new QFrame(centralWidget);
        label_17_below_tab = new QLabel(centralWidget);
        lineEdit_below_tab = new QLineEdit(centralWidget);
        //label_18_below_tab = new QLabel(centralWidget);
        //tabWidget2 = new QTabWidget(centralWidget);
        //tab2_1 = new QWidget(tabWidget2);
        //label_tab_2_bX = new QLabel(tab2_1);
        spinBox_assignX = new QSpinBox(tab);
        pushButton_assignX_port = new QPushButton(tab);
        //label_tab_2_bY = new QLabel(tab2_1);
        spinBox_assignY = new QSpinBox(tab);
        pushButton_assignY_port = new QPushButton(tab);
        //tab2_2 = new QWidget(tabWidget2);
        //label_tab_2_2 = new QLabel(tab2_2);
        //label_tab_2_2X = new QLabel(tab2_2);
        //label_tab_2_2Y = new QLabel(tab2_2);
        pushButton_tab_2_2X = new QPushButton(tab);
        pushButton_tab_2_2Y = new QPushButton(tab);
//        pushButton_tab_2_2XY = new QPushButton(tab2_2);
        pushButton_tab_2_2ABORT = new QPushButton(tab);
        tab2_3 = new QWidget(tabWidget);
        MOVE_label = new QLabel(tab2_3);
        MOVEUP_pushButton = new QPushButton(tab2_3);
        MOVELEFT_pushButton = new QPushButton(tab2_3);
        MOVERIGHT_pushButton = new QPushButton(tab2_3);
        MOVEDOWN_pushButton = new QPushButton(tab2_3);
        STOPMOVE_pushButton = new QPushButton(tab2_3);
        MOVE_TO_X_pushButton = new QPushButton(tab2_3);
        MOVE_TO_X_doubleSpinBox = new QDoubleSpinBox(tab2_3);
        MOVE_TO_Y_pushButton = new QPushButton(tab2_3);
        MOVE_TO_Y_doubleSpinBox = new QDoubleSpinBox(tab2_3);
        tab2_4 = new QWidget(tabWidget);
        SCAN_X_label = new QLabel(tab2_4);
        SET_SCAN_X_MIN_doubleSpinBox = new QDoubleSpinBox(tab2_4);
        SET_SCAN_X_MAX_doubleSpinBox = new QDoubleSpinBox(tab2_4);
        SCAN_Y_label = new QLabel(tab2_4);
        SET_SCAN_Y_MIN_doubleSpinBox = new QDoubleSpinBox(tab2_4);
        SET_SCAN_Y_MAX_doubleSpinBox = new QDoubleSpinBox(tab2_4);
        SCANXY_pushButton = new QPushButton(tab2_4);
        SCANYX_pushButton = new QPushButton(tab2_4);
        SCAN_ABORT_pushButton = new QPushButton(tab2_4);
        XY_SPEED_label = new QLabel(centralWidget);
        XY_SPEED_doubleSpinBox = new QDoubleSpinBox(centralWidget);
        XY_SPEED_line = new QFrame(centralWidget);
        X_STEP_label = new QLabel(centralWidget);
        X_STEP_doubleSpinBox = new QDoubleSpinBox(centralWidget);
        XY_STEP_line = new QFrame(centralWidget);
        Y_STEP_label = new QLabel(centralWidget);
        Y_STEP_doubleSpinBox = new QDoubleSpinBox(centralWidget);
        X_POSITION_label = new QLabel(centralWidget);
        X_POSITION_lineEdit = new QLineEdit(centralWidget);                 ///// linedit 
        Y_POSITION_label = new QLabel(centralWidget);
        Y_POSITION_lineEdit = new QLineEdit(centralWidget);                 ///// linedit_2

///////////////////// MOTOR SELECTION /////////////
/*
        QComboBox *comboBox_XMotor = new QComboBox(tab2_2); ////// X Motor Selection
        comboBox_XMotor->insertItems(0, QStringList()
        << QApplication::translate("mywidget", "-- select motor --", 0)
        << QApplication::translate("mywidget", "M404-2PD", 0)
        << QApplication::translate("mywidget", "M404-4PD", 0)
        << QApplication::translate("mywidget", "M404-8PD", 0)
        << QApplication::translate("mywidget", "VT80-100", 0)
        << QApplication::translate("mywidget", "VT80-150", 0)
        << QApplication::translate("mywidget", "VT80-200", 0)
        );

        QComboBox *comboBox_YMotor = new QComboBox(tab2_2); ////// Y Motor Selection
        comboBox_YMotor->insertItems(0, QStringList()
        << QApplication::translate("mywidget", "-- select motor --", 0)
        << QApplication::translate("mywidget", "M404-2PD", 0)
        << QApplication::translate("mywidget", "M404-4PD", 0)
        << QApplication::translate("mywidget", "M404-8PD", 0)
        << QApplication::translate("mywidget", "VT80-100", 0)
        << QApplication::translate("mywidget", "VT80-150", 0)
        << QApplication::translate("mywidget", "VT80-200", 0)
        );

        QComboBox *comboBox_ZMotor = new QComboBox(tab); ////// Z Motor Selection
        comboBox_ZMotor->insertItems(0, QStringList()
        << QApplication::translate("mywidget", "-- select motor --", 0)
        << QApplication::translate("mywidget", "M404-2PD", 0)
        << QApplication::translate("mywidget", "M404-4PD", 0)
        << QApplication::translate("mywidget", "M404-8PD", 0)
        << QApplication::translate("mywidget", "VT80-100", 0)
        << QApplication::translate("mywidget", "VT80-150", 0)
        << QApplication::translate("mywidget", "VT80-200", 0)
        );

*/

   
//   qDebug()<<"GuiMode: "<<Resolution_mode;

   switch (Resolution_mode)
   {
    case(0):
    {
///////////////////////////////// FRAMES



        FRAME_line1->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line1->setGeometry(QRect(1120, 518, 620, 392));
        FRAME_line1->setFrameShape(QFrame::StyledPanel);
        FRAME_line1->setFrameShadow(QFrame::Plain);

        FRAME_line2->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line2->setGeometry(QRect(1770, 518, 620, 392));
        FRAME_line2->setFrameShape(QFrame::StyledPanel);
        FRAME_line2->setFrameShadow(QFrame::Plain);

        FRAME_line3->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line3->setGeometry(QRect(1770, 766, 620, 144));
        FRAME_line3->setFrameShape(QFrame::StyledPanel);
        FRAME_line3->setFrameShadow(QFrame::Plain);

        FRAME_line4->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line4->setGeometry(QRect(20, 924, 2370, 90));
        FRAME_line4->setFrameShape(QFrame::StyledPanel);
        FRAME_line4->setFrameShadow(QFrame::Plain);

//////////////////////////// CREATI IN SEQUENZA ////////////////////////


        CAMERA_label->setObjectName(QString::fromUtf8("label_9"));
        CAMERA_label->setGeometry(QRect(40, 940, 120, 56));
        QFont font;
        font.setFamily(QString::fromUtf8("Cantarell"));
        font.setPointSize(12);
        CAMERA_label->setFont(font);
        CAMERA_label->setText(QApplication::translate("MainWindow", "Camera", 0));

        VLC_Button->setObjectName(QString::fromUtf8("pushButton_4"));
        VLC_Button->setGeometry(QRect(170, 940, 160, 56));
        VLC_Button->setText(QApplication::translate("MainWindow", "VLC", 0));

        LASER_label->setObjectName(QString::fromUtf8("label_11"));
        LASER_label->setGeometry(QRect(420, 940, 120, 56));
        LASER_label->setFont(font);
        LASER_label->setText(QApplication::translate("MainWindow", "Laser", 0));

        LASER_Button->setObjectName(QString::fromUtf8("pushButton_5"));
        LASER_Button->setGeometry(QRect(530, 940, 160, 56));
        LASER_Button->setText(QApplication::translate("MainWindow", "On/Off", 0));
/*
 *      Deprecated this entire section, as the controls are only relevant to helium control, and we're choosing a hardware based negative feedback control loop,
 *      as opposed to a software based negative feedback loop.
 *
        HELIUM_label->setObjectName(QString::fromUtf8("label_13"));
        HELIUM_label->setGeometry(QRect(780, 940, 120, 56));
        HELIUM_label->setFont(font);
        HELIUM_label->setText(QApplication::translate("MainWindow", "Helium", 0));

        HELIUM_Button->setObjectName(QString::fromUtf8("pushButton_6"));
        HELIUM_Button->setGeometry(QRect(920, 940, 160, 56));
        HELIUM_Button->setText(QApplication::translate("MainWindow", "On/Off", 0));

        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(1560, 936, 140, 64));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Cantarell"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        lcdNumber->setFont(font2);
        lcdNumber->setLineWidth(2);
        lcdNumber->setMidLineWidth(0);
        lcdNumber->setDigitCount(3);
        lcdNumber->setSegmentStyle(QLCDNumber::Filled);

        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(1320, 948, 220, 40));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
*/
///////////////////////////////////////////////// BOTTONI E LINE_EDIT A DX


        DAQ_label->setObjectName(QString::fromUtf8("label"));
        DAQ_label->setGeometry(QRect(870, 20, 282, 42));
        DAQ_label->setFont(font);
        DAQ_label->setText(QApplication::translate("MainWindow", "DAQ Settings", 0));
        
        groupBox->setGeometry(QRect(850, 70, 240, 120));
        radioUSB->setGeometry(QRect(30, 10, 200, 40));
        radioUSB->setText(QApplication::translate("MainWindow", " USB-link", 0));
        radioOptical->setGeometry(QRect(30, 60, 200, 40));
        radioOptical->setText(QApplication::translate("MainWindow", " Optical-link", 0));
        radioUSB->setChecked(true);

        Digitizer_Button->setObjectName(QString::fromUtf8("pushButton"));
        Digitizer_Button->setGeometry(QRect(850, 210, 242, 54));
        Digitizer_Button->setText(QApplication::translate("MainWindow", "Digitizer", 0));

        Detector_Button->setObjectName(QString::fromUtf8("pushButton_2"));
        Detector_Button->setGeometry(QRect(850, 280, 242, 54));
        Detector_Button->setText(QApplication::translate("MainWindow", "Detector", 0));

        SW_treshold_Button->setObjectName(QString::fromUtf8("pushButton_2"));
        SW_treshold_Button->setGeometry(QRect(850, 350, 242, 54));
        SW_treshold_Button->setText(QApplication::translate("MainWindow", "SW_threshold", 0));

        TTY_label->setObjectName(QString::fromUtf8("label_2"));
        TTY_label->setGeometry(QRect(900, 450, 222, 42));
        TTY_label->setFont(font);
        TTY_label->setText(QApplication::translate("MainWindow", "TTY status", 0));

        CurrentActionX->setObjectName(QString::fromUtf8("CurrentActionX"));
        CurrentActionX->setEnabled(false);
        CurrentActionX->setGeometry(QRect(850, 500, 242, 58));
        CurrentActionX->setText("X on TTYUSB0");

        CurrentActionY->setObjectName(QString::fromUtf8("CurrentActionY"));
        CurrentActionY->setEnabled(false);
        CurrentActionY->setGeometry(QRect(850, 570, 242, 58));
        CurrentActionY->setText("Y on TTYUSB1");

        CurrentActionZ->setObjectName(QString::fromUtf8("CurrentActionY_2"));
        CurrentActionZ->setEnabled(false);
        CurrentActionZ->setGeometry(QRect(850, 640, 242, 58));
        CurrentActionZ->setText("Z on TTYUSB2");

        Export_Button->setObjectName(QString::fromUtf8("pushButton_3"));
        Export_Button->setGeometry(QRect(850, 750, 242, 54));
        Export_Button->setText(QApplication::translate("MainWindow", "Export Map", 0));

/*        MapImage_label = new QLabel(centralWidget);
        MapImage_label->setObjectName(QString::fromUtf8("label_3"));
        MapImage_label->setGeometry(QRect(900, 680, 222, 42));
        MapImage_label->setFont(font);
        MapImage_label->setText(QApplication::translate("MainWindow", "Map Image", 0));
*/
        CurrentAction->setObjectName(QString::fromUtf8("CurrentActionY_3"));
        CurrentAction->setEnabled(false);
        CurrentAction->setGeometry(QRect(20, 850, 1070, 60));
	CurrentAction->setText("Ready");

/////////////////////////////////// LINES ///////////////////////////////////////


        line_1->setObjectName(QString::fromUtf8("line_6"));
        line_1->setGeometry(QRect(370, 930, 20, 76));
        line_1->setFrameShape(QFrame::VLine);
        line_1->setFrameShadow(QFrame::Sunken);
        line_2->setObjectName(QString::fromUtf8("line_7"));
        line_2->setGeometry(QRect(730, 930, 20, 76));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3->setObjectName(QString::fromUtf8("line_9"));
        line_3->setGeometry(QRect(880, 720, 202, 40));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4->setObjectName(QString::fromUtf8("line_10"));
        line_4->setGeometry(QRect(880, 420, 202, 40));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);


////////////////////////////////////// GAS LABEL
/*
        GAS_label->setObjectName(QString::fromUtf8("label_12"));
        GAS_label->setGeometry(QRect(1120, 940, 180, 56));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Cantarell"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        GAS_label->setFont(font1);
        GAS_label->setText(QApplication::translate("MainWindow", "Flux (0-100)%", 0));
*/

////////////////////////////// XRAY TABLE BUTTON /////////////////////////////////


        XRAY_TABLE_Button->setObjectName(QString::fromUtf8("pushButton_X"));
        XRAY_TABLE_Button->setText(QApplication::translate("MainWindow", "X-Ray Table", 0));
        XRAY_TABLE_Button->setGeometry(QRect(1800, 940, 560, 60));


///////////////// INHERITED FROM AUTOFOCUS



//     scaleFactor = 1.0;
//     scrollArea->setWidgetResizable(fitToWindow); 




///////////////////////// TABWIDGET AUTOFOCUS ////////////////////

        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(1120, 20, 620, 500));
//        tabWidget->setMaximumSize(QSize(16777215, 16777215));


///////////////////////////////// WIDGET 1 TAB1  

        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setEnabled(true);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////MODIFIED
        //TTY_Z_label->setObjectName(QString::fromUtf8("label_22"));
        //TTY_Z_label->setGeometry(QRect(20, 50, 250, 54));     // Y MOD (-10)
        //TTY_Z_label->setText(QApplication::translate("MyWidget", "/dev/ttyUSB", 0));

        spinBox_assignZ->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignZ->setGeometry(QRect(330, 50, 250, 54)); // Y MOD (-10)
        spinBox_assignZ->setMaximum(3);
        spinBox_assignZ->setValue(2);

        AssignZ_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        AssignZ_pushButton->setGeometry(QRect(20, 140, 250, 64)); // Y MOD (-20)
        AssignZ_pushButton->setText(QApplication::translate("MyWidget", "Assign port Z", 0));

        //linedit_Z_Assignement->setObjectName(QString::fromUtf8("lineEdit_4"));
        //linedit_Z_Assignement->setEnabled(false);
        //linedit_Z_Assignement->setGeometry(QRect(330, 140, 250, 64)); // Y MOD (-20)

        //line_6_tab->setObjectName(QString::fromUtf8("line_6"));
        //line_6_tab->setGeometry(QRect(10, 220, 600, 20)); // Y MOD (-20)
        //line_6_tab->setFrameShape(QFrame::HLine);
        //line_6_tab->setFrameShadow(QFrame::Sunken);

        //comboBox_ZMotor->setGeometry(QRect(20, 270, 250, 60));

        //label_6_tab->setObjectName(QString::fromUtf8("label_6"));
        //label_6_tab->setGeometry(QRect(420, 270, 134, 44)); // X MOD (+150)
        //label_6_tab->setFont(font);
        //label_6_tab->setText(QApplication::translate("MyWidget", "Z Init", 0));

        INIT_Z_pushButton->setObjectName(QString::fromUtf8("INIT_Z_pushButton"));
        INIT_Z_pushButton->setGeometry(QRect(20, 350, 250, 64)); // Y MOD (+20)
        INIT_Z_pushButton->setText(QApplication::translate("MyWidget", " INIT Z", 0));

        STOP_Z_INIT_pushButton->setObjectName(QString::fromUtf8("STOP_Z_INIT_pushButton"));
        STOP_Z_INIT_pushButton->setGeometry(QRect(330, 350, 250, 64)); // Y MOD (+20)
        STOP_Z_INIT_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));


        tabWidget->addTab(tab, QString());

///////////////////////////////// WIDGET 1 TAB2

        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setEnabled(false);

        MOVE_Z_MOTOR_label_1->setObjectName(QString::fromUtf8("MOVE_Z_MOTOR_label_1"));
        MOVE_Z_MOTOR_label_1->setGeometry(QRect(40, 20, 340, 40));
        MOVE_Z_MOTOR_label_1->setFont(font2);
        MOVE_Z_MOTOR_label_1->setText(QApplication::translate("MyWidget", "Move Z Motor", 0));

        MOVE_Z_To_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_To_pushButton"));
        MOVE_Z_To_pushButton->setGeometry(QRect(40, 80, 180, 54));
        MOVE_Z_To_pushButton->setText(QApplication::translate("MyWidget", "Move Z ->", 0));

        MOVE_Z_To_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_Z_To_doubleSpinBox"));
        MOVE_Z_To_doubleSpinBox->setGeometry(QRect(280, 80, 280, 54));
        MOVE_Z_To_doubleSpinBox->setDecimals(3);
        MOVE_Z_To_doubleSpinBox->setMaximum(50);
        MOVE_Z_To_doubleSpinBox->setSingleStep(0.001);
        MOVE_Z_To_doubleSpinBox->setValue(25);

        MOVE_Z_FORWARD_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_FORWARD_pushButton"));
        MOVE_Z_FORWARD_pushButton->setGeometry(QRect(40, 200, 180, 56));
        MOVE_Z_FORWARD_pushButton->setText(QApplication::translate("MyWidget", "Forward", 0));

        MOVE_Z_MOTOR_label_2->setObjectName(QString::fromUtf8("MOVE_Z_MOTOR_label_2"));
        MOVE_Z_MOTOR_label_2->setGeometry(QRect(260, 200, 120, 56));
        MOVE_Z_MOTOR_label_2->setFont(font);
        MOVE_Z_MOTOR_label_2->setText(QApplication::translate("MyWidget", "Move", 0));

        MOVE_Z_BACKWARD_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_BACKWARD_pushButton"));
        MOVE_Z_BACKWARD_pushButton->setGeometry(QRect(380, 200, 180, 56));
        MOVE_Z_BACKWARD_pushButton->setText(QApplication::translate("MyWidget", "Backward", 0));

        STOP_Z_MOVE_pushButton->setObjectName(QString::fromUtf8("STOP_Z_MOVE_pushButton"));
        STOP_Z_MOVE_pushButton->setGeometry(QRect(130, 320, 330, 56));
        STOP_Z_MOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));


///////////////////////////////// WIDGET 1 TAB3

        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab_3->setEnabled(true);

        //ACM_label->setObjectName(QString::fromUtf8("label_22"));
        //ACM_label->setGeometry(QRect(20, 60, 250, 54));
        //ACM_label->setText(QApplication::translate("MyWidget", "/dev/ttyACM", 0));

        spinBox_assignACM->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignACM->setGeometry(QRect(330, 60, 250, 54));
        spinBox_assignACM->setMaximum(3);

        AssignACM_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        AssignACM_pushButton->setGeometry(QRect(20, 160, 250, 64));
        AssignACM_pushButton->setText(QApplication::translate("MyWidget", "Assign ACM port", 0));

        CurrentActionACM->setObjectName(QString::fromUtf8("CurrentActionACM"));
        CurrentActionACM->setEnabled(false);
        CurrentActionACM->setGeometry(QRect(170, 290, 330, 58));
        CurrentActionACM->setText("Arduino on TTYACM0");

///////////////////////////////// WIDGET 1 TAB4

        //tab_4->setObjectName(QString::fromUtf8("tab_4"));
        //tab_4->setEnabled(false);

        AUTOFOCUS_ON_pushButton->setObjectName(QString::fromUtf8("AUTOFOCUS_ON_pushButton"));
        AUTOFOCUS_ON_pushButton->setGeometry(QRect(100, 60, 400, 60));
        AUTOFOCUS_ON_pushButton->setText(QApplication::translate("MyWidget", "Autofocus ON (ttyACM)", 0));

        //AUTOFOCUS_STATUS_checkBox->setObjectName(QString::fromUtf8("checkBox_3"));
        //AUTOFOCUS_STATUS_checkBox->setEnabled(false);
        //AUTOFOCUS_STATUS_checkBox->setGeometry(QRect(150, 140, 300, 50));
        //AUTOFOCUS_STATUS_checkBox->setCheckable(true);
        //AUTOFOCUS_STATUS_checkBox->setText(QApplication::translate("MyWidget", "Autofocus status", 0));

        label_23_tab_4->setObjectName(QString::fromUtf8("label_23"));
        label_23_tab_4->setGeometry(QRect(100, 240, 420, 50));
        label_23_tab_4->setText(QApplication::translate("MyWidget", "Distance from target (mm)", 0));

        lineEdit_2_tab_4->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2_tab_4->setEnabled(false);
        lineEdit_2_tab_4->setGeometry(QRect(120, 300, 360, 54));

        ENABLE_TRACKING_checkBox->setObjectName(QString::fromUtf8("ENABLE_TRACKING_checkBox"));
        ENABLE_TRACKING_checkBox->setGeometry(QRect(160, 360, 280, 50));
        ENABLE_TRACKING_checkBox->setText(QApplication::translate("MyWidget", "Enable tracking", 0));


        tabWidget->addTab(tab_2, QString());
        tabWidget->addTab(tab_3, QString());
        //tabWidget->addTab(tab_4, QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MyWidget", "TTY_Z", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MyWidget", "Move_Z", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MyWidget", "ACM", 0));
        //tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MyWidget", "Autofocus", 0));
        tabWidget->setCurrentIndex(0);

//////////////////////// zone below WIDGET 1 //// X da 640 

        //label_7_below_tab->setObjectName(QString::fromUtf8("label_7"));
        //label_7_below_tab->setGeometry(QRect(1160, 540, 240, 54));
        //label_7_below_tab->setFont(font);
        //label_7_below_tab->setText(QApplication::translate("MyWidget", "Z_Speed (mm/s)", 0));

        //Z_VELOCITY_doubleSpinBox->setObjectName(QString::fromUtf8("Z_VELOCITY_doubleSpinBox"));
        //Z_VELOCITY_doubleSpinBox->setGeometry(QRect(1420, 540, 300, 54));
        //Z_VELOCITY_doubleSpinBox->setDecimals(3);
        //Z_VELOCITY_doubleSpinBox->setMaximum(50);
        //Z_VELOCITY_doubleSpinBox->setSingleStep(0.001);
        //Z_VELOCITY_doubleSpinBox->setValue(10);

        //line_11->setObjectName(QString::fromUtf8("line_11"));
        //line_11->setGeometry(QRect(1130, 610, 600, 20));
        //line_11->setFrameShape(QFrame::HLine);
        //line_11->setFrameShadow(QFrame::Sunken);

        //label_8_below_tab->setObjectName(QString::fromUtf8("label_8"));
        //label_8_below_tab->setGeometry(QRect(1160, 650, 240, 54));
        //label_8_below_tab->setFont(font);
        //label_8_below_tab->setText(QApplication::translate("MyWidget", "Step Z (mm)", 0));

        //Z_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("Z_STEP_doubleSpinBox"));
        //Z_STEP_doubleSpinBox->setGeometry(QRect(1420, 650, 300, 54));
        //Z_STEP_doubleSpinBox->setDecimals(3);
        //Z_STEP_doubleSpinBox->setMaximum(50);
        //Z_STEP_doubleSpinBox->setSingleStep(0.001);
        //Z_STEP_doubleSpinBox->setValue(1);

        //line_14->setObjectName(QString::fromUtf8("line_14"));
        //line_14->setGeometry(QRect(1130, 720, 600, 20));
        //line_14->setFrameShape(QFrame::HLine);
        //line_14->setFrameShadow(QFrame::Sunken);

        label_17_below_tab->setObjectName(QString::fromUtf8("label_17"));
        label_17_below_tab->setGeometry(QRect(1160, 750, 240, 54));
        label_17_below_tab->setFont(font);
        label_17_below_tab->setText(QApplication::translate("MyWidget", "Z - position:", 0));

       // lineEdit_below_tab->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_below_tab->setEnabled(false);
        lineEdit_below_tab->setGeometry(QRect(1420, 750, 300, 54));

        //label_18_below_tab->setObjectName(QString::fromUtf8("label_18"));
        //label_18_below_tab->setGeometry(QRect(1160, 810, 520, 50));

        //label_18_below_tab->setFont(font7);
        //label_18_below_tab->setText(QApplication::translate("MyWidget", "Z-axis linear stage absolute position", 0));


///////////////////////// TABWIDGET X-Y WIDGET 2 ////////////////////

        //tabWidget2->setObjectName(QString::fromUtf8("tabWidget2"));
        //tabWidget2->setEnabled(true);
        //tabWidget2->setGeometry(QRect(1770, 20, 620, 500));
//        tabWidget->setMaximumSize(QSize(16777215, 16777215));

///////////////////////////////// WIDGET 2 TAB1 ASSIGN X & Y PORT

        //tab2_1->setObjectName(QString::fromUtf8("tab2_1"));
        //tab2_1->setEnabled(true);

        //label_tab_2_bX->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_bX->setGeometry(QRect(230, 40, 340, 40));
        //label_tab_2_bX->setFont(font);
        //label_tab_2_bX->setText(QApplication::translate("MyWidget", "X Motor", 0));

        spinBox_assignX->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignX->setGeometry(QRect(330, 120, 250, 54));
        spinBox_assignX->setMaximum(3);

        pushButton_assignX_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignX_port->setGeometry(QRect(40, 120, 240, 54));
        pushButton_assignX_port->setText(QApplication::translate("MyWidget", "Assign port X", 0));

        //label_tab_2_bY->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_bY->setGeometry(QRect(230, 240, 340, 40));
        //label_tab_2_bY->setFont(font);
        //label_tab_2_bY->setText(QApplication::translate("MyWidget", "Y Motor", 0));

        spinBox_assignY->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignY->setGeometry(QRect(330, 320, 250, 54));
        spinBox_assignY->setMaximum(3);
        spinBox_assignY->setValue(1);

        pushButton_assignY_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignY_port->setGeometry(QRect(40, 320, 240, 54));
        pushButton_assignY_port->setText(QApplication::translate("MyWidget", "Assign port Y", 0));


///////////////////////////////////////WIDGET 2 TAB 2///////////INIT////////////////////////////////

        //tab2_2->setObjectName(QString::fromUtf8("tab2_2"));
        //tab2_2->setEnabled(false);

        //label_tab_2_2->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_2->setGeometry(QRect(170, 40, 340, 40));
        //label_tab_2_2->setFont(font);
        //label_tab_2_2->setText(QApplication::translate("MyWidget", "Motor initialisation", 0));

        //label_tab_2_2X->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_2X->setGeometry(QRect(100, 100, 340, 40)); // X moved (-30)
        //label_tab_2_2X->setFont(font);
        //label_tab_2_2X->setText(QApplication::translate("MyWidget", "X motor", 0));

        //label_tab_2_2Y->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_2Y->setGeometry(QRect(400, 100, 340, 40)); // X moved (30)
        //label_tab_2_2Y->setFont(font);
        //label_tab_2_2Y->setText(QApplication::translate("MyWidget", "Y motor", 0));

        //comboBox_XMotor->setGeometry(QRect(30, 170, 250, 60));
        //comboBox_YMotor->setGeometry(QRect(340, 170, 250, 60));

        pushButton_tab_2_2X->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2X->setGeometry(QRect(70, 270, 180, 54)); // XY moved (-30, +130)
        pushButton_tab_2_2X->setText(QApplication::translate("MyWidget", "Init X", 0));

        pushButton_tab_2_2Y->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2Y->setGeometry(QRect(370, 270, 180, 54)); // XY moved (+30, +130)
        pushButton_tab_2_2Y->setText(QApplication::translate("MyWidget", "Init Y", 0));

//        pushButton_tab_2_2XY->setObjectName(QString::fromUtf8("pushButton_8"));
//        pushButton_tab_2_2XY->setGeometry(QRect(190, 240, 260, 54));
//        pushButton_tab_2_2XY->setText(QApplication::translate("MyWidget", "Init X+Y", 0));

        pushButton_tab_2_2ABORT->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2ABORT->setGeometry(QRect(150, 360, 340, 54)); // Y moved (+20)
        pushButton_tab_2_2ABORT->setText(QApplication::translate("MyWidget", "STOP", 0));

///////////////////////////////// WIDGET 2 TAB3 (MOVE XY)

        tab2_3->setObjectName(QString::fromUtf8("tab2_3"));
        tab2_3->setEnabled(false);

        MOVE_label->setObjectName(QString::fromUtf8("label_22"));
        MOVE_label->setGeometry(QRect(130, 100, 100, 54));
        MOVE_label->setFont(font);
        MOVE_label->setText(QApplication::translate("MyWidget", "Move", 0));

        MOVEUP_pushButton->setObjectName(QString::fromUtf8("MOVEUP_pushButton"));
        MOVEUP_pushButton->setGeometry(QRect(120, 40, 100, 50));
        MOVEUP_pushButton->setText(QApplication::translate("MyWidget", "UP", 0));

        MOVELEFT_pushButton->setObjectName(QString::fromUtf8("MOVELEFT_pushButton"));
        MOVELEFT_pushButton->setGeometry(QRect(20, 100, 100, 50));
        MOVELEFT_pushButton->setText(QApplication::translate("MyWidget", "LEFT", 0));

        MOVERIGHT_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        MOVERIGHT_pushButton->setGeometry(QRect(220, 100, 100, 50));
        MOVERIGHT_pushButton->setText(QApplication::translate("MyWidget", "RIGHT", 0));

        MOVEDOWN_pushButton->setObjectName(QString::fromUtf8("MOVEDOWN_pushButton"));
        MOVEDOWN_pushButton->setGeometry(QRect(120, 160, 100, 50));
        MOVEDOWN_pushButton->setText(QApplication::translate("MyWidget", "DOWN", 0));

        STOPMOVE_pushButton->setObjectName(QString::fromUtf8("STOPMOVE_pushButton"));
        STOPMOVE_pushButton->setGeometry(QRect(400, 70, 160, 110));
        STOPMOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));

        MOVE_TO_X_pushButton->setObjectName(QString::fromUtf8("MOVE_TO_X_pushButton"));
        MOVE_TO_X_pushButton->setGeometry(QRect(40, 270, 180, 54));
        MOVE_TO_X_pushButton->setText(QApplication::translate("MyWidget", "Move X to", 0));

        MOVE_TO_X_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_X_doubleSpinBox"));
        MOVE_TO_X_doubleSpinBox->setGeometry(QRect(260, 270, 300, 54));
        MOVE_TO_X_doubleSpinBox->setDecimals(3);
        MOVE_TO_X_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        MOVE_TO_X_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_X_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

        MOVE_TO_Y_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        MOVE_TO_Y_pushButton->setGeometry(QRect(40, 350, 180, 50));
        MOVE_TO_Y_pushButton->setText(QApplication::translate("MyWidget", "Move to Y", 0));

        MOVE_TO_Y_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_Y_doubleSpinBox"));
        MOVE_TO_Y_doubleSpinBox->setGeometry(QRect(260, 350, 300, 54));
        MOVE_TO_Y_doubleSpinBox->setDecimals(3);
        MOVE_TO_Y_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 50; 14/07/2016
        MOVE_TO_Y_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_Y_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

///////////////////////////////// WIDGET 2 TAB4 (SCAN)

        tab2_4->setObjectName(QString::fromUtf8("tab2_4"));
        tab2_4->setEnabled(false);

        SCAN_X_label->setObjectName(QString::fromUtf8("SCAN_X_label"));
        SCAN_X_label->setGeometry(QRect(80, 10, 600, 54));
//        SCAN_X_label->setFont(font);
        SCAN_X_label->setText(QApplication::translate("MyWidget", "X min (mm)                  X max. (mm)", 0));

        SET_SCAN_X_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MIN"));
        SET_SCAN_X_MIN_doubleSpinBox->setGeometry(QRect(20, 60, 260, 54));
        SET_SCAN_X_MIN_doubleSpinBox->setDecimals(3);
        SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200); //cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_X_MIN_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_X_MIN_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

        SET_SCAN_X_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MAX"));
        SET_SCAN_X_MAX_doubleSpinBox->setGeometry(QRect(320, 60, 260, 54));
        SET_SCAN_X_MAX_doubleSpinBox->setDecimals(3);
        SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200); //cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_X_MAX_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_X_MAX_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

///////// Y

        SCAN_Y_label->setObjectName(QString::fromUtf8("label_22"));
        SCAN_Y_label->setGeometry(QRect(80, 140, 600, 54));
        SCAN_Y_label->setText(QApplication::translate("MyWidget", "Y min (mm)                  Y max. (mm)", 0));

        SET_SCAN_Y_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MIN"));
        SET_SCAN_Y_MIN_doubleSpinBox->setGeometry(QRect(20, 190, 260, 54));
        SET_SCAN_Y_MIN_doubleSpinBox->setDecimals(3);
        SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_Y_MIN_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_Y_MIN_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

        SET_SCAN_Y_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MAX"));
        SET_SCAN_Y_MAX_doubleSpinBox->setGeometry(QRect(320, 190, 260, 54));
        SET_SCAN_Y_MAX_doubleSpinBox->setDecimals(3);
        SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_Y_MAX_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_Y_MAX_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

        SCANXY_pushButton->setObjectName(QString::fromUtf8("SCANXY_pushButton"));
        SCANXY_pushButton->setGeometry(QRect(30, 260, 240, 54));
        SCANXY_pushButton->setText(QApplication::translate("MyWidget", "SCAN XY", 0));

        SCANYX_pushButton->setObjectName(QString::fromUtf8("SCANYX_pushButton"));
        SCANYX_pushButton->setGeometry(QRect(330, 260, 240, 54));
        SCANYX_pushButton->setText(QApplication::translate("MyWidget", "SCAN YX", 0));

        SCAN_ABORT_pushButton->setObjectName(QString::fromUtf8("SCAN_ABORT_pushButton"));
        SCAN_ABORT_pushButton->setGeometry(QRect(160, 340, 280, 54));
        SCAN_ABORT_pushButton->setText(QApplication::translate("MyWidget", "ABORT SCAN", 0));


///////////////////////// AREA BELOW TABWIDGET2

        XY_SPEED_label->setObjectName(QString::fromUtf8("XY_SPEED_label"));
        XY_SPEED_label->setGeometry(QRect(1800, 540, 240, 54));
        XY_SPEED_label->setFont(font);
        XY_SPEED_label->setText(QApplication::translate("MyWidget", "XY_Speed (mm/s)", 0));

        XY_SPEED_doubleSpinBox->setObjectName(QString::fromUtf8("XY_SPEED_doubleSpinBox"));
        XY_SPEED_doubleSpinBox->setGeometry(QRect(2050, 540, 300, 54));
        XY_SPEED_doubleSpinBox->setDecimals(3);
        XY_SPEED_doubleSpinBox->setMaximum(50);
        XY_SPEED_doubleSpinBox->setSingleStep(0.001);
        XY_SPEED_doubleSpinBox->setValue(5);

        XY_SPEED_line->setObjectName(QString::fromUtf8("XY_SPEED_line"));
        XY_SPEED_line->setGeometry(QRect(1780, 594, 600, 20));
        XY_SPEED_line->setFrameShape(QFrame::HLine);
        XY_SPEED_line->setFrameShadow(QFrame::Sunken);

        X_STEP_label->setObjectName(QString::fromUtf8("X_STEP_label"));
        X_STEP_label->setGeometry(QRect(1800, 620, 240, 54));
        X_STEP_label->setFont(font);
        X_STEP_label->setText(QApplication::translate("MyWidget", "Step X (mm)", 0));

        X_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("X_STEP_doubleSpinBox"));
        X_STEP_doubleSpinBox->setGeometry(QRect(2050, 620, 300, 54));
        X_STEP_doubleSpinBox->setDecimals(3);
        X_STEP_doubleSpinBox->setMaximum(50);
        X_STEP_doubleSpinBox->setSingleStep(0.001);
        X_STEP_doubleSpinBox->setValue(1);

        XY_STEP_line->setObjectName(QString::fromUtf8("XY_STEP_line"));
        XY_STEP_line->setGeometry(QRect(1780, 676, 600, 20));
        XY_STEP_line->setFrameShape(QFrame::HLine);
        XY_STEP_line->setFrameShadow(QFrame::Sunken);

        Y_STEP_label->setObjectName(QString::fromUtf8("Y_STEP_label"));
        Y_STEP_label->setGeometry(QRect(1800, 700, 240, 54));
        Y_STEP_label->setFont(font);
        Y_STEP_label->setText(QApplication::translate("MyWidget", "Step Y (mm)", 0));

        Y_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("Y_STEP_doubleSpinBox"));
        Y_STEP_doubleSpinBox->setGeometry(QRect(2050, 700, 300, 54));
        Y_STEP_doubleSpinBox->setDecimals(3);
        Y_STEP_doubleSpinBox->setMaximum(50);
        Y_STEP_doubleSpinBox->setSingleStep(0.001);
        Y_STEP_doubleSpinBox->setValue(1);

        X_POSITION_label->setObjectName(QString::fromUtf8("X_POSITION_label"));
        X_POSITION_label->setGeometry(QRect(1800, 780, 240, 54));
        X_POSITION_label->setFont(font);
        X_POSITION_label->setText(QApplication::translate("MyWidget", "X - position:", 0));

        X_POSITION_lineEdit->setObjectName(QString::fromUtf8("X_POSITION_lineEdit"));
        X_POSITION_lineEdit->setEnabled(false);
        X_POSITION_lineEdit->setGeometry(QRect(2050, 780, 300, 54));

        Y_POSITION_label->setObjectName(QString::fromUtf8("Y_POSITION_label"));
        Y_POSITION_label->setGeometry(QRect(1800, 840, 240, 54));
        Y_POSITION_label->setFont(font);
        Y_POSITION_label->setText(QApplication::translate("MyWidget", "Y - position:", 0));

        Y_POSITION_lineEdit->setObjectName(QString::fromUtf8("Y_POSITION_lineEdit"));
        Y_POSITION_lineEdit->setEnabled(false);
        Y_POSITION_lineEdit->setGeometry(QRect(2050, 840, 300, 54));

     break;
     }

    case(1):
     {

        FRAME_line1->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line1->setGeometry(QRect(840, 389, 465, 294));
        FRAME_line1->setFrameShape(QFrame::StyledPanel);
        FRAME_line1->setFrameShadow(QFrame::Plain);

        FRAME_line2->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line2->setGeometry(QRect(1328, 389, 465, 294));
        FRAME_line2->setFrameShape(QFrame::StyledPanel);
        FRAME_line2->setFrameShadow(QFrame::Plain);

        FRAME_line3->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line3->setGeometry(QRect(1328, 575, 465, 108));
        FRAME_line3->setFrameShape(QFrame::StyledPanel);
        FRAME_line3->setFrameShadow(QFrame::Plain);

        FRAME_line4->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line4->setGeometry(QRect(15, 693, 1778, 68));
        FRAME_line4->setFrameShape(QFrame::StyledPanel);
        FRAME_line4->setFrameShadow(QFrame::Plain);





//////////////////////////// CREATI IN SEQUENZA ////////////////////////


        CAMERA_label->setObjectName(QString::fromUtf8("label_9"));
        CAMERA_label->setGeometry(QRect(15, 705, 90, 42));
        QFont font;
        font.setFamily(QString::fromUtf8("Cantarell"));
        font.setPointSize(12);
        CAMERA_label->setFont(font);
        CAMERA_label->setText(QApplication::translate("MainWindow", "Camera", 0));

        VLC_Button->setObjectName(QString::fromUtf8("pushButton_4"));
        VLC_Button->setGeometry(QRect(113, 705, 120, 42));
        VLC_Button->setText(QApplication::translate("MainWindow", "VLC", 0));

        LASER_label->setObjectName(QString::fromUtf8("label_11"));
        LASER_label->setGeometry(QRect(300, 705, 90, 42));
        LASER_label->setFont(font);
        LASER_label->setText(QApplication::translate("MainWindow", "Laser", 0));

        LASER_Button->setObjectName(QString::fromUtf8("pushButton_5"));
        LASER_Button->setGeometry(QRect(383, 705, 120, 42));
        LASER_Button->setText(QApplication::translate("MainWindow", "On/Off", 0));
/*
        HELIUM_label->setObjectName(QString::fromUtf8("label_13"));
        HELIUM_label->setGeometry(QRect(585, 705, 90, 42));
        HELIUM_label->setFont(font);
        HELIUM_label->setText(QApplication::translate("MainWindow", "Helium", 0));

        HELIUM_Button->setObjectName(QString::fromUtf8("pushButton_6"));
        HELIUM_Button->setGeometry(QRect(690, 705, 120, 42));
        HELIUM_Button->setText(QApplication::translate("MainWindow", "On/Off", 0));

        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(1170, 702, 105, 48));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Cantarell"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        lcdNumber->setFont(font2);
        lcdNumber->setLineWidth(2);
        lcdNumber->setMidLineWidth(0);
        lcdNumber->setDigitCount(3);
        lcdNumber->setSegmentStyle(QLCDNumber::Filled);

        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(990, 711, 165, 30));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
*/

///////////////////////////////////////////////// BOTTONI E LINE_EDIT A DX


        DAQ_label->setObjectName(QString::fromUtf8("label"));
        DAQ_label->setGeometry(QRect(652, 15, 211, 31));
        DAQ_label->setFont(font);
        DAQ_label->setText(QApplication::translate("MainWindow", "DAQ Settings", 0));

        //QGroupBox *groupBox = new QGroupBox(centralWidget); //QGroupBox(tr("DAQ Type"));
        groupBox->setGeometry(QRect(637, 52, 180, 90));
        //QRadioButton *radioUSB = new QRadioButton(groupBox);
        radioUSB->setGeometry(QRect(15, 7, 150, 30));
        radioUSB->setText(QApplication::translate("MainWindow", " USB-link", 0));
       // QRadioButton *radioOptical = new QRadioButton(groupBox);
        radioOptical->setGeometry(QRect(15, 45, 150, 30));
        radioOptical->setText(QApplication::translate("MainWindow", " Optical-link", 0));
        radioUSB->setChecked(true);

        Digitizer_Button->setObjectName(QString::fromUtf8("pushButton"));
        Digitizer_Button->setGeometry(QRect(637, 157, 182, 41));
        Digitizer_Button->setText(QApplication::translate("MainWindow", "Digitizer", 0));

        Detector_Button->setObjectName(QString::fromUtf8("pushButton_2"));
        Detector_Button->setGeometry(QRect(637, 210, 182, 41));
        Detector_Button->setText(QApplication::translate("MainWindow", "Detector", 0));

        SW_treshold_Button->setObjectName(QString::fromUtf8("pushButton_2"));
        SW_treshold_Button->setGeometry(QRect(637, 262, 182, 41));
        SW_treshold_Button->setText(QApplication::translate("MainWindow", "SW_threshold", 0));

        TTY_label->setObjectName(QString::fromUtf8("label_2"));
        TTY_label->setGeometry(QRect(675, 337, 167, 32));
        TTY_label->setFont(font);
        TTY_label->setText(QApplication::translate("MainWindow", "Ports Status", 0));

        CurrentActionX->setObjectName(QString::fromUtf8("CurrentActionX"));
        CurrentActionX->setEnabled(false);
        CurrentActionX->setGeometry(QRect(637, 375, 182, 30));
        CurrentActionX->setText("Motor X Disconnected");

        CurrentActionY->setObjectName(QString::fromUtf8("CurrentActionY"));
        CurrentActionY->setEnabled(false);
        CurrentActionY->setGeometry(QRect(637, 415, 182, 30));
        CurrentActionY->setText("Motor Y Disconnected");

        CurrentActionZ->setObjectName(QString::fromUtf8("CurrentActionY_2"));
        CurrentActionZ->setEnabled(false);
        CurrentActionZ->setGeometry(QRect(637, 455, 182, 30));
        CurrentActionZ->setText("Motor Z Disconnected");

        CurrentActionACM->setObjectName(QString::fromUtf8("CurrentActionACM"));
        CurrentActionACM->setEnabled(false);
        CurrentActionACM->setGeometry(QRect(637, 495, 182, 30));
        CurrentActionACM->setText("Arduino Disconnected");

        Export_Button->setObjectName(QString::fromUtf8("pushButton_3"));
        Export_Button->setGeometry(QRect(637, 562, 182, 41));
        Export_Button->setText(QApplication::translate("MainWindow", "Export Map", 0));

        CurrentAction->setObjectName(QString::fromUtf8("CurrentActionY_3"));
        CurrentAction->setEnabled(false);
        CurrentAction->setGeometry(QRect(15, 638, 803, 45));
        CurrentAction->setText("Ready");

/////////////////////////////////// LINES ///////////////////////////////////////

        line_1->setObjectName(QString::fromUtf8("line_6"));
        line_1->setGeometry(QRect(278, 698, 15, 57));
        line_1->setFrameShape(QFrame::VLine);
        line_1->setFrameShadow(QFrame::Sunken);
        line_2->setObjectName(QString::fromUtf8("line_7"));
        line_2->setGeometry(QRect(548, 698, 10, 57));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3->setObjectName(QString::fromUtf8("line_9"));
        line_3->setGeometry(QRect(660, 540, 152, 30));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4->setObjectName(QString::fromUtf8("line_10"));
        line_4->setGeometry(QRect(660, 305, 152, 30));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

/*
////////////////////////////////////// GAS LABEL

        GAS_label->setObjectName(QString::fromUtf8("label_12"));
        GAS_label->setGeometry(QRect(840, 705, 135, 42));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Cantarell"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        GAS_label->setFont(font1);
        GAS_label->setText(QApplication::translate("MainWindow", "Flux (0-100)%", 0));
*/


////////////////////////////// XRAY TABLE BUTTON /////////////////////////////////


        XRAY_TABLE_Button->setObjectName(QString::fromUtf8("pushButton_X"));
        XRAY_TABLE_Button->setText(QApplication::translate("MainWindow", "X-Ray Table", 0));
        XRAY_TABLE_Button->setGeometry(QRect(585, 705, 233, 42));




///////////////// INHERITED FROM AUTOFOCUS



//     scaleFactor = 1.0;
//     scrollArea->setWidgetResizable(fitToWindow); 







///////////////////////// TABWIDGET AUTOFOCUS ////////////////////

        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(840, 15, 465, 375));
//        tabWidget->setMaximumSize(QSize(1677732c5, 1677732c5));


///////////////////////////////// WIDGET 1 TAB1  

        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setEnabled(true);

        //TTY_Z_label->setObjectName(QString::fromUtf8("label_22"));
        //TTY_Z_label->setGeometry(QRect(15, 45, 188, 41));
        //TTY_Z_label->setText(QApplication::translate("MyWidget", "/dev/ttyUSB", 0));

        pushButton_assignX_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignX_port->setGeometry(QRect(15, 85, 180, 31));
        pushButton_assignX_port->setText(QApplication::translate("MyWidget", "Assign port X", 0));

        spinBox_assignX->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignX->setGeometry(QRect(222, 85, 100, 31));
        spinBox_assignX->setMaximum(3);
        spinBox_assignX->setValue(2);

        pushButton_tab_2_2X->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2X->setGeometry(QRect(360, 85, 80, 31));
        pushButton_tab_2_2X->setText(QApplication::translate("MyWidget", "Init X", 0));

        pushButton_assignY_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignY_port->setGeometry(QRect(15, 135, 180, 31));
        pushButton_assignY_port->setText(QApplication::translate("MyWidget", "Assign port Y", 0));

        spinBox_assignY->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignY->setGeometry(QRect(222, 135, 100, 31));
        spinBox_assignY->setMaximum(3);
        spinBox_assignY->setValue(0);

        pushButton_tab_2_2Y->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2Y->setGeometry(QRect(360, 135, 80, 31));
        pushButton_tab_2_2Y->setText(QApplication::translate("MyWidget", "Init Y", 0));

        AssignZ_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        AssignZ_pushButton->setGeometry(QRect(15, 35, 180, 31));
        AssignZ_pushButton->setText(QApplication::translate("MyWidget", "Assign port Z", 0));

        spinBox_assignZ->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignZ->setGeometry(QRect(222, 35, 100, 31));
        spinBox_assignZ->setMaximum(3);
        spinBox_assignZ->setValue(1);

        INIT_Z_pushButton->setObjectName(QString::fromUtf8("INIT_Z_pushButton"));
        INIT_Z_pushButton->setGeometry(QRect(360, 35, 80, 31));
        INIT_Z_pushButton->setText(QApplication::translate("MyWidget", " Init Z", 0));

        STOP_Z_INIT_pushButton->setObjectName(QString::fromUtf8("STOP_Z_INIT_pushButton"));
        STOP_Z_INIT_pushButton->setGeometry(QRect(25, 248, 150, 50));
        STOP_Z_INIT_pushButton->setText(QApplication::translate("MyWidget", "ABORT Z", 0));

        pushButton_tab_2_2ABORT->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2ABORT->setGeometry(QRect(290, 248, 150, 50));
        pushButton_tab_2_2ABORT->setText(QApplication::translate("MyWidget", "ABORT X OR Y", 0));


        tabWidget->addTab(tab, QString());

///////////////////////////////// WIDGET 1 TAB2

        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setEnabled(false);

        MOVE_Z_MOTOR_label_1->setObjectName(QString::fromUtf8("MOVE_Z_MOTOR_label_1"));
        MOVE_Z_MOTOR_label_1->setGeometry(QRect(30, 15, 255, 30));
        MOVE_Z_MOTOR_label_1->setFont(font2);
        MOVE_Z_MOTOR_label_1->setText(QApplication::translate("MyWidget", "Move Z Motor", 0));

        MOVE_Z_To_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_To_pushButton"));
        MOVE_Z_To_pushButton->setGeometry(QRect(30, 60, 135, 41));
        MOVE_Z_To_pushButton->setText(QApplication::translate("MyWidget", "Move Z ->", 0));

        MOVE_Z_To_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_Z_To_doubleSpinBox"));
        MOVE_Z_To_doubleSpinBox->setGeometry(QRect(210, 60, 210, 41));
        MOVE_Z_To_doubleSpinBox->setDecimals(3);
        MOVE_Z_To_doubleSpinBox->setMaximum(50);
        MOVE_Z_To_doubleSpinBox->setSingleStep(0.001);
        MOVE_Z_To_doubleSpinBox->setValue(25);

        MOVE_Z_FORWARD_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_FORWARD_pushButton"));
        MOVE_Z_FORWARD_pushButton->setGeometry(QRect(30, 150, 135, 42));
        MOVE_Z_FORWARD_pushButton->setText(QApplication::translate("MyWidget", "Forward", 0));

        MOVE_Z_MOTOR_label_2->setObjectName(QString::fromUtf8("MOVE_Z_MOTOR_label_2"));
        MOVE_Z_MOTOR_label_2->setGeometry(QRect(195, 150, 90, 42));
        MOVE_Z_MOTOR_label_2->setFont(font);
        MOVE_Z_MOTOR_label_2->setText(QApplication::translate("MyWidget", "Move", 0));

        MOVE_Z_BACKWARD_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_BACKWARD_pushButton"));
        MOVE_Z_BACKWARD_pushButton->setGeometry(QRect(285, 150, 135, 42));
        MOVE_Z_BACKWARD_pushButton->setText(QApplication::translate("MyWidget", "Backward", 0));

        STOP_Z_MOVE_pushButton->setObjectName(QString::fromUtf8("STOP_Z_MOVE_pushButton"));
        STOP_Z_MOVE_pushButton->setGeometry(QRect(98, 240, 248, 42));
        STOP_Z_MOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));


///////////////////////////////// WIDGET 1 TAB3

        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab_3->setEnabled(true);

        //ACM_label->setObjectName(QString::fromUtf8("label_22"));
        //ACM_label->setGeometry(QRect(15, 45, 194, 41));
        //ACM_label->setText(QApplication::translate("MyWidget", "/dev/ttyACM", 0));

        spinBox_assignACM->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignACM->setGeometry(QRect(248, 45, 194, 40));
        spinBox_assignACM->setMaximum(3);

        AssignACM_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        AssignACM_pushButton->setGeometry(QRect(15, 45, 194, 40));
        AssignACM_pushButton->setText(QApplication::translate("MyWidget", "Assign ACM port", 0));



///////////////////////////////// WIDGET 1 TAB4

        //tab_4->setObjectName(QString::fromUtf8("tab_4"));
        //tab_4->setEnabled(false);

        AUTOFOCUS_ON_pushButton->setObjectName(QString::fromUtf8("AUTOFOCUS_ON_pushButton"));
        AUTOFOCUS_ON_pushButton->setGeometry(QRect(248, 100, 194, 40));
        AUTOFOCUS_ON_pushButton->setText(QApplication::translate("MyWidget", "Autofocus ON (ttyACM)", 0));
        AUTOFOCUS_ON_pushButton->setEnabled(false);

        //AUTOFOCUS_STATUS_checkBox->setObjectName(QString::fromUtf8("checkBox_3"));
        //AUTOFOCUS_STATUS_checkBox->setEnabled(false);
        //AUTOFOCUS_STATUS_checkBox->setGeometry(QRect(113, 105, 225, 38));
        //AUTOFOCUS_STATUS_checkBox->setCheckable(true);
        //AUTOFOCUS_STATUS_checkBox->setText(QApplication::translate("MyWidget", "Autofocus status", 0));

        label_23_tab_4->setObjectName(QString::fromUtf8("label_23"));
        label_23_tab_4->setGeometry(QRect(15, 180, 315, 40));
        label_23_tab_4->setText(QApplication::translate("MyWidget", "Distance from target (mm)", 0));

        lineEdit_2_tab_4->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2_tab_4->setEnabled(false);
        lineEdit_2_tab_4->setGeometry(QRect(15, 225, 270, 40));

        ENABLE_TRACKING_checkBox->setObjectName(QString::fromUtf8("ENABLE_TRACKING_checkBox"));
        ENABLE_TRACKING_checkBox->setGeometry(QRect(15, 270, 210, 40));
        ENABLE_TRACKING_checkBox->setText(QApplication::translate("MyWidget", "Enable tracking", 0));
        ENABLE_TRACKING_checkBox->setEnabled(false);

        tabWidget->addTab(tab_2, QString());
        tabWidget->addTab(tab_3, QString());
        //tabWidget->addTab(tab_4, QString());
        tabWidget->addTab(tab2_3, QString());
        tabWidget->addTab(tab2_4, QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MyWidget", "TTY_Z", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MyWidget", "Move_Z", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MyWidget", "ACM", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab2_3), QApplication::translate("MyWidget", "Move XY", 0));
        //tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MyWidget", "Autofocus", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab2_4), QApplication::translate("MyWidget", "Scan", 0));
        tabWidget->setCurrentIndex(0);

//////////////////////// zone below WIDGET 1 //// X da 640 

        XY_SPEED_label->setObjectName(QString::fromUtf8("XY_SPEED_label"));
        XY_SPEED_label->setGeometry(QRect(865, 405, 180, 41));
        XY_SPEED_label->setFont(font);
        XY_SPEED_label->setText(QApplication::translate("MyWidget", "XY_Speed (mm/s)", 0));

        XY_SPEED_doubleSpinBox->setObjectName(QString::fromUtf8("XY_SPEED_doubleSpinBox"));
        XY_SPEED_doubleSpinBox->setGeometry(QRect(1053, 405, 225, 41));
        XY_SPEED_doubleSpinBox->setDecimals(3);
        XY_SPEED_doubleSpinBox->setMaximum(50);
        XY_SPEED_doubleSpinBox->setSingleStep(0.001);
        XY_SPEED_doubleSpinBox->setValue(1);

        XY_SPEED_line->setObjectName(QString::fromUtf8("XY_SPEED_line"));
        XY_SPEED_line->setGeometry(QRect(850, 446, 450, 15));
        XY_SPEED_line->setFrameShape(QFrame::HLine);
        XY_SPEED_line->setFrameShadow(QFrame::Sunken);

        X_STEP_label->setObjectName(QString::fromUtf8("X_STEP_label"));
        X_STEP_label->setGeometry(QRect(865, 465, 180, 41));
        X_STEP_label->setFont(font);
        X_STEP_label->setText(QApplication::translate("MyWidget", "Step X (mm)", 0));

        X_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("X_STEP_doubleSpinBox"));
        X_STEP_doubleSpinBox->setGeometry(QRect(1053, 465, 225, 41));
        X_STEP_doubleSpinBox->setDecimals(3);
        X_STEP_doubleSpinBox->setMaximum(50);
        X_STEP_doubleSpinBox->setSingleStep(0.001);
        X_STEP_doubleSpinBox->setValue(1);

        XY_STEP_line->setObjectName(QString::fromUtf8("XY_STEP_line"));
        XY_STEP_line->setGeometry(QRect(850, 507, 450, 15));
        XY_STEP_line->setFrameShape(QFrame::HLine);
        XY_STEP_line->setFrameShadow(QFrame::Sunken);

        Y_STEP_label->setObjectName(QString::fromUtf8("Y_STEP_label"));
        Y_STEP_label->setGeometry(QRect(865, 525, 180, 41));
        Y_STEP_label->setFont(font);
        Y_STEP_label->setText(QApplication::translate("MyWidget", "Step Y (mm)", 0));

        Y_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("Y_STEP_doubleSpinBox"));
        Y_STEP_doubleSpinBox->setGeometry(QRect(1053, 525, 225, 41));
        Y_STEP_doubleSpinBox->setDecimals(3);
        Y_STEP_doubleSpinBox->setMaximum(50);
        Y_STEP_doubleSpinBox->setSingleStep(0.001);
        Y_STEP_doubleSpinBox->setValue(1);

        X_POSITION_label->setObjectName(QString::fromUtf8("X_POSITION_label"));
        X_POSITION_label->setGeometry(QRect(865, 585, 180, 41));
        X_POSITION_label->setFont(font);
        X_POSITION_label->setText(QApplication::translate("MyWidget", "X - position:", 0));

        X_POSITION_lineEdit->setObjectName(QString::fromUtf8("X_POSITION_lineEdit"));
        X_POSITION_lineEdit->setEnabled(false);
        X_POSITION_lineEdit->setGeometry(QRect(1053, 585, 225, 41));

        Y_POSITION_label->setObjectName(QString::fromUtf8("Y_POSITION_label"));
        Y_POSITION_label->setGeometry(QRect(865, 630, 180, 41));
        Y_POSITION_label->setFont(font);
        Y_POSITION_label->setText(QApplication::translate("MyWidget", "Y - position:", 0));

        Y_POSITION_lineEdit->setObjectName(QString::fromUtf8("Y_POSITION_lineEdit"));
        Y_POSITION_lineEdit->setEnabled(false);
        Y_POSITION_lineEdit->setGeometry(QRect(1053, 630, 225, 41));

        label_17_below_tab->setObjectName(QString::fromUtf8("label_17"));
        label_17_below_tab->setGeometry(QRect(865, 675, 180, 41));
        label_17_below_tab->setFont(font);
        label_17_below_tab->setText(QApplication::translate("MyWidget", "Z - position:", 0));

        lineEdit_below_tab->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_below_tab->setEnabled(false);
        lineEdit_below_tab->setGeometry(QRect(1053, 675, 225, 41));

///////////////////////////////// WIDGET 2 TAB3 (MOVE XY)

        tab2_3->setObjectName(QString::fromUtf8("tab2_3"));
        tab2_3->setEnabled(false);

        MOVE_label->setObjectName(QString::fromUtf8("label_22"));
        MOVE_label->setGeometry(QRect(98, 75, 75, 41));
        MOVE_label->setFont(font);
        MOVE_label->setText(QApplication::translate("MyWidget", "Move", 0));

        MOVEUP_pushButton->setObjectName(QString::fromUtf8("MOVEUP_pushButton"));
        MOVEUP_pushButton->setGeometry(QRect(90, 30, 75, 38));
        MOVEUP_pushButton->setText(QApplication::translate("MyWidget", "UP", 0));

        MOVELEFT_pushButton->setObjectName(QString::fromUtf8("MOVELEFT_pushButton"));
        MOVELEFT_pushButton->setGeometry(QRect(15, 75, 75, 38));
        MOVELEFT_pushButton->setText(QApplication::translate("MyWidget", "LEFT", 0));

        MOVERIGHT_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        MOVERIGHT_pushButton->setGeometry(QRect(165, 75, 75, 38));
        MOVERIGHT_pushButton->setText(QApplication::translate("MyWidget", "RIGHT", 0));

        MOVEDOWN_pushButton->setObjectName(QString::fromUtf8("MOVEDOWN_pushButton"));
        MOVEDOWN_pushButton->setGeometry(QRect(90, 120, 75, 38));
        MOVEDOWN_pushButton->setText(QApplication::translate("MyWidget", "DOWN", 0));

        STOPMOVE_pushButton->setObjectName(QString::fromUtf8("STOPMOVE_pushButton"));
        STOPMOVE_pushButton->setGeometry(QRect(300, 53, 120, 83));
        STOPMOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));

        MOVE_TO_X_pushButton->setObjectName(QString::fromUtf8("MOVE_TO_X_pushButton"));
        MOVE_TO_X_pushButton->setGeometry(QRect(30, 203, 135, 41));
        MOVE_TO_X_pushButton->setText(QApplication::translate("MyWidget", "Move X to", 0));

        MOVE_TO_X_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_X_doubleSpinBox"));
        MOVE_TO_X_doubleSpinBox->setGeometry(QRect(195, 203, 225, 41));
        MOVE_TO_X_doubleSpinBox->setDecimals(3);
        MOVE_TO_X_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        MOVE_TO_X_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_X_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

        MOVE_TO_Y_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        MOVE_TO_Y_pushButton->setGeometry(QRect(30, 263, 135, 38));
        MOVE_TO_Y_pushButton->setText(QApplication::translate("MyWidget", "Move to Y", 0));

        MOVE_TO_Y_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_Y_doubleSpinBox"));
        MOVE_TO_Y_doubleSpinBox->setGeometry(QRect(195, 263, 225, 41));
        MOVE_TO_Y_doubleSpinBox->setDecimals(3);
        MOVE_TO_Y_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 50; 14/07/2016
        MOVE_TO_Y_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_Y_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016


///////////////////////////////// WIDGET 2 TAB4 (SCAN)

        tab2_4->setObjectName(QString::fromUtf8("tab2_4"));
        tab2_4->setEnabled(false);

        SCAN_X_label->setObjectName(QString::fromUtf8("SCAN_X_label"));
        SCAN_X_label->setGeometry(QRect(60, 8, 450, 41));
//        SCAN_X_label->setFont(font);
        SCAN_X_label->setText(QApplication::translate("MyWidget", "X min (mm)                  X max. (mm)", 0));

        SET_SCAN_X_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MIN"));
        SET_SCAN_X_MIN_doubleSpinBox->setGeometry(QRect(15, 45, 195, 41));
        SET_SCAN_X_MIN_doubleSpinBox->setDecimals(3);
        SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200); //cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_X_MIN_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_X_MIN_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

        SET_SCAN_X_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MAX"));
        SET_SCAN_X_MAX_doubleSpinBox->setGeometry(QRect(240, 45, 195, 41));
        SET_SCAN_X_MAX_doubleSpinBox->setDecimals(3);
        SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200); //cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_X_MAX_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_X_MAX_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

///////// Y

        SCAN_Y_label->setObjectName(QString::fromUtf8("label_22"));
        SCAN_Y_label->setGeometry(QRect(60, 105, 450, 41));
        SCAN_Y_label->setText(QApplication::translate("MyWidget", "Y min (mm)                  Y max. (mm)", 0));

        SET_SCAN_Y_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MIN"));
        SET_SCAN_Y_MIN_doubleSpinBox->setGeometry(QRect(15, 143, 195, 41));
        SET_SCAN_Y_MIN_doubleSpinBox->setDecimals(3);
        SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_Y_MIN_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_Y_MIN_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

        SET_SCAN_Y_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MAX"));
        SET_SCAN_Y_MAX_doubleSpinBox->setGeometry(QRect(240, 143, 195, 41));
        SET_SCAN_Y_MAX_doubleSpinBox->setDecimals(3);
        SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_Y_MAX_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_Y_MAX_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

        SCANXY_pushButton->setObjectName(QString::fromUtf8("SCANXY_pushButton"));
        SCANXY_pushButton->setGeometry(QRect(23, 195, 180, 41));
        SCANXY_pushButton->setText(QApplication::translate("MyWidget", "SCAN XY", 0));

        SCANYX_pushButton->setObjectName(QString::fromUtf8("SCANYX_pushButton"));
        SCANYX_pushButton->setGeometry(QRect(248, 195, 180, 41));
        SCANYX_pushButton->setText(QApplication::translate("MyWidget", "SCAN YX", 0));

        SCAN_ABORT_pushButton->setObjectName(QString::fromUtf8("SCAN_ABORT_pushButton"));
        SCAN_ABORT_pushButton->setGeometry(QRect(120, 255, 210, 41));
        SCAN_ABORT_pushButton->setText(QApplication::translate("MyWidget", "ABORT SCAN", 0));



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        centralWidget->setMinimumSize(QSize(1320, 780));
        centralWidget->setMaximumSize(QSize(1320, 780));


        break;

     }

    case(2):
     {

        FRAME_line1->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line1->setGeometry(QRect(560, 259, 310, 196));
        FRAME_line1->setFrameShape(QFrame::StyledPanel);
        FRAME_line1->setFrameShadow(QFrame::Plain);

        FRAME_line2->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line2->setGeometry(QRect(885, 259, 310, 196));
        FRAME_line2->setFrameShape(QFrame::StyledPanel);
        FRAME_line2->setFrameShadow(QFrame::Plain);

        FRAME_line3->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line3->setGeometry(QRect(885, 383, 310, 72));
        FRAME_line3->setFrameShape(QFrame::StyledPanel);
        FRAME_line3->setFrameShadow(QFrame::Plain);

        FRAME_line4->setObjectName(QString::fromUtf8("line_6"));
        FRAME_line4->setGeometry(QRect(10, 462, 1185, 45));
        FRAME_line4->setFrameShape(QFrame::StyledPanel);
        FRAME_line4->setFrameShadow(QFrame::Plain);




//////////////////////////// CREATI IN SEQUENZA ////////////////////////


        CAMERA_label->setObjectName(QString::fromUtf8("label_9"));
        CAMERA_label->setGeometry(QRect(20, 470, 60, 28));
        QFont font;
        font.setFamily(QString::fromUtf8("Cantarell"));
        font.setPointSize(12);
        CAMERA_label->setFont(font);
        CAMERA_label->setText(QApplication::translate("MainWindow", "Camera", 0));

        VLC_Button->setObjectName(QString::fromUtf8("pushButton_4"));
        VLC_Button->setGeometry(QRect(85, 470, 80, 28));
        VLC_Button->setText(QApplication::translate("MainWindow", "VLC", 0));

        LASER_label->setObjectName(QString::fromUtf8("label_11"));
        LASER_label->setGeometry(QRect(210, 470, 60, 28));
        LASER_label->setFont(font);
        LASER_label->setText(QApplication::translate("MainWindow", "Laser", 0));

        LASER_Button->setObjectName(QString::fromUtf8("pushButton_5"));
        LASER_Button->setGeometry(QRect(265, 470, 80, 28));
        LASER_Button->setText(QApplication::translate("MainWindow", "On/Off", 0));
/*
        HELIUM_label->setObjectName(QString::fromUtf8("label_13"));
        HELIUM_label->setGeometry(QRect(390, 470, 60, 28));
        HELIUM_label->setFont(font);
        HELIUM_label->setText(QApplication::translate("MainWindow", "Helium", 0));

        HELIUM_Button->setObjectName(QString::fromUtf8("pushButton_6"));
        HELIUM_Button->setGeometry(QRect(460, 470, 80, 28));
        HELIUM_Button->setText(QApplication::translate("MainWindow", "On/Off", 0));

        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(780, 468, 70, 32));
        lcdNumber->setFont(font2);
        lcdNumber->setLineWidth(2);
        lcdNumber->setMidLineWidth(0);
        lcdNumber->setDigitCount(3);
        lcdNumber->setSegmentStyle(QLCDNumber::Filled);

        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(660, 474, 110, 20));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
*/



///////////////////////////////////////////////// BOTTONI E LINE_EDIT A DX


        DAQ_label->setObjectName(QString::fromUtf8("label"));
        DAQ_label->setGeometry(QRect(435, 10, 141, 21));
        DAQ_label->setFont(font);
        DAQ_label->setText(QApplication::translate("MainWindow", "DAQ Settings", 0));

       // QGroupBox *groupBox = new QGroupBox(centralWidget); //QGroupBox(tr("DAQ Type"));
        groupBox->setGeometry(QRect(425, 35, 120, 60));
       // QRadioButton *radioUSB = new QRadioButton(groupBox);
        radioUSB->setGeometry(QRect(2, 5, 100, 20));
        radioUSB->setText(QApplication::translate("MainWindow", " USB-link", 0));
        //QRadioButton *radioOptical = new QRadioButton(groupBox);
        radioOptical->setGeometry(QRect(2, 30, 100, 20));
        radioOptical->setText(QApplication::translate("MainWindow", " Optical-link", 0));
        radioUSB->setChecked(true);

        Digitizer_Button->setObjectName(QString::fromUtf8("pushButton"));
        Digitizer_Button->setGeometry(QRect(425, 105, 121, 27));
        Digitizer_Button->setText(QApplication::translate("MainWindow", "Digitizer", 0));

        Detector_Button->setObjectName(QString::fromUtf8("pushButton_2"));
        Detector_Button->setGeometry(QRect(425, 140, 121, 27));
        Detector_Button->setText(QApplication::translate("MainWindow", "Detector", 0));

        SW_treshold_Button->setObjectName(QString::fromUtf8("pushButton_2"));
        SW_treshold_Button->setGeometry(QRect(425, 175, 121, 27));
        SW_treshold_Button->setText(QApplication::translate("MainWindow", "SW_threshold", 0));

        TTY_label->setObjectName(QString::fromUtf8("label_2"));
        TTY_label->setGeometry(QRect(450, 225, 111, 21));
        TTY_label->setFont(font);
        TTY_label->setText(QApplication::translate("MainWindow", "TTY status", 0));

        CurrentActionX->setObjectName(QString::fromUtf8("CurrentActionX"));
        CurrentActionX->setEnabled(false);
        CurrentActionX->setGeometry(QRect(425, 250, 121, 29));
        CurrentActionX->setText("X on TTYUSB0");

        CurrentActionY->setObjectName(QString::fromUtf8("CurrentActionY"));
        CurrentActionY->setEnabled(false);
        CurrentActionY->setGeometry(QRect(425, 285, 121, 29));
        CurrentActionY->setText("Y on TTYUSB1");

        CurrentActionZ->setObjectName(QString::fromUtf8("CurrentActionY_2"));
        CurrentActionZ->setEnabled(false);
        CurrentActionZ->setGeometry(QRect(425, 320, 121, 29));
        CurrentActionZ->setText("Z on TTYUSB2");

        Export_Button->setObjectName(QString::fromUtf8("pushButton_3"));
        Export_Button->setGeometry(QRect(425, 375, 121, 27));
        Export_Button->setText(QApplication::translate("MainWindow", "Export Map", 0));



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        CurrentAction->setObjectName(QString::fromUtf8("CurrentActionY_3"));
        CurrentAction->setEnabled(false);
        CurrentAction->setGeometry(QRect(10, 425, 535, 30));
	CurrentAction->setText("Ready");

/////////////////////////////////// LINES ///////////////////////////////////////

        line_1->setObjectName(QString::fromUtf8("line_6"));
        line_1->setGeometry(QRect(185, 465, 10, 38));
        line_1->setFrameShape(QFrame::VLine);
        line_1->setFrameShadow(QFrame::Sunken);
        line_2->setObjectName(QString::fromUtf8("line_7"));
        line_2->setGeometry(QRect(365, 465, 10, 38));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3->setObjectName(QString::fromUtf8("line_9"));
        line_3->setGeometry(QRect(440, 360, 101, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4->setObjectName(QString::fromUtf8("line_10"));
        line_4->setGeometry(QRect(440, 210, 101, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);



////////////////////////////////////// GAS LABEL
/*
        GAS_label->setObjectName(QString::fromUtf8("label_12"));
        GAS_label->setGeometry(QRect(560, 470, 90, 28));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Cantarell"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        GAS_label->setFont(font1);
        GAS_label->setText(QApplication::translate("MainWindow", "Flux (0-100)%", 0));

*/

////////////////////////////// XRAY TABLE BUTTON /////////////////////////////////


        XRAY_TABLE_Button->setObjectName(QString::fromUtf8("pushButton_X"));
        XRAY_TABLE_Button->setText(QApplication::translate("MainWindow", "X-Ray Table", 0));
        XRAY_TABLE_Button->setGeometry(QRect(900, 470, 280, 30));


///////////////// INHERITED FROM AUTOFOCUS



//     scaleFactor = 1.0;
//     scrollArea->setWidgetResizable(fitToWindow); 




///////////////////////// TABWIDGET AUTOFOCUS ////////////////////

        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(560, 10, 310, 250));
//        tabWidget->setMaximumSize(QSize(16777215, 16777215));


///////////////////////////////// WIDGET 1 TAB1  

        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setEnabled(true);


        //TTY_Z_label->setObjectName(QString::fromUtf8("label_22"));
        //TTY_Z_label->setGeometry(QRect(10, 30, 125, 27));
        //TTY_Z_label->setText(QApplication::translate("MyWidget", "/dev/ttyUSB", 0));

        spinBox_assignZ->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignZ->setGeometry(QRect(165, 30, 125, 27));
        spinBox_assignZ->setMaximum(3);
        spinBox_assignZ->setValue(2);

        AssignZ_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        AssignZ_pushButton->setGeometry(QRect(10, 80, 125, 32));
        AssignZ_pushButton->setText(QApplication::translate("MyWidget", "Assign port Z", 0));

        //linedit_Z_Assignement->setObjectName(QString::fromUtf8("lineEdit_4"));
        //linedit_Z_Assignement->setEnabled(false);
        //linedit_Z_Assignement->setGeometry(QRect(165, 80, 125, 32));

        //line_6_tab->setObjectName(QString::fromUtf8("line_6"));
        //line_6_tab->setGeometry(QRect(5, 120, 300, 10));
        //line_6_tab->setFrameShape(QFrame::HLine);
        //line_6_tab->setFrameShadow(QFrame::Sunken);

        //label_6_tab->setObjectName(QString::fromUtf8("label_6"));
        //label_6_tab->setGeometry(QRect(135, 135, 67, 22));
        //label_6_tab->setFont(font);
        //label_6_tab->setText(QApplication::translate("MyWidget", "Z Init", 0));

        INIT_Z_pushButton->setObjectName(QString::fromUtf8("INIT_Z_pushButton"));
        INIT_Z_pushButton->setGeometry(QRect(10, 165, 125, 32));
        INIT_Z_pushButton->setText(QApplication::translate("MyWidget", " INIT Z", 0));

        STOP_Z_INIT_pushButton->setObjectName(QString::fromUtf8("STOP_Z_INIT_pushButton"));
        STOP_Z_INIT_pushButton->setGeometry(QRect(165, 165, 125, 32));
        STOP_Z_INIT_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));


        tabWidget->addTab(tab, QString());

///////////////////////////////// WIDGET 1 TAB2

        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setEnabled(false);

        MOVE_Z_MOTOR_label_1->setObjectName(QString::fromUtf8("MOVE_Z_MOTOR_label_1"));
        MOVE_Z_MOTOR_label_1->setGeometry(QRect(20, 10, 170, 20));
        MOVE_Z_MOTOR_label_1->setFont(font2);
        MOVE_Z_MOTOR_label_1->setText(QApplication::translate("MyWidget", "Move Z Motor", 0));

        MOVE_Z_To_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_To_pushButton"));
        MOVE_Z_To_pushButton->setGeometry(QRect(20, 40, 90, 27));
        MOVE_Z_To_pushButton->setText(QApplication::translate("MyWidget", "Move Z ->", 0));

        MOVE_Z_To_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_Z_To_doubleSpinBox"));
        MOVE_Z_To_doubleSpinBox->setGeometry(QRect(140, 40, 140, 27));
        MOVE_Z_To_doubleSpinBox->setDecimals(3);
        MOVE_Z_To_doubleSpinBox->setMaximum(50);
        MOVE_Z_To_doubleSpinBox->setSingleStep(0.001);
        MOVE_Z_To_doubleSpinBox->setValue(25);

        MOVE_Z_FORWARD_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_FORWARD_pushButton"));
        MOVE_Z_FORWARD_pushButton->setGeometry(QRect(20, 100, 90, 28));
        MOVE_Z_FORWARD_pushButton->setText(QApplication::translate("MyWidget", "Forward", 0));

        MOVE_Z_MOTOR_label_2->setObjectName(QString::fromUtf8("MOVE_Z_MOTOR_label_2"));
        MOVE_Z_MOTOR_label_2->setGeometry(QRect(130, 100, 60, 28));
        MOVE_Z_MOTOR_label_2->setFont(font);
        MOVE_Z_MOTOR_label_2->setText(QApplication::translate("MyWidget", "Move", 0));

        MOVE_Z_BACKWARD_pushButton->setObjectName(QString::fromUtf8("MOVE_Z_BACKWARD_pushButton"));
        MOVE_Z_BACKWARD_pushButton->setGeometry(QRect(190, 100, 90, 28));
        MOVE_Z_BACKWARD_pushButton->setText(QApplication::translate("MyWidget", "Backward", 0));

        STOP_Z_MOVE_pushButton->setObjectName(QString::fromUtf8("STOP_Z_MOVE_pushButton"));
        STOP_Z_MOVE_pushButton->setGeometry(QRect(65, 160, 165, 28));
        STOP_Z_MOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));


///////////////////////////////// WIDGET 1 TAB3

        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab_3->setEnabled(false);

        //ACM_label->setObjectName(QString::fromUtf8("label_22"));
        //ACM_label->setGeometry(QRect(10, 30, 125, 27));
        //ACM_label->setText(QApplication::translate("MyWidget", "/dev/ttyACM", 0));

        spinBox_assignACM->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignACM->setGeometry(QRect(165, 30, 125, 27));
        spinBox_assignACM->setMaximum(3);

        AssignACM_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        AssignACM_pushButton->setGeometry(QRect(10, 80, 125, 32));
        AssignACM_pushButton->setText(QApplication::translate("MyWidget", "Assign ACM port", 0));

        CurrentActionACM->setObjectName(QString::fromUtf8("CurrentActionACM"));
        CurrentActionACM->setEnabled(false);
        CurrentActionACM->setGeometry(QRect(85, 145, 165, 29));
        CurrentActionACM->setText("Arduino on TTYACM0");

///////////////////////////////// WIDGET 1 TAB4

        //tab_4->setObjectName(QString::fromUtf8("tab_4"));
        //tab_4->setEnabled(false);

        AUTOFOCUS_ON_pushButton->setObjectName(QString::fromUtf8("AUTOFOCUS_ON_pushButton"));
        AUTOFOCUS_ON_pushButton->setGeometry(QRect(50, 30, 200, 30));
        AUTOFOCUS_ON_pushButton->setText(QApplication::translate("MyWidget", "Autofocus ON (ttyACM)", 0));

        //AUTOFOCUS_STATUS_checkBox->setObjectName(QString::fromUtf8("checkBox_3"));
        //AUTOFOCUS_STATUS_checkBox->setEnabled(false);
        //AUTOFOCUS_STATUS_checkBox->setGeometry(QRect(75, 70, 150, 25));
        //AUTOFOCUS_STATUS_checkBox->setCheckable(true);
        //AUTOFOCUS_STATUS_checkBox->setText(QApplication::translate("MyWidget", "Autofocus status", 0));

        label_23_tab_4->setObjectName(QString::fromUtf8("label_23"));
        label_23_tab_4->setGeometry(QRect(50, 120, 210, 25));
        label_23_tab_4->setText(QApplication::translate("MyWidget", "Distance from target (mm)", 0));

        lineEdit_2_tab_4->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2_tab_4->setEnabled(false);
        lineEdit_2_tab_4->setGeometry(QRect(60, 150, 180, 27));

        ENABLE_TRACKING_checkBox->setObjectName(QString::fromUtf8("ENABLE_TRACKING_checkBox"));
        ENABLE_TRACKING_checkBox->setGeometry(QRect(80, 180, 140, 25));
        ENABLE_TRACKING_checkBox->setText(QApplication::translate("MyWidget", "Enable tracking", 0));


        tabWidget->addTab(tab_2, QString());
        tabWidget->addTab(tab_3, QString());
        //tabWidget->addTab(tab_4, QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MyWidget", "TTY_Z", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MyWidget", "Move_Z", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MyWidget", "ACM", 0));
        //tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MyWidget", "Autofocus", 0));
        tabWidget->setCurrentIndex(0);


//////////////////////// zone below WIDGET 1 //// X da 640 

        //label_7_below_tab->setObjectName(QString::fromUtf8("label_7"));
        //label_7_below_tab->setGeometry(QRect(580, 270, 120, 27));
        //label_7_below_tab->setFont(font);
        //label_7_below_tab->setText(QApplication::translate("MyWidget", "Z_Speed (mm/s)", 0));

        //Z_VELOCITY_doubleSpinBox->setObjectName(QString::fromUtf8("Z_VELOCITY_doubleSpinBox"));
        //Z_VELOCITY_doubleSpinBox->setGeometry(QRect(710, 270, 150, 27));
        //Z_VELOCITY_doubleSpinBox->setDecimals(3);
        //Z_VELOCITY_doubleSpinBox->setMaximum(50);
        //Z_VELOCITY_doubleSpinBox->setSingleStep(0.001);
        //Z_VELOCITY_doubleSpinBox->setValue(10);

        //line_11->setObjectName(QString::fromUtf8("line_11"));
        //line_11->setGeometry(QRect(565, 305, 300, 10));
        //line_11->setFrameShape(QFrame::HLine);
        //line_11->setFrameShadow(QFrame::Sunken);

        //label_8_below_tab->setObjectName(QString::fromUtf8("label_8"));
        //label_8_below_tab->setGeometry(QRect(580, 325, 120, 27));
        //label_8_below_tab->setFont(font);
        //label_8_below_tab->setText(QApplication::translate("MyWidget", "Step Z (mm)", 0));

        //Z_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("Z_STEP_doubleSpinBox"));
        //Z_STEP_doubleSpinBox->setGeometry(QRect(710, 325, 150, 27));
        //Z_STEP_doubleSpinBox->setDecimals(3);
        //Z_STEP_doubleSpinBox->setMaximum(50);
        //Z_STEP_doubleSpinBox->setSingleStep(0.001);
        //Z_STEP_doubleSpinBox->setValue(1);

        //line_14->setObjectName(QString::fromUtf8("line_14"));
        //line_14->setGeometry(QRect(565, 360, 300, 10));
        //line_14->setFrameShape(QFrame::HLine);
        //line_14->setFrameShadow(QFrame::Sunken);

        label_17_below_tab->setObjectName(QString::fromUtf8("label_17"));
        label_17_below_tab->setGeometry(QRect(580, 375, 120, 27));
        label_17_below_tab->setFont(font);
        label_17_below_tab->setText(QApplication::translate("MyWidget", "Z - position:", 0));

        lineEdit_below_tab->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_below_tab->setEnabled(false);
        lineEdit_below_tab->setGeometry(QRect(710, 375, 150, 27));

        //label_18_below_tab->setObjectName(QString::fromUtf8("label_18"));
        //label_18_below_tab->setGeometry(QRect(580, 405, 260, 25));

        //label_18_below_tab->setFont(font7);
        //label_18_below_tab->setText(QApplication::translate("MyWidget", "Z-axis linear stage absolute position", 0));




///////////////////////// TABWIDGET X-Y WIDGET 2 ////////////////////

        //tabWidget2->setObjectName(QString::fromUtf8("tabWidget2"));
        //tabWidget2->setEnabled(true);
        //tabWidget2->setGeometry(QRect(885, 10, 310, 250));
//        tabWidget->setMaximumSize(QSize(16777215, 16777215));

///////////////////////////////// WIDGET 2 TAB1 ASSIGN X & Y PORT

        //tab2_1->setObjectName(QString::fromUtf8("tab2_1"));
        //tab2_1->setEnabled(true);

        //label_tab_2_bX->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_bX->setGeometry(QRect(115, 20, 170, 20));
        //label_tab_2_bX->setFont(font);
        //label_tab_2_bX->setText(QApplication::translate("MyWidget", "X Motor", 0));

        spinBox_assignX->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignX->setGeometry(QRect(165, 60, 125, 27));
        spinBox_assignX->setMaximum(3);

        pushButton_assignX_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignX_port->setGeometry(QRect(20, 60, 120, 27));
        pushButton_assignX_port->setText(QApplication::translate("MyWidget", "Assign port X", 0));

        //label_tab_2_bY->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_bY->setGeometry(QRect(115, 120, 170, 20));
        //label_tab_2_bY->setFont(font);
        //label_tab_2_bY->setText(QApplication::translate("MyWidget", "Y Motor", 0));

        spinBox_assignY->setObjectName(QString::fromUtf8("spinBox"));
        spinBox_assignY->setGeometry(QRect(165, 160, 125, 27));
        spinBox_assignY->setMaximum(3);
        spinBox_assignY->setValue(1);

        pushButton_assignY_port->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_assignY_port->setGeometry(QRect(20, 160, 120, 27));
        pushButton_assignY_port->setText(QApplication::translate("MyWidget", "Assign port Y", 0));

///////////////////////////////////////WIDGET 2 TAB 2///////////INIT////////////////////////////////

        //tab2_2->setObjectName(QString::fromUtf8("tab2_2"));
        //tab2_2->setEnabled(false);

        //label_tab_2_2->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_2->setGeometry(QRect(85, 20, 170, 20));
        //label_tab_2_2->setFont(font);
        //label_tab_2_2->setText(QApplication::translate("MyWidget", "Motor initialisation", 0));

        //label_tab_2_2X->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_2X->setGeometry(QRect(65, 50, 170, 20));
        //label_tab_2_2X->setFont(font);
        //label_tab_2_2X->setText(QApplication::translate("MyWidget", "X motor", 0));

        //label_tab_2_2Y->setObjectName(QString::fromUtf8("label_10"));
        //label_tab_2_2Y->setGeometry(QRect(185, 50, 170, 20));
        //label_tab_2_2Y->setFont(font);
        //label_tab_2_2Y->setText(QApplication::translate("MyWidget", "Y motor", 0));

        //comboBox_XMotor->setGeometry(QRect(10, 120, 140, 30));
        //comboBox_YMotor->setGeometry(QRect(160, 120, 140, 30));

        pushButton_tab_2_2X->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2X->setGeometry(QRect(55, 75, 90, 27));
        pushButton_tab_2_2X->setText(QApplication::translate("MyWidget", "Init X", 0));

        pushButton_tab_2_2Y->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2Y->setGeometry(QRect(170, 75, 90, 27));
        pushButton_tab_2_2Y->setText(QApplication::translate("MyWidget", "Init Y", 0));

   //     pushButton_tab_2_2XY->setObjectName(QString::fromUtf8("pushButton_8"));
   //     pushButton_tab_2_2XY->setGeometry(QRect(95, 120, 130, 27));
   //     pushButton_tab_2_2XY->setText(QApplication::translate("MyWidget", "Init X+Y", 0));

        pushButton_tab_2_2ABORT->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_tab_2_2ABORT->setGeometry(QRect(75, 170, 170, 27));
        pushButton_tab_2_2ABORT->setText(QApplication::translate("MyWidget", "STOP", 0));

///////////////////////////////// WIDGET 2 TAB3 (MOVE XY)

        tab2_3->setObjectName(QString::fromUtf8("tab2_3"));
        tab2_3->setEnabled(false);

        MOVE_label->setObjectName(QString::fromUtf8("label_22"));
        MOVE_label->setGeometry(QRect(65, 50, 50, 27));
        MOVE_label->setFont(font);
        MOVE_label->setText(QApplication::translate("MyWidget", "Move", 0));

        MOVEUP_pushButton->setObjectName(QString::fromUtf8("MOVEUP_pushButton"));
        MOVEUP_pushButton->setGeometry(QRect(60, 20, 50, 25));
        MOVEUP_pushButton->setText(QApplication::translate("MyWidget", "UP", 0));

        MOVELEFT_pushButton->setObjectName(QString::fromUtf8("MOVELEFT_pushButton"));
        MOVELEFT_pushButton->setGeometry(QRect(10, 50, 50, 25));
        MOVELEFT_pushButton->setText(QApplication::translate("MyWidget", "LEFT", 0));

        MOVERIGHT_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        MOVERIGHT_pushButton->setGeometry(QRect(110, 50, 50, 25));
        MOVERIGHT_pushButton->setText(QApplication::translate("MyWidget", "RIGHT", 0));

        MOVEDOWN_pushButton->setObjectName(QString::fromUtf8("MOVEDOWN_pushButton"));
        MOVEDOWN_pushButton->setGeometry(QRect(60, 80, 50, 25));
        MOVEDOWN_pushButton->setText(QApplication::translate("MyWidget", "DOWN", 0));

        STOPMOVE_pushButton->setObjectName(QString::fromUtf8("STOPMOVE_pushButton"));
        STOPMOVE_pushButton->setGeometry(QRect(200, 35, 80, 55));
        STOPMOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP", 0));

        MOVE_TO_X_pushButton->setObjectName(QString::fromUtf8("MOVE_TO_X_pushButton"));
        MOVE_TO_X_pushButton->setGeometry(QRect(20, 135, 90, 27));
        MOVE_TO_X_pushButton->setText(QApplication::translate("MyWidget", "Move X to", 0));

        MOVE_TO_X_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_X_doubleSpinBox"));
        MOVE_TO_X_doubleSpinBox->setGeometry(QRect(130, 135, 150, 27));
        MOVE_TO_X_doubleSpinBox->setDecimals(3);
        MOVE_TO_X_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        MOVE_TO_X_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_X_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

        MOVE_TO_Y_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
        MOVE_TO_Y_pushButton->setGeometry(QRect(20, 175, 90, 25));
        MOVE_TO_Y_pushButton->setText(QApplication::translate("MyWidget", "Move to Y", 0));

        MOVE_TO_Y_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_Y_doubleSpinBox"));
        MOVE_TO_Y_doubleSpinBox->setGeometry(QRect(130, 175, 150, 27));
        MOVE_TO_Y_doubleSpinBox->setDecimals(3);
        MOVE_TO_Y_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 50; 14/07/2016
        MOVE_TO_Y_doubleSpinBox->setSingleStep(0.001);
        MOVE_TO_Y_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016


///////////////////////////////// WIDGET 2 TAB4 (SCAN)

        tab2_4->setObjectName(QString::fromUtf8("tab2_4"));
        tab2_4->setEnabled(false);

        SCAN_X_label->setObjectName(QString::fromUtf8("SCAN_X_label"));
        SCAN_X_label->setGeometry(QRect(40, 5, 300, 27));
//        SCAN_X_label->setFont(font);
        SCAN_X_label->setText(QApplication::translate("MyWidget", "X min (mm)                  X max. (mm)", 0));

        SET_SCAN_X_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MIN"));
        SET_SCAN_X_MIN_doubleSpinBox->setGeometry(QRect(10, 30, 130, 27));
        SET_SCAN_X_MIN_doubleSpinBox->setDecimals(3);
        SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200); //cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_X_MIN_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_X_MIN_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

        SET_SCAN_X_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MAX"));
        SET_SCAN_X_MAX_doubleSpinBox->setGeometry(QRect(160, 30, 130, 27));
        SET_SCAN_X_MAX_doubleSpinBox->setDecimals(3);
        SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200); //cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_X_MAX_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_X_MAX_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

///////// Y

        SCAN_Y_label->setObjectName(QString::fromUtf8("label_22"));
        SCAN_Y_label->setGeometry(QRect(40, 70, 300, 27));
        SCAN_Y_label->setText(QApplication::translate("MyWidget", "Y min (mm)                  Y max. (mm)", 0));

        SET_SCAN_Y_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MIN"));
        SET_SCAN_Y_MIN_doubleSpinBox->setGeometry(QRect(10, 95, 130, 27));
        SET_SCAN_Y_MIN_doubleSpinBox->setDecimals(3);
        SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_Y_MIN_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_Y_MIN_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

        SET_SCAN_Y_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MAX"));
        SET_SCAN_Y_MAX_doubleSpinBox->setGeometry(QRect(160, 95, 130, 27));
        SET_SCAN_Y_MAX_doubleSpinBox->setDecimals(3);
        SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
        SET_SCAN_Y_MAX_doubleSpinBox->setSingleStep(0.001);
        SET_SCAN_Y_MAX_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

        SCANXY_pushButton->setObjectName(QString::fromUtf8("SCANXY_pushButton"));
        SCANXY_pushButton->setGeometry(QRect(15, 130, 120, 27));
        SCANXY_pushButton->setText(QApplication::translate("MyWidget", "SCAN XY", 0));

        SCANYX_pushButton->setObjectName(QString::fromUtf8("SCANYX_pushButton"));
        SCANYX_pushButton->setGeometry(QRect(165, 130, 120, 27));
        SCANYX_pushButton->setText(QApplication::translate("MyWidget", "SCAN YX", 0));

        SCAN_ABORT_pushButton->setObjectName(QString::fromUtf8("SCAN_ABORT_pushButton"));
        SCAN_ABORT_pushButton->setGeometry(QRect(80, 170, 140, 27));
        SCAN_ABORT_pushButton->setText(QApplication::translate("MyWidget", "ABORT SCAN", 0));



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //tabWidget2->addTab(tab2_1, QString());
        //tabWidget2->addTab(tab2_2, QString());
        //tabWidget2->addTab(tab2_3, QString());
        //tabWidget2->addTab(tab2_4, QString());
        //tabWidget2->setTabText(tabWidget2->indexOf(tab2_1), QApplication::translate("MyWidget", "TTY_XY", 0));
        //tabWidget2->setTabText(tabWidget2->indexOf(tab2_2), QApplication::translate("MyWidget", "Init", 0));
        //tabWidget2->setTabText(tabWidget2->indexOf(tab2_3), QApplication::translate("MyWidget", "Move_XY", 0));
        //tabWidget2->setTabText(tabWidget2->indexOf(tab2_4), QApplication::translate("MyWidget", "Scan", 0));
        //tabWidget2->setCurrentIndex(0);

////////////////////////// AREA BELOW TABWIDGET2

        XY_SPEED_label->setObjectName(QString::fromUtf8("XY_SPEED_label"));
        XY_SPEED_label->setGeometry(QRect(900, 270, 120, 27));
        XY_SPEED_label->setFont(font);
        XY_SPEED_label->setText(QApplication::translate("MyWidget", "XY_Speed (mm/s)", 0));

        XY_SPEED_doubleSpinBox->setObjectName(QString::fromUtf8("XY_SPEED_doubleSpinBox"));
        XY_SPEED_doubleSpinBox->setGeometry(QRect(1025, 270, 150, 27));
        XY_SPEED_doubleSpinBox->setDecimals(3);
        XY_SPEED_doubleSpinBox->setMaximum(50);
        XY_SPEED_doubleSpinBox->setSingleStep(0.001);
        XY_SPEED_doubleSpinBox->setValue(1);

        XY_SPEED_line->setObjectName(QString::fromUtf8("XY_SPEED_line"));
        XY_SPEED_line->setGeometry(QRect(890, 297, 300, 10));
        XY_SPEED_line->setFrameShape(QFrame::HLine);
        XY_SPEED_line->setFrameShadow(QFrame::Sunken);

        X_STEP_label->setObjectName(QString::fromUtf8("X_STEP_label"));
        X_STEP_label->setGeometry(QRect(900, 310, 120, 27));
        X_STEP_label->setFont(font);
        X_STEP_label->setText(QApplication::translate("MyWidget", "Step X (mm)", 0));

        X_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("X_STEP_doubleSpinBox"));
        X_STEP_doubleSpinBox->setGeometry(QRect(1025, 310, 150, 27));
        X_STEP_doubleSpinBox->setDecimals(3);
        X_STEP_doubleSpinBox->setMaximum(50);
        X_STEP_doubleSpinBox->setSingleStep(0.001);
        X_STEP_doubleSpinBox->setValue(1);

        XY_STEP_line->setObjectName(QString::fromUtf8("XY_STEP_line"));
        XY_STEP_line->setGeometry(QRect(890, 338, 300, 10));
        XY_STEP_line->setFrameShape(QFrame::HLine);
        XY_STEP_line->setFrameShadow(QFrame::Sunken);

        Y_STEP_label->setObjectName(QString::fromUtf8("Y_STEP_label"));
        Y_STEP_label->setGeometry(QRect(900, 350, 120, 27));
        Y_STEP_label->setFont(font);
        Y_STEP_label->setText(QApplication::translate("MyWidget", "Step Y (mm)", 0));

        Y_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("Y_STEP_doubleSpinBox"));
        Y_STEP_doubleSpinBox->setGeometry(QRect(1025, 350, 150, 27));
        Y_STEP_doubleSpinBox->setDecimals(3);
        Y_STEP_doubleSpinBox->setMaximum(50);
        Y_STEP_doubleSpinBox->setSingleStep(0.001);
        Y_STEP_doubleSpinBox->setValue(1);

        X_POSITION_label->setObjectName(QString::fromUtf8("X_POSITION_label"));
        X_POSITION_label->setGeometry(QRect(900, 390, 120, 27));
        X_POSITION_label->setFont(font);
        X_POSITION_label->setText(QApplication::translate("MyWidget", "X - position:", 0));

        X_POSITION_lineEdit->setObjectName(QString::fromUtf8("X_POSITION_lineEdit"));
        X_POSITION_lineEdit->setEnabled(false);
        X_POSITION_lineEdit->setGeometry(QRect(1025, 390, 150, 27));

        Y_POSITION_label->setObjectName(QString::fromUtf8("Y_POSITION_label"));
        Y_POSITION_label->setGeometry(QRect(900, 420, 120, 27));
        Y_POSITION_label->setFont(font);
        Y_POSITION_label->setText(QApplication::translate("MyWidget", "Y - position:", 0));

        Y_POSITION_lineEdit->setObjectName(QString::fromUtf8("Y_POSITION_lineEdit"));
        Y_POSITION_lineEdit->setEnabled(false);
        Y_POSITION_lineEdit->setGeometry(QRect(1025, 420, 150, 27));

        break;

     }

    } //swich closed


////////////////////////////// BUTTONS CONNECTIONS /////////////////////////////////


        //connect(radioUSB, SIGNAL(clicked()), this, SLOT(USB_DAQ()));
        //connect(radioOptical, SIGNAL(clicked()), this, SLOT(OPTICAL_DAQ()));

        connect(Digitizer_Button, SIGNAL(clicked()), this, SLOT(Digitiser2()));
        connect(Detector_Button, SIGNAL(clicked()), this, SLOT(Detector()));
        connect(LASER_Button, SIGNAL(clicked()), this, SLOT(Laser_interface()));
        //connect(HELIUM_Button, SIGNAL(clicked()), this, SLOT(Helium_interface()));
        connect(VLC_Button, SIGNAL(clicked()), this, SLOT(VLC_interface()));

        connect(Export_Button, SIGNAL(clicked()), this, SLOT(Image_Export()));
        connect(XRAY_TABLE_Button, SIGNAL(clicked()), this, SLOT(XrayTable()));


/////////////////////////////// OBJECTs CONNECTION //////////////////////////////////

        //QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        //QObject::connect(AUTOFOCUS_ON_pushButton, SIGNAL(released()), AUTOFOCUS_STATUS_checkBox, SLOT(toggle()));


/////////////////////////////// USB CONNECTION //////////////////////////////////
  connect(radioUSB, SIGNAL(clicked()), this, SLOT(USB_DAQ()));
  connect(radioOptical, SIGNAL(clicked()), this, SLOT(OPTICAL_DAQ()));

/////////////////////////////// USB COMBO_BOX //////////////////////////////////

     //connect(comboBox_XMotor, SIGNAL(activated(int)), this, SLOT(X_Motor_selection(int)));
     //connect(comboBox_YMotor, SIGNAL(activated(int)), this, SLOT(Y_Motor_selection(int)));
     //connect(comboBox_ZMotor, SIGNAL(activated(int)), this, SLOT(Z_Motor_selection(int)));
}












