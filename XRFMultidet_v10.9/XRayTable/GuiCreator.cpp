#include <QtGui>
#include <QSpinBox>
#include <QFont>
#include "mainwindow.h"

extern int Resolution_mode;

void MainWindow::GuiCreator() {

    QWidget *mywidget= new QWidget;
    setCentralWidget(mywidget);



//////////////// INPUT SIGNAL/////////////////////////////

    QLabel   *labelcombo = new QLabel(mywidget);
    labelcombo->setText("Select by Element");
    QComboBox *comboBox = new QComboBox(mywidget);
    comboBox->insertItems(0, QStringList()
         << QApplication::translate("mywidget", "--", 0)
         << QApplication::translate("mywidget", "H", 0)
         << QApplication::translate("mywidget", "He", 0)
         << QApplication::translate("mywidget", "Li", 0)
         << QApplication::translate("mywidget", "Be", 0)
         << QApplication::translate("mywidget", "B", 0)
         << QApplication::translate("mywidget", "C", 0)
         << QApplication::translate("mywidget", "N", 0)
         << QApplication::translate("mywidget", "O", 0)
         << QApplication::translate("mywidget", "F", 0)
         << QApplication::translate("mywidget", "Ne", 0)
         << QApplication::translate("mywidget", "Na", 0)
         << QApplication::translate("mywidget", "Mg", 0)
         << QApplication::translate("mywidget", "Al", 0)
         << QApplication::translate("mywidget", "Si", 0)
         << QApplication::translate("mywidget", "P", 0)
         << QApplication::translate("mywidget", "S", 0)
         << QApplication::translate("mywidget", "Cl", 0)
         << QApplication::translate("mywidget", "Ar", 0)
         << QApplication::translate("mywidget", "K", 0)
         << QApplication::translate("mywidget", "Ca", 0)
         << QApplication::translate("mywidget", "Sc", 0)
         << QApplication::translate("mywidget", "Ti", 0)
         << QApplication::translate("mywidget", "V", 0)
         << QApplication::translate("mywidget", "Cr", 0)
         << QApplication::translate("mywidget", "Mn", 0)
         << QApplication::translate("mywidget", "Fe", 0)
         << QApplication::translate("mywidget", "Co", 0)
         << QApplication::translate("mywidget", "Ni", 0)
         << QApplication::translate("mywidget", "Cu", 0)
         << QApplication::translate("mywidget", "Zn", 0)
         << QApplication::translate("mywidget", "Ga", 0)
         << QApplication::translate("mywidget", "Ge", 0)
         << QApplication::translate("mywidget", "As", 0)
         << QApplication::translate("mywidget", "Se", 0)
         << QApplication::translate("mywidget", "Br", 0)
         << QApplication::translate("mywidget", "Kr", 0)
         << QApplication::translate("mywidget", "Rb", 0)
         << QApplication::translate("mywidget", "Sr", 0)
         << QApplication::translate("mywidget", "Y", 0)
         << QApplication::translate("mywidget", "Zr", 0)
         << QApplication::translate("mywidget", "Nb", 0)
         << QApplication::translate("mywidget", "Mo", 0)
         << QApplication::translate("mywidget", "Tc", 0)
         << QApplication::translate("mywidget", "Ru", 0)
         << QApplication::translate("mywidget", "Rh", 0)
         << QApplication::translate("mywidget", "Pd", 0)
         << QApplication::translate("mywidget", "Ag", 0)
         << QApplication::translate("mywidget", "Cd", 0)
         << QApplication::translate("mywidget", "In", 0)
         << QApplication::translate("mywidget", "Sn", 0)
         << QApplication::translate("mywidget", "Sb", 0)
         << QApplication::translate("mywidget", "Te", 0)
         << QApplication::translate("mywidget", "I", 0)
         << QApplication::translate("mywidget", "Xe", 0)
         << QApplication::translate("mywidget", "Cs", 0)
         << QApplication::translate("mywidget", "Ba", 0)
         << QApplication::translate("mywidget", "Hf", 0)
         << QApplication::translate("mywidget", "Ta", 0)
         << QApplication::translate("mywidget", "W", 0)
         << QApplication::translate("mywidget", "Re", 0)
         << QApplication::translate("mywidget", "Os", 0)
         << QApplication::translate("mywidget", "Ir", 0)
         << QApplication::translate("mywidget", "Pt", 0)
         << QApplication::translate("mywidget", "Au", 0)
         << QApplication::translate("mywidget", "Hg", 0)
         << QApplication::translate("mywidget", "Tl", 0)
         << QApplication::translate("mywidget", "Pb", 0)
         << QApplication::translate("mywidget", "Bi", 0)
         << QApplication::translate("mywidget", "Po", 0)
         << QApplication::translate("mywidget", "At", 0)
         << QApplication::translate("mywidget", "Rn", 0)
         << QApplication::translate("mywidget", "Fr", 0)
         << QApplication::translate("mywidget", "Ra", 0)
        );

    qDebug()<<"... Loading data files";

    QTableWidget* table = new QTableWidget(mywidget);
    table->setRowCount(266);
    table->setColumnCount(3);


    QFile filedata(":File/Data/Energy_data");
    filedata.open(QIODevice::ReadOnly);
    if (filedata.exists()) {
        for(int i=0; i<266; i++) {
            QString line;
            line = filedata.readLine();
            table->setItem(i, 0, new QTableWidgetItem(line));
        }
    }
    else {
        qDebug()<<"[!] Energy data missing";
        return ;
    }

    filedata.close();

    QFile file(":File/Data/Energy_element");
    file.open(QIODevice::ReadOnly);
    if(file.exists()) {
        for(int i=0; i<266; i++) {
            QString line;
            line = file.readLine();
            table->setItem(i, 1, new QTableWidgetItem(line));
        }
    }
    else {
        qDebug()<<"[!] Element data missing";
        return ;
    }
    file.close();

    QString textlevel=":File/Data/Energy_level";
    QFile filelevel(textlevel);
    filelevel.open(QIODevice::ReadOnly);
    if(filelevel.exists()) {
        for(int i=0; i<266; i++) {
            QString line;
            line = filelevel.readLine();
            table->setItem(i, 2, new QTableWidgetItem(line));
        }
    }

    else {
        qDebug()<<"[!] Energy level data missing";
        return ;
    }

    filelevel.close();

    qDebug()<<"... All data files loaded";

    QLabel *labelSpin = new QLabel(mywidget);
    labelSpin->setText("<big><center><b> Orderered Energy List (keV) </b></center></big>");

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


    //QPushButton *button2 = new QPushButton(mywidget);
    //button2->setText("quit");



    qDebug()<<"... Setting geometry";

    table->setColumnWidth(0,80);
    table->setColumnWidth(1,80);
    table->setColumnWidth(2,80);

    labelcombo->setGeometry(QRect(40, 70, 150, 25));
    comboBox->setGeometry(QRect(40, 100, 150, 40));
    linedit_Element->setGeometry(QRect(40, 160, 150, 40));
    linedit_Element1->setGeometry(QRect(40, 210, 150, 40));
    linedit_Element2->setGeometry(QRect(40, 260, 150, 40));
    linedit_Element3->setGeometry(QRect(40, 310, 150, 40));
    linedit_Element4->setGeometry(QRect(40, 360, 150, 40));
    linedit_Element5->setGeometry(QRect(40, 410, 150, 40));


    labelSpin->setGeometry(QRect(255, 40, 280, 50));
    table->setGeometry(QRect(250, 100, 290, 350));

    //button2->setGeometry(QRect(600, 480, 100, 50));

    // CONNECTIONS

    qDebug()<<"... Setting connections";

    connect(comboBox, SIGNAL(activated(int)), this, SLOT(Digi_range(int)));
    //connect(button2, SIGNAL(clicked()), this, SLOT(Exit()));
}



