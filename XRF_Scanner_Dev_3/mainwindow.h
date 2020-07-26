#ifndef MAINWINDOW_H
#define MAINWINDOW_H


////////////////////// uic-qt4 file.ui > file.h

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox> 
#include <QCheckBox>
#include <QMenu>
#include <QComboBox>
#include <QMessageBox>
#include <QScrollArea>
#include <QStatusBar>
#include <QAction>
#include <QSize>
#include <QMenuBar>

 class QMenuBar;
 class QSize;
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
 class QComboBox;
 class QMessageBox;
 class QStatusBar; 
 class QDialogButtonBox;
//////////////////////from motore.h///////////

#include <QApplication>
#include <QFont>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
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
//#include <QtWidgets>
#include <QObject>
#include <QTimerEvent>
#include <QFileDialog>
#include <QByteArray>
#include <QGroupBox>
#include <QRadioButton>
//#include <ImgLabel>
#include <QDebug>

// Added for SharedMemory
#include <QBuffer>
#include <qsharedmemory.h>
#include <../Shm.h>

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
     void MENU_CONNECTIONS_CREATOR();
     void CONNECTIONS_CREATOR();
     void SHM_CREATOR();
     void CREATE_MENU();


     void SetCurrentAction(QString);

     QVector2D map(int pixX, int pixY);
     void LoadNewFileWithCorrection_SHM();
     void LoadNewFileWithNoCorrection_SHM();
     void LoadSHM_SumMap();
     
     



/////////////////////////////////////////////////////////////
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QWidget *tab_3;
    QWidget *tab_4;
////////////////////////////////////////////////////////////
    //QTabWidget *tabWidget2;
    QWidget *tab2_1;
    //QWidget *tab2_2;
    QWidget *tab2_3;
    QWidget *tab2_4;

///////////////////////////////// TAB1


 protected:
//     void closeEvent(QCloseEvent *event);

 private slots:

       void Info1_1();
       void Info1_2();
       void Info2_1();
       void Exit();

       //void X_Motor_selection(int);
       //void Y_Motor_selection(int);
       //void Z_Motor_selection(int);

       void open_MAP();
       void Define_Pixels();
       void GoOnLine();
       void displayImage_SHM();
       void displaySumImage_SHM();
       void hideImage();
       void LoadNewFile_SHM();
       void LoadElementsMapSum();
       void SelectionElement1(int);
       void SelectionElement2(int);
       void SelectionElement3(int);
       void SelectChannels();
       void StartVme();
       void Stop_Vme();
       void SelMeasTime();
       void SelDigiCh0();
       void SelDigiCh1();
       void SelDigiCh0and1();
       void TreD();
       void DueD();

       void Pixels();
       void ShowHistogram();
       void MapCorrection();
       void PixelCrct();
       void RateMeter();
       void CutB();
       void LoadTxt();
       void MergeTxt();
       void readmultidetcalpar();
       void Changeparameters();

       //void exportpymca();

       void Autofocus2();
       void Digitiser2();
       void Detector();
       void Laser_interface();
       void Helium_interface();
       void VLC_interface();
       void caenoscilloscope();

       void USB_DAQ();
       void OPTICAL_DAQ();


      void Enabling_Tabwidget();
      void Enable_TabWidget_3_4_XY();

      void Image_Export();
      void ClickedOK();
      void ClickedCANCEL();


//////from motore.h////
void Velocity(double);
       void PassoX_Func(double);
       void Xminimo(double);
       void Xmassimo(double);
       void PassoY_Func(double);
       void Yminimo(double);
       void Ymassimo(double);
       void X_to(double);
       void Y_to(double);

       // Found in XY_Init.cpp
       void movetoref_Xmotor(float refpositionX);
       void Init_Xmotor();
       void movetoref_Ymotor(float refpositionY);
       void Init_Ymotor();
       void movetoref_Zmotor(float refpositionZ);

       //
       void StartX();
       void StartY();
       bool StartXYScan();
       bool StartYXScan();
       void ScanXY();
       void ScanYX();
       void MoveX(double);
       void MoveY(double);
       void Move_up();
       void MoveX_To();
       void MoveY_To();
       void Move_down();
       void Move_right();
       void Move_left();
       void MoveDoubleClick();
       void Abort();
       void Stop();
       void CheckXOnTarget();
       void CheckYOnTarget();
       void timerEvent();


       void CheckSegFault();
       void SaveTxt();
       QString SetSerialXName(int number);
       QString SetSerialYName(int number);
       void AssignY();
       void AssignX();
       void NameY(int numbery);
       void NameX(int numberx);


       void XrayTable();


       void PI_motor_test();
       void PI_motor_table();



 public:

     QByteArray MapImage;
     QLineEdit *CurrentAction;


//       void display(int);

 private:
     void createStatusBar();

     QComboBox *comboBox_XMotor;  ////// Motor Selection
     QComboBox *comboBox_YMotor;
     QComboBox *comboBox_ZMotor;
     QComboBox *element1comboBox;
     QComboBox *element2comboBox;
     QComboBox *element3comboBox;


     QScrollArea *scrollArea;
     QLabel *imageLabel;

     QImage *Image_to_save;
     QImage *MyImage;
     double scaleFactor;

     QWidget *centralWidget;
     QMenu *fileMenu;
     QMenu *MapMenu;
     QMenu *AcqMenu;
     QMenu *SpectrumMenu;

     QMenu *TreDMenu;
     //QMenu *PyMcaMenu;
     QMenu *TOOLMenu;
     QMenu *RunMenu;
     QMenu *HowToMenu;



     QMenu* SoftWare;
     QMenu* Kernel;
     QMenu *ActiveChannel;

     void openAct();
     void createActions();


     QGroupBox *groupBox;
     QRadioButton *radioUSB;
     QDialog *elementsdlg;
     QGroupBox *elementsgroupBox;
     QRadioButton *radioOptical;
     QVBoxLayout *vbox;
     QVBoxLayout *elementsLayout;


//FileMenu
     QAction *openAct1;
     QAction *MergeTxtAct;
     QAction *SaveTxtAct;
     QAction *exitAct;
//MapMenu
     QAction *ChSelAct;
     QAction *PxAct;
     QAction *CutBordersAct;
     QAction *CorrectMapAct;
     QAction *FileReloadAct_SHM;
     QAction *MapSumReloadAct_SHM;
     QAction *MapShowAct_SHM;
     QAction *MapHideAct;
     QAction *ViewOnLineAct;
     //QActionGroup *DisplayGroup;
//AcqMenu
//   QAction *RunDaq;
     QAction *SelTime;
     QAction *DigitizerChannel0;
     QAction *DigitizerChannel1;
     QAction *DigitizerChannel0and1;
     QAction *Changemultidetcalpar;
     QAction *RunVme;
     QAction *StopVme;
     QAction *RateAct;

//SpectrumMenu
     QAction *HistoAct;
//3DMenu
     QAction *DueDAct;
     QAction *TreDAct;

     //QAction *SaveTxtAct;
//PyMcaMenu
     //QAction *PyMcaExpAct;
     //QAction *RunPyMcaAct;
     //QAction *InstallPyMcaAct;
//TOOLMenu
     QAction *Open_motor_test;
     QAction *Open_PI_motor_parameter_table;
     QAction *actionOpen_settings;

//HOWTOMenu
     QAction *actionOpen_Info1_1;
     QAction *actionOpen_Info1_2;
     QAction *actionOpen_Info2_1;

     QAction *Autofocus;
     QAction *Digitiser;





     QLineEdit *CurrentActionX;
     QLineEdit *CurrentActionY;
     QLineEdit *CurrentActionZ;
     QLineEdit *CurrentActionACM;
//     QLineEdit *CurrentAction;
     QTimer *timer2;
     QCursor *Cursor;
     QTransform *myTransform;

     QPushButton *XRAY_TABLE_Button;

     QPushButton *Digitizer_Button;
     QPushButton *Detector_Button;
     QPushButton *Export_Button;
     QPushButton *VLC_Button;
     QPushButton *LASER_Button;
     //QPushButton *HELIUM_Button;
     QPushButton *SW_treshold_Button;

     QLabel *DAQ_label;
     QLabel *TTY_label;
     QLabel *MapImage_label;
     QLabel *CAMERA_label;
     QLabel *LASER_label;
     //QLabel *HELIUM_label;
     //QLabel *GAS_label;
     QLabel *element1label;
     QLabel *element2label;
     QLabel *element3label;

     //QLCDNumber *lcdNumber;
     //QSlider *horizontalSlider;


     QFrame *line_1;
     QFrame *line_2;
     QFrame *line_3;
     QFrame *line_4;

////////////////////////////////// TAB WIDGET
     QSpinBox *spinBox_assignZ;
     QPushButton *AssignZ_pushButton;
     //QLabel *TTY_Z_label;
     //QLineEdit *linedit_Z_Assignement;
     QLabel *label_6_tab;

     QPushButton *pushButton_tab;
     QPushButton *quit_5_tab;

     //QFrame *line_6_tab;


////////////////////////////////// TAB_4

     QLabel *ACM_label;
     QSpinBox *spinBox_assignACM;
     QPushButton *AssignACM_pushButton;

////////////////////////////////// TAB_4

     QPushButton *pushButton_2_tab_4;
     QCheckBox *checkBox_3_tab_4;
     QLabel *label_23_tab_4;
     QLineEdit *lineEdit_2_tab_4;
     QCheckBox *checkBox_tab_4;

////////////////////////////////////////// SOTTO IL TAB WIDGET

     QDoubleSpinBox *doubleSpinBox;
     QDoubleSpinBox *doubleSpinBox_2;
     //QFrame *line_11;
     //QLabel *label_7_below_tab;
     //QLabel *label_8_below_tab;

     //QFrame *line_14;
     QLabel *label_17_below_tab;
     QLineEdit *lineEdit_below_tab;
     QLabel *label_18_below_tab;

     QLabel *MOVE_Z_MOTOR_label_1;
     QPushButton *pushButton_8_tab_2_a;
     QDoubleSpinBox *doubleSpinBox_9_tab_2_a;
     QPushButton *pushButton_6_tab_2_a;
     QLabel *MOVE_Z_MOTOR_label_2;
     QPushButton *pushButton_7_tab_2_a;
     QPushButton *quit_4_tab_2_a;


//////////////////////////////////////////// TAB WIDGET 2
///// TAB2_1

     QLabel *label_tab_2_bX;
     QSpinBox *spinBox_assignX;
     QPushButton *pushButton_assignX_port;
     QLabel *label_tab_2_bY;
     QSpinBox *spinBox_assignY;
     QPushButton *pushButton_assignY_port;

///// TAB2_2

     //QLabel *label_tab_2_2;
     //QLabel *label_tab_2_2X;
     //QLabel *label_tab_2_2Y;
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
     QPushButton *MOVE_TO_Y_pushButton;
     QDoubleSpinBox *MOVE_TO_Y_doubleSpinBox;

///// TAB2_4 SCAN

     QLabel *SCAN_X_label;
     QDoubleSpinBox *SET_SCAN_X_MIN_doubleSpinBox;
     QDoubleSpinBox *SET_SCAN_X_MAX_doubleSpinBox;
     QLabel *SCAN_Y_label;
     QDoubleSpinBox *SET_SCAN_Y_MIN_doubleSpinBox;
     QDoubleSpinBox *SET_SCAN_Y_MAX_doubleSpinBox;
     QPushButton *SCANXY_pushButton;
     QPushButton *SCANYX_pushButton;
     QPushButton *SCAN_ABORT_pushButton;

/////// XY SCAN SETTINGS (AREA BELOW TAB2)

     QLabel *XY_SPEED_label;
     QDoubleSpinBox *XY_SPEED_doubleSpinBox;
     QFrame *XY_SPEED_line;
     QLabel *X_STEP_label;
     QDoubleSpinBox *X_STEP_doubleSpinBox;
     QFrame *XY_STEP_line;
     QLabel *Y_STEP_label;
     QDoubleSpinBox *Y_STEP_doubleSpinBox;
     QLabel *X_POSITION_label;
     QLineEdit *X_POSITION_lineEdit;
     QLabel *Y_POSITION_label;
     QLineEdit *Y_POSITION_lineEdit;



     QFrame *FRAME_line1;
     QFrame *FRAME_line2;
     QFrame *FRAME_line3;
     QFrame *FRAME_line4;

//////////from motore.h
     QTimer *timer;
     QTimer *timerPos;
     QTimer *timerS;

///////////////////////////// From Motore_Z.h


   public slots:

       //void Write_coordinates();
       void Treshold();

       //void VelocityZ(double);
       //void PassoZ_Func(double);
       void Zminimo(double);
       void Zmassimo(double);

       void Z_to(double);
       void Init_Zmotor();
       void StartZ();

       void Init_KeyenceLaser();

       void MoveZ(double);
       void MoveZ_To();
       void Move_forward();
       void Move_backward();
       void AbortZ();
       void StopZ();
       void CheckZOnTarget();
 //      void timerEventZ();
       void Focustimer();
       void readKeyence();
 //      void SetAutoFocusReferenceDistance();

       void TrackingON();
       void AutoFocusRunning();
//       void Camera();

///////////////////////// LASER
//       void Laser();
       void Laser_switching();
       void LaserOn();
       void LaserOff();

//////////////////////////////

       QString SetSerialZName(int number);
       void AssignZ();
       void NameZ(int numberz);

       void AssignACM();
       void NameACM(int numberacm);



private:

       QTimer *timerAutofocus;
       QTimer *timerZ;


     QPushButton *INIT_Z_pushButton;
     QPushButton *STOP_Z_INIT_pushButton;
     QPushButton *MOVE_Z_To_pushButton;
     QDoubleSpinBox *MOVE_Z_To_doubleSpinBox;
     QPushButton *MOVE_Z_FORWARD_pushButton;
     QPushButton *MOVE_Z_BACKWARD_pushButton;
     QPushButton *STOP_Z_MOVE_pushButton;
     QPushButton *AUTOFOCUS_ON_pushButton;
     QCheckBox *AUTOFOCUS_STATUS_checkBox;
     QCheckBox *ENABLE_TRACKING_checkBox;

     QDoubleSpinBox *Z_VELOCITY_doubleSpinBox;
     //QDoubleSpinBox *Z_STEP_doubleSpinBox;
     QDialogButtonBox *buttonBox;

    QPushButton *OKbutton;
    QPushButton *CANCELbutton;


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

















