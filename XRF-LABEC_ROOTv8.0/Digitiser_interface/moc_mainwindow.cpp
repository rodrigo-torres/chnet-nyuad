/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      25,   11,   11,   11, 0x08,
      41,   11,   11,   11, 0x08,
      60,   11,   11,   11, 0x08,
      75,   11,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
     110,   11,   11,   11, 0x08,
     125,   11,   11,   11, 0x08,
     140,   11,   11,   11, 0x08,
     154,   11,   11,   11, 0x08,
     173,   11,   11,   11, 0x08,
     193,   11,   11,   11, 0x08,
     213,   11,   11,   11, 0x08,
     228,   11,   11,   11, 0x08,
     246,   11,   11,   11, 0x08,
     286,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0GuiCreator()\0Digi_range(int)\0"
    "Digi_treshold(int)\0DC_Offset(int)\0"
    "HoldOff(int)\0Smoothing_Factor(int)\0"
    "Rise_Time(int)\0Fall_Time(int)\0"
    "Flat_Top(int)\0Peaking_Delay(int)\0"
    "Base_Line_Mean(int)\0Trapezoid_Gain(int)\0"
    "Peak_Mean(int)\0Peak_Holdoff(int)\0"
    "Digitiser_data_download_enable_ON(bool)\0"
    "Exit()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->GuiCreator(); break;
        case 1: _t->Digi_range((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->Digi_treshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->DC_Offset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->HoldOff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->Smoothing_Factor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->Rise_Time((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->Fall_Time((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->Flat_Top((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->Peaking_Delay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->Base_Line_Mean((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->Trapezoid_Gain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->Peak_Mean((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->Peak_Holdoff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->Digitiser_data_download_enable_ON((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->Exit(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
