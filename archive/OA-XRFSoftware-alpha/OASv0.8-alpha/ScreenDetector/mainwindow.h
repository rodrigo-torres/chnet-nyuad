#ifndef MAINWINDOW_H
#define MAINWINDOW_H


////////////////////// uic-qt4 file.ui > file.h

#include <QtGui/QMainWindow>
#include <QMainWindow>

#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QtGui> //necessaria per il qDebug
#include <qsharedmemory.h>
#include <QThread>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};


 class MainWindow : public QMainWindow
 {
     Q_OBJECT


 public:
//     explicit MainWindow(QWidget *parent = 0);
//     ~MainWindow();


 
 private:
//    Ui::MainWindow *ui;


 protected:
//     void closeEvent(QCloseEvent *event);

 private slots:



 };



 #endif

















