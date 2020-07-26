#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

extern int Resolution_mode;

void MainWindow::GUI_CREATOR()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    imageLabel = new ImgLabel;
    imageLabel1 = new ImgLabel;
    imageLabel2 = new ImgLabel;
    imageLabel3 = new ImgLabel;

    QImage image("IMG/TT_CHNet_extended_395_395_3");
    QImage image1("IMG/TT_CHNet_extended_395_395_3");
    QImage image2("IMG/TT_CHNet_extended_795_795_3");     
    QImage image3("IMG/TT_CHNet_extended_395_395_3");

    switch (Resolution_mode)
     {
      case(0):
              {
	       imageLabel->setPixmap(QPixmap::fromImage(image));
               imageLabel->setBackgroundRole(QPalette::Base);
               imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
               imageLabel->setScaledContents(true);

               imageLabel1->setPixmap(QPixmap::fromImage(image));
               imageLabel1->setBackgroundRole(QPalette::Base);
               imageLabel1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
               imageLabel1->setScaledContents(true);

               imageLabel2->setPixmap(QPixmap::fromImage(image));
               imageLabel2->setBackgroundRole(QPalette::Base);
               imageLabel2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
               imageLabel2->setScaledContents(true);

               imageLabel3->setPixmap(QPixmap::fromImage(image));
               imageLabel3->setBackgroundRole(QPalette::Base);
               imageLabel3->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
               imageLabel3->setScaledContents(true);
               break;
              }

    case(1):
            {
	     imageLabel->setPixmap(QPixmap::fromImage(image1));
             imageLabel->setBackgroundRole(QPalette::Base);
             imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel->setScaledContents(true);

             imageLabel1->setPixmap(QPixmap::fromImage(image1));
             imageLabel1->setBackgroundRole(QPalette::Base);
             imageLabel1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel1->setScaledContents(true);

             imageLabel2->setPixmap(QPixmap::fromImage(image1));
             imageLabel2->setBackgroundRole(QPalette::Base);
             imageLabel2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel2->setScaledContents(true);

             imageLabel3->setPixmap(QPixmap::fromImage(image1));
             imageLabel3->setBackgroundRole(QPalette::Base);
             imageLabel3->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel3->setScaledContents(true);
             break;
            }

    case(2):
            {
	     imageLabel->setPixmap(QPixmap::fromImage(image2));
             imageLabel->setBackgroundRole(QPalette::Base);
             imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel->setScaledContents(true);

             imageLabel1->setPixmap(QPixmap::fromImage(image2));
             imageLabel1->setBackgroundRole(QPalette::Base);
             imageLabel1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel1->setScaledContents(true);

             imageLabel2->setPixmap(QPixmap::fromImage(image2));
             imageLabel2->setBackgroundRole(QPalette::Base);
             imageLabel2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel2->setScaledContents(true);

             imageLabel3->setPixmap(QPixmap::fromImage(image2));
             imageLabel3->setBackgroundRole(QPalette::Base);
             imageLabel3->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
             imageLabel3->setScaledContents(true);
             break;
            }
        }



    scrollArea = new QScrollArea(centralWidget);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    scrollArea1 = new QScrollArea(centralWidget);
    scrollArea1->setBackgroundRole(QPalette::Dark);
    scrollArea1->setWidget(imageLabel1);

    scrollArea2 = new QScrollArea(centralWidget);
    scrollArea2->setBackgroundRole(QPalette::Dark);
    scrollArea2->setWidget(imageLabel2);

    scrollArea3 = new QScrollArea(centralWidget);
    scrollArea3->setBackgroundRole(QPalette::Dark);
    scrollArea3->setWidget(imageLabel3);

      switch (Resolution_mode)
       {
        case(0):
               {scrollArea->setGeometry(QRect(20, 20, 400, 400));
                scrollArea->setMinimumSize(QSize(400, 400));
                scrollArea->setMaximumSize(QSize(400, 400));

                scrollArea1->setGeometry(QRect(440, 20, 400, 400));
                scrollArea1->setMinimumSize(QSize(400, 400));
                scrollArea1->setMaximumSize(QSize(400, 400));

                scrollArea2->setGeometry(QRect(20, 440, 400, 400));
                scrollArea2->setMinimumSize(QSize(400, 400));
                scrollArea2->setMaximumSize(QSize(400, 400));

                scrollArea3->setGeometry(QRect(440, 440, 400, 400));
                scrollArea3->setMinimumSize(QSize(400, 400));
                scrollArea3->setMaximumSize(QSize(400, 400));

                break;
               }

        case(1):
               {scrollArea->setGeometry(QRect(18, 15, 300, 300));
                scrollArea->setMinimumSize(QSize(300, 300));
                scrollArea->setMaximumSize(QSize(300, 300));

                scrollArea1->setGeometry(QRect(330, 15, 300, 300));
                scrollArea1->setMinimumSize(QSize(300, 300));
                scrollArea1->setMaximumSize(QSize(300, 300));

                scrollArea2->setGeometry(QRect(15, 330, 300, 300));
                scrollArea2->setMinimumSize(QSize(300, 300));
                scrollArea2->setMaximumSize(QSize(300, 300));

                scrollArea3->setGeometry(QRect(330, 330, 300, 300));
                scrollArea3->setMinimumSize(QSize(300, 300));
                scrollArea3->setMaximumSize(QSize(300, 300));

                               break;
               }

        case(2): 
               {scrollArea->setGeometry(QRect(10, 10, 200, 200));
                scrollArea->setMinimumSize(QSize(200, 200));
                scrollArea->setMaximumSize(QSize(200, 200));

                scrollArea1->setGeometry(QRect(220, 10, 200, 200));
                scrollArea1->setMinimumSize(QSize(200, 200));
                scrollArea1->setMaximumSize(QSize(200, 200));

                scrollArea2->setGeometry(QRect(10, 220, 200, 200));
                scrollArea2->setMinimumSize(QSize(200, 200));
                scrollArea2->setMaximumSize(QSize(200, 200));

                scrollArea3->setGeometry(QRect(220, 220, 200, 200));
                scrollArea3->setMinimumSize(QSize(200, 200));
                scrollArea3->setMaximumSize(QSize(200, 200));

                break;
               }
       }



    QDockWidget *dock = new QDockWidget(tr("OnLine Map Parameters"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *DockBox = new QWidget(this);

    QGridLayout *layout = new QGridLayout( DockBox );
    layout->addWidget( new QLabel( "Map 1" ), 0,0 );
    layout->addWidget( new QLabel( "Map 2" ), 0,1 );
    layout->addWidget( new QLabel( "Map 3" ), 0,2 );
    layout->addWidget( new QLabel( "Map 4" ), 0,3 );


    QGroupBox *groupBoxLabel = new QGroupBox; //QGroupBox(tr("DAQ Type"));
    QLabel *Co_Label = new QLabel("Co", groupBoxLabel);
    QLabel *K_Label = new QLabel("K", groupBoxLabel);
    QLabel *Ba_Label = new QLabel("Ba", groupBoxLabel);
    QLabel *Ca_Label = new QLabel("Ca", groupBoxLabel);

    QGroupBox *groupBoxLabel1 = new QGroupBox;
    QLabel *Ag_Label = new QLabel("Ag", groupBoxLabel1);
    QLabel *Cr_Label = new QLabel("Cr", groupBoxLabel1);
    QLabel *Cu_Label = new QLabel("Cu", groupBoxLabel1);
    QLabel *PbL_Label = new QLabel("PbL", groupBoxLabel1);

    QGroupBox *groupBoxLabel2 = new QGroupBox;
    QLabel *Au_Label = new QLabel("Au", groupBoxLabel2);
    QLabel *Hg_Label = new QLabel("Hg", groupBoxLabel2);
    QLabel *Si_Label = new QLabel("Si", groupBoxLabel2);
    QLabel *Ti_Label = new QLabel("Ti", groupBoxLabel2);

    QGroupBox *groupBoxLabel3 = new QGroupBox;
    QLabel *Sn_Label = new QLabel("Sn", groupBoxLabel3);
    QLabel *Zn_Label = new QLabel("Zn", groupBoxLabel3);
    QLabel *PbM_Label = new QLabel("PbM", groupBoxLabel3);
    QLabel *Fe_Label = new QLabel("Fe", groupBoxLabel3);




    QGroupBox *groupBox = new QGroupBox;
    QRadioButton *radio_Co = new QRadioButton(groupBox);
    QRadioButton *radio_K = new QRadioButton(groupBox);
    QRadioButton *radio_Ba = new QRadioButton(groupBox);
    QRadioButton *radio_Ca = new QRadioButton(groupBox);

    QGroupBox *groupBox1 = new QGroupBox;
    QRadioButton *radio_Ag = new QRadioButton(groupBox1);
    QRadioButton *radio_Cr = new QRadioButton(groupBox1);
    QRadioButton *radio_Cu = new QRadioButton(groupBox1);
    QRadioButton *radio_PbL = new QRadioButton(groupBox1);

    QGroupBox *groupBox2 = new QGroupBox;
    QRadioButton *radio_Au = new QRadioButton(groupBox2);
    QRadioButton *radio_Hg = new QRadioButton(groupBox2);
    QRadioButton *radio_Si = new QRadioButton(groupBox2);
    QRadioButton *radio_Ti = new QRadioButton(groupBox2);

    QGroupBox *groupBox3 = new QGroupBox;
    QRadioButton *radio_Sn = new QRadioButton(groupBox3);
    QRadioButton *radio_Zn = new QRadioButton(groupBox3);
    QRadioButton *radio_PbM = new QRadioButton(groupBox3);
    QRadioButton *radio_Fe = new QRadioButton(groupBox3);



    switch (Resolution_mode)
     {
      case(0):
             {
    layout->setSpacing( 20 );


    groupBoxLabel->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBoxLabel, 1, 0 );

    Co_Label->setGeometry(QRect(0, 0, 60, 40));
    K_Label->setGeometry(QRect(50, 0, 60, 40));
    Ba_Label->setGeometry(QRect(100, 0, 60, 40));
    Ca_Label->setGeometry(QRect(150, 0, 60, 40));


    groupBoxLabel1->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBoxLabel1, 1, 1 );
    Ag_Label->setGeometry(QRect(0, 0, 60, 40));
    Cr_Label->setGeometry(QRect(50, 0, 60, 40));
    Cu_Label->setGeometry(QRect(100, 0, 60, 40));
    PbL_Label->setGeometry(QRect(150, 0, 60, 40));


    groupBoxLabel2->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBoxLabel2, 1, 2 );
    Au_Label->setGeometry(QRect(0, 0, 60, 40));
    Hg_Label->setGeometry(QRect(50, 0, 60, 40));
    Si_Label->setGeometry(QRect(100, 0, 60, 40));
    Ti_Label->setGeometry(QRect(150, 0, 60, 40));



    groupBoxLabel3->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBoxLabel3, 1, 3 );
    Sn_Label->setGeometry(QRect(0, 0, 60, 40));
    Zn_Label->setGeometry(QRect(50, 0, 60, 40));
    PbM_Label->setGeometry(QRect(100, 0, 60, 40));
    Fe_Label->setGeometry(QRect(150, 0, 60, 40));


    groupBox->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBox, 2, 0 );
    radio_Co->setText(QApplication::translate("MainWindow", "", 0));
    radio_Co->setGeometry(QRect(10, 0, 60, 40));
    //radio_Co->setChecked(true);
    radio_K->setText(QApplication::translate("MainWindow", "", 0));
    radio_K->setGeometry(QRect(60, 0, 60, 40));
    radio_Ba->setText(QApplication::translate("MainWindow", "", 0));
    radio_Ba->setGeometry(QRect(110, 0, 60, 40));
    radio_Ca->setText(QApplication::translate("MainWindow", "", 0));
    radio_Ca->setGeometry(QRect(160, 0, 60, 40));

    groupBox1->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBox1, 2, 1 );
    radio_Ag->setText(QApplication::translate("MainWindow", "", 0));
    radio_Ag->setGeometry(QRect(10, 0, 60, 40));
    //radio_Co->setChecked(true);
    radio_Cr->setText(QApplication::translate("MainWindow", "", 0));
    radio_Cr->setGeometry(QRect(60, 0, 60, 40));
    radio_Cu->setText(QApplication::translate("MainWindow", "", 0));
    radio_Cu->setGeometry(QRect(110, 0, 60, 40));
    radio_PbL->setText(QApplication::translate("MainWindow", "", 0));
    radio_PbL->setGeometry(QRect(160, 0, 60, 40));


    groupBox2->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBox2, 2, 2 );
    radio_Au->setText(QApplication::translate("MainWindow", "", 0));
    radio_Au->setGeometry(QRect(10, 0, 60, 40));
    //radio_Hg->setChecked(true);
    radio_Hg->setText(QApplication::translate("MainWindow", "", 0));
    radio_Hg->setGeometry(QRect(60, 0, 60, 40));
    radio_Si->setText(QApplication::translate("MainWindow", "", 0));
    radio_Si->setGeometry(QRect(110, 0, 60, 40));
    radio_Ti->setText(QApplication::translate("MainWindow", "", 0));
    radio_Ti->setGeometry(QRect(160, 0, 60, 40));



    groupBox3->setGeometry(QRect(0, 0, 200, 200));
    layout->addWidget(groupBox3, 2, 3 );
    radio_Sn->setText(QApplication::translate("MainWindow", "", 0));
    radio_Sn->setGeometry(QRect(10, 0, 60, 40));
    //radio_Sn->setChecked(true);
    radio_Zn->setText(QApplication::translate("MainWindow", "", 0));
    radio_Zn->setGeometry(QRect(60, 0, 60, 40));
    radio_PbM->setText(QApplication::translate("MainWindow", "", 0));
    radio_PbM->setGeometry(QRect(110, 0, 60, 40));
    radio_Fe->setText(QApplication::translate("MainWindow", "", 0));
    radio_Fe->setGeometry(QRect(160, 0, 60, 40));	

    break;
   }






    case(2):
           {
        layout->setSpacing( 8 );


        groupBoxLabel->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBoxLabel, 1, 0 );

        Co_Label->setGeometry(QRect(0, 0, 30, 20));
        K_Label->setGeometry(QRect(25, 0, 30, 20));
        Ba_Label->setGeometry(QRect(50, 0, 30, 20));
        Ca_Label->setGeometry(QRect(75, 0, 30, 20));


        groupBoxLabel1->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBoxLabel1, 1, 1 );
        Ag_Label->setGeometry(QRect(0, 0, 30, 20));
        Cr_Label->setGeometry(QRect(25, 0, 30, 20));
        Cu_Label->setGeometry(QRect(50, 0, 30, 20));
        PbL_Label->setGeometry(QRect(75, 0, 30, 20));


        groupBoxLabel2->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBoxLabel2, 1, 2 );
        Au_Label->setGeometry(QRect(0, 0, 30, 20));
        Hg_Label->setGeometry(QRect(25, 0, 30, 20));
        Si_Label->setGeometry(QRect(50, 0, 30, 20));
        Ti_Label->setGeometry(QRect(75, 0, 30, 20));



        groupBoxLabel3->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBoxLabel3, 1, 3 );
        Sn_Label->setGeometry(QRect(0, 0, 30, 20));
        Zn_Label->setGeometry(QRect(25, 0, 30, 20));
        PbM_Label->setGeometry(QRect(50, 0, 30, 20));
	Fe_Label->setGeometry(QRect(75, 0, 30, 20));

//////////////////////////////////////////////////////////////////////////////////

        groupBox->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBox, 2, 0 );
        radio_Co->setText(QApplication::translate("MainWindow", "", 0));
        radio_Co->setGeometry(QRect(0, 0, 30, 20));
    radio_K->setText(QApplication::translate("MainWindow", "", 0));
        radio_K->setGeometry(QRect(25, 0, 30, 20));
        //radio_Ag->setChecked(true);
        radio_Ba->setText(QApplication::translate("MainWindow", "", 0));
        radio_Ba->setGeometry(QRect(50, 0, 30, 20));
        radio_Ca->setText(QApplication::translate("MainWindow", "", 0));
        radio_Ca->setGeometry(QRect(75, 0, 30, 20));

        groupBox1->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBox1, 2, 1 );
        //radio_Co->setChecked(true);
    radio_Ag->setText(QApplication::translate("MainWindow", "", 0));
        radio_Ag->setGeometry(QRect(0, 0, 30, 20));
        radio_Cr->setText(QApplication::translate("MainWindow", "", 0));
        radio_Cr->setGeometry(QRect(25, 0, 30, 20));
        radio_Cu->setText(QApplication::translate("MainWindow", "", 0));
        radio_Cu->setGeometry(QRect(50, 0, 30, 20));
    radio_PbL->setText(QApplication::translate("MainWindow", "", 0));
        radio_PbL->setGeometry(QRect(75, 0, 30, 20));
        


        groupBox2->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBox2, 2, 2 );
        radio_Au->setText(QApplication::translate("MainWindow", "", 0));
        radio_Au->setGeometry(QRect(0, 0, 30, 20));
        radio_Hg->setText(QApplication::translate("MainWindow", "", 0));
        radio_Hg->setGeometry(QRect(25, 0, 30, 20));
        //radio_Hg->setChecked(true);
        radio_Si->setText(QApplication::translate("MainWindow", "", 0));
        radio_Si->setGeometry(QRect(50, 0, 30, 20));
    radio_Ti->setText(QApplication::translate("MainWindow", "", 0));
        radio_Ti->setGeometry(QRect(75, 0, 30, 20));
	


        groupBox3->setGeometry(QRect(0, 0, 100, 100));
        layout->addWidget(groupBox3, 2, 3 );
        radio_Sn->setText(QApplication::translate("MainWindow", "", 0));
        radio_Sn->setGeometry(QRect(0, 0, 30, 20));
        //radio_Sn->setChecked(true);        
        radio_Zn->setText(QApplication::translate("MainWindow", "", 0));
        radio_Zn->setGeometry(QRect(25, 0, 30, 20));
        radio_PbM->setText(QApplication::translate("MainWindow", "", 0));
        radio_PbM->setGeometry(QRect(50, 0, 30, 20));
    radio_Fe->setText(QApplication::translate("MainWindow", "", 0));
        radio_Fe->setGeometry(QRect(75, 0, 30, 20));





        break;
       }
}


    QCheckBox *OnTop = new QCheckBox( this );
    OnTop->setText( "Win On Top" );
    OnTop->setCheckable( true );
    layout->addWidget( OnTop, 3,0 );
    connect( OnTop, SIGNAL( toggled( bool ) ), SLOT ( enableOnTop( bool ) ));


    QCheckBox *enOnLine = new QCheckBox( this );
    enOnLine->setText( "OnLine" );
    enOnLine->setCheckable( true );
    layout->addWidget( enOnLine, 3,1 );
    connect( enOnLine, SIGNAL( toggled( bool ) ), SLOT ( enableOnLine( bool ) ));

    QPushButton *quitButton = new QPushButton( this );
    quitButton->setText( "Quit" );
    layout->addWidget( quitButton, 3,3 );
    connect( quitButton, SIGNAL( clicked( ) ), SLOT ( quit(  ) ));


    ////////////////// CONNECTIONS

    connect(radio_Ag, SIGNAL(clicked()), this, SLOT(Element_Ag()));
    connect(radio_Au, SIGNAL(clicked()), this, SLOT(Element_Au()));
    connect(radio_Ba, SIGNAL(clicked()), this, SLOT(Element_Ba()));
    connect(radio_Ca, SIGNAL(clicked()), this, SLOT(Element_Ca()));

    connect(radio_Co, SIGNAL(clicked()), this, SLOT(Element_Co()));
    connect(radio_Cr, SIGNAL(clicked()), this, SLOT(Element_Cr()));
    connect(radio_Cu, SIGNAL(clicked()), this, SLOT(Element_Cu()));
    connect(radio_Fe, SIGNAL(clicked()), this, SLOT(Element_Fe()));

    connect(radio_Hg, SIGNAL(clicked()), this, SLOT(Element_Hg()));
    connect(radio_K, SIGNAL(clicked()), this, SLOT(Element_K()));
    connect(radio_PbL, SIGNAL(clicked()), this, SLOT(Element_PbL()));
    connect(radio_Si, SIGNAL(clicked()), this, SLOT(Element_Si()));

    connect(radio_Sn, SIGNAL(clicked()), this, SLOT(Element_Sn()));
    connect(radio_Ti, SIGNAL(clicked()), this, SLOT(Element_Ti()));
    connect(radio_Zn, SIGNAL(clicked()), this, SLOT(Element_Zn()));
    connect(radio_PbM, SIGNAL(clicked()), this, SLOT(Element_PbM()));

    dock->setWidget(DockBox);
    addDockWidget(Qt::BottomDockWidgetArea, dock);


}





