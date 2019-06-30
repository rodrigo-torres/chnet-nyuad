/****************************************************************************
** Meta object code from reading C++ file 'tty.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tty.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tty.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_tty_t {
    QByteArrayData data[18];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tty_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tty_t qt_meta_stringdata_tty = {
    {
QT_MOC_LITERAL(0, 0, 3), // "tty"
QT_MOC_LITERAL(1, 4, 11), // "toggle_tab1"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 14), // "update_statbar"
QT_MOC_LITERAL(4, 32, 14), // "update_monitor"
QT_MOC_LITERAL(5, 47, 14), // "toggle_widgets"
QT_MOC_LITERAL(6, 62, 9), // "save_file"
QT_MOC_LITERAL(7, 72, 10), // "stage_init"
QT_MOC_LITERAL(8, 83, 8), // "tty_init"
QT_MOC_LITERAL(9, 92, 10), // "set_target"
QT_MOC_LITERAL(10, 103, 12), // "set_df_minor"
QT_MOC_LITERAL(11, 116, 12), // "set_velocity"
QT_MOC_LITERAL(12, 129, 10), // "move_stage"
QT_MOC_LITERAL(13, 140, 17), // "move_double_click"
QT_MOC_LITERAL(14, 158, 12), // "enable_servo"
QT_MOC_LITERAL(15, 171, 11), // "start_servo"
QT_MOC_LITERAL(16, 183, 4), // "scan"
QT_MOC_LITERAL(17, 188, 5) // "abort"

    },
    "tty\0toggle_tab1\0\0update_statbar\0"
    "update_monitor\0toggle_widgets\0save_file\0"
    "stage_init\0tty_init\0set_target\0"
    "set_df_minor\0set_velocity\0move_stage\0"
    "move_double_click\0enable_servo\0"
    "start_servo\0scan\0abort"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tty[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       3,    1,   97,    2, 0x06 /* Public */,
       4,    3,  100,    2, 0x06 /* Public */,
       5,    1,  107,    2, 0x06 /* Public */,
       6,    0,  110,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,  111,    2, 0x0a /* Public */,
       8,    1,  114,    2, 0x0a /* Public */,
       9,    2,  117,    2, 0x0a /* Public */,
      10,    1,  122,    2, 0x0a /* Public */,
      11,    1,  125,    2, 0x0a /* Public */,
      12,    1,  128,    2, 0x0a /* Public */,
      13,    0,  131,    2, 0x0a /* Public */,
      14,    1,  132,    2, 0x0a /* Public */,
      15,    1,  135,    2, 0x0a /* Public */,
      16,    0,  138,    2, 0x0a /* Public */,
      17,    0,  139,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void tty::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<tty *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toggle_tab1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->update_statbar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->update_monitor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->toggle_widgets((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->save_file(); break;
        case 5: _t->stage_init((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->tty_init((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->set_target((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->set_df_minor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->set_velocity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->move_stage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->move_double_click(); break;
        case 12: _t->enable_servo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->start_servo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->scan(); break;
        case 15: _t->abort(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (tty::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tty::toggle_tab1)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (tty::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tty::update_statbar)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (tty::*)(QString , QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tty::update_monitor)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (tty::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tty::toggle_widgets)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (tty::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tty::save_file)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject tty::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_tty.data,
    qt_meta_data_tty,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *tty::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tty::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_tty.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int tty::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void tty::toggle_tab1(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void tty::update_statbar(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void tty::update_monitor(QString _t1, QString _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void tty::toggle_widgets(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void tty::save_file()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
