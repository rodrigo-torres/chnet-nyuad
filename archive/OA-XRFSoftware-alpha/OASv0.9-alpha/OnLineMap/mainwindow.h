#ifndef MAINWINDOW_H
#define MAINWINDOW_H


////////////////////// uic-qt4 file.ui > file.h

#include <QtGui/QMainWindow>
#include <QMainWindow>
#include <QLabel>
#include <QSpinBox> 
#include <QCheckBox>
#include <../Shm.h>

 class QAction;
 class QActionGroup;
 class QMenu;
 class QPlainTextEdit;
 class QObject;

 class QScrollArea;
 class QScrollBar;
 class QPainter;
 class QImage;
 class QLineEdit;
 class QTimer;
 class QVector2D;

//////////////////////from motore.h///////////

#include <QApplication>
#include <QFont>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
// #include </usr/include/kde4/kled.h>
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

// Added for SharedMemory
#include <QBuffer>
#include <qsharedmemory.h>
#include <QThread>

class QTextEdit;

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};


struct punto {
              int point_x;
              int point_y;             
              };

struct Pixel_BIG {
                    int centro[2]; ///coordinate del centro pixel 0x 1y
                    int dimensione;  //deve essere dispari 3,5,7....(centro+2,+4...)
                    struct punto *total;  /// (dimensione*dimensione) punti
                    
                  };

namespace Ui
{
class MainWindow;
}


 class MainWindow : public QMainWindow
 {
     Q_OBJECT


 public:
     explicit MainWindow(QWidget *parent = 0);
     ~MainWindow();

     void GUI_CREATOR();
/*     void MENU_CONNECTIONS_CREATOR();
     void CONNECTIONS_CREATOR();
     void SHM_CREATOR();


     void SetCurrentAction(QString);

     QVector2D map(int pixX, int pixY);
     void LoadNewFileWithCorrection_SHM();
     void LoadNewFileWithNoCorrection_SHM();

*/


     QTimer *timerOnline;



//    QGridLayout *f_gridLayout;
    QCheckBox *OnTop;
    QCheckBox *enOnTop;
    

    QLabel *Ag_Label;
    QLabel *Au_Label;
    QLabel *Ba_Label;
    QLabel *Ca_Label;
    QLabel *Co_Label;
    QLabel *Cr_Label;
    QLabel *Cu_Label;
    QLabel *Fe_Label;
    QLabel *Hg_Label;
    QLabel *K_Label;
    QLabel *Pb_Label;
    QLabel *Si_Label;
    QLabel *Sn_Label;
    QLabel *Ti_Label;
    QLabel *Zn_Label;

    QPushButton *quitButton;
    QByteArray MapImage;

public Q_SLOTS:

     void Element_Ag();  /////////MAP 1
     void Element_Au();
     void Element_Ba();
     void Element_Ca();
     void Element_Co();  /////////MAP 2
     void Element_Cr();
     void Element_Cu();
     void Element_Fe();
     void Element_Hg();  /////////MAP 3
     void Element_K();
     void Element_PbL();
     void Element_Si();
     void Element_Sn();  /////////MAP 4
     void Element_Ti();
     void Element_Zn();
     void Element_PbM();

     void quit();

     void displayImageOnLine();
     void enableOnTop(bool); 
     void enableOnLine(bool); 

     
 
 private:
//    Ui::MainWindow *ui;


 protected:
//     void closeEvent(QCloseEvent *event);

 private slots:

//       void Exit();

 void Define_Pixels();
/*
       void open_MAP();
       void Define_Pixels();
       void GoOnLine();
       void displayImageOnLine();
       void displayImage();
       void displayImage_SHM();
       void hideImage();
       void LoadNewFile_SHM();
       void SelectChannels();
       void CallScan();
*/

 private:
     void createStatusBar();


     QScrollArea *scrollArea;
     QScrollArea *scrollArea1;
     QScrollArea *scrollArea2;
     QScrollArea *scrollArea3;
     QLabel *imageLabel;
     QLabel *imageLabel1;
     QLabel *imageLabel2;
     QLabel *imageLabel3;

//     QImage *Image_to_save;
     QImage *MyImage1;
     QImage *MyImage2;
     QImage *MyImage3;
     QImage *MyImage4;
     double scaleFactor;

     QWidget *centralWidget;
//     QMenu *fileMenu;
//     QMenu *RunMenu;

    QGroupBox *groupBoxLabel;
    QGroupBox *groupBox;
    QGroupBox *groupBox1;
    QGroupBox *groupBox2;
    QGroupBox *groupBox3;
    QRadioButton *radio_Ba;
    QRadioButton *radio_Ca;
    QRadioButton *radio_Co;
    QRadioButton *radio_Cu;
    QRadioButton *radio_Fe;
    QRadioButton *radio_Hg;
    QRadioButton *radio_K;
    QRadioButton *radio_PbL;
    QRadioButton *radio_Si;
    QRadioButton *radio_Sn;
    QRadioButton *radio_Ti;
    QRadioButton *radio_Zn;
    QRadioButton *radio_PbM;

QCheckBox *enOnLine;


 };





class ImgLabel : public QLabel
 {
     
 public:
     void setPixmap( const QPixmap & );
 };









 #endif

















