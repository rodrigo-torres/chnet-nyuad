/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata0[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "enableRunMode"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "run"
QT_MOC_LITERAL(4, 30, 17), // "update_fileStatus"
QT_MOC_LITERAL(5, 48, 8), // "filename"
QT_MOC_LITERAL(6, 57, 15), // "enableAutoScale"
QT_MOC_LITERAL(7, 73, 11), // "enableOnTop"
QT_MOC_LITERAL(8, 85, 8), // "showInfo"
QT_MOC_LITERAL(9, 94, 15), // "mousePressEvent"
QT_MOC_LITERAL(10, 110, 12), // "QMouseEvent*"
QT_MOC_LITERAL(11, 123, 5), // "event"
QT_MOC_LITERAL(12, 129, 7), // "PreOpen"
QT_MOC_LITERAL(13, 137, 14), // "toggleLogScale"
QT_MOC_LITERAL(14, 152, 12), // "toggleEnergy"
QT_MOC_LITERAL(15, 165, 6), // "active"
QT_MOC_LITERAL(16, 172, 13), // "AutoCalibrate"
QT_MOC_LITERAL(17, 186, 9), // "exportTxt"
QT_MOC_LITERAL(18, 196, 14) // "enableZoomMode"

    },
    "MainWindow\0enableRunMode\0\0run\0"
    "update_fileStatus\0filename\0enableAutoScale\0"
    "enableOnTop\0showInfo\0mousePressEvent\0"
    "QMouseEvent*\0event\0PreOpen\0toggleLogScale\0"
    "toggleEnergy\0active\0AutoCalibrate\0"
    "exportTxt\0enableZoomMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a /* Public */,
       4,    1,   77,    2, 0x0a /* Public */,
       6,    1,   80,    2, 0x0a /* Public */,
       7,    1,   83,    2, 0x0a /* Public */,
       8,    0,   86,    2, 0x0a /* Public */,
       9,    1,   87,    2, 0x0a /* Public */,
      12,    0,   90,    2, 0x0a /* Public */,
      13,    0,   91,    2, 0x0a /* Public */,
      14,    1,   92,    2, 0x0a /* Public */,
      16,    0,   95,    2, 0x08 /* Private */,
      17,    0,   96,    2, 0x08 /* Private */,
      18,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enableRunMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->update_fileStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->enableAutoScale((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->enableOnTop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->showInfo(); break;
        case 5: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->PreOpen(); break;
        case 7: _t->toggleLogScale(); break;
        case 8: _t->toggleEnergy((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->AutoCalibrate(); break;
        case 10: _t->exportTxt(); break;
        case 11: _t->enableZoomMode(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
