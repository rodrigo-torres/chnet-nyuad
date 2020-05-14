/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[56];
    char stringdata0[795];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 18), // "request_tty_action"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "set_target"
QT_MOC_LITERAL(4, 42, 15), // "keyence_reading"
QT_MOC_LITERAL(5, 58, 11), // "start_servo"
QT_MOC_LITERAL(6, 70, 11), // "toggle_tab1"
QT_MOC_LITERAL(7, 82, 14), // "toggle_widgets"
QT_MOC_LITERAL(8, 97, 14), // "update_monitor"
QT_MOC_LITERAL(9, 112, 15), // "tab3_set_target"
QT_MOC_LITERAL(10, 128, 18), // "handle_pushbuttons"
QT_MOC_LITERAL(11, 147, 14), // "LoadSHM_SumMap"
QT_MOC_LITERAL(12, 162, 12), // "enable_servo"
QT_MOC_LITERAL(13, 175, 12), // "time_monitor"
QT_MOC_LITERAL(14, 188, 22), // "SelectLowIntegralLimit"
QT_MOC_LITERAL(15, 211, 3), // "val"
QT_MOC_LITERAL(16, 215, 23), // "SelectHighIntegralLimit"
QT_MOC_LITERAL(17, 239, 15), // "scan_parameters"
QT_MOC_LITERAL(18, 255, 7), // "Info1_1"
QT_MOC_LITERAL(19, 263, 7), // "Info1_2"
QT_MOC_LITERAL(20, 271, 7), // "Info2_1"
QT_MOC_LITERAL(21, 279, 8), // "GoOnLine"
QT_MOC_LITERAL(22, 288, 17), // "DisplayImageLabel"
QT_MOC_LITERAL(23, 306, 19), // "displaySumImage_SHM"
QT_MOC_LITERAL(24, 326, 15), // "LoadNewFile_SHM"
QT_MOC_LITERAL(25, 342, 18), // "LoadElementsMapSum"
QT_MOC_LITERAL(26, 361, 18), // "writeCompMapLimits"
QT_MOC_LITERAL(27, 380, 14), // "SelectChannels"
QT_MOC_LITERAL(28, 395, 15), // "start_point_daq"
QT_MOC_LITERAL(29, 411, 14), // "stop_point_daq"
QT_MOC_LITERAL(30, 426, 21), // "set_PMAcquisitionTime"
QT_MOC_LITERAL(31, 448, 10), // "SelDigiCh0"
QT_MOC_LITERAL(32, 459, 10), // "SelDigiCh1"
QT_MOC_LITERAL(33, 470, 14), // "SelDigiCh0and1"
QT_MOC_LITERAL(34, 485, 4), // "TreD"
QT_MOC_LITERAL(35, 490, 4), // "DueD"
QT_MOC_LITERAL(36, 495, 20), // "ChangePixelDimension"
QT_MOC_LITERAL(37, 516, 13), // "ShowHistogram"
QT_MOC_LITERAL(38, 530, 9), // "RateMeter"
QT_MOC_LITERAL(39, 540, 14), // "load_optimized"
QT_MOC_LITERAL(40, 555, 17), // "LoadImageDataFile"
QT_MOC_LITERAL(41, 573, 18), // "readmultidetcalpar"
QT_MOC_LITERAL(42, 592, 16), // "Changeparameters"
QT_MOC_LITERAL(43, 609, 22), // "enable_keyence_reading"
QT_MOC_LITERAL(44, 632, 12), // "openDPPInter"
QT_MOC_LITERAL(45, 645, 8), // "Detector"
QT_MOC_LITERAL(46, 654, 16), // "Helium_interface"
QT_MOC_LITERAL(47, 671, 13), // "VLC_interface"
QT_MOC_LITERAL(48, 685, 16), // "caenoscilloscope"
QT_MOC_LITERAL(49, 702, 7), // "USB_DAQ"
QT_MOC_LITERAL(50, 710, 11), // "OPTICAL_DAQ"
QT_MOC_LITERAL(51, 722, 16), // "ExportImageToPNG"
QT_MOC_LITERAL(52, 739, 14), // "set_abort_flag"
QT_MOC_LITERAL(53, 754, 13), // "CheckSegFault"
QT_MOC_LITERAL(54, 768, 16), // "saveImageXRFData"
QT_MOC_LITERAL(55, 785, 9) // "XrayTable"

    },
    "MainWindow\0request_tty_action\0\0"
    "set_target\0keyence_reading\0start_servo\0"
    "toggle_tab1\0toggle_widgets\0update_monitor\0"
    "tab3_set_target\0handle_pushbuttons\0"
    "LoadSHM_SumMap\0enable_servo\0time_monitor\0"
    "SelectLowIntegralLimit\0val\0"
    "SelectHighIntegralLimit\0scan_parameters\0"
    "Info1_1\0Info1_2\0Info2_1\0GoOnLine\0"
    "DisplayImageLabel\0displaySumImage_SHM\0"
    "LoadNewFile_SHM\0LoadElementsMapSum\0"
    "writeCompMapLimits\0SelectChannels\0"
    "start_point_daq\0stop_point_daq\0"
    "set_PMAcquisitionTime\0SelDigiCh0\0"
    "SelDigiCh1\0SelDigiCh0and1\0TreD\0DueD\0"
    "ChangePixelDimension\0ShowHistogram\0"
    "RateMeter\0load_optimized\0LoadImageDataFile\0"
    "readmultidetcalpar\0Changeparameters\0"
    "enable_keyence_reading\0openDPPInter\0"
    "Detector\0Helium_interface\0VLC_interface\0"
    "caenoscilloscope\0USB_DAQ\0OPTICAL_DAQ\0"
    "ExportImageToPNG\0set_abort_flag\0"
    "CheckSegFault\0saveImageXRFData\0XrayTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      54,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  284,    2, 0x06 /* Public */,
       1,    1,  289,    2, 0x26 /* Public | MethodCloned */,
       3,    2,  292,    2, 0x06 /* Public */,
       4,    1,  297,    2, 0x06 /* Public */,
       5,    1,  300,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,  303,    2, 0x0a /* Public */,
       7,    1,  306,    2, 0x0a /* Public */,
       8,    3,  309,    2, 0x0a /* Public */,
       9,    0,  316,    2, 0x0a /* Public */,
      10,    0,  317,    2, 0x0a /* Public */,
      11,    0,  318,    2, 0x0a /* Public */,
      12,    0,  319,    2, 0x0a /* Public */,
      13,    1,  320,    2, 0x0a /* Public */,
      14,    1,  323,    2, 0x0a /* Public */,
      16,    1,  326,    2, 0x0a /* Public */,
      17,    1,  329,    2, 0x08 /* Private */,
      18,    0,  332,    2, 0x08 /* Private */,
      19,    0,  333,    2, 0x08 /* Private */,
      20,    0,  334,    2, 0x08 /* Private */,
      21,    0,  335,    2, 0x08 /* Private */,
      22,    0,  336,    2, 0x08 /* Private */,
      23,    0,  337,    2, 0x08 /* Private */,
      24,    0,  338,    2, 0x08 /* Private */,
      25,    0,  339,    2, 0x08 /* Private */,
      26,    1,  340,    2, 0x08 /* Private */,
      27,    0,  343,    2, 0x08 /* Private */,
      28,    0,  344,    2, 0x08 /* Private */,
      29,    0,  345,    2, 0x08 /* Private */,
      30,    0,  346,    2, 0x08 /* Private */,
      31,    0,  347,    2, 0x08 /* Private */,
      32,    0,  348,    2, 0x08 /* Private */,
      33,    0,  349,    2, 0x08 /* Private */,
      34,    0,  350,    2, 0x08 /* Private */,
      35,    0,  351,    2, 0x08 /* Private */,
      36,    0,  352,    2, 0x08 /* Private */,
      37,    0,  353,    2, 0x08 /* Private */,
      38,    0,  354,    2, 0x08 /* Private */,
      39,    0,  355,    2, 0x08 /* Private */,
      40,    0,  356,    2, 0x08 /* Private */,
      41,    0,  357,    2, 0x08 /* Private */,
      42,    0,  358,    2, 0x08 /* Private */,
      43,    0,  359,    2, 0x08 /* Private */,
      44,    0,  360,    2, 0x08 /* Private */,
      45,    0,  361,    2, 0x08 /* Private */,
      46,    0,  362,    2, 0x08 /* Private */,
      47,    0,  363,    2, 0x08 /* Private */,
      48,    0,  364,    2, 0x08 /* Private */,
      49,    1,  365,    2, 0x08 /* Private */,
      50,    0,  368,    2, 0x08 /* Private */,
      51,    0,  369,    2, 0x08 /* Private */,
      52,    0,  370,    2, 0x08 /* Private */,
      53,    0,  371,    2, 0x08 /* Private */,
      54,    0,  372,    2, 0x08 /* Private */,
      55,    0,  373,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
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
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->request_tty_action((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->request_tty_action((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->set_target((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->keyence_reading((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->start_servo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->toggle_tab1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->toggle_widgets((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->update_monitor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->tab3_set_target(); break;
        case 9: _t->handle_pushbuttons(); break;
        case 10: _t->LoadSHM_SumMap(); break;
        case 11: _t->enable_servo(); break;
        case 12: _t->time_monitor((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->SelectLowIntegralLimit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->SelectHighIntegralLimit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->scan_parameters((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->Info1_1(); break;
        case 17: _t->Info1_2(); break;
        case 18: _t->Info2_1(); break;
        case 19: _t->GoOnLine(); break;
        case 20: _t->DisplayImageLabel(); break;
        case 21: _t->displaySumImage_SHM(); break;
        case 22: _t->LoadNewFile_SHM(); break;
        case 23: _t->LoadElementsMapSum(); break;
        case 24: _t->writeCompMapLimits((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->SelectChannels(); break;
        case 26: _t->start_point_daq(); break;
        case 27: _t->stop_point_daq(); break;
        case 28: _t->set_PMAcquisitionTime(); break;
        case 29: _t->SelDigiCh0(); break;
        case 30: _t->SelDigiCh1(); break;
        case 31: _t->SelDigiCh0and1(); break;
        case 32: _t->TreD(); break;
        case 33: _t->DueD(); break;
        case 34: _t->ChangePixelDimension(); break;
        case 35: _t->ShowHistogram(); break;
        case 36: _t->RateMeter(); break;
        case 37: _t->load_optimized(); break;
        case 38: _t->LoadImageDataFile(); break;
        case 39: _t->readmultidetcalpar(); break;
        case 40: _t->Changeparameters(); break;
        case 41: _t->enable_keyence_reading(); break;
        case 42: _t->openDPPInter(); break;
        case 43: _t->Detector(); break;
        case 44: _t->Helium_interface(); break;
        case 45: _t->VLC_interface(); break;
        case 46: _t->caenoscilloscope(); break;
        case 47: _t->USB_DAQ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->OPTICAL_DAQ(); break;
        case 49: _t->ExportImageToPNG(); break;
        case 50: _t->set_abort_flag(); break;
        case 51: _t->CheckSegFault(); break;
        case 52: _t->saveImageXRFData(); break;
        case 53: _t->XrayTable(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::request_tty_action)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::set_target)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::keyence_reading)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::start_servo)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 54)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 54;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 54)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 54;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::request_tty_action(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void MainWindow::set_target(int _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::keyence_reading(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::start_servo(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
