#ifndef MAINWINDOW_H
#define MAINWINDOW_H


////////////////////// uic-qt4 file.ui > file.h

#include <QtGui/QMainWindow>
#include <QMainWindow>
#include <QLabel>
#include <QSpinBox> 
#include <QCheckBox>
#include <QMenu>

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
#include <QGroupBox>
#include <QRadioButton>
//#include <ImgLabel>

// Added for SharedMemory
#include <QBuffer>
#include <qsharedmemory.h>
#include <../Shm.h>
#include <time.h>
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

 class MainWindow : public QMainWindow
 {
     Q_OBJECT


 public:
     explicit MainWindow(QWidget *parent = 0);
     ~MainWindow();

     void GUI_CREATOR();
//     void CONNECTIONS_CREATOR();


     void SetCurrentAction(QString);


/////////////////////////////////////////////////////////////
/*    QTabWidget *tabWidget;
    QWidget *tab2_1;
    QWidget *tab2_2;
    QWidget *tab2_3;
    QWidget *tab_4; */
////////////////////////////////////////////////////////////

///////////////////////////////// TAB1


 protected:
//     void closeEvent(QCloseEvent *event);

 private slots:

       void Exit();
       void Send_CMD();

       void InizializzazioneX();
       void StartX();

//       void MoveX(double);
//       void Abort();
       void Stop();
       void CheckXOnTarget();
       void timerEvent(); 

       QString SetSerialXName(int number);
       void AssignX();
       void NameX(int numberx);

       void MoveX_To();
       void Velocity(double);
       void Motor_selection(int motortype);
       void X_To(double);
       void Motor_CMD(int);

       void SPA_Parameter(int input);
       void SPA_Parameter_value(double input);
       void Show_PI_Parameters();

 public:

     QLineEdit *CurrentAction;


//       void display(int);

 private:
     void createStatusBar();



     QLabel *imageLabel;


     QWidget *centralWidget;
     QMenu *fileMenu;
     QMenu *MapMenu;
     QMenu *AcqMenu;
     QMenu *SpectrumMenu;


     void openAct();
     void createActions();


     QLabel *label_send;
     QPushButton *pushButton_send;
//     QLineEdit *SendCommandX;




     QLineEdit *CurrentActionX;
     QTimer *timer2;
     QCursor *Cursor;
     QTransform *myTransform;



     QPushButton *pushButton_tab;
     QPushButton *quit_5_tab;
     QPushButton *pushButton_PI;

     QDoubleSpinBox *doubleSpinBox;
     QDoubleSpinBox *doubleSpinBox_2;
     QFrame *line_11;
     QLabel *label_7_below_tab;
     QLabel *label_8_below_tab;

     QFrame *line_14;
     QLabel *label_17_below_tab;
     QLineEdit *lineEdit_below_tab;
     QLabel *label_18_below_tab;


//////////////////////////////////////////// TAB WIDGET 2
///// TAB2_1

     QLabel *label_tab_2_bX;
     QSpinBox *spinBox_assignX;
     QPushButton *pushButton_assignX_port;
     QLabel *label_tab_2_bY;
     QSpinBox *spinBox_assignY;
     QPushButton *pushButton_assignY_port;
     QSpinBox *spinBox_SPA_CH;
     QDoubleSpinBox *spinBox_SPA_value;

     QLabel *label_SPA_CH;
     QLabel *label_SPA_VALUE;



///// TAB2_2

     QLabel *label_tab_2_2;
     QLabel *label_tab_2_2X;
     QLabel *label_tab_2_2Y;
     QPushButton *pushButton_tab_2_2X;
     QPushButton *pushButton_tab_2_2Y;
     QPushButton *pushButton_tab_2_2XY;
     QPushButton *pushButton_tab_2_2ABORT;

///// TAB2_3 MOVE

     QLabel *MOVE_label; 
     QPushButton *MOVEUP_pushButton;
     QPushButton *MOVELEFT_pushButton;
     QPushButton *MOVERIGHT_pushButton;
     QPushButton *MOVEDOWN_pushButton;
     QPushButton *STOPMOVE_pushButton;
     QPushButton *MOVE_TO_X_pushButton;
     QDoubleSpinBox *MOVE_TO_X_doubleSpinBox;

     QComboBox *comboBox;
     QComboBox *comboBox_CMD;

     QLabel *XY_SPEED_label;
     QDoubleSpinBox *XY_SPEED_doubleSpinBox;
     QLabel *X_STEP_label;
     QDoubleSpinBox *X_STEP_doubleSpinBox;

     QLabel *X_POSITION_label;
     QLineEdit *X_POSITION_lineEdit;
     QLineEdit *ANSWER_lineEdit;
//////////from motore.h
     QTimer *timer;
     QTimer *timerPos;
     QTimer *timerS; 

///////////////////////////// From Motore_Z.h



 };





class ImgLabel : public QLabel
 {
     
 public:
     void setPixmap( const QPixmap & );
//     friend void SetCurrentAction(QString, MainWindow &);

     
 private:
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
/*
 private:
  MainWindow MainWindow_object;

 public:
  void MyOtherFunc(){  MainWindow_object.SetCurrentAction(QString );}
*/
 };









 #endif

















