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
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   12,   11,   11, 0x0a,
      36,   11,   11,   11, 0x0a,
      58,   11,   11,   11, 0x0a,
      76,   11,   11,   11, 0x0a,
      93,   87,   11,   11, 0x0a,
     123,   11,   11,   11, 0x0a,
     133,   11,   11,   11, 0x0a,
     145,   11,   11,   11, 0x0a,
     157,   11,   11,   11, 0x0a,
     170,   11,   11,   11, 0x0a,
     181,   11,   11,   11, 0x08,
     202,   11,  197,   11, 0x08,
     214,   11,  197,   11, 0x08,
     228,   11,   11,   11, 0x08,
     240,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0run\0enableRunMode(bool)\0"
    "enableAutoScale(bool)\0enableOnTop(bool)\0"
    "showInfo()\0event\0mousePressEvent(QMouseEvent*)\0"
    "PreOpen()\0on_k4(bool)\0on_k8(bool)\0"
    "on_k16(bool)\0logSpace()\0AutoCalibrate()\0"
    "bool\0okClicked()\0cancClicked()\0"
    "exportTxt()\0enableZoomMode()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->enableRunMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->enableAutoScale((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->enableOnTop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->showInfo(); break;
        case 4: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: _t->PreOpen(); break;
        case 6: _t->on_k4((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_k8((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_k16((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->logSpace(); break;
        case 10: _t->AutoCalibrate(); break;
        case 11: { bool _r = _t->okClicked();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->cancClicked();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: _t->exportTxt(); break;
        case 14: _t->enableZoomMode(); break;
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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
