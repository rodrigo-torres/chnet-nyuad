#include "h/mainwindow.h"
#include <QTextEdit>
#include <QToolBox>
#include <QStackedWidget>
#include <QDockWidget>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::GUI_CREATOR() {
    using namespace widgets;
    auto total_pushbuttons = index_of(pushbuttons::TOTAL_NO);
    buttons.resize(total_pushbuttons);

    QSize size;


    centralWidget = new QWidget();
    setCentralWidget(centralWidget);


    QTabWidget* tabWidget = new QTabWidget(centralWidget);
    tab1 = new QWidget(tabWidget);
    tab2 = new QWidget(tabWidget);
    tab3 = new QWidget(tabWidget);

    QGridLayout *layoutMaster = new QGridLayout(centralWidget);
    layoutMaster->setVerticalSpacing(10);
    layoutMaster->setHorizontalSpacing(15);



    /*******************************************************************/
    /********* TAB 1 in Tab Widget. Assign DFs and open FDs   **********/
    /*******************************************************************/

    tab1->setObjectName(QString::fromUtf8("tab"));
    tab1->setEnabled(true);

    //QLabel *labelTab1a = new QLabel(tab1);
    QLabel *labelTab1b = new QLabel(tab1);
    QLabel *labelServo = new QLabel(tab1);
    //labelTab1a->setText("<b><center>Device File Assignment</center></b>");
    labelTab1b->setText("<b>Assign ports for stages</b>");
    labelServo->setText("<b>Laser Servomechanism</b>");

    auto ref1 = pushbuttons::openttyx;
    auto ref2 = pushbuttons::initstagex;

    for (ulong i = 0 ; i < 4; i++)
    {
        buttons[ref1 + i] = new QPushButton(tab1);
        buttons[ref1 + i]->setText("Confirm");


        tab1_df_number[i] = new QSpinBox(tab1);
        tab1_df_number[i]->setMaximum(3);
        tab1_df_number[i]->findChild<QLineEdit*>()->setReadOnly(false);

        if (i != 3)
        {
            buttons[ref2 + i] = new QPushButton(tab1);
            tab1_df_number[i]->setValue(i);
        }
        else
            tab1_df_number[i]->setValue(0);
    }
    tab1_df_number[0]->setPrefix("X-axis: ");
    tab1_df_number[1]->setPrefix("Y-axis: ");
    tab1_df_number[2]->setPrefix("Z-axis: ");
    tab1_df_number[3]->setPrefix("Assign ACM to port: ");

    buttons[index_of(ref2)]->setText("Initiate X");
    buttons[ref2 + 1]->setText("Initiate Y");
    buttons[ref2 + 2]->setText("Initiate Z");


    laser_checkbox = new QCheckBox(tab1);
    servo_checkbox = new QCheckBox(tab1);

    laser_checkbox->setText("Enable distance reading");
    servo_checkbox->setText("Enable tracking servo");
    laser_checkbox->setEnabled(false);
    servo_checkbox->setEnabled(false);

    QGridLayout *portTabLayout = new QGridLayout(tab1);
    portTabLayout->setHorizontalSpacing(20);
    portTabLayout->setVerticalSpacing(10);

    //portTabLayout->addWidget(labelTab1a, 0, 0, 1, 5);
    portTabLayout->addWidget(labelTab1b, 1, 1, 1, 3);
    for (ulong i = 0 ; i < 3; i++) {
        portTabLayout->addWidget(tab1_df_number[i], i+2, 1);
        portTabLayout->addWidget(buttons[ref1 + i], i+2, 2);
        portTabLayout->addWidget(buttons[ref2 + i], i+2, 3);
    }
    portTabLayout->addWidget(labelServo, 5, 1, 1, 3);
    portTabLayout->addWidget(tab1_df_number[3], 6, 1, 1, 2);
    portTabLayout->addWidget(buttons[ref1 + 3], 6, 3);
    portTabLayout->addWidget(laser_checkbox, 7, 1);
    portTabLayout->addWidget(servo_checkbox, 8, 1);

    /* ################################################################# */
    /* ####################   MOVE in tab Widget   #################### */
    /* ################################################################# */

    tab2->setObjectName(QString::fromUtf8("tab2_3"));
    tab2->setEnabled(false);

    QPixmap pixmap1("IMG/icons/up.xpm");
    QPixmap pixmap2("IMG/icons/down.xpm");
    QPixmap pixmap3("IMG/icons/left.xpm");
    QPixmap pixmap4("IMG/icons/right.xpm");


    buttonTab3[0] = new QPushButton(tab2);   // Move X
    buttonTab3[1] = new QPushButton(tab2);   // Move Y
    buttonTab3[2] = new QPushButton(tab2);   // Move Z
    buttonTab3[3] = new QPushButton(pixmap4, "", tab2);  // Right
    buttonTab3[4] = new QPushButton(pixmap3, "", tab2);   // Left
    buttonTab3[5] = new QPushButton(pixmap2, "", tab2);   // Down
    buttonTab3[6] = new QPushButton(pixmap1, "", tab2);     // Up
    buttonTab3[7] = new QPushButton(tab2);         // Backwards
    buttonTab3[8] = new QPushButton(tab2);          // Forwards

    buttonTab3[7]->setText("Backward");
    buttonTab3[8]->setText("Forward");

    /* DoubleSpinBoxes general properties */
    using namespace widgets;
    for (auto &ptr : dspinboxes)
    {
        ptr = new QDoubleSpinBox(tab2);
    }

    /* DoubleSpinBoxes specific properties */
    //spinboxTab3[0] = new QDoubleSpinBox(tab3);
    //spinboxTab3[1] = new QDoubleSpinBox(tab3);
    //spinboxTab3[2] = new QDoubleSpinBox(tab2);
    //spinboxTab3[0]->setPrefix("X: ");
    //spinboxTab3[1]->setPrefix("Y: ");
    //spinboxTab3[2]->setPrefix("Z: ");
    dspinboxes.at(spinboxes::targetx)->setPrefix("X: ");
    dspinboxes.at(spinboxes::targety)->setPrefix("Y: ");
    dspinboxes.at(spinboxes::targetz)->setPrefix("Z: ");

    QGridLayout *moveTabLayout = new QGridLayout(tab2);
    moveTabLayout->setHorizontalSpacing(20);
    moveTabLayout->setVerticalSpacing(10);

    QGridLayout *fineLayout = new QGridLayout();
    fineLayout->setHorizontalSpacing(20);
    fineLayout->setVerticalSpacing(10);

    fineLayout->addWidget(buttonTab3[3], 1, 2);
    fineLayout->addWidget(buttonTab3[4], 1, 0);
    fineLayout->addWidget(buttonTab3[5], 1, 1);
    fineLayout->addWidget(buttonTab3[6], 0, 1);

    for (int i = 0; i < 3; i++) {
        buttonTab3[i]->setText("Confirm");
        dspinboxes.at(i)->setDecimals(3);
        dspinboxes.at(i)->setSingleStep(0.5);
//        spinboxTab3[i]->setDecimals(3);
//        spinboxTab3[i]->setSingleStep(0.5);
        if (i < 2) {
            dspinboxes.at(i)->setMaximum(200);
            dspinboxes.at(i)->setValue(100);
            //spinboxTab3[i]->setMaximum(200);
            //spinboxTab3[i]->setValue(100);
        }
        else {
            dspinboxes.at(i)->setMaximum(50);
            dspinboxes.at(i)->setValue(25);
            //spinboxTab3[i]->setMaximum(50);
            //spinboxTab3[i]->setValue(25);
        }
        dspinboxes.at(i)->setFixedWidth(220);
        dspinboxes.at(i)->setSuffix(" mm");
        //spinboxTab3[i]->setFixedWidth(220);
        //spinboxTab3[i]->setSuffix(" mm");

        //connect(spinboxTab3[i], SIGNAL(valueChanged(double)), this, SLOT(tab3_set_target()));
        connect(dspinboxes.at(i), SIGNAL(valueChanged(double)), this, SLOT(tab3_set_target()));

        //moveTabLayout->addWidget(spinboxTab3[i], i + 2, 1, 1, 2);
        moveTabLayout->addWidget(dspinboxes.at(i), i + 2, 1, 1, 2);
        moveTabLayout->addWidget(buttonTab3[i], i + 2, 3);
    }


    QLabel *labelTab3a = new QLabel();
    QLabel *labelTab3b = new QLabel();
    QLabel *labelTab3c = new QLabel();
    labelTab3a->setText("<b>Specify stage position</b>");
    labelTab3b->setText("<b>Fine stage adjustments</b>");
    labelTab3c->setText("<b><center>Manual Stage Control</center></b>");

    moveTabLayout->addWidget(labelTab3a, 1, 1, 1, 2);
    moveTabLayout->addWidget(labelTab3b, 5, 1, 1, 2);
    moveTabLayout->addWidget(labelTab3c, 0, 0, 1, 5);
    moveTabLayout->addLayout(fineLayout, 6, 1, 2, 2);
    moveTabLayout->addWidget(buttonTab3[7], 8, 1);
    moveTabLayout->addWidget(buttonTab3[8], 8, 2);
    moveTabLayout->setColumnStretch(0, 3);
    moveTabLayout->setColumnStretch(4, 3);


    /* ################################################################# */
    /* ####################   SCAN in tab Widget   #################### */
    /* ################################################################# */

    tab3->setObjectName(QString::fromUtf8("tab2_4"));
    tab3->setEnabled(true);


    QGridLayout *scanTabLayout = new QGridLayout(tab3);
    tab4_start_scan = new QPushButton();

    QLabel *scanLabel1 = new QLabel(tab3);
    QLabel *scanLabel2 = new QLabel(tab3);
    QLabel *scanLabel3 = new QLabel(tab3);
    scanLabel1->setText("<b>Scan area extrema</b>");
    scanLabel2->setText("<b>Pixel dimensions</b>");
    scanLabel3->setText("<b>Speed of scanning stages</b");


    for (int i = 0; i < 4; i++)
    {
        scan_params[i] = new QDoubleSpinBox(tab3);
        scan_params[i]->setDecimals(2);
        scan_params[i]->setMaximum(200);
        scan_params[i]->setSingleStep(0.5);
        scan_params[i]->setValue(100);
        scan_params[i]->setSuffix(" mm");
        connect(scan_params[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::time_monitor);
    }

    scan_params[0]->setPrefix("Min. X: ");
    scan_params[1]->setPrefix("Max. X: ");
    scan_params[2]->setPrefix("Min. Y: ");
    scan_params[3]->setPrefix("Max. Y: ");

    for (int i = 4; i < 6; i++) {
        scan_params[i] = new QDoubleSpinBox(tab3);
        scan_params[i]->setDecimals(2);
        scan_params[i]->setMaximum(50);
        scan_params[i]->setSingleStep(0.5);
        scan_params[i]->setValue(1);
        scan_params[i]->setSuffix(" mm");
        connect(scan_params[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::time_monitor);

    }

    scan_params[4]->setPrefix("Step X: ");
    scan_params[5]->setPrefix("Step Y: ");

    tab4_start_scan->setText("Start\nScan");
    tab4_start_scan->setMinimumHeight(60);
    tab4_start_scan->setStyleSheet("QPushButton {background-color: #02757D; font-weight: bold; color: white;}");

    scan_params[6] = new QDoubleSpinBox(tab3);
    scan_params[6]->setValue(1);
    scan_params[6]->setDecimals(2);
    scan_params[6]->setMaximum(10);
    scan_params[6]->setSingleStep(0.5);
    scan_params[6]->setSuffix(" mm/s");
    connect(scan_params[6], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::time_monitor);


    scanTabLayout->setHorizontalSpacing(20);
    scanTabLayout->setVerticalSpacing(10);
    scanTabLayout->addWidget(scanLabel1, 0, 0, 1, 3);
    scanTabLayout->addWidget(scanLabel2, 5, 0, 1, 2);
    scanTabLayout->addWidget(scanLabel3, 5, 2, 1, 2);
    scanTabLayout->addWidget(scan_params[0], 1, 0, 1, 3, Qt::AlignLeft);
    scanTabLayout->addWidget(scan_params[1], 2, 0, 1, 3, Qt::AlignLeft);
    scanTabLayout->addWidget(scan_params[2], 3, 0, 1, 3, Qt::AlignLeft);
    scanTabLayout->addWidget(scan_params[3], 4, 0, 1, 3, Qt::AlignLeft);
    scanTabLayout->addWidget(scan_params[4], 6, 0, 1, 2);
    scanTabLayout->addWidget(scan_params[5], 7, 0, 1, 2);
    scanTabLayout->addWidget(scan_params[6], 6, 2, 1, 2);
    scanTabLayout->addWidget(tab4_start_scan, 1, 3, 2, 1);

    /*******************************************************************/
    /********* TAB WIDGET. Contains all tabs and their children ********/
    /*******************************************************************/

    tabWidget->addTab(tab1, "Devices Assignment");
    tabWidget->addTab(tab2, "Move Stages");
    tabWidget->addTab(tab3, "Scan");

    tabWidget->adjustSize();
    tabWidget->setFixedSize(tabWidget->size());

    /* ################################################################# */
    /* ####################  DAQ BOX ################################### */
    /* ################################################################# */


    QGroupBox *groupBox = new QGroupBox("DAQ && Scan Settings", this);

    QComboBox *daq_combo = new QComboBox(groupBox);
    daq_combo->insertItems(0, QStringList()<<"Optical"<<"USB");
    daq_combo->setCurrentIndex(1);

    QVBoxLayout *daq_layout = new QVBoxLayout(groupBox);
    daq_layout->addWidget(daq_combo);

    groupBox->setFlat(true);
    groupBox->adjustSize();
    groupBox->setFixedSize(groupBox->size());

    connect(daq_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::USB_DAQ);

    /* ################################################################# */
    /* ####################   MONITORS BOX   ########################### */
    /* ################################################################# */

    QGroupBox *monitorsBox = new QGroupBox("Status Monitors");
    QVBoxLayout *monitorsLayout = new QVBoxLayout(monitorsBox);
    //monitorsBox->setAlignment(Qt::AlignCenter);
    //monitorsBox->setFlat(false);


    QString temp_msg[] = { "Stage X: ", "Stage Y: ", "Stage Z: ", "Servo: " };
    QString stylesheet1 = "QLineEdit {background-color: #7C0A02; font-weight: bold; color: white;}";


    //QFontMetrics for correct width of QLineEdit
    QFont font("", 0);
    QFontMetrics fm(font);
    QString metric(22, 'a');
    int length = fm.width(metric);

    for (int i = 0; i < 4; i++) {
        dev_monitor[i] = new QLineEdit(monitorsBox);
        dev_monitor[i]->setEnabled(false);
        dev_monitor[i]->setStyleSheet(stylesheet1);
        dev_monitor[i]->setText(temp_msg[i] + "Disconnected");
        dev_monitor[i]->setMinimumWidth(length);

        monitorsLayout->addWidget(dev_monitor[i]);
    }
    dev_monitor[4] = new QLineEdit(monitorsBox);
    dev_monitor[4]->setEnabled(false);
    dev_monitor[4]->setText("DAQ time: ");
    monitorsLayout->addWidget(dev_monitor[4]);


    tab1_stop = new QPushButton(centralWidget);
    tab1_stop->setText("STOP");
    tab1_stop->setMinimumHeight(60);
    tab1_stop->setMaximumWidth(60);
    tab1_stop->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    monitorsLayout->addWidget(tab1_stop);
    monitorsBox->setFlat(true);
    monitorsBox->adjustSize();
    monitorsBox->setFixedSize(monitorsBox->size());



//#ifndef QT_NO_STATUSBAR
//    status = statusBar();
//    status->setAutoFillBackground(true);
//    status->showMessage("Ready");
//#endif

    QDockWidget* dock = new QDockWidget(centralWidget);
    QWidget* dock_layout_container = new QWidget(dock);
    QVBoxLayout* dock_layout = new QVBoxLayout(dock_layout_container);

    /* General layout */
    QComboBox* dock_select = new QComboBox;
    dock_select->addItem("XRF map");
    dock_select->addItem("XRF spectra");

    QStackedWidget* dock_stack = new QStackedWidget(dock);
    connect(dock_select, QOverload<int>::of(&QComboBox::activated),
            dock_stack, &QStackedWidget::setCurrentIndex);

    dock_layout->addWidget(dock_select);
    dock_layout->addWidget(dock_stack);

    QWidget* dock_tab1 = new QWidget(dock_stack);
    QVBoxLayout* dlayout_tab1 = new QVBoxLayout(dock_tab1);

    QWidget* dock_tab2 = new QWidget(dock_stack);
    QVBoxLayout* dlayout_tab2 = new QVBoxLayout(dock_tab2);
    QLabel* dock_label = new QLabel("IN CONSTRUCTION");


    scrollArea = new QScrollArea(dock_tab1);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setMinimumSize(450, 450);

    dlayout_tab1->addWidget(scrollArea);
    dlayout_tab2->addWidget(dock_label, Qt::AlignCenter);
    dock_stack->addWidget(dock_tab1);
    dock_stack->addWidget(dock_tab2);
    dock ->setWidget(dock_layout_container);
    dock ->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dock ->setAllowedAreas(Qt::AllDockWidgetAreas);


    // Debug console
    auto debug_console_groupbox = new QGroupBox{ tr("Debug Console") };
    auto debug_console_layout = new QVBoxLayout{};

    debug_console_ = new QTextEdit{};
    debug_console_layout->addWidget(debug_console_);
    //debug_console_layout->addStretch(1);
    debug_console_groupbox->setLayout(debug_console_layout);
    debug_console_groupbox->setFlat(true);

    //layoutMaster->addWidget(dock_map, 0, 4, 4, 1, Qt::AlignTop);
    layoutMaster->addWidget(tabWidget, 0, 1, 3, 2, Qt::AlignTop);
    layoutMaster->addWidget(monitorsBox, 0, 0, 2, 1);
    layoutMaster->addWidget(groupBox, 2, 0, Qt::AlignTop);
    layoutMaster->addWidget(debug_console_groupbox, 3, 0, 1, 3);
    addDockWidget(Qt::RightDockWidgetArea, dock);


    size = centralWidget->sizeHint();
    centralWidget->setFixedSize(size);
}

void MainWindow::time_monitor(double val)
{
    auto number_lines = 1. + (scan_params[3]->value() - scan_params[2]->value()) / scan_params[5]->value();
    auto time_per_line = (scan_params[1]->value() - scan_params[0]->value()) / scan_params[6]->value();
    auto total = static_cast<int>(number_lines * time_per_line);
    QString stotal(QString::fromStdString(to_string(total)));

    dev_monitor[4]->setText(stotal);
}
