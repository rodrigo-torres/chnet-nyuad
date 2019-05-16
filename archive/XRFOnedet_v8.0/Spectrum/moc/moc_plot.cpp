/****************************************************************************
** Meta object code from reading C++ file 'plot.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../plot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Plot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,   25,   29,   29, 0x0a,
      30,   50,   29,   29, 0x0a,
      59,   74,   29,   29, 0x0a,
      78,   29,   29,   29, 0x0a,
      98,   29,   29,   29, 0x0a,
     105,   29,   29,   29, 0x0a,
     117,   29,   29,   29, 0x0a,
     134,   29,   29,   29, 0x0a,
     146,   29,   29,   29, 0x0a,
     158,   29,   29,   29, 0x0a,
     170,   29,   29,   29, 0x0a,
     181,   29,   29,   29, 0x0a,
     192,   29,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Plot[] = {
    "Plot\0enableRunMode(bool)\0run\0\0"
    "Calibration(double)\0constant\0"
    "Offset(double)\0off\0timerRefreshEvent()\0"
    "Open()\0Check_SHM()\0showPixelHisto()\0"
    "Retta_Cal()\0ch1(double)\0ch2(double)\0"
    "E1(double)\0E2(double)\0logSpace()\0"
};

void Plot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Plot *_t = static_cast<Plot *>(_o);
        switch (_id) {
        case 0: _t->enableRunMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->Calibration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->Offset((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->timerRefreshEvent(); break;
        case 4: _t->Open(); break;
        case 5: _t->Check_SHM(); break;
        case 6: _t->showPixelHisto(); break;
        case 7: _t->Retta_Cal(); break;
        case 8: _t->ch1((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->ch2((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->E1((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->E2((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->logSpace(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Plot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Plot::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_Plot,
      qt_meta_data_Plot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Plot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Plot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Plot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Plot))
        return static_cast<void*>(const_cast< Plot*>(this));
    return QwtPlot::qt_metacast(_clname);
}

int Plot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
