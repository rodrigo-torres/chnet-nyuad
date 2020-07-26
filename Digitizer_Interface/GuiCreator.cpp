#include "mainwindow.h"
#include <QtGui>
#include <QSpinBox>

extern int Resolution_mode;

void MainWindow::GuiCreator()
{

    QWidget *mywidget= new QWidget;
    setCentralWidget(mywidget);


    QFrame *line1 = new QFrame(mywidget);
    line1->setObjectName(QString::fromUtf8("line1"));
    line1->setGeometry(QRect(100, 100, 300, 300));
    line1->setFrameShape(QFrame::StyledPanel);
    line1->setFrameShadow(QFrame::Plain);



    QCheckBox *checkBox = new QCheckBox(mywidget);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));
    checkBox->setEnabled(true);
    checkBox->setCheckable(true);
    checkBox->setText("  Allow reading from digitiser"); 



//////////////// INPUT SIGNAL/////////////////////////////


    QLabel   *labelcombo = new QLabel(mywidget);
    labelcombo->setText("Input Range (V)");
    QComboBox *comboBox = new QComboBox(mywidget); ////// INPUT RANGE
    comboBox->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "0.6V", 0)
         << QApplication::translate("mywidget", "1.4V", 0)
         << QApplication::translate("mywidget", "3.7V", 0)
         << QApplication::translate("mywidget", "9.5V", 0)
        );


///// TRIGGER LEVEL
    QLabel   *label = new QLabel(mywidget);
    label->setText("Treshold");
    QSpinBox *spinBox = new QSpinBox(mywidget);
    spinBox->setMaximum(255); // TRIGGER
    spinBox->setValue(40);


///// DC OFFSET
    QLabel   *label2 = new QLabel(mywidget);
    label2->setText("DC OffSet");
    QSpinBox *spinBox2 = new QSpinBox(mywidget);
    spinBox2->setMaximum(1600); // DC OFFSET
    spinBox2->setMinimum(-1600); // 
    spinBox2->setValue(0);

///// HOLDOFF
    QLabel   *label3 = new QLabel(mywidget);
    label3->setText("Hold Off (ns)");
    QSpinBox *spinBox3 = new QSpinBox(mywidget);
    spinBox3->setMaximum(64000); //HOLDOFF
    spinBox3->setValue(1200);

///// INPUT SMOOTHING
    QLabel   *labelcombo2 = new QLabel(mywidget);
    labelcombo2->setText("Smoothing Factor");
    QComboBox *comboBox2 = new QComboBox(mywidget); ////// SMOOTHING FACTOR
    comboBox2->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "1", 0)
         << QApplication::translate("mywidget", "2", 0)
         << QApplication::translate("mywidget", "4", 0)
         << QApplication::translate("mywidget", "8", 0)
         << QApplication::translate("mywidget", "16", 0)
         << QApplication::translate("mywidget", "32", 0)
        );

//baseline mean 64 256 1024, trap gain (0.1 10) peak mean (1 4 16) peak holdhoff (ns)

/////////////////////////////////////// TRAPEZOID SECTION

    QLabel   *label4 = new QLabel(mywidget);
    label4->setText("Rise Time (ns)");
    QSpinBox *spinBox4 = new QSpinBox(mywidget);
    spinBox4->setMaximum(6400); // Rise Time
    spinBox4->setValue(800);


    QLabel   *label5 = new QLabel(mywidget);
    label5->setText("Fall Time (ns)");
    QSpinBox *spinBox5 = new QSpinBox(mywidget);
    spinBox5->setMaximum(10000); // Fall Time
    spinBox5->setValue(12000);


    QLabel   *label6 = new QLabel(mywidget);
    label6->setText("Flat Top (ns)");
    QSpinBox *spinBox6 = new QSpinBox(mywidget);
    spinBox6->setMaximum(6400); // Flat Top
    spinBox6->setValue(2500);


    QLabel   *label7 = new QLabel(mywidget);
    label7->setText("Peaking delay (ns)");
    QSpinBox *spinBox7 = new QSpinBox(mywidget);
    spinBox7->setMaximum(6400); // Peaking delay
    spinBox7->setValue(1500);


    QLabel   *labelcombo3 = new QLabel(mywidget);
    labelcombo3->setText("Base Line Mean");
    QComboBox *comboBox3 = new QComboBox(mywidget); ////// Base Line Mean
    comboBox3->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "16", 0)
         << QApplication::translate("mywidget", "64", 0)
         << QApplication::translate("mywidget", "256", 0)
         << QApplication::translate("mywidget", "1024", 0)
        );


    QLabel   *label8 = new QLabel(mywidget);
    label8->setText("Trapeziod Gain");
    QSpinBox *spinBox8 = new QSpinBox(mywidget);
    spinBox8->setMaximum(10); // Trapezoid Gain
    spinBox8->setValue(1);


    QLabel   *labelcombo4 = new QLabel(mywidget);
    labelcombo4->setText("Peak Mean");
    QComboBox *comboBox4 = new QComboBox(mywidget); ////// PEAK MEAN
    comboBox4->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "1", 0)
         << QApplication::translate("mywidget", "4", 0)
         << QApplication::translate("mywidget", "16", 0)
         << QApplication::translate("mywidget", "64", 0)
        );


    QLabel   *label9 = new QLabel(mywidget);
    label9->setText("Peak holdoff (ns)");
    QSpinBox *spinBox9 = new QSpinBox(mywidget);
    spinBox9->setMaximum(16000); // Peak Holdoff
    spinBox9->setValue(2700);


// peak holdhoff (ns)






    QPushButton *button = new QPushButton(mywidget);
    QPushButton *button2 = new QPushButton(mywidget);
    button->setText("Restart DAQ");
    button2->setText("quit");

// GEOMETRY ///////////////////////////////////////////////////////////////////

//qDebug()<<Resolution_mode;

   switch (Resolution_mode)
   {
    case(0):
    {
/////INPUT
    labelcombo->setGeometry(QRect(20, 40, 280, 24));
    comboBox->setGeometry(QRect(340, 30, 200, 44));

    label->setGeometry(QRect(20, 110, 360, 24));
    spinBox->setGeometry(QRect(340, 100, 200, 40));
    label2->setGeometry(QRect(20, 190, 360, 24));
    spinBox2->setGeometry(QRect(340, 180, 200, 40));
    label3->setGeometry(QRect(20, 270, 360, 24));
    spinBox3->setGeometry(QRect(340, 260, 200, 40));

    labelcombo2->setGeometry(QRect(20, 350, 280, 24));
    comboBox2->setGeometry(QRect(340, 340, 200, 44));

/////TRAPEZOID //vertical align: 40-110-190-280-360-430-510

    label4->setGeometry(QRect(620, 40, 360, 24));
    spinBox4->setGeometry(QRect(940, 30, 200, 40));

    label5->setGeometry(QRect(620, 110, 360, 24));
    spinBox5->setGeometry(QRect(940, 100, 200, 40));

    label6->setGeometry(QRect(620, 190, 360, 24));
    spinBox6->setGeometry(QRect(940, 180, 200, 40));

    label7->setGeometry(QRect(620, 270, 360, 24));
    spinBox7->setGeometry(QRect(940, 260, 200, 40));

    labelcombo3->setGeometry(QRect(620, 350, 280, 24));
    comboBox3->setGeometry(QRect(940, 340, 200, 44));

    label8->setGeometry(QRect(620, 430, 360, 24));
    spinBox8->setGeometry(QRect(940, 420, 200, 40));

    labelcombo4->setGeometry(QRect(620, 510, 280, 24));
    comboBox4->setGeometry(QRect(940, 500, 200, 44));

    label9->setGeometry(QRect(620, 590, 360, 24));
    spinBox9->setGeometry(QRect(940, 580, 200, 40));


    checkBox->setGeometry(QRect(90, 450, 450, 50));

    button->setGeometry(QRect(100, 520, 400, 80));
    button2->setGeometry(QRect(20, 670, 1120, 56));
    break;
    }

    case(1): ////////// LOW RESOLUTION CASES
    {
    labelcombo->setGeometry(QRect(15, 30, 210, 18));
    comboBox->setGeometry(QRect(255, 22, 150, 33));

    label->setGeometry(QRect(15, 77, 270, 18));
    spinBox->setGeometry(QRect(255, 75, 150, 30));
    label2->setGeometry(QRect(15, 127, 270, 18));
    spinBox2->setGeometry(QRect(255, 135, 150, 30));
    label3->setGeometry(QRect(15, 202, 270, 18));
    spinBox3->setGeometry(QRect(255, 195, 150, 30));

    labelcombo2->setGeometry(QRect(15, 262, 210, 18));
    comboBox2->setGeometry(QRect(255, 255, 150, 33));

    label4->setGeometry(QRect(465, 30, 270, 18));
    spinBox4->setGeometry(QRect(705, 22, 150, 30));

    label5->setGeometry(QRect(465, 77, 270, 18));
    spinBox5->setGeometry(QRect(705, 75, 150, 30));

    label6->setGeometry(QRect(465, 127, 270, 18));
    spinBox6->setGeometry(QRect(705, 135, 150, 30));

    label7->setGeometry(QRect(465, 202, 270, 18));
    spinBox7->setGeometry(QRect(705, 195, 150, 30));

    labelcombo3->setGeometry(QRect(465, 262, 210, 18));
    comboBox3->setGeometry(QRect(705, 510, 300, 66));

    label8->setGeometry(QRect(465, 322, 270, 18));
    spinBox8->setGeometry(QRect(705, 315, 150, 30));

    labelcombo4->setGeometry(QRect(465, 382, 210, 18));
    comboBox4->setGeometry(QRect(705, 375, 150, 33));

    label9->setGeometry(QRect(465, 442, 270, 18));
    spinBox9->setGeometry(QRect(705, 435, 150, 30));


    checkBox->setGeometry(QRect(67, 337, 337, 37));

    button->setGeometry(QRect(75, 390, 300, 60));
    button2->setGeometry(QRect(15, 502, 840, 42));
    break;
    }

    case(2):
    {
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
    comboBox3->setGeometry(QRect(470, 340, 200, 44));

    label8->setGeometry(QRect(310, 215, 180, 12));
    spinBox8->setGeometry(QRect(470, 210, 100, 20));

    labelcombo4->setGeometry(QRect(310, 255, 140, 12));
    comboBox4->setGeometry(QRect(470, 250, 100, 22));

    label9->setGeometry(QRect(310, 295, 180, 12));
    spinBox9->setGeometry(QRect(470, 290, 100, 20));


    checkBox->setGeometry(QRect(45, 225, 225, 25));

    button->setGeometry(QRect(50, 260, 200, 40));
    button2->setGeometry(QRect(10, 335, 560, 28));
    break;
    }
   }


// COONNECTIONS

     connect(spinBox, SIGNAL(valueChanged(int)), 
          this, SLOT(Digi_treshold(int)));

     connect(comboBox, SIGNAL(activated(int)), 
          this, SLOT(Digi_range(int)));

     connect(spinBox2, SIGNAL(valueChanged(int)), 
          this, SLOT(DC_Offset(int)));

     connect(spinBox3, SIGNAL(valueChanged(int)), 
          this, SLOT(HoldOff(int)));

     connect(comboBox2, SIGNAL(activated(int)), 
          this, SLOT(Smoothing_Factor(int)));

     connect(spinBox4, SIGNAL(valueChanged(int)), 
          this, SLOT(Rise_Time(int)));

     connect(spinBox5, SIGNAL(valueChanged(int)), 
          this, SLOT(Fall_Time(int)));

     connect(spinBox6, SIGNAL(valueChanged(int)), 
          this, SLOT(Flat_Top(int)));

     connect(spinBox7, SIGNAL(valueChanged(int)), 
          this, SLOT(Peaking_Delay(int)));



     connect(comboBox3, SIGNAL(activated(int)), 
          this, SLOT(Base_Line_Mean(int)));

     connect(spinBox8, SIGNAL(valueChanged(int)), 
          this, SLOT(Trapezoid_Gain(int)));

     connect(comboBox4, SIGNAL(activated(int)), 
          this, SLOT(Peak_Mean(int)));

     connect(spinBox9, SIGNAL(valueChanged(int)), 
          this, SLOT(Peak_Holdoff(int)));

     connect(checkBox, SIGNAL(clicked(bool)),
            this, SLOT(Digitiser_data_download_enable_ON(bool)));



     connect(button2, SIGNAL(clicked()), this, SLOT(Exit())); ///aggiungere check se è fermo qApp, SLOT(quit())




}



