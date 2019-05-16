/****************************************************************************
** Meta object code from reading C++ file 'DppGui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DppGui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DppGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindowDPP_t {
    QByteArrayData data[17];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindowDPP_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindowDPP_t qt_meta_stringdata_MainWindowDPP = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MainWindowDPP"
QT_MOC_LITERAL(1, 14, 13), // "creatorDppGui"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "overrideDppParam"
QT_MOC_LITERAL(4, 46, 10), // "Digi_range"
QT_MOC_LITERAL(5, 57, 13), // "Digi_treshold"
QT_MOC_LITERAL(6, 71, 13), // "paramDCOffset"
QT_MOC_LITERAL(7, 85, 7), // "HoldOff"
QT_MOC_LITERAL(8, 93, 16), // "Smoothing_Factor"
QT_MOC_LITERAL(9, 110, 9), // "Rise_Time"
QT_MOC_LITERAL(10, 120, 9), // "Fall_Time"
QT_MOC_LITERAL(11, 130, 8), // "Flat_Top"
QT_MOC_LITERAL(12, 139, 13), // "Peaking_Delay"
QT_MOC_LITERAL(13, 153, 14), // "Base_Line_Mean"
QT_MOC_LITERAL(14, 168, 14), // "Trapezoid_Gain"
QT_MOC_LITERAL(15, 183, 9), // "Peak_Mean"
QT_MOC_LITERAL(16, 193, 12) // "Peak_Holdoff"

    },
    "MainWindowDPP\0creatorDppGui\0\0"
    "overrideDppParam\0Digi_range\0Digi_treshold\0"
    "paramDCOffset\0HoldOff\0Smoothing_Factor\0"
    "Rise_Time\0Fall_Time\0Flat_Top\0Peaking_Delay\0"
    "Base_Line_Mean\0Trapezoid_Gain\0Peak_Mean\0"
    "Peak_Holdoff"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindowDPP[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       4,    1,   93,    2, 0x08 /* Private */,
       5,    1,   96,    2, 0x08 /* Private */,
       6,    1,   99,    2, 0x08 /* Private */,
       7,    1,  102,    2, 0x08 /* Private */,
       8,    1,  105,    2, 0x08 /* Private */,
       9,    1,  108,    2, 0x08 /* Private */,
      10,    1,  111,    2, 0x08 /* Private */,
      11,    1,  114,    2, 0x08 /* Private */,
      12,    1,  117,    2, 0x08 /* Private */,
      13,    1,  120,    2, 0x08 /* Private */,
      14,    1,  123,    2, 0x08 /* Private */,
      15,    1,  126,    2, 0x08 /* Private */,
      16,    1,  129,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MainWindowDPP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindowDPP *_t = static_cast<MainWindowDPP *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->creatorDppGui(); break;
        case 1: _t->overrideDppParam((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->Digi_range((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->Digi_treshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->paramDCOffset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->HoldOff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->Smoothing_Factor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->Rise_Time((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->Fall_Time((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->Flat_Top((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->Peaking_Delay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->Base_Line_Mean((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->Trapezoid_Gain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->Peak_Mean((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->Peak_Holdoff((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindowDPP::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindowDPP.data,
      qt_meta_data_MainWindowDPP,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindowDPP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindowDPP::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindowDPP.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindowDPP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
