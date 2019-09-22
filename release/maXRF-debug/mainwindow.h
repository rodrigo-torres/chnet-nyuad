#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../qt_dependencies.h"
#include <../Shm.h>
#include <dpp.h>

class QTextEdit;

class Sleeper : public QThread {
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

class tty;




class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread test_thread;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void closeEvent(QCloseEvent *event);

    void GUI_CREATOR();
    void MENU_CONNECTIONS_CREATOR();
    void handle_connections();
    void SHM_CREATOR();
    void builder_Menu();
    void start_thread_tty();


    QVector2D map(int pixX, int pixY);
    void LoadNewFileWithNoCorrection_SHM();

    friend class tty;

    QTabWidget *tabWidget;
    QWidget *tab1;
    QWidget *tab2;
    QWidget *tab3;
    QWidget *tab4;

public slots:
    void toggle_tab1(bool);
    void toggle_widgets(int);
    void update_monitor(QString, QString, int);

    void tab3_set_target();

signals:
    void set_target(int, double);
    void keyence_reading(bool);
    void start_servo(bool);

    void abort();

private slots:

    void scan_parameters(double);

    void Info1_1();
    void Info1_2();
    void Info2_1();

    void open_MAP();
    void Define_Pixels();
    void GoOnLine();
    void displayImage_SHM();
    void displaySumImage_SHM();
    void hideImage();
    void LoadNewFile_SHM();
    void LoadElementsMapSum();
    void writeCompMapLimits(int);
    void SelectChannels();
    void StartVme();
    void Stop_Vme();
    void set_PMAcquisitionTime();
    void SelDigiCh0();
    void SelDigiCh1();
    void SelDigiCh0and1();
    void TreD();
    void DueD();

    void Pixels();
    void ShowHistogram();
    void RateMeter();
    void LoadTxt();
    void readmultidetcalpar();
    void Changeparameters();

    void enable_keyence_reading();
    void openDPPInter();
    void Detector();
    void Helium_interface();
    void VLC_interface();
    void caenoscilloscope();

    void USB_DAQ();
    void OPTICAL_DAQ();

    void export_map();

    void set_abort_flag();
    void tty_timer();

    void CheckSegFault();
    void SaveTxt();

    void XrayTable();

public:

    QByteArray MapImage;

private:
    MainWindowDPP  *d_guiDPP;

    QComboBox *comboBox_XMotor;  ////// Motor Selection
    QComboBox *comboBox_YMotor;
    QComboBox *comboBox_ZMotor;
    QComboBox *element1comboBox;
    QComboBox *element2comboBox;
    QComboBox *element3comboBox;


    QScrollArea *scrollArea;
    QLabel *imageLabel;

    QImage *MyImage;
    double scaleFactor;

    QWidget *centralWidget;
    QMenu *fileMenu;
    QMenu *MapMenu;
    QMenu *AcqMenu;
    QMenu *SpectrumMenu;

    QMenu *TreDMenu;
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
    QAction *FileReloadAct_SHM;
    QAction *MapSumReloadAct_SHM;
    QAction *MapShowAct_SHM;
    QAction *MapHideAct;
    QAction *ViewOnLineAct;
    //QActionGroup *DisplayGroup;

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


    QAction *actionOpen_settings;

    //HOWTOMenu
    QAction *actionOpen_Info1_1;
    QAction *actionOpen_Info1_2;
    QAction *actionOpen_Info2_1;

    QCursor *Cursor;

    QLineEdit *lineEdit_2_tab_4;

    /* Tab Widget - tab 1 & tab 2. Assign DFs and open FDs */

    QSpinBox *tab1_df_number[4];

    QPushButton *tab1_df_open[4];
    QPushButton *tab1_stage_init[3];

    QCheckBox *AUTOFOCUS_ON_pushButton;
    QCheckBox *servo_checkbox;

    QPushButton *tab1_stop;

    /* Tab Widget - tab 3. Move motors (manual) */

    QDoubleSpinBox *spinboxTab3[3];
    QPushButton *buttonTab3[9];

    QPushButton *tab3_stop;
    /* Tab Widget - tab 4. Set scan variables */

    QDoubleSpinBox *scan_params[7];

    QPushButton *tab4_start_scan;
    QPushButton *SCAN_ABORT_pushButton;

    /* Below Tab Widget. Monitors and other scan variables */

    QLineEdit *dev_monitor[4];

    /* Besides Tab Widget. Utility programs */

    QPushButton *Digitizer_Button;
    QPushButton *Export_Button;
    QPushButton *VLC_Button;
    QPushButton *XRAY_TABLE_Button;


    QTimer *timer;
    QStatusBar *status;


public slots:

    void LoadSHM_SumMap();
    void enable_servo();


private:
    QPushButton *OKbutton;
    QPushButton *CANCELbutton;

};


class ImgLabel : public QLabel {
public:
    void setPixmap( const QPixmap & );

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif

















