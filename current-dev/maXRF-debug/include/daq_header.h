#ifndef DAQ_HEADER_H
#define DAQ_HEADER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

namespace daq
{

class DAQHeader : public QMainWindow
{
    Q_OBJECT
public:
    explicit DAQHeader(QWidget *parent = nullptr);

private:
    void setupUi();

    QWidget* header_widget_ {nullptr};

signals:

public slots:
};

} // namespace daq

#endif // DAQ_HEADER_H
/*
 * first we create a mainwindow object and raise it
 * the DAQHEader class will create a widget object which is passed to the
 * mainwindow as the mainwidget.
 * The class we'll have some methods to pass on the formatted string back to main
 * program.
 */
