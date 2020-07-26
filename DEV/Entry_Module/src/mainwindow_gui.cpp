/** MAXRF_Main: Main point of access to control the MAXRF instrument
 *  Copyright (C) 2020 Rodrigo Torres and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

void MAXRF_UI::CreateGUI() {
    using namespace widgets;
    auto total_pushbuttons = index_of(pushbuttons::TOTAL_NO);
    buttons.resize(total_pushbuttons);

    QSize size;


    centralWidget = new QWidget();
    setCentralWidget(centralWidget);


    QTabWidget* tabWidget = new QTabWidget(centralWidget);
    tab1 = new QWidget(tabWidget);
    tab2 = new QWidget(tabWidget);

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

    QPixmap pixmap1(":/icons/up.png");
    QPixmap pixmap2(":/icons/down.png");
    QPixmap pixmap3(":/icons/left.png");
    QPixmap pixmap4(":/icons/right.png");


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

    /*******************************************************************/
    /********* TAB WIDGET. Contains all tabs and their children ********/
    /*******************************************************************/

    tabWidget->addTab(tab1, "Devices Assignment");
    tabWidget->addTab(tab2, "Move Stages");
    tabWidget->adjustSize();
    tabWidget->setFixedSize(tabWidget->size());


    /* ################################################################# */
    /* ####################   MONITORS BOX   ########################### */
    /* ################################################################# */

    QGroupBox *monitorsBox = new QGroupBox("Status Monitors");
    QVBoxLayout *monitorsLayout = new QVBoxLayout(monitorsBox);
    //monitorsBox->setAlignment(Qt::AlignCenter);
    //monitorsBox->setFlat(false);


    QString temp_msg[] = { "Stage X: ", "Stage Y: ", "Stage Z: ", "Servo: " };
    QString stylesheet1 = "QLineEdit {background-color: #7C0A02; font-weight: bold; color: white;}";
    QFontMetrics fm { QFont {"Helvetica"} };
    int length = fm.averageCharWidth() * 20;

    for (int i = 0; i < 4; i++) {
        dev_monitor[i] = new QLineEdit(monitorsBox);
        dev_monitor[i]->setEnabled(false);
        dev_monitor[i]->setStyleSheet(stylesheet1);
        dev_monitor[i]->setText(temp_msg[i] + "Disconnected");
        dev_monitor[i]->setMinimumWidth(length);

        monitorsLayout->addWidget(dev_monitor[i]);
    }


    tab1_stop = new QPushButton(centralWidget);
    tab1_stop->setText("STOP");
    tab1_stop->setMinimumHeight(60);
    tab1_stop->setMaximumWidth(60);
    tab1_stop->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    monitorsLayout->addWidget(tab1_stop);
    monitorsBox->setFlat(true);
    monitorsBox->adjustSize();
    monitorsBox->setFixedSize(monitorsBox->size());


    layoutMaster->addWidget(tabWidget, 0, 1, 3, 2, Qt::AlignTop);
    layoutMaster->addWidget(monitorsBox, 0, 0, 2, 1);

    size = centralWidget->sizeHint();
    //centralWidget->setFixedSize(size);
}

void MAXRF_UI::CreateConnections() {

    connect(tab1_stop, &QPushButton::clicked, this, &MAXRF_UI::set_abort_flag);


    connect(laser_checkbox, SIGNAL(clicked()), this, SLOT(enable_keyence_reading()));
    connect(servo_checkbox, SIGNAL(clicked()), this, SLOT(enable_servo()));


    /* Tab 1 connections */
    auto ref = widgets::pushbuttons::openttyx;
    for (auto &i : {0, 1, 2, 3, 4, 5, 6})
        connect(buttons[ref + i], &QPushButton::released, this, &MAXRF_UI::handle_pushbuttons);

}

void MAXRF_UI::handle_pushbuttons()
{
  using std::to_string;

    /* Identify the sender */
    auto *p_sender = static_cast<QPushButton*>(this->sender());
    auto i = get_index(buttons, p_sender);
    std::string s;

    switch (i)
    {
    case 0: case 1: case 2:
        /* Prepare message */
        s = to_string(i) + " /dev/ttyUSB" + to_string(tab1_df_number[i]->value());

        /* Send signal to device handler class */
        emit request_tty_action(1, QString::fromStdString(s));
        break;
    case 3:
        /* Prepare message */
        s = to_string(i) + " /dev/ttyACM" + to_string(tab1_df_number[i]->value());

        /* Send signal to device handler class */
        emit request_tty_action(1, QString::fromStdString(s));
        break;
    case 4:
    case 5:
    case 6:
        s = to_string(i - 4);
        emit request_tty_action(2, QString::fromStdString(s));
        break;
    default:
        break;
    }
}

