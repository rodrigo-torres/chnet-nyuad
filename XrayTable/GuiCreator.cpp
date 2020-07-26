#include <QtGui>
#include <QSpinBox>
#include "mainwindow.h"

extern int Resolution_mode;

void MainWindow::GuiCreator()
{

    QWidget *mywidget= new QWidget;
    setCentralWidget(mywidget);


//////////////// INPUT SIGNAL/////////////////////////////


    QLabel   *labelcombo = new QLabel(mywidget);
    labelcombo->setText("Select by Element");
    QComboBox *comboBox = new QComboBox(mywidget); ////// INPUT RANGE
    comboBox->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "--", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "H", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "He", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Li", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Be", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "C", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "N", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "O", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "F", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ne", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Na", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Mg", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Al", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Si", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "P", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "S", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Cl", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ar", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "K", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ca", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Sc", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ti", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Cr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Mn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Fe", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Co", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ni", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Cu", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Zn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ga", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ge", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "As", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Se", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Br", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Kr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Rb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Sr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Y", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Zr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Nb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Mo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Tc", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ru", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Rh", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Pd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ag", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Cd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "In", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Sn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Sb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Te", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "I", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Xe", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Cs", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ba", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Hf", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ta", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "W", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Re", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Os", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ir", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Pt", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Au", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Hg", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Tl", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Pb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Bi", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Po", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "At", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Rn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Fr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mywidget", "Ra", 0, QApplication::UnicodeUTF8)
        );


    QLabel   *labelSpin = new QLabel(mywidget);
    labelSpin->setText("Energy List (keV)");

/*    Energy_SpinBox = new QDoubleSpinBox(mywidget);
    Energy_SpinBox->setDecimals(1);
    Energy_SpinBox->setMaximum(100);
    Energy_SpinBox->setSingleStep(0.1);
    Energy_SpinBox->setValue(0);
*/

    linedit_Element = new QLineEdit(mywidget);
    linedit_Element->setEnabled(false);

    linedit_Element1 = new QLineEdit(mywidget);
    linedit_Element1->setEnabled(false);

    linedit_Element2 = new QLineEdit(mywidget);
    linedit_Element2->setEnabled(false);

    linedit_Element3 = new QLineEdit(mywidget);
    linedit_Element3->setEnabled(false);

    linedit_Element4 = new QLineEdit(mywidget);
    linedit_Element4->setEnabled(false);

    linedit_Element5 = new QLineEdit(mywidget);
    linedit_Element5->setEnabled(false);


//    linedit_Energy = new QLineEdit(mywidget);
//    linedit_Energy->setEnabled(false);


    QPushButton *button2 = new QPushButton(mywidget);
    button2->setText("quit");




//    QScrollArea *scrollArea = new QScrollArea(mywidget);
//    area->setWidget(client);

    // Make the scroll step the same width as the fixed widgets in the grid
//    scrollArea->verticalScrollBar();//->setSingleStep(client->width() / 10);

//    QVBoxLayout *layout = new QVBoxLayout(scrollArea);
//    layout->addWidget(area);

      QTableWidget* table = new QTableWidget(mywidget);
      table->setRowCount(265);
      table->setColumnCount(3);


///////////////////////////////////// LOADING DATA FILES

      QString textdata="Program_Data_Files/Energy_data";
      QFile filedata(textdata);
      filedata.open(QIODevice::ReadOnly);
      if(filedata.exists())
      {
       for(int i=0; i<265; i++)
        {
         QString line;
         line = filedata.readLine();
//         int numero=line.toInt();
         table->setItem(i, 0, new QTableWidgetItem(line));
//         table->setItem(i, 0, new QTableWidgetItem(numero));
        }
      }
     else {qDebug()<<"energy data missing....!!!\n"; exit ;}
     filedata.close();



      QString text="Program_Data_Files/Energy_element";
      QFile file(text);
      file.open(QIODevice::ReadOnly);
      if(file.exists())
      {
       for(int i=0; i<265; i++)
        {
         QString line;
         line = file.readLine();
         table->setItem(i, 1, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"element data missing....!!!\n"; exit ;}
     file.close();


      QString textlevel="Program_Data_Files/Energy_level";
      QFile filelevel(textlevel);
      filelevel.open(QIODevice::ReadOnly);
      if(filelevel.exists())
      {
       for(int i=0; i<265; i++)
        {
         QString line;
         line = filelevel.readLine();
         table->setItem(i, 2, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"energy level data missing....!!!\n"; exit ;}
     filelevel.close();




// GEOMETRY ///////////////////////////////////////////////////////////////////

//qDebug()<<Resolution_mode;

   switch (Resolution_mode)
   {
    case(0):
    {
/////INPUT

      table->setColumnWidth(0,120);
      table->setColumnWidth(1,80);
      table->setColumnWidth(2,70);

    labelcombo->setGeometry(QRect(60, 40, 280, 24));
    comboBox->setGeometry(QRect(70, 90, 200, 44));

    labelSpin->setGeometry(QRect(400, 40, 280, 24));
//    Energy_SpinBox->setGeometry(QRect(400, 80, 280, 54));
    linedit_Element->setGeometry(QRect(40, 160, 250, 40));
    linedit_Element1->setGeometry(QRect(40, 210, 250, 40));
    linedit_Element2->setGeometry(QRect(40, 260, 250, 40));
    linedit_Element3->setGeometry(QRect(40, 310, 250, 40));
    linedit_Element4->setGeometry(QRect(40, 360, 250, 40));
    linedit_Element5->setGeometry(QRect(40, 410, 250, 40));

    table->setGeometry(QRect(350, 90, 380, 360));
//    scrollArea->setMinimumSize(QSize(800, 800));
//    scrollArea->setMaximumSize(QSize(800, 800));
//    linedit_Energy->setGeometry(QRect(370, 160, 250, 300));

    button2->setGeometry(QRect(20, 480, 720, 56));
    break;
    }

    case(1): ////////// LOW RESOLUTION CASES
    {
    labelcombo->setGeometry(QRect(45, 30, 210, 18));
    comboBox->setGeometry(QRect(52, 67, 150, 33));
    labelSpin->setGeometry(QRect(300, 30, 210, 18));
    linedit_Element->setGeometry(QRect(30, 120, 187, 30));
    linedit_Element1->setGeometry(QRect(30, 157, 187, 30));
    linedit_Element2->setGeometry(QRect(30, 195, 187, 30));
    linedit_Element3->setGeometry(QRect(30, 232, 187, 30));
    linedit_Element4->setGeometry(QRect(30, 270, 187, 30));
    linedit_Element5->setGeometry(QRect(30, 307, 187, 30));
    table->setGeometry(QRect(262, 67, 285, 270));
 
    button2->setGeometry(QRect(15, 360, 540, 42));
    break;
    }

    case(2):
    {
    labelcombo->setGeometry(QRect(30, 20, 140, 12));
    comboBox->setGeometry(QRect(35, 45, 100, 22));
    labelSpin->setGeometry(QRect(200, 20, 140, 12));
    linedit_Element->setGeometry(QRect(20, 80, 125, 20));
    linedit_Element1->setGeometry(QRect(20, 105, 125, 20));
    linedit_Element2->setGeometry(QRect(20, 130, 125, 20));
    linedit_Element3->setGeometry(QRect(20, 155, 125, 20));
    linedit_Element4->setGeometry(QRect(20, 180, 125, 20));
    linedit_Element5->setGeometry(QRect(20, 205, 125, 20));
    table->setGeometry(QRect(175, 45, 190, 180));
    table->setColumnWidth(0, 60);
    table->setColumnWidth(1, 40);
    table->setColumnWidth(2, 40);

    button2->setGeometry(QRect(10, 240, 360, 28));
    break;
    }
   }


// COONNECTIONS

     connect(comboBox, SIGNAL(activated(int)), 
          this, SLOT(Digi_range(int)));


     connect(button2, SIGNAL(clicked()), this, SLOT(Exit())); ///aggiungere check se è fermo qApp, SLOT(quit())




}



