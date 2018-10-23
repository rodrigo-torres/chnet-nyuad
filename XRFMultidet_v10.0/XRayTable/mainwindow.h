#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox> 
#include <QCheckBox>
#include <QApplication>
#include <QFont>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QFile>
#include <QDir>
#include <QTextStream> 
#include <QString> 
#include <QtGui> //necessaria per il qDebug
#include <QLineEdit>
#include <QFont>
#include <QTabWidget>
#include <QWidget>
#include <QObject>
#include <QTimerEvent>
#include <QFileDialog>
#include <QByteArray>
#include <QScrollArea>
#include <QTableWidgetItem>
#include <QBuffer>
#include <qsharedmemory.h>
#include <QComboBox>


class QAction;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

public slots:
    void GuiCreator();
    void Digi_range(int);

private slots:


    void Exit();

public:


    QWidget *mywidget;
    QComboBox *comboBox;
    QLabel *labelcombo;
    QLabel *labelSpin;
    QPushButton *button2;
    QDoubleSpinBox *Energy_SpinBox;

    QLineEdit *linedit_Element;
    QLineEdit *linedit_Element1;
    QLineEdit *linedit_Element2;
    QLineEdit *linedit_Element3;
    QLineEdit *linedit_Element4;
    QLineEdit *linedit_Element5;
    QLineEdit *linedit_Energy;

    QScrollArea  *scrollArea;
    QVBoxLayout  *layout;
    QTableWidget *table;


};

#endif
