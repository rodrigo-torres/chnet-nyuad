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
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      19,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      50,   11,   11,   11, 0x08,
      59,   11,   11,   11, 0x08,
      66,   11,   11,   11, 0x08,
      83,   11,   11,   11, 0x08,
     111,  104,   96,   11, 0x08,
     131,   11,   11,   11, 0x08,
     149,  141,   11,   11, 0x08,
     160,   11,   11,   11, 0x08,
     171,   11,   11,   11, 0x08,
     198,  188,   11,   11, 0x08,
     219,   11,   11,   11, 0x08,
     232,   11,   11,   11, 0x08,
     253,  247,   11,   11, 0x08,
     272,  247,   11,   11, 0x08,
     300,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0Exit()\0Send_CMD()\0"
    "InizializzazioneX()\0StartX()\0Stop()\0"
    "CheckXOnTarget()\0timerEvent()\0QString\0"
    "number\0SetSerialXName(int)\0AssignX()\0"
    "numberx\0NameX(int)\0MoveX_To()\0"
    "Velocity(double)\0motortype\0"
    "Motor_selection(int)\0X_To(double)\0"
    "Motor_CMD(int)\0input\0SPA_Parameter(int)\0"
    "SPA_Parameter_value(double)\0"
    "Show_PI_Parameters()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->Exit(); break;
        case 1: _t->Send_CMD(); break;
        case 2: _t->InizializzazioneX(); break;
        case 3: _t->StartX(); break;
        case 4: _t->Stop(); break;
        case 5: _t->CheckXOnTarget(); break;
        case 6: _t->timerEvent(); break;
        case 7: { QString _r = _t->SetSerialXName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: _t->AssignX(); break;
        case 9: _t->NameX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->MoveX_To(); break;
        case 11: _t->Velocity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->Motor_selection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->X_To((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->Motor_CMD((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->SPA_Parameter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->SPA_Parameter_value((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 17: _t->Show_PI_Parameters(); break;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
