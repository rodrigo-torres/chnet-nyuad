#ifndef MAINWINDOW_DPP
#define MAINWINDOW_DPP

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFile>
#include <QString>
#include <QObject>

class QAction;
class QWidget;

class MainWindowDPP : public QMainWindow {
    Q_OBJECT

public:
    MainWindowDPP();
    ~MainWindowDPP();
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void creatorDppGui();
    void overrideDppParam(bool);
    void Digi_range(int);
    void Digi_treshold(int);
    void paramDCOffset(int);
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
    QSpinBox *spinBox8;
    QSpinBox *spinBox9;
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
};

#endif
