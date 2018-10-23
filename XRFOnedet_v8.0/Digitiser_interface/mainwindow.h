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
//#include <ImgLabel>

#include <QBuffer>
#include <qsharedmemory.h>




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
    void Digi_range(int);
    void Digi_treshold(int);

    void DC_Offset(int);
    void HoldOff(int);
    void Smoothing_Factor(int);
    void Rise_Time(int);
    void Fall_Time(int);
    void Flat_Top(int);
    void Peaking_Delay(int);

    void Base_Line_Mean(int);
    void Trapezoid_Gain(int);
    void Peak_Mean(int);
    void Peak_Holdoff(int);
    void Digitiser_data_download_enable_ON(bool);


    void Exit();

/*    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
*/
public:


    QWidget *mywidget;

    QFrame *line1;
    QCheckBox *checkBox;

    QComboBox *comboBox;
    QComboBox *comboBox2;
    QComboBox *comboBox3;
    QComboBox *comboBox4;

    QSpinBox *spinBox;
    QSpinBox *spinBox2;
    QSpinBox *spinBox3;
    QSpinBox *spinBox4;
    QSpinBox *spinBox5;
    QSpinBox *spinBox6;
    QSpinBox *spinBox7;


    QLabel *labelcombo;
    QLabel *label;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLabel *label6;
    QLabel *label7;
    QLabel *label8;
    QLabel *label9;

    QPushButton *button;
    QPushButton *button2;


/*    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
*/

};

#endif
