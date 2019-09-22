/****************************************************************************
** Meta object code from reading C++ file 'plot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "plot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Plot_t {
    QByteArrayData data[16];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Plot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Plot_t qt_meta_stringdata_Plot = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Plot"
QT_MOC_LITERAL(1, 5, 17), // "calibrationActive"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "active"
QT_MOC_LITERAL(4, 31, 13), // "statusChanged"
QT_MOC_LITERAL(5, 45, 8), // "filename"
QT_MOC_LITERAL(6, 54, 11), // "Calibration"
QT_MOC_LITERAL(7, 66, 8), // "gradient"
QT_MOC_LITERAL(8, 75, 6), // "offset"
QT_MOC_LITERAL(9, 82, 4), // "Open"
QT_MOC_LITERAL(10, 87, 9), // "Check_SHM"
QT_MOC_LITERAL(11, 97, 14), // "showPixelHisto"
QT_MOC_LITERAL(12, 112, 15), // "loadCalibration"
QT_MOC_LITERAL(13, 128, 11), // "setCalParam"
QT_MOC_LITERAL(14, 140, 4), // "data"
QT_MOC_LITERAL(15, 145, 13) // "writeCalParam"

    },
    "Plot\0calibrationActive\0\0active\0"
    "statusChanged\0filename\0Calibration\0"
    "gradient\0offset\0Open\0Check_SHM\0"
    "showPixelHisto\0loadCalibration\0"
    "setCalParam\0data\0writeCalParam"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Plot[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   70,    2, 0x0a /* Public */,
       9,    0,   75,    2, 0x0a /* Public */,
      10,    0,   76,    2, 0x0a /* Public */,
      11,    0,   77,    2, 0x0a /* Public */,
      12,    0,   78,    2, 0x0a /* Public */,
      13,    0,   79,    2, 0x0a /* Public */,
      13,    1,   80,    2, 0x0a /* Public */,
      15,    1,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void Plot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Plot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->calibrationActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->Calibration((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->Open(); break;
        case 4: _t->Check_SHM(); break;
        case 5: _t->showPixelHisto(); break;
        case 6: _t->loadCalibration(); break;
        case 7: _t->setCalParam(); break;
        case 8: _t->setCalParam((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->writeCalParam((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Plot::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Plot::calibrationActive)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Plot::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Plot::statusChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Plot::staticMetaObject = { {
    &QwtPlot::staticMetaObject,
    qt_meta_stringdata_Plot.data,
    qt_meta_data_Plot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Plot::calibrationActive(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Plot::statusChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
