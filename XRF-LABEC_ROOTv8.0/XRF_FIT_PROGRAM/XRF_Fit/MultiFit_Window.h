
#ifndef MULTIFIT_WINDOW_H
#define MULTIFIT_WINDOW_H

//#include <QtWidgets>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox> 
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QMenu>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QSizePolicy>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <QScreen>
#include <time.h>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDir>
#include <QComboBox>
#include <QInputDialog>

class QComboBox;
class QCheckBox;
class QComboBox;
class QGridLayout;
class QGroupBox;
class QLabel;
class QPushButton;
class QLineEdit;
class QRadioButton;
class QTextEdit;
class QDoubleSpinBox;

class MultiFit_Window : public QWidget
{
    Q_OBJECT

public:
      MultiFit_Window(QWidget *parent = 0);//MultiFit_Window();
      ~MultiFit_Window();// {} //virtual 

    QGroupBox *ROIGroupBox;
    QGridLayout *ROILayout;
    QGroupBox *selBox1;
    QRadioButton *ROI1;
    QRadioButton *ROI2;
    QLabel *Label1;
    QLabel *Label2;
    QLineEdit *E_lowBox;
    QLineEdit *E_highBox;
    QPushButton *Startbutton1;
    QLineEdit *ROILineEdit;
    QPushButton *state1;

    QGroupBox *BaselineGroupBox;
    QComboBox *BlCombo;
    QLabel *BlLabel;
    QCheckBox *BlCheckBox;
    QRadioButton *Bl1;
    QRadioButton *Bl2;
    QRadioButton *Bl3;
    QPushButton *state2;

    QGroupBox *PeaksGroupBox;
    QGroupBox *selBox2;
    QGroupBox *selBox3;
    QRadioButton *Peaks1;
    QRadioButton *Peaks2;
    QRadioButton *Peaks3;
    QTextEdit *PeaksTextEdit;
    QPushButton *Startbutton2;
    QPushButton *Stopbutton2;
    QPushButton *Cancelbutton2;
    QPushButton *BigStart;
    QPushButton *BigReset;
    QPushButton *state3;

    QGroupBox *FitFunctionsGroupBox;
    QLabel *Label3;
    QLabel *Label4;
    QLabel *Label5;
    QTextEdit *textEditP;
    QTextEdit *textEditE;
    QComboBox *FCombo;
    QComboBox *FCombo1;
    QComboBox *FCombo2;
    QComboBox *FCombo3;
    QComboBox *FCombo4;
    QComboBox *FCombo5;
    QComboBox *FCombo6;
    QComboBox *FCombo7;
    QComboBox *FCombo8;
    QComboBox *FCombo9;
    QComboBox *FCombo10;
    QComboBox *FCombo11;

    QPushButton *state4;

public slots:
    void ResetAll();

private slots:
    void StartROIDef();
    void ROIMod();
    void BKGMod();
    void PeaksMod();
    void BaselineSub();
    void StartFindPeaks();
    void StopPeaks();
    void CancelPeaks();
    void StartMultiFit();
    void SetFitFunction0(int k);
    void SetFitFunction1(int k);
    void SetFitFunction2(int k);
    void SetFitFunction3(int k);
    void SetFitFunction4(int k);
    void SetFitFunction5(int k);
    void SetFitFunction6(int k);
    void SetFitFunction7(int k);
    void SetFitFunction8(int k);
    void SetFitFunction9(int k);
    void SetFitFunction10(int k);
    void SetFitFunction11(int k);


private:
    void createROIGroupBox();
    void createBaselineGroupBox();
    void createFitFunctionsGroupBox();
    void createPeaksGroupBox();


signals:
    void FP();
    void StopFindPeaks();
    void Fit_tutti();
    void ClearAll();
    void PTable();



};

#endif
