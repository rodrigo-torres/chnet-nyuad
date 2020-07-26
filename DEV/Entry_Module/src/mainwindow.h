#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>

#include <QtWidgets>
#include <QThread>

#include "enums_and_wrappers.h"
#include "MAXRF/ipc_methods.h"

extern int * shared_memory_cmd;
extern int * shared_memory;

extern template class maxrf::ipc::shmarray<int>;
extern maxrf::ipc::SHMHandle shm;
class tty_agent;

class MAXRF_UI : public QMainWindow
{
  Q_OBJECT

  maxrf::ipc::SHMHandle experimental_shm_ {};
  maxrf::ipc::shmarray<int> shm0_ {};
  maxrf::ipc::shmarray<int> shm1_ {};
  QThread motors_thread_ {};


public:
  explicit MAXRF_UI(QWidget *parent = nullptr);
  ~MAXRF_UI();

  virtual void closeEvent(QCloseEvent *event);

  void CreateGUI();
  void CreateConnections();
  void CreateSHMResources();
  void CreateMenu();
  void StartThreadTTY();

  friend class tty_agent;

  QWidget *tab1;
  QWidget *tab2;

public slots:
  void toggle_tab1(bool);
  void toggle_widgets(int);
  void update_monitor(QString, QString, int);
//  void tab3_set_target();
  void handle_pushbuttons();
  void enable_servo();


signals:
  void request_tty_action(int, QString = "");
//  void set_target(int, double);
  void keyence_reading(bool);
  void start_servo(bool);

private slots:


  void Info1_1();
  void Info1_2();
  void Info2_1();

  // Composed XRF_image methods
  void OpenDAQWindow();
  void SelDigiCh0();
  void SelDigiCh1();
  void SelDigiCh0and1();

  void ShowHistogram();
  void RateMeter();

  void enable_keyence_reading();

  void set_abort_flag();
  void XrayTable();

private:
  void openAct();
  void create_menu_actions();

  QImage *MyImage;
  QWidget *centralWidget;

  /* Menu Options */
  wrapper<QMenu*, widgets::menus> w_menus;
  wrapper<QAction*, widgets::actions> w_actions;

  /* Tab Widget - tab 1 & tab 2. Assign DFs and open FDs */
  std::vector<QPushButton*> buttons;
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
  /* Below Tab Widget. Monitors and other scan variables */

  QLineEdit *dev_monitor[4];

  QTimer *timer;
  QStatusBar *status;
  QPushButton *OKbutton;
  QPushButton *CANCELbutton;

};


#endif

















