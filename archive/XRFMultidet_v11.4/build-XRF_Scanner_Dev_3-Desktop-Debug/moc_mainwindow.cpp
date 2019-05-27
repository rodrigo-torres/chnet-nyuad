/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../XRF_Scanner_Dev_3/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[106];
    char stringdata0[1142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 7), // "Info1_1"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 7), // "Info1_2"
QT_MOC_LITERAL(4, 28, 7), // "Info2_1"
QT_MOC_LITERAL(5, 36, 8), // "open_MAP"
QT_MOC_LITERAL(6, 45, 13), // "Define_Pixels"
QT_MOC_LITERAL(7, 59, 8), // "GoOnLine"
QT_MOC_LITERAL(8, 68, 16), // "displayImage_SHM"
QT_MOC_LITERAL(9, 85, 19), // "displaySumImage_SHM"
QT_MOC_LITERAL(10, 105, 9), // "hideImage"
QT_MOC_LITERAL(11, 115, 15), // "LoadNewFile_SHM"
QT_MOC_LITERAL(12, 131, 18), // "LoadElementsMapSum"
QT_MOC_LITERAL(13, 150, 18), // "writeCompMapLimits"
QT_MOC_LITERAL(14, 169, 14), // "SelectChannels"
QT_MOC_LITERAL(15, 184, 8), // "StartVme"
QT_MOC_LITERAL(16, 193, 8), // "Stop_Vme"
QT_MOC_LITERAL(17, 202, 21), // "set_PMAcquisitionTime"
QT_MOC_LITERAL(18, 224, 10), // "SelDigiCh0"
QT_MOC_LITERAL(19, 235, 10), // "SelDigiCh1"
QT_MOC_LITERAL(20, 246, 14), // "SelDigiCh0and1"
QT_MOC_LITERAL(21, 261, 4), // "TreD"
QT_MOC_LITERAL(22, 266, 4), // "DueD"
QT_MOC_LITERAL(23, 271, 6), // "Pixels"
QT_MOC_LITERAL(24, 278, 13), // "ShowHistogram"
QT_MOC_LITERAL(25, 292, 9), // "RateMeter"
QT_MOC_LITERAL(26, 302, 7), // "LoadTxt"
QT_MOC_LITERAL(27, 310, 8), // "MergeTxt"
QT_MOC_LITERAL(28, 319, 18), // "readmultidetcalpar"
QT_MOC_LITERAL(29, 338, 16), // "Changeparameters"
QT_MOC_LITERAL(30, 355, 10), // "Autofocus2"
QT_MOC_LITERAL(31, 366, 12), // "openDPPInter"
QT_MOC_LITERAL(32, 379, 8), // "Detector"
QT_MOC_LITERAL(33, 388, 16), // "Helium_interface"
QT_MOC_LITERAL(34, 405, 13), // "VLC_interface"
QT_MOC_LITERAL(35, 419, 16), // "caenoscilloscope"
QT_MOC_LITERAL(36, 436, 7), // "USB_DAQ"
QT_MOC_LITERAL(37, 444, 11), // "OPTICAL_DAQ"
QT_MOC_LITERAL(38, 456, 18), // "Enabling_Tabwidget"
QT_MOC_LITERAL(39, 475, 23), // "Enable_TabWidget_3_4_XY"
QT_MOC_LITERAL(40, 499, 12), // "Image_Export"
QT_MOC_LITERAL(41, 512, 8), // "Velocity"
QT_MOC_LITERAL(42, 521, 11), // "PassoX_Func"
QT_MOC_LITERAL(43, 533, 7), // "Xminimo"
QT_MOC_LITERAL(44, 541, 8), // "Xmassimo"
QT_MOC_LITERAL(45, 550, 11), // "PassoY_Func"
QT_MOC_LITERAL(46, 562, 7), // "Yminimo"
QT_MOC_LITERAL(47, 570, 8), // "Ymassimo"
QT_MOC_LITERAL(48, 579, 4), // "X_to"
QT_MOC_LITERAL(49, 584, 4), // "Y_to"
QT_MOC_LITERAL(50, 589, 9), // "moveToRef"
QT_MOC_LITERAL(51, 599, 6), // "refpos"
QT_MOC_LITERAL(52, 606, 6), // "serial"
QT_MOC_LITERAL(53, 613, 11), // "Init_Xmotor"
QT_MOC_LITERAL(54, 625, 11), // "Init_Ymotor"
QT_MOC_LITERAL(55, 637, 6), // "StartX"
QT_MOC_LITERAL(56, 644, 6), // "StartY"
QT_MOC_LITERAL(57, 651, 11), // "StartXYScan"
QT_MOC_LITERAL(58, 663, 6), // "ScanXY"
QT_MOC_LITERAL(59, 670, 15), // "MoveDoubleClick"
QT_MOC_LITERAL(60, 686, 5), // "Abort"
QT_MOC_LITERAL(61, 692, 12), // "stop_motorXY"
QT_MOC_LITERAL(62, 705, 14), // "CheckXOnTarget"
QT_MOC_LITERAL(63, 720, 14), // "CheckYOnTarget"
QT_MOC_LITERAL(64, 735, 10), // "timerEvent"
QT_MOC_LITERAL(65, 746, 13), // "CheckSegFault"
QT_MOC_LITERAL(66, 760, 7), // "SaveTxt"
QT_MOC_LITERAL(67, 768, 14), // "SetSerialXName"
QT_MOC_LITERAL(68, 783, 6), // "number"
QT_MOC_LITERAL(69, 790, 14), // "SetSerialYName"
QT_MOC_LITERAL(70, 805, 7), // "AssignY"
QT_MOC_LITERAL(71, 813, 7), // "AssignX"
QT_MOC_LITERAL(72, 821, 5), // "NameY"
QT_MOC_LITERAL(73, 827, 7), // "numbery"
QT_MOC_LITERAL(74, 835, 5), // "NameX"
QT_MOC_LITERAL(75, 841, 7), // "numberx"
QT_MOC_LITERAL(76, 849, 9), // "XrayTable"
QT_MOC_LITERAL(77, 859, 12), // "slotMoveFine"
QT_MOC_LITERAL(78, 872, 2), // "id"
QT_MOC_LITERAL(79, 875, 13), // "slotMoveStage"
QT_MOC_LITERAL(80, 889, 9), // "moveStage"
QT_MOC_LITERAL(81, 899, 3), // "pos"
QT_MOC_LITERAL(82, 903, 8), // "moveStep"
QT_MOC_LITERAL(83, 912, 4), // "step"
QT_MOC_LITERAL(84, 917, 5), // "bool*"
QT_MOC_LITERAL(85, 923, 9), // "condition"
QT_MOC_LITERAL(86, 933, 3), // "dir"
QT_MOC_LITERAL(87, 937, 14), // "LoadSHM_SumMap"
QT_MOC_LITERAL(88, 952, 7), // "Zminimo"
QT_MOC_LITERAL(89, 960, 8), // "Zmassimo"
QT_MOC_LITERAL(90, 969, 4), // "Z_to"
QT_MOC_LITERAL(91, 974, 11), // "Init_Zmotor"
QT_MOC_LITERAL(92, 986, 6), // "StartZ"
QT_MOC_LITERAL(93, 993, 17), // "Init_KeyenceLaser"
QT_MOC_LITERAL(94, 1011, 14), // "CheckZOnTarget"
QT_MOC_LITERAL(95, 1026, 10), // "Focustimer"
QT_MOC_LITERAL(96, 1037, 11), // "readKeyence"
QT_MOC_LITERAL(97, 1049, 10), // "TrackingON"
QT_MOC_LITERAL(98, 1060, 16), // "AutoFocusRunning"
QT_MOC_LITERAL(99, 1077, 14), // "SetSerialZName"
QT_MOC_LITERAL(100, 1092, 7), // "AssignZ"
QT_MOC_LITERAL(101, 1100, 5), // "NameZ"
QT_MOC_LITERAL(102, 1106, 7), // "numberz"
QT_MOC_LITERAL(103, 1114, 9), // "AssignACM"
QT_MOC_LITERAL(104, 1124, 7), // "NameACM"
QT_MOC_LITERAL(105, 1132, 9) // "numberacm"

    },
    "MainWindow\0Info1_1\0\0Info1_2\0Info2_1\0"
    "open_MAP\0Define_Pixels\0GoOnLine\0"
    "displayImage_SHM\0displaySumImage_SHM\0"
    "hideImage\0LoadNewFile_SHM\0LoadElementsMapSum\0"
    "writeCompMapLimits\0SelectChannels\0"
    "StartVme\0Stop_Vme\0set_PMAcquisitionTime\0"
    "SelDigiCh0\0SelDigiCh1\0SelDigiCh0and1\0"
    "TreD\0DueD\0Pixels\0ShowHistogram\0RateMeter\0"
    "LoadTxt\0MergeTxt\0readmultidetcalpar\0"
    "Changeparameters\0Autofocus2\0openDPPInter\0"
    "Detector\0Helium_interface\0VLC_interface\0"
    "caenoscilloscope\0USB_DAQ\0OPTICAL_DAQ\0"
    "Enabling_Tabwidget\0Enable_TabWidget_3_4_XY\0"
    "Image_Export\0Velocity\0PassoX_Func\0"
    "Xminimo\0Xmassimo\0PassoY_Func\0Yminimo\0"
    "Ymassimo\0X_to\0Y_to\0moveToRef\0refpos\0"
    "serial\0Init_Xmotor\0Init_Ymotor\0StartX\0"
    "StartY\0StartXYScan\0ScanXY\0MoveDoubleClick\0"
    "Abort\0stop_motorXY\0CheckXOnTarget\0"
    "CheckYOnTarget\0timerEvent\0CheckSegFault\0"
    "SaveTxt\0SetSerialXName\0number\0"
    "SetSerialYName\0AssignY\0AssignX\0NameY\0"
    "numbery\0NameX\0numberx\0XrayTable\0"
    "slotMoveFine\0id\0slotMoveStage\0moveStage\0"
    "pos\0moveStep\0step\0bool*\0condition\0dir\0"
    "LoadSHM_SumMap\0Zminimo\0Zmassimo\0Z_to\0"
    "Init_Zmotor\0StartZ\0Init_KeyenceLaser\0"
    "CheckZOnTarget\0Focustimer\0readKeyence\0"
    "TrackingON\0AutoFocusRunning\0SetSerialZName\0"
    "AssignZ\0NameZ\0numberz\0AssignACM\0NameACM\0"
    "numberacm"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      91,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  469,    2, 0x08 /* Private */,
       3,    0,  470,    2, 0x08 /* Private */,
       4,    0,  471,    2, 0x08 /* Private */,
       5,    0,  472,    2, 0x08 /* Private */,
       6,    0,  473,    2, 0x08 /* Private */,
       7,    0,  474,    2, 0x08 /* Private */,
       8,    0,  475,    2, 0x08 /* Private */,
       9,    0,  476,    2, 0x08 /* Private */,
      10,    0,  477,    2, 0x08 /* Private */,
      11,    0,  478,    2, 0x08 /* Private */,
      12,    0,  479,    2, 0x08 /* Private */,
      13,    1,  480,    2, 0x08 /* Private */,
      14,    0,  483,    2, 0x08 /* Private */,
      15,    0,  484,    2, 0x08 /* Private */,
      16,    0,  485,    2, 0x08 /* Private */,
      17,    0,  486,    2, 0x08 /* Private */,
      18,    0,  487,    2, 0x08 /* Private */,
      19,    0,  488,    2, 0x08 /* Private */,
      20,    0,  489,    2, 0x08 /* Private */,
      21,    0,  490,    2, 0x08 /* Private */,
      22,    0,  491,    2, 0x08 /* Private */,
      23,    0,  492,    2, 0x08 /* Private */,
      24,    0,  493,    2, 0x08 /* Private */,
      25,    0,  494,    2, 0x08 /* Private */,
      26,    0,  495,    2, 0x08 /* Private */,
      27,    0,  496,    2, 0x08 /* Private */,
      28,    0,  497,    2, 0x08 /* Private */,
      29,    0,  498,    2, 0x08 /* Private */,
      30,    0,  499,    2, 0x08 /* Private */,
      31,    0,  500,    2, 0x08 /* Private */,
      32,    0,  501,    2, 0x08 /* Private */,
      33,    0,  502,    2, 0x08 /* Private */,
      34,    0,  503,    2, 0x08 /* Private */,
      35,    0,  504,    2, 0x08 /* Private */,
      36,    0,  505,    2, 0x08 /* Private */,
      37,    0,  506,    2, 0x08 /* Private */,
      38,    0,  507,    2, 0x08 /* Private */,
      39,    0,  508,    2, 0x08 /* Private */,
      40,    0,  509,    2, 0x08 /* Private */,
      41,    1,  510,    2, 0x08 /* Private */,
      42,    1,  513,    2, 0x08 /* Private */,
      43,    1,  516,    2, 0x08 /* Private */,
      44,    1,  519,    2, 0x08 /* Private */,
      45,    1,  522,    2, 0x08 /* Private */,
      46,    1,  525,    2, 0x08 /* Private */,
      47,    1,  528,    2, 0x08 /* Private */,
      48,    1,  531,    2, 0x08 /* Private */,
      49,    1,  534,    2, 0x08 /* Private */,
      50,    2,  537,    2, 0x08 /* Private */,
      53,    0,  542,    2, 0x08 /* Private */,
      54,    0,  543,    2, 0x08 /* Private */,
      55,    0,  544,    2, 0x08 /* Private */,
      56,    0,  545,    2, 0x08 /* Private */,
      57,    0,  546,    2, 0x08 /* Private */,
      58,    0,  547,    2, 0x08 /* Private */,
      59,    0,  548,    2, 0x08 /* Private */,
      60,    0,  549,    2, 0x08 /* Private */,
      61,    0,  550,    2, 0x08 /* Private */,
      62,    0,  551,    2, 0x08 /* Private */,
      63,    0,  552,    2, 0x08 /* Private */,
      64,    0,  553,    2, 0x08 /* Private */,
      65,    0,  554,    2, 0x08 /* Private */,
      66,    0,  555,    2, 0x08 /* Private */,
      67,    1,  556,    2, 0x08 /* Private */,
      69,    1,  559,    2, 0x08 /* Private */,
      70,    0,  562,    2, 0x08 /* Private */,
      71,    0,  563,    2, 0x08 /* Private */,
      72,    1,  564,    2, 0x08 /* Private */,
      74,    1,  567,    2, 0x08 /* Private */,
      76,    0,  570,    2, 0x08 /* Private */,
      77,    1,  571,    2, 0x08 /* Private */,
      79,    1,  574,    2, 0x08 /* Private */,
      80,    2,  577,    2, 0x08 /* Private */,
      82,    4,  582,    2, 0x08 /* Private */,
      87,    0,  591,    2, 0x0a /* Public */,
      88,    1,  592,    2, 0x0a /* Public */,
      89,    1,  595,    2, 0x0a /* Public */,
      90,    1,  598,    2, 0x0a /* Public */,
      91,    0,  601,    2, 0x0a /* Public */,
      92,    0,  602,    2, 0x0a /* Public */,
      93,    0,  603,    2, 0x0a /* Public */,
      94,    0,  604,    2, 0x0a /* Public */,
      95,    0,  605,    2, 0x0a /* Public */,
      96,    0,  606,    2, 0x0a /* Public */,
      97,    0,  607,    2, 0x0a /* Public */,
      98,    0,  608,    2, 0x0a /* Public */,
      99,    1,  609,    2, 0x0a /* Public */,
     100,    0,  612,    2, 0x0a /* Public */,
     101,    1,  613,    2, 0x0a /* Public */,
     103,    0,  616,    2, 0x0a /* Public */,
     104,    1,  617,    2, 0x0a /* Public */,

 // slots: parameters
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
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Float, QMetaType::Int,   51,   52,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   68,
    QMetaType::QString, QMetaType::Int,   68,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   73,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   78,
    QMetaType::Void, QMetaType::Int,   78,
    QMetaType::Void, QMetaType::Double, QMetaType::Int,   81,   52,
    QMetaType::Double, QMetaType::Double, QMetaType::Int, 0x80000000 | 84, QMetaType::Bool,   83,   52,   85,   86,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   68,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  102,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  105,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Info1_1(); break;
        case 1: _t->Info1_2(); break;
        case 2: _t->Info2_1(); break;
        case 3: _t->open_MAP(); break;
        case 4: _t->Define_Pixels(); break;
        case 5: _t->GoOnLine(); break;
        case 6: _t->displayImage_SHM(); break;
        case 7: _t->displaySumImage_SHM(); break;
        case 8: _t->hideImage(); break;
        case 9: _t->LoadNewFile_SHM(); break;
        case 10: _t->LoadElementsMapSum(); break;
        case 11: _t->writeCompMapLimits((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->SelectChannels(); break;
        case 13: _t->StartVme(); break;
        case 14: _t->Stop_Vme(); break;
        case 15: _t->set_PMAcquisitionTime(); break;
        case 16: _t->SelDigiCh0(); break;
        case 17: _t->SelDigiCh1(); break;
        case 18: _t->SelDigiCh0and1(); break;
        case 19: _t->TreD(); break;
        case 20: _t->DueD(); break;
        case 21: _t->Pixels(); break;
        case 22: _t->ShowHistogram(); break;
        case 23: _t->RateMeter(); break;
        case 24: _t->LoadTxt(); break;
        case 25: _t->MergeTxt(); break;
        case 26: _t->readmultidetcalpar(); break;
        case 27: _t->Changeparameters(); break;
        case 28: _t->Autofocus2(); break;
        case 29: _t->openDPPInter(); break;
        case 30: _t->Detector(); break;
        case 31: _t->Helium_interface(); break;
        case 32: _t->VLC_interface(); break;
        case 33: _t->caenoscilloscope(); break;
        case 34: _t->USB_DAQ(); break;
        case 35: _t->OPTICAL_DAQ(); break;
        case 36: _t->Enabling_Tabwidget(); break;
        case 37: _t->Enable_TabWidget_3_4_XY(); break;
        case 38: _t->Image_Export(); break;
        case 39: _t->Velocity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 40: _t->PassoX_Func((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 41: _t->Xminimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 42: _t->Xmassimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 43: _t->PassoY_Func((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 44: _t->Yminimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 45: _t->Ymassimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 46: _t->X_to((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 47: _t->Y_to((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 48: _t->moveToRef((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 49: _t->Init_Xmotor(); break;
        case 50: _t->Init_Ymotor(); break;
        case 51: _t->StartX(); break;
        case 52: _t->StartY(); break;
        case 53: { bool _r = _t->StartXYScan();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 54: _t->ScanXY(); break;
        case 55: _t->MoveDoubleClick(); break;
        case 56: _t->Abort(); break;
        case 57: _t->stop_motorXY(); break;
        case 58: _t->CheckXOnTarget(); break;
        case 59: _t->CheckYOnTarget(); break;
        case 60: _t->timerEvent(); break;
        case 61: _t->CheckSegFault(); break;
        case 62: _t->SaveTxt(); break;
        case 63: { QString _r = _t->SetSerialXName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 64: { QString _r = _t->SetSerialYName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 65: _t->AssignY(); break;
        case 66: _t->AssignX(); break;
        case 67: _t->NameY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 68: _t->NameX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 69: _t->XrayTable(); break;
        case 70: _t->slotMoveFine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 71: _t->slotMoveStage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 72: _t->moveStage((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 73: { double _r = _t->moveStep((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool*(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        case 74: _t->LoadSHM_SumMap(); break;
        case 75: _t->Zminimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 76: _t->Zmassimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 77: _t->Z_to((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 78: _t->Init_Zmotor(); break;
        case 79: _t->StartZ(); break;
        case 80: _t->Init_KeyenceLaser(); break;
        case 81: _t->CheckZOnTarget(); break;
        case 82: _t->Focustimer(); break;
        case 83: _t->readKeyence(); break;
        case 84: _t->TrackingON(); break;
        case 85: _t->AutoFocusRunning(); break;
        case 86: { QString _r = _t->SetSerialZName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 87: _t->AssignZ(); break;
        case 88: _t->NameZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 89: _t->AssignACM(); break;
        case 90: _t->NameACM((*reinterpret_cast< int(*)>(_a[1]))); break;
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
        if (_id < 91)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 91;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 91)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 91;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
