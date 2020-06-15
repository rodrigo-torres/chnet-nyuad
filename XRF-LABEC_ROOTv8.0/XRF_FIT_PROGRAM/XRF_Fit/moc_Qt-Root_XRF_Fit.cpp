/****************************************************************************
** Meta object code from reading C++ file 'Qt-Root_XRF_Fit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qt-Root_XRF_Fit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Qt-Root_XRF_Fit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QRootCanvas_t {
    QByteArrayData data[5];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QRootCanvas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QRootCanvas_t qt_meta_stringdata_QRootCanvas = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QRootCanvas"
QT_MOC_LITERAL(1, 12, 8), // "sendText"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "QString&"
QT_MOC_LITERAL(4, 31, 7) // "newtext"

    },
    "QRootCanvas\0sendText\0\0QString&\0newtext"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QRootCanvas[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void QRootCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QRootCanvas *_t = static_cast<QRootCanvas *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (QRootCanvas::*_t)(QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QRootCanvas::sendText)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QRootCanvas::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QRootCanvas.data,
      qt_meta_data_QRootCanvas,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QRootCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QRootCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QRootCanvas.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QRootCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QRootCanvas::sendText(QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[25];
    char stringdata0[315];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 15), // "LogStateChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "state"
QT_MOC_LITERAL(4, 34, 16), // "GridStateChanged"
QT_MOC_LITERAL(5, 51, 16), // "SetCurrentAction"
QT_MOC_LITERAL(6, 68, 8), // "QString&"
QT_MOC_LITERAL(7, 77, 4), // "text"
QT_MOC_LITERAL(8, 82, 18), // "handle_root_events"
QT_MOC_LITERAL(9, 101, 17), // "Select_Background"
QT_MOC_LITERAL(10, 119, 9), // "SingleFit"
QT_MOC_LITERAL(11, 129, 9), // "DoubleFit"
QT_MOC_LITERAL(12, 139, 9), // "Calibrate"
QT_MOC_LITERAL(13, 149, 9), // "FindPeaks"
QT_MOC_LITERAL(14, 159, 12), // "LoadSpectrum"
QT_MOC_LITERAL(15, 172, 8), // "LoadCaen"
QT_MOC_LITERAL(16, 181, 9), // "OpenTable"
QT_MOC_LITERAL(17, 191, 12), // "PTtimerEvent"
QT_MOC_LITERAL(18, 204, 18), // "openMultiFitWindow"
QT_MOC_LITERAL(19, 223, 11), // "FP_rimbalzo"
QT_MOC_LITERAL(20, 235, 13), // "IndicatePeaks"
QT_MOC_LITERAL(21, 249, 13), // "Fit_All_Peaks"
QT_MOC_LITERAL(22, 263, 14), // "Clear_MultiFit"
QT_MOC_LITERAL(23, 278, 13), // "ClearSpectrum"
QT_MOC_LITERAL(24, 292, 22) // "SuperImposeNewSpectrum"

    },
    "MainWindow\0LogStateChanged\0\0state\0"
    "GridStateChanged\0SetCurrentAction\0"
    "QString&\0text\0handle_root_events\0"
    "Select_Background\0SingleFit\0DoubleFit\0"
    "Calibrate\0FindPeaks\0LoadSpectrum\0"
    "LoadCaen\0OpenTable\0PTtimerEvent\0"
    "openMultiFitWindow\0FP_rimbalzo\0"
    "IndicatePeaks\0Fit_All_Peaks\0Clear_MultiFit\0"
    "ClearSpectrum\0SuperImposeNewSpectrum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x0a /* Public */,
       4,    1,  117,    2, 0x0a /* Public */,
       5,    1,  120,    2, 0x0a /* Public */,
       8,    0,  123,    2, 0x0a /* Public */,
       9,    0,  124,    2, 0x0a /* Public */,
      10,    0,  125,    2, 0x0a /* Public */,
      11,    0,  126,    2, 0x0a /* Public */,
      12,    0,  127,    2, 0x0a /* Public */,
      13,    0,  128,    2, 0x0a /* Public */,
      14,    0,  129,    2, 0x0a /* Public */,
      15,    0,  130,    2, 0x0a /* Public */,
      16,    0,  131,    2, 0x0a /* Public */,
      17,    0,  132,    2, 0x0a /* Public */,
      18,    0,  133,    2, 0x0a /* Public */,
      19,    0,  134,    2, 0x0a /* Public */,
      20,    0,  135,    2, 0x0a /* Public */,
      21,    0,  136,    2, 0x0a /* Public */,
      22,    0,  137,    2, 0x0a /* Public */,
      23,    0,  138,    2, 0x0a /* Public */,
      24,    0,  139,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LogStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->GridStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SetCurrentAction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->handle_root_events(); break;
        case 4: _t->Select_Background(); break;
        case 5: _t->SingleFit(); break;
        case 6: _t->DoubleFit(); break;
        case 7: _t->Calibrate(); break;
        case 8: _t->FindPeaks(); break;
        case 9: _t->LoadSpectrum(); break;
        case 10: _t->LoadCaen(); break;
        case 11: _t->OpenTable(); break;
        case 12: _t->PTtimerEvent(); break;
        case 13: _t->openMultiFitWindow(); break;
        case 14: _t->FP_rimbalzo(); break;
        case 15: _t->IndicatePeaks(); break;
        case 16: _t->Fit_All_Peaks(); break;
        case 17: _t->Clear_MultiFit(); break;
        case 18: _t->ClearSpectrum(); break;
        case 19: _t->SuperImposeNewSpectrum(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
