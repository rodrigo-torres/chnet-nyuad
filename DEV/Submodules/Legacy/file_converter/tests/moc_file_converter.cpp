/****************************************************************************
** Meta object code from reading C++ file 'file_converter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/file_converter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'file_converter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_maxrf__FileConverter_t {
    QByteArrayData data[7];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_maxrf__FileConverter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_maxrf__FileConverter_t qt_meta_stringdata_maxrf__FileConverter = {
    {
QT_MOC_LITERAL(0, 0, 20), // "maxrf::FileConverter"
QT_MOC_LITERAL(1, 21, 17), // "UpdateProgressBar"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "value"
QT_MOC_LITERAL(4, 46, 15), // "UpdateStatusBar"
QT_MOC_LITERAL(5, 62, 11), // "std::string"
QT_MOC_LITERAL(6, 74, 7) // "message"

    },
    "maxrf::FileConverter\0UpdateProgressBar\0"
    "\0value\0UpdateStatusBar\0std::string\0"
    "message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_maxrf__FileConverter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void maxrf::FileConverter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileConverter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateProgressBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->UpdateStatusBar((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FileConverter::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileConverter::UpdateProgressBar)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FileConverter::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileConverter::UpdateStatusBar)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject maxrf::FileConverter::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_maxrf__FileConverter.data,
    qt_meta_data_maxrf__FileConverter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *maxrf::FileConverter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *maxrf::FileConverter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_maxrf__FileConverter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int maxrf::FileConverter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void maxrf::FileConverter::UpdateProgressBar(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void maxrf::FileConverter::UpdateStatusBar(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
