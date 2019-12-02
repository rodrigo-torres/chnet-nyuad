#include "DppGui.h"

void MainWindowDPP::creatorDppGui() {
    QWidget *mywidget= new QWidget;
    setCentralWidget(mywidget);

    QFrame *line1 = new QFrame(mywidget);
    line1->setGeometry(QRect(100, 100, 300, 300));
    line1->setFrameShape(QFrame::StyledPanel);
    line1->setFrameShadow(QFrame::Plain);

    QCheckBox *checkBox = new QCheckBox(mywidget);
    checkBox->setEnabled(true);
    checkBox->setCheckable(true);
    checkBox->setText("Override coded parameters");

    QLabel *labelcombo = new QLabel(mywidget);
    labelcombo->setText("Input Range (V)");

    QComboBox *comboBox = new QComboBox(mywidget);
    comboBox->insertItems(0, QStringList()<<"0.6V"<<"1.4V"<<"3.7V"<<"9.5V");

    QLabel *label = new QLabel(mywidget);
    label->setText("Threshold");

    QSpinBox *spinBox = new QSpinBox(mywidget);
    spinBox->setValue(150);
    spinBox->setMaximum(1000);

    QLabel *label2 = new QLabel(mywidget);
    label2->setText("DC OffSet");

    QSpinBox *spinBox2 = new QSpinBox(mywidget);
    spinBox2->setMinimum(0);
    spinBox2->setMaximum(100);
    spinBox2->setValue(0);

    QLabel *label3 = new QLabel(mywidget);
    label3->setText("Holdoff (ns)");

    QSpinBox *spinBox3 = new QSpinBox(mywidget);
    spinBox3->setMaximum(20000);
    spinBox3->setValue(1200);

    QLabel *labelcombo2 = new QLabel(mywidget);
    labelcombo2->setText("Smoothing Factor");

    QComboBox *comboBox2 = new QComboBox(mywidget);
    comboBox2->insertItems(0, QStringList()<<"1"<<"2"<<"4"<<"8"<<"16"<<"32");

    QLabel *label4 = new QLabel(mywidget);
    label4->setText("Rise Time (ns)");

    QSpinBox *spinBox4 = new QSpinBox(mywidget);
    spinBox4->setMaximum(10000);
    spinBox4->setValue(2000);


    QLabel *label5 = new QLabel(mywidget);
    label5->setText("Decay Time (ns)");

    QSpinBox *spinBox5 = new QSpinBox(mywidget);
    spinBox5->setMaximum(10000);
    spinBox5->setValue(6000);

    QLabel   *label6 = new QLabel(mywidget);
    label6->setText("Flat Top (ns)");

    QSpinBox *spinBox6 = new QSpinBox(mywidget);
    spinBox6->setMaximum(10000);
    spinBox6->setValue(2000);

    QLabel *label7 = new QLabel(mywidget);
    label7->setText("Peaking delay (ns)");

    QSpinBox *spinBox7 = new QSpinBox(mywidget);
    spinBox7->setMaximum(6000);
    spinBox7->setValue(1500);


    QLabel *labelcombo3 = new QLabel(mywidget);
    labelcombo3->setText("Base Line Mean");

    QComboBox *comboBox3 = new QComboBox(mywidget);
    comboBox3->insertItems(0, QStringList()<<"0"<<"16"<<"64"<<"256"<<"1024"<<"4096"<<"16384");

    QLabel   *label8 = new QLabel(mywidget);
    label8->setText("Delay(b) (ns)");

    QSpinBox *spinBox8 = new QSpinBox(mywidget);
    spinBox8->setMaximum(500);
    spinBox8->setValue(100);

    QLabel *labelcombo4 = new QLabel(mywidget);
    labelcombo4->setText("Peak Mean");

    QComboBox *comboBox4 = new QComboBox(mywidget);
    comboBox4->insertItems(0, QStringList()<<"1"<<"4"<<"16"<<"64");

    QLabel *label9 = new QLabel(mywidget);
    label9->setText("Peak Holdoff (ns)");

    QSpinBox *spinBox9 = new QSpinBox(mywidget);
    spinBox9->setMaximum(16000); // Peak Holdoff
    spinBox9->setValue(2700);

    QLabel *label10 = new QLabel(mywidget);
    label10->setText("Baseline Holdoff (ns)");

    QSpinBox *spinBox10 = new QSpinBox(mywidget);
    spinBox10->setMaximum(10000);
    spinBox10->setValue(3100);

    /*          GEOMETRY            */
    labelcombo->setGeometry(QRect(10, 20, 140, 12));
    comboBox->setGeometry(QRect(170, 15, 100, 22));

    label->setGeometry(QRect(10, 55, 180, 12));
    spinBox->setGeometry(QRect(170, 50, 100, 20));
    label2->setGeometry(QRect(10, 85, 180, 12));
    spinBox2->setGeometry(QRect(170, 90, 100, 20));
    label3->setGeometry(QRect(10, 135, 180, 12));
    spinBox3->setGeometry(QRect(170, 130, 100, 20));

    labelcombo2->setGeometry(QRect(10, 175, 140, 12));
    comboBox2->setGeometry(QRect(170, 170, 100, 22));

    label4->setGeometry(QRect(310, 20, 180, 12));
    spinBox4->setGeometry(QRect(470, 15, 100, 20));

    label5->setGeometry(QRect(310, 55, 180, 12));
    spinBox5->setGeometry(QRect(470, 50, 100, 20));

    label6->setGeometry(QRect(310, 85, 180, 12));
    spinBox6->setGeometry(QRect(470, 90, 100, 20));

    label7->setGeometry(QRect(310, 135, 180, 12));
    spinBox7->setGeometry(QRect(470, 130, 100, 20));

    labelcombo3->setGeometry(QRect(310, 175, 140, 12));
    comboBox3->setGeometry(QRect(470, 175, 100, 22));

    label8->setGeometry(QRect(310, 215, 180, 12));
    spinBox8->setGeometry(QRect(470, 210, 100, 20));

    labelcombo4->setGeometry(QRect(310, 255, 140, 12));
    comboBox4->setGeometry(QRect(470, 250, 100, 22));

    label9->setGeometry(QRect(310, 295, 180, 12));
    spinBox9->setGeometry(QRect(470, 290, 100, 20));

    label10->setGeometry(QRect(310, 335, 180, 12));
    spinBox10->setGeometry(QRect(470, 330, 100, 20));


    checkBox->setGeometry(QRect(10, 270, 225, 25));

    connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(overrideDppParam(bool)));
    connect(comboBox, SIGNAL(activated(int)), this, SLOT(Digi_range(int)));
    connect(comboBox2, SIGNAL(activated(int)), this, SLOT(Smoothing_Factor(int)));
    connect(comboBox3, SIGNAL(activated(int)), this, SLOT(Base_Line_Mean(int)));
    connect(comboBox4, SIGNAL(activated(int)), this, SLOT(Peak_Mean(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(Digi_treshold(int)));
    connect(spinBox2, SIGNAL(valueChanged(int)), this, SLOT(paramDCOffset(int)));
    connect(spinBox3, SIGNAL(valueChanged(int)), this, SLOT(HoldOff(int)));
    connect(spinBox4, SIGNAL(valueChanged(int)), this, SLOT(Rise_Time(int)));
    connect(spinBox5, SIGNAL(valueChanged(int)), this, SLOT(Fall_Time(int)));
    connect(spinBox6, SIGNAL(valueChanged(int)), this, SLOT(Flat_Top(int)));
    connect(spinBox7, SIGNAL(valueChanged(int)), this, SLOT(Peaking_Delay(int)));
    connect(spinBox8, SIGNAL(valueChanged(int)), this, SLOT(Trapezoid_Gain(int)));
    connect(spinBox9, SIGNAL(valueChanged(int)), this, SLOT(Peak_Holdoff(int)));


}


