/****************************************************************************
** Meta object code from reading C++ file 'Dial.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Dial.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x08,
      26,    8,    8,    8, 0x08,
      44,    8,   40,    8, 0x08,
      64,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DialBox[] = {
    "DialBox\0\0v\0setNum(double)\0EventEnable()\0"
    "int\0setNeedleValue(int)\0TimerEvent()\0"
};

void DialBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DialBox *_t = static_cast<DialBox *>(_o);
        switch (_id) {
        case 0: _t->setNum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->EventEnable(); break;
        case 2: { int _r = _t->setNeedleValue((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: _t->TimerEvent(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DialBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DialBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DialBox,
      qt_meta_data_DialBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialBox))
        return static_cast<void*>(const_cast< DialBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int DialBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
