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
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      25,   11,   11,   11, 0x0a,
      38,   11,   11,   11, 0x0a,
      51,   11,   11,   11, 0x0a,
      64,   11,   11,   11, 0x0a,
      77,   11,   11,   11, 0x0a,
      90,   11,   11,   11, 0x0a,
     103,   11,   11,   11, 0x0a,
     116,   11,   11,   11, 0x0a,
     129,   11,   11,   11, 0x0a,
     141,   11,   11,   11, 0x0a,
     155,   11,   11,   11, 0x0a,
     168,   11,   11,   11, 0x0a,
     181,   11,   11,   11, 0x0a,
     194,   11,   11,   11, 0x0a,
     207,   11,   11,   11, 0x0a,
     221,   11,   11,   11, 0x0a,
     228,   11,   11,   11, 0x0a,
     249,   11,   11,   11, 0x0a,
     267,   11,   11,   11, 0x0a,
     286,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0Element_Ag()\0Element_Au()\0"
    "Element_Ba()\0Element_Ca()\0Element_Co()\0"
    "Element_Cr()\0Element_Cu()\0Element_Fe()\0"
    "Element_Hg()\0Element_K()\0Element_PbL()\0"
    "Element_Si()\0Element_Sn()\0Element_Ti()\0"
    "Element_Zn()\0Element_PbM()\0quit()\0"
    "displayImageOnLine()\0enableOnTop(bool)\0"
    "enableOnLine(bool)\0Define_Pixels()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->Element_Ag(); break;
        case 1: _t->Element_Au(); break;
        case 2: _t->Element_Ba(); break;
        case 3: _t->Element_Ca(); break;
        case 4: _t->Element_Co(); break;
        case 5: _t->Element_Cr(); break;
        case 6: _t->Element_Cu(); break;
        case 7: _t->Element_Fe(); break;
        case 8: _t->Element_Hg(); break;
        case 9: _t->Element_K(); break;
        case 10: _t->Element_PbL(); break;
        case 11: _t->Element_Si(); break;
        case 12: _t->Element_Sn(); break;
        case 13: _t->Element_Ti(); break;
        case 14: _t->Element_Zn(); break;
        case 15: _t->Element_PbM(); break;
        case 16: _t->quit(); break;
        case 17: _t->displayImageOnLine(); break;
        case 18: _t->enableOnTop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->enableOnLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->Define_Pixels(); break;
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
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
