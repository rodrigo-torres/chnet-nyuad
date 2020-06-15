#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QFont>
#include <QWidget>
#include <QFile>
#include <QDir>
#include <QTextStream> 
#include <QString> 
#include <QtGui> //necessaria per il qDebug
#include <QObject>
#include <QTimerEvent>
#include <QFileDialog>
#include <QByteArray>
#include <QBuffer>
#include <qsharedmemory.h>


class QAction;
class QWidget;

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
    MainWindow();
    ~MainWindow();


protected:
//    void closeEvent(QCloseEvent *event);

private slots:
    void Check_USB_Status();

public:
     QTimer *timer;



};

#endif
