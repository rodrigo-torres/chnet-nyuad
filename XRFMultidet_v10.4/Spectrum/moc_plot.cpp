/****************************************************************************
** Meta object code from reading C++ file 'plot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "plot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Plot_t {
    QByteArrayData data[23];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Plot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Plot_t qt_meta_stringdata_Plot = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Plot"
QT_MOC_LITERAL(1, 5, 15), // "filenameChanged"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "filename"
QT_MOC_LITERAL(4, 31, 17), // "calibrationActive"
QT_MOC_LITERAL(5, 49, 6), // "active"
QT_MOC_LITERAL(6, 56, 11), // "Calibration"
QT_MOC_LITERAL(7, 68, 8), // "gradient"
QT_MOC_LITERAL(8, 77, 6), // "offset"
QT_MOC_LITERAL(9, 84, 11), // "setGradient"
QT_MOC_LITERAL(10, 96, 4), // "grad"
QT_MOC_LITERAL(11, 101, 9), // "setOffset"
QT_MOC_LITERAL(12, 111, 3), // "off"
QT_MOC_LITERAL(13, 115, 17), // "timerRefreshEvent"
QT_MOC_LITERAL(14, 133, 4), // "Open"
QT_MOC_LITERAL(15, 138, 9), // "Check_SHM"
QT_MOC_LITERAL(16, 148, 14), // "showPixelHisto"
QT_MOC_LITERAL(17, 163, 20), // "set_calibrationparam"
QT_MOC_LITERAL(18, 184, 3), // "ch1"
QT_MOC_LITERAL(19, 188, 3), // "ch2"
QT_MOC_LITERAL(20, 192, 2), // "E1"
QT_MOC_LITERAL(21, 195, 2), // "E2"
QT_MOC_LITERAL(22, 198, 15) // "loadCalibration"

    },
    "Plot\0filenameChanged\0\0filename\0"
    "calibrationActive\0active\0Calibration\0"
    "gradient\0offset\0setGradient\0grad\0"
    "setOffset\0off\0timerRefreshEvent\0Open\0"
    "Check_SHM\0showPixelHisto\0set_calibrationparam\0"
    "ch1\0ch2\0E1\0E2\0loadCalibration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Plot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   95,    2, 0x0a /* Public */,
       9,    1,  100,    2, 0x0a /* Public */,
      11,    1,  103,    2, 0x0a /* Public */,
      13,    0,  106,    2, 0x0a /* Public */,
      14,    0,  107,    2, 0x0a /* Public */,
      15,    0,  108,    2, 0x0a /* Public */,
      16,    0,  109,    2, 0x0a /* Public */,
      17,    0,  110,    2, 0x0a /* Public */,
      18,    1,  111,    2, 0x0a /* Public */,
      19,    1,  114,    2, 0x0a /* Public */,
      20,    1,  117,    2, 0x0a /* Public */,
      21,    1,  120,    2, 0x0a /* Public */,
      22,    0,  123,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,

       0        // eod
};

void Plot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Plot *_t = static_cast<Plot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->calibrationActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->Calibration((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->setGradient((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setOffset((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->timerRefreshEvent(); break;
        case 6: _t->Open(); break;
        case 7: _t->Check_SHM(); break;
        case 8: _t->showPixelHisto(); break;
        case 9: _t->set_calibrationparam(); break;
        case 10: _t->ch1((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->ch2((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->E1((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->E2((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->loadCalibration(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Plot::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Plot::filenameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Plot::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Plot::calibrationActive)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Plot::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_Plot.data,
      qt_meta_data_Plot,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Plot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Plot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Plot.stringdata0))
        return static_cast<void*>(this);
    return QwtPlot::qt_metacast(_clname);
}

int Plot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Plot::filenameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Plot::calibrationActive(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
