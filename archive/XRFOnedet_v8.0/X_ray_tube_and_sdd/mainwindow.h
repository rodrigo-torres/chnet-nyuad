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
//#include <ImgLabel>
#include <QTableWidgetItem>
#include <QBuffer>
#include <qsharedmemory.h>

class QTimer;
class QAction;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
//    void closeEvent(QCloseEvent *event);

private slots:

    void GuiCreator();
//    void Digi_range(int);
    void Exit();

public slots:
    void Arduino_serial_init();
    void NameACM(int);

    void Sdd_power();
    void Xray_voltage(double);
    void Xray_current(double);
    void Xray_power();
    void timerXRF();
    void MainLoop();
    void ReadAnalogValue();

public:

     QTimer *timer_XRF;
     QTimer *timer_Reading;


    QWidget *mywidget;
    QTabWidget *tabWidget_sx;
    QTabWidget *tabWidget_dx;
    QWidget *tab1;
    QWidget *tab2;
    QWidget *tab3;


    QComboBox *comboBox;

    QLabel *labelcombo;
    QLabel *label_X_Voltage;
    QLabel *label_X_Current;
    QLabel *labelSDDvoltage;
    QLabel *label_SDD_monitor;
    QLabel *label_XRAY_monitor;
    QLabel *label_SDD_volt_mon;
    QLabel *label_XRAY_volt_mon;
    QLabel *label_XRAY_curr_mon;


    QDoubleSpinBox *X_voltage_SpinBox;
    QDoubleSpinBox *X_current_SpinBox;

    QPushButton *SDD_ON_OFF_button;
    QPushButton *XRAY_ON_OFF_button;
    QPushButton *button2;
    QPushButton *TTY_connect_button;

    QLineEdit *linedit_SDD_HV_status;
    QLineEdit *linedit_XRAY_HV_status;
    QLineEdit *linedit_TTY;
    QLineEdit *linedit_Xray_V;
    QLineEdit *linedit_Xray_I;
    QLineEdit *linedit_SDD_V;


    QVBoxLayout  *layout;



};

#endif
