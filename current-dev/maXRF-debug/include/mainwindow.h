#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>
#include <cstdint>

#include <unistd.h> //POSIX
#include <QThread>

#include "include/dpp.h"
#include "include/enums_and_wrappers.h"
#include <../Shm.h>
#include "../qt_dependencies.h"

class QTextEdit;

class Sleeper : public QThread {
public:
  static void usleep(unsigned long usecs){QThread::usleep(usecs);}
  static void msleep(unsigned long msecs){QThread::msleep(msecs);}
  static void sleep(unsigned long secs){QThread::sleep(secs);}
};

class tty_agent;
class ImgLabel;


using namespace std;
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
  void create_menus();
  void start_thread_tty();


  void LoadNewFileWithNoCorrection_SHM();

  friend class tty_agent;

  QWidget *tab1;
  QWidget *tab2;
  QWidget *tab3;
  QTextEdit * debug_console_;

private:
  // Dock area on the right
  QTabWidget * MakeDockWidget();
  QWidget * MakeImageDockTab();
  QWidget * MakeSpectrumDisplayDockTab();

  ImgLabel * image_label_;
  QScrollArea * scroll_area_;

public slots:
  void toggle_tab1(bool);
  void toggle_widgets(int);
  void update_monitor(QString, QString, int);
  void tab3_set_target();
  void handle_pushbuttons();
  void LoadSHM_SumMap();
  void enable_servo();

  /* Internal slots */
  void time_monitor(double);

  void SelectLowIntegralLimit(int val);
  void SelectHighIntegralLimit(int val);
signals:
  void request_tty_action(int, QString = "");
  void set_target(int, double);
  void keyence_reading(bool);
  void start_servo(bool);

private slots:

  void scan_parameters(double);

  void Info1_1();
  void Info1_2();
  void Info2_1();

  void GoOnLine();
  void DisplayImageLabel();
  void displaySumImage_SHM();
  void LoadNewFile_SHM();
  void LoadElementsMapSum();
  void writeCompMapLimits(int);
  void SelectChannels();
  void start_point_daq();
  void stop_point_daq();
  void set_PMAcquisitionTime();
  void SelDigiCh0();
  void SelDigiCh1();
  void SelDigiCh0and1();
  void TreD();
  void DueD();

  void ChangePixelDimension();
  void ShowHistogram();
  void RateMeter();
  void load_optimized();
  void LoadImageDataFile();
  void readmultidetcalpar();
  void Changeparameters();

  void enable_keyence_reading();
  void openDPPInter();
  void Detector();
  void Helium_interface();
  void VLC_interface();
  void caenoscilloscope();

  void USB_DAQ(int);
  void OPTICAL_DAQ();

  void ExportImageToPNG();

  void set_abort_flag();

  void CheckSegFault();
  void saveImageXRFData();

  void XrayTable();

private:
  void openAct();
  void create_menu_actions();

  MainWindowDPP  *d_guiDPP;
  QImage *MyImage;
  QWidget *centralWidget;

  QDialog *elementsdlg;

  /* Menu Options */
  wrapper<QMenu*, widgets::menus> w_menus;
  wrapper<QAction*, widgets::actions> w_actions;

  /* Tab Widget - tab 1 & tab 2. Assign DFs and open FDs */
  vector<QPushButton*> buttons;
  wrapper<QSpinBox*, widgets::spinboxes> spinboxes;
  wrapper<QDoubleSpinBox*, widgets::spinboxes> dspinboxes;
  QSpinBox *tab1_df_number[4];
  QCheckBox *laser_checkbox;
  QCheckBox *servo_checkbox;
  QPushButton *tab1_stop;
  /* Tab Widget - tab 3. Move motors (manual) */
  QDoubleSpinBox *spinboxTab3[3];
  QPushButton *buttonTab3[9];
  /* Tab Widget - tab 4. Set scan variables */
  QDoubleSpinBox *scan_params[7];
  QPushButton *tab4_start_scan;
  /* Below Tab Widget. Monitors and other scan variables */

  QLineEdit *dev_monitor[5];



  QTimer *timer;
  QStatusBar *status;
  QPushButton *OKbutton;
  QPushButton *CANCELbutton;


};


#endif

















