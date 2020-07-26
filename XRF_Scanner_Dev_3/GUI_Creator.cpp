#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::GUI_CREATOR() {
    // Putting this here for future use.
    QFont font;
    font.setFamily(QString::fromUtf8("Cantarell"));
    font.setPointSize(12);
    font.setBold(true);

    QFont font2;
    font2.setFamily(QString::fromUtf8("Cantarell"));
    font2.setPointSize(12);
    font2.setBold(true);
    font2.setWeight(75);

    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    QGroupBox *groupBox = new QGroupBox(centralWidget);
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
    XRAY_TABLE_Button = new QPushButton(centralWidget);
    tabWidget = new QTabWidget(centralWidget);
    tab = new QWidget(tabWidget);
    spinBox_assignZ = new QSpinBox(tab);
    AssignZ_pushButton = new QPushButton(tab);

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
    label_17_below_tab = new QLabel(centralWidget);
    lineEdit_below_tab = new QLineEdit(centralWidget);
    spinBox_assignX = new QSpinBox(tab);
    pushButton_assignX_port = new QPushButton(tab);
    spinBox_assignY = new QSpinBox(tab);
    pushButton_assignY_port = new QPushButton(tab);
    pushButton_tab_2_2X = new QPushButton(tab);
    pushButton_tab_2_2Y = new QPushButton(tab);
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
    //SCANYX_pushButton = new QPushButton(tab2_4);
    SCAN_ABORT_pushButton = new QPushButton(tab2_4);
    XY_SPEED_label = new QLabel(centralWidget);
    XY_SPEED_doubleSpinBox = new QDoubleSpinBox(centralWidget);
    XY_SPEED_line = new QFrame(centralWidget);
    X_STEP_label = new QLabel(tab2_4);
    X_STEP_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    XY_STEP_line = new QLabel(tab2_4);
    Y_STEP_label = new QLabel(tab2_4);
    Y_STEP_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    X_POSITION_label = new QLabel(centralWidget);
    X_POSITION_lineEdit = new QLineEdit(centralWidget);                 ///// linedit
    Y_POSITION_label = new QLabel(centralWidget);
    Y_POSITION_lineEdit = new QLineEdit(centralWidget);                 ///// linedit_2


    /******************************/
    /********** GEOMETRY **********/
    /******************************/

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

    VLC_Button->setObjectName(QString::fromUtf8("pushButton_4"));
    VLC_Button->setGeometry(QRect(1135, 620, 120, 40));
    VLC_Button->setText(QApplication::translate("MainWindow", "Camera", 0));

    LASER_Button->setObjectName(QString::fromUtf8("pushButton_5"));
    LASER_Button->setGeometry(QRect(1000, 620, 120, 40));
    LASER_Button->setText(QApplication::translate("MainWindow", "Toggle Laser", 0));

    XRAY_TABLE_Button->setObjectName(QString::fromUtf8("pushButton_X"));
    XRAY_TABLE_Button->setText(QApplication::translate("MainWindow", "X-Ray Table", 0));
    XRAY_TABLE_Button->setGeometry(QRect(865, 620, 120, 40));

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
    CurrentAction->setGeometry(QRect(17
                                     , 615, 803, 45));
    CurrentAction->setText("Ready");

    /////////////////////////////////// LINES ///////////////////////////////////////

    line_3->setObjectName(QString::fromUtf8("line_9"));
    line_3->setGeometry(QRect(660, 540, 152, 30));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    line_4->setObjectName(QString::fromUtf8("line_10"));
    line_4->setGeometry(QRect(660, 305, 152, 30));
    line_4->setFrameShape(QFrame::HLine);
    line_4->setFrameShadow(QFrame::Sunken);


    ///////////////// INHERITED FROM AUTOFOCUS

    //     scaleFactor = 1.0;
    //     scrollArea->setWidgetResizable(fitToWindow);


    ///////////////////////// TABWIDGET AUTOFOCUS ////////////////////

    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setEnabled(true);
    tabWidget->setGeometry(QRect(840, 15, 465, 375));

    ///////////////////////////////// WIDGET 1 TAB1

    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setEnabled(true);

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

    spinBox_assignACM->setObjectName(QString::fromUtf8("spinBox"));
    spinBox_assignACM->setGeometry(QRect(248, 45, 194, 40));
    spinBox_assignACM->setMaximum(3);

    AssignACM_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
    AssignACM_pushButton->setGeometry(QRect(15, 45, 194, 40));
    AssignACM_pushButton->setText(QApplication::translate("MyWidget", "Assign ACM port", 0));



    ///////////////////////////////// WIDGET 1 TAB4

    AUTOFOCUS_ON_pushButton->setObjectName(QString::fromUtf8("AUTOFOCUS_ON_pushButton"));
    AUTOFOCUS_ON_pushButton->setGeometry(QRect(248, 100, 194, 40));
    AUTOFOCUS_ON_pushButton->setText(QApplication::translate("MyWidget", "Autofocus ON (ttyACM)", 0));
    AUTOFOCUS_ON_pushButton->setEnabled(false);

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

    /* ################################################################# */
    /* ####################   Tab 5 in tab Widget   #################### */
    /* ################################################################# */

    tab2_4->setObjectName(QString::fromUtf8("tab2_4"));
    tab2_4->setEnabled(false);

    SCAN_X_label->setObjectName(QString::fromUtf8("SCAN_X_label"));
    SCAN_X_label->setGeometry(QRect(15, 5, 450, 40));
    SCAN_X_label->setText(QApplication::translate("MyWidget", "X min\t\t\tY max", 0));

    SET_SCAN_X_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MIN"));
    SET_SCAN_X_MIN_doubleSpinBox->setGeometry(QRect(15, 45, 150, 40));
    SET_SCAN_X_MIN_doubleSpinBox->setDecimals(2);
    SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200);
    SET_SCAN_X_MIN_doubleSpinBox->setSingleStep(0.5);
    SET_SCAN_X_MIN_doubleSpinBox->setValue(100);

    SET_SCAN_X_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_X_MAX"));
    SET_SCAN_X_MAX_doubleSpinBox->setGeometry(QRect(180, 45, 150, 40));
    SET_SCAN_X_MAX_doubleSpinBox->setDecimals(2);
    SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200);
    SET_SCAN_X_MAX_doubleSpinBox->setSingleStep(0.5);
    SET_SCAN_X_MAX_doubleSpinBox->setValue(100);

    SCAN_Y_label->setObjectName(QString::fromUtf8("label_22"));
    SCAN_Y_label->setGeometry(QRect(15, 105, 450, 40));
    SCAN_Y_label->setText(QApplication::translate("MyWidget", "Y min\t\t\tY max", 0));

    SET_SCAN_Y_MIN_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MIN"));
    SET_SCAN_Y_MIN_doubleSpinBox->setGeometry(QRect(15, 145, 150, 40));
    SET_SCAN_Y_MIN_doubleSpinBox->setDecimals(2);
    SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(200);
    SET_SCAN_Y_MIN_doubleSpinBox->setSingleStep(0.5);
    SET_SCAN_Y_MIN_doubleSpinBox->setValue(100);

    SET_SCAN_Y_MAX_doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_SCAN_Y_MAX"));
    SET_SCAN_Y_MAX_doubleSpinBox->setGeometry(QRect(180, 145, 150, 40));
    SET_SCAN_Y_MAX_doubleSpinBox->setDecimals(2);
    SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(200);
    SET_SCAN_Y_MAX_doubleSpinBox->setSingleStep(0.5);
    SET_SCAN_Y_MAX_doubleSpinBox->setValue(100);

    X_STEP_label->setObjectName(QString::fromUtf8("X_STEP_label"));
    X_STEP_label->setGeometry(QRect(15, 205, 150, 40));
    X_STEP_label->setText(QApplication::translate("MyWidget", "Step X", 0));

    X_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("X_STEP_doubleSpinBox"));
    X_STEP_doubleSpinBox->setGeometry(QRect(15, 245, 150, 40));
    X_STEP_doubleSpinBox->setDecimals(2);
    X_STEP_doubleSpinBox->setMaximum(50);
    X_STEP_doubleSpinBox->setSingleStep(0.5);
    X_STEP_doubleSpinBox->setValue(1);

    Y_STEP_label->setObjectName(QString::fromUtf8("Y_STEP_label"));
    Y_STEP_label->setGeometry(QRect(180, 205, 150, 40));
    Y_STEP_label->setText(QApplication::translate("MyWidget", "Step Y", 0));

    Y_STEP_doubleSpinBox->setObjectName(QString::fromUtf8("Y_STEP_doubleSpinBox"));
    Y_STEP_doubleSpinBox->setGeometry(QRect(180, 245, 150, 40));
    Y_STEP_doubleSpinBox->setDecimals(2);
    Y_STEP_doubleSpinBox->setMaximum(50);
    Y_STEP_doubleSpinBox->setSingleStep(0.5);
    Y_STEP_doubleSpinBox->setValue(1);

    SCANXY_pushButton->setObjectName(QString::fromUtf8("SCANXY_pushButton"));
    SCANXY_pushButton->setGeometry(QRect(345, 45, 100, 60));
    SCANXY_pushButton->setText(QApplication::translate("MyWidget", "Start\nScan", 0));

    SCAN_ABORT_pushButton->setObjectName(QString::fromUtf8("SCAN_ABORT_pushButton"));
    SCAN_ABORT_pushButton->setGeometry(QRect(345, 125, 100, 60));
    SCAN_ABORT_pushButton->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");
    SCAN_ABORT_pushButton->setText("ABORT");



    XY_STEP_line->setObjectName(QString::fromUtf8("XY_STEP_line"));
    XY_STEP_line->setGeometry(QRect(15, 300, 450, 30));
    XY_STEP_line->setText("All units are in milimeters (mm)");

    /* ################################################################# */
    /* ####################  Area under tab Widget  #################### */
    /* ################################################################# */

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

    float scale = static_cast<float>(0.8);

    X_POSITION_label->setObjectName(QString::fromUtf8("X_POSITION_label"));
    X_POSITION_label->setGeometry(QRect(865, 585*scale, 180, 41));
    X_POSITION_label->setFont(font);
    X_POSITION_label->setText(QApplication::translate("MyWidget", "X - position:", 0));

    X_POSITION_lineEdit->setObjectName(QString::fromUtf8("X_POSITION_lineEdit"));
    X_POSITION_lineEdit->setEnabled(false);
    X_POSITION_lineEdit->setGeometry(QRect(1053, 585*scale, 225, 41));

    Y_POSITION_label->setObjectName(QString::fromUtf8("Y_POSITION_label"));
    Y_POSITION_label->setGeometry(QRect(865, 630*scale, 180, 41));
    Y_POSITION_label->setFont(font);
    Y_POSITION_label->setText(QApplication::translate("MyWidget", "Y - position:", 0));

    Y_POSITION_lineEdit->setObjectName(QString::fromUtf8("Y_POSITION_lineEdit"));
    Y_POSITION_lineEdit->setEnabled(false);
    Y_POSITION_lineEdit->setGeometry(QRect(1053, 630*scale, 225, 41));

    label_17_below_tab->setObjectName(QString::fromUtf8("label_17"));
    label_17_below_tab->setGeometry(QRect(865, 675*scale, 180, 41));
    label_17_below_tab->setFont(font);
    label_17_below_tab->setText(QApplication::translate("MyWidget", "Z - position:", 0));

    lineEdit_below_tab->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit_below_tab->setEnabled(false);
    lineEdit_below_tab->setGeometry(QRect(1053, 675*scale, 225, 41));

    centralWidget->setMinimumSize(QSize(1320, 670));
    centralWidget->setMaximumSize(QSize(1320, 670));


    ////////////////////////////// BUTTONS CONNECTIONS /////////////////////////////////


    connect(Digitizer_Button, SIGNAL(clicked()), this, SLOT(openDPPInter()));
    connect(Detector_Button, SIGNAL(clicked()), this, SLOT(Detector()));
    connect(LASER_Button, SIGNAL(clicked()), this, SLOT(Laser_interface()));
    //connect(HELIUM_Button, SIGNAL(clicked()), this, SLOT(Helium_interface()));
    connect(VLC_Button, SIGNAL(clicked()), this, SLOT(VLC_interface()));

    connect(Export_Button, SIGNAL(clicked()), this, SLOT(Image_Export()));
    connect(XRAY_TABLE_Button, SIGNAL(clicked()), this, SLOT(XrayTable()));


    /////////////////////////////// USB CONNECTION //////////////////////////////////
    connect(radioUSB, SIGNAL(clicked()), this, SLOT(USB_DAQ()));
    connect(radioOptical, SIGNAL(clicked()), this, SLOT(OPTICAL_DAQ()));

}
