/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
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
    QByteArrayData data[118];
    char stringdata0[1312];
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
QT_MOC_LITERAL(33, 388, 15), // "Laser_interface"
QT_MOC_LITERAL(34, 404, 16), // "Helium_interface"
QT_MOC_LITERAL(35, 421, 13), // "VLC_interface"
QT_MOC_LITERAL(36, 435, 16), // "caenoscilloscope"
QT_MOC_LITERAL(37, 452, 7), // "USB_DAQ"
QT_MOC_LITERAL(38, 460, 11), // "OPTICAL_DAQ"
QT_MOC_LITERAL(39, 472, 18), // "Enabling_Tabwidget"
QT_MOC_LITERAL(40, 491, 23), // "Enable_TabWidget_3_4_XY"
QT_MOC_LITERAL(41, 515, 12), // "Image_Export"
QT_MOC_LITERAL(42, 528, 8), // "Velocity"
QT_MOC_LITERAL(43, 537, 11), // "PassoX_Func"
QT_MOC_LITERAL(44, 549, 7), // "Xminimo"
QT_MOC_LITERAL(45, 557, 8), // "Xmassimo"
QT_MOC_LITERAL(46, 566, 11), // "PassoY_Func"
QT_MOC_LITERAL(47, 578, 7), // "Yminimo"
QT_MOC_LITERAL(48, 586, 8), // "Ymassimo"
QT_MOC_LITERAL(49, 595, 4), // "X_to"
QT_MOC_LITERAL(50, 600, 4), // "Y_to"
QT_MOC_LITERAL(51, 605, 16), // "movetoref_Xmotor"
QT_MOC_LITERAL(52, 622, 12), // "refpositionX"
QT_MOC_LITERAL(53, 635, 11), // "Init_Xmotor"
QT_MOC_LITERAL(54, 647, 16), // "movetoref_Ymotor"
QT_MOC_LITERAL(55, 664, 12), // "refpositionY"
QT_MOC_LITERAL(56, 677, 11), // "Init_Ymotor"
QT_MOC_LITERAL(57, 689, 16), // "movetoref_Zmotor"
QT_MOC_LITERAL(58, 706, 12), // "refpositionZ"
QT_MOC_LITERAL(59, 719, 6), // "StartX"
QT_MOC_LITERAL(60, 726, 6), // "StartY"
QT_MOC_LITERAL(61, 733, 11), // "StartXYScan"
QT_MOC_LITERAL(62, 745, 6), // "ScanXY"
QT_MOC_LITERAL(63, 752, 5), // "MoveX"
QT_MOC_LITERAL(64, 758, 5), // "MoveY"
QT_MOC_LITERAL(65, 764, 7), // "Move_up"
QT_MOC_LITERAL(66, 772, 8), // "MoveX_To"
QT_MOC_LITERAL(67, 781, 8), // "MoveY_To"
QT_MOC_LITERAL(68, 790, 9), // "Move_down"
QT_MOC_LITERAL(69, 800, 10), // "Move_right"
QT_MOC_LITERAL(70, 811, 9), // "Move_left"
QT_MOC_LITERAL(71, 821, 15), // "MoveDoubleClick"
QT_MOC_LITERAL(72, 837, 5), // "Abort"
QT_MOC_LITERAL(73, 843, 12), // "stop_motorXY"
QT_MOC_LITERAL(74, 856, 14), // "CheckXOnTarget"
QT_MOC_LITERAL(75, 871, 14), // "CheckYOnTarget"
QT_MOC_LITERAL(76, 886, 10), // "timerEvent"
QT_MOC_LITERAL(77, 897, 13), // "CheckSegFault"
QT_MOC_LITERAL(78, 911, 7), // "SaveTxt"
QT_MOC_LITERAL(79, 919, 14), // "SetSerialXName"
QT_MOC_LITERAL(80, 934, 6), // "number"
QT_MOC_LITERAL(81, 941, 14), // "SetSerialYName"
QT_MOC_LITERAL(82, 956, 7), // "AssignY"
QT_MOC_LITERAL(83, 964, 7), // "AssignX"
QT_MOC_LITERAL(84, 972, 5), // "NameY"
QT_MOC_LITERAL(85, 978, 7), // "numbery"
QT_MOC_LITERAL(86, 986, 5), // "NameX"
QT_MOC_LITERAL(87, 992, 7), // "numberx"
QT_MOC_LITERAL(88, 1000, 9), // "XrayTable"
QT_MOC_LITERAL(89, 1010, 14), // "LoadSHM_SumMap"
QT_MOC_LITERAL(90, 1025, 8), // "Treshold"
QT_MOC_LITERAL(91, 1034, 7), // "Zminimo"
QT_MOC_LITERAL(92, 1042, 8), // "Zmassimo"
QT_MOC_LITERAL(93, 1051, 4), // "Z_to"
QT_MOC_LITERAL(94, 1056, 11), // "Init_Zmotor"
QT_MOC_LITERAL(95, 1068, 6), // "StartZ"
QT_MOC_LITERAL(96, 1075, 17), // "Init_KeyenceLaser"
QT_MOC_LITERAL(97, 1093, 5), // "MoveZ"
QT_MOC_LITERAL(98, 1099, 8), // "MoveZ_To"
QT_MOC_LITERAL(99, 1108, 12), // "Move_forward"
QT_MOC_LITERAL(100, 1121, 13), // "Move_backward"
QT_MOC_LITERAL(101, 1135, 6), // "AbortZ"
QT_MOC_LITERAL(102, 1142, 5), // "StopZ"
QT_MOC_LITERAL(103, 1148, 14), // "CheckZOnTarget"
QT_MOC_LITERAL(104, 1163, 10), // "Focustimer"
QT_MOC_LITERAL(105, 1174, 11), // "readKeyence"
QT_MOC_LITERAL(106, 1186, 10), // "TrackingON"
QT_MOC_LITERAL(107, 1197, 16), // "AutoFocusRunning"
QT_MOC_LITERAL(108, 1214, 15), // "Laser_switching"
QT_MOC_LITERAL(109, 1230, 7), // "LaserOn"
QT_MOC_LITERAL(110, 1238, 8), // "LaserOff"
QT_MOC_LITERAL(111, 1247, 14), // "SetSerialZName"
QT_MOC_LITERAL(112, 1262, 7), // "AssignZ"
QT_MOC_LITERAL(113, 1270, 5), // "NameZ"
QT_MOC_LITERAL(114, 1276, 7), // "numberz"
QT_MOC_LITERAL(115, 1284, 9), // "AssignACM"
QT_MOC_LITERAL(116, 1294, 7), // "NameACM"
QT_MOC_LITERAL(117, 1302, 9) // "numberacm"

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
    "Detector\0Laser_interface\0Helium_interface\0"
    "VLC_interface\0caenoscilloscope\0USB_DAQ\0"
    "OPTICAL_DAQ\0Enabling_Tabwidget\0"
    "Enable_TabWidget_3_4_XY\0Image_Export\0"
    "Velocity\0PassoX_Func\0Xminimo\0Xmassimo\0"
    "PassoY_Func\0Yminimo\0Ymassimo\0X_to\0"
    "Y_to\0movetoref_Xmotor\0refpositionX\0"
    "Init_Xmotor\0movetoref_Ymotor\0refpositionY\0"
    "Init_Ymotor\0movetoref_Zmotor\0refpositionZ\0"
    "StartX\0StartY\0StartXYScan\0ScanXY\0MoveX\0"
    "MoveY\0Move_up\0MoveX_To\0MoveY_To\0"
    "Move_down\0Move_right\0Move_left\0"
    "MoveDoubleClick\0Abort\0stop_motorXY\0"
    "CheckXOnTarget\0CheckYOnTarget\0timerEvent\0"
    "CheckSegFault\0SaveTxt\0SetSerialXName\0"
    "number\0SetSerialYName\0AssignY\0AssignX\0"
    "NameY\0numbery\0NameX\0numberx\0XrayTable\0"
    "LoadSHM_SumMap\0Treshold\0Zminimo\0"
    "Zmassimo\0Z_to\0Init_Zmotor\0StartZ\0"
    "Init_KeyenceLaser\0MoveZ\0MoveZ_To\0"
    "Move_forward\0Move_backward\0AbortZ\0"
    "StopZ\0CheckZOnTarget\0Focustimer\0"
    "readKeyence\0TrackingON\0AutoFocusRunning\0"
    "Laser_switching\0LaserOn\0LaserOff\0"
    "SetSerialZName\0AssignZ\0NameZ\0numberz\0"
    "AssignACM\0NameACM\0numberacm"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
     108,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  554,    2, 0x08 /* Private */,
       3,    0,  555,    2, 0x08 /* Private */,
       4,    0,  556,    2, 0x08 /* Private */,
       5,    0,  557,    2, 0x08 /* Private */,
       6,    0,  558,    2, 0x08 /* Private */,
       7,    0,  559,    2, 0x08 /* Private */,
       8,    0,  560,    2, 0x08 /* Private */,
       9,    0,  561,    2, 0x08 /* Private */,
      10,    0,  562,    2, 0x08 /* Private */,
      11,    0,  563,    2, 0x08 /* Private */,
      12,    0,  564,    2, 0x08 /* Private */,
      13,    1,  565,    2, 0x08 /* Private */,
      14,    0,  568,    2, 0x08 /* Private */,
      15,    0,  569,    2, 0x08 /* Private */,
      16,    0,  570,    2, 0x08 /* Private */,
      17,    0,  571,    2, 0x08 /* Private */,
      18,    0,  572,    2, 0x08 /* Private */,
      19,    0,  573,    2, 0x08 /* Private */,
      20,    0,  574,    2, 0x08 /* Private */,
      21,    0,  575,    2, 0x08 /* Private */,
      22,    0,  576,    2, 0x08 /* Private */,
      23,    0,  577,    2, 0x08 /* Private */,
      24,    0,  578,    2, 0x08 /* Private */,
      25,    0,  579,    2, 0x08 /* Private */,
      26,    0,  580,    2, 0x08 /* Private */,
      27,    0,  581,    2, 0x08 /* Private */,
      28,    0,  582,    2, 0x08 /* Private */,
      29,    0,  583,    2, 0x08 /* Private */,
      30,    0,  584,    2, 0x08 /* Private */,
      31,    0,  585,    2, 0x08 /* Private */,
      32,    0,  586,    2, 0x08 /* Private */,
      33,    0,  587,    2, 0x08 /* Private */,
      34,    0,  588,    2, 0x08 /* Private */,
      35,    0,  589,    2, 0x08 /* Private */,
      36,    0,  590,    2, 0x08 /* Private */,
      37,    0,  591,    2, 0x08 /* Private */,
      38,    0,  592,    2, 0x08 /* Private */,
      39,    0,  593,    2, 0x08 /* Private */,
      40,    0,  594,    2, 0x08 /* Private */,
      41,    0,  595,    2, 0x08 /* Private */,
      42,    1,  596,    2, 0x08 /* Private */,
      43,    1,  599,    2, 0x08 /* Private */,
      44,    1,  602,    2, 0x08 /* Private */,
      45,    1,  605,    2, 0x08 /* Private */,
      46,    1,  608,    2, 0x08 /* Private */,
      47,    1,  611,    2, 0x08 /* Private */,
      48,    1,  614,    2, 0x08 /* Private */,
      49,    1,  617,    2, 0x08 /* Private */,
      50,    1,  620,    2, 0x08 /* Private */,
      51,    1,  623,    2, 0x08 /* Private */,
      53,    0,  626,    2, 0x08 /* Private */,
      54,    1,  627,    2, 0x08 /* Private */,
      56,    0,  630,    2, 0x08 /* Private */,
      57,    1,  631,    2, 0x08 /* Private */,
      59,    0,  634,    2, 0x08 /* Private */,
      60,    0,  635,    2, 0x08 /* Private */,
      61,    0,  636,    2, 0x08 /* Private */,
      62,    0,  637,    2, 0x08 /* Private */,
      63,    1,  638,    2, 0x08 /* Private */,
      64,    1,  641,    2, 0x08 /* Private */,
      65,    0,  644,    2, 0x08 /* Private */,
      66,    0,  645,    2, 0x08 /* Private */,
      67,    0,  646,    2, 0x08 /* Private */,
      68,    0,  647,    2, 0x08 /* Private */,
      69,    0,  648,    2, 0x08 /* Private */,
      70,    0,  649,    2, 0x08 /* Private */,
      71,    0,  650,    2, 0x08 /* Private */,
      72,    0,  651,    2, 0x08 /* Private */,
      73,    0,  652,    2, 0x08 /* Private */,
      74,    0,  653,    2, 0x08 /* Private */,
      75,    0,  654,    2, 0x08 /* Private */,
      76,    0,  655,    2, 0x08 /* Private */,
      77,    0,  656,    2, 0x08 /* Private */,
      78,    0,  657,    2, 0x08 /* Private */,
      79,    1,  658,    2, 0x08 /* Private */,
      81,    1,  661,    2, 0x08 /* Private */,
      82,    0,  664,    2, 0x08 /* Private */,
      83,    0,  665,    2, 0x08 /* Private */,
      84,    1,  666,    2, 0x08 /* Private */,
      86,    1,  669,    2, 0x08 /* Private */,
      88,    0,  672,    2, 0x08 /* Private */,
      89,    0,  673,    2, 0x0a /* Public */,
      90,    0,  674,    2, 0x0a /* Public */,
      91,    1,  675,    2, 0x0a /* Public */,
      92,    1,  678,    2, 0x0a /* Public */,
      93,    1,  681,    2, 0x0a /* Public */,
      94,    0,  684,    2, 0x0a /* Public */,
      95,    0,  685,    2, 0x0a /* Public */,
      96,    0,  686,    2, 0x0a /* Public */,
      97,    1,  687,    2, 0x0a /* Public */,
      98,    0,  690,    2, 0x0a /* Public */,
      99,    0,  691,    2, 0x0a /* Public */,
     100,    0,  692,    2, 0x0a /* Public */,
     101,    0,  693,    2, 0x0a /* Public */,
     102,    0,  694,    2, 0x0a /* Public */,
     103,    0,  695,    2, 0x0a /* Public */,
     104,    0,  696,    2, 0x0a /* Public */,
     105,    0,  697,    2, 0x0a /* Public */,
     106,    0,  698,    2, 0x0a /* Public */,
     107,    0,  699,    2, 0x0a /* Public */,
     108,    0,  700,    2, 0x0a /* Public */,
     109,    0,  701,    2, 0x0a /* Public */,
     110,    0,  702,    2, 0x0a /* Public */,
     111,    1,  703,    2, 0x0a /* Public */,
     112,    0,  706,    2, 0x0a /* Public */,
     113,    1,  707,    2, 0x0a /* Public */,
     115,    0,  710,    2, 0x0a /* Public */,
     116,    1,  711,    2, 0x0a /* Public */,

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
    QMetaType::Void, QMetaType::Float,   52,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   55,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   58,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   80,
    QMetaType::QString, QMetaType::Int,   80,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   85,
    QMetaType::Void, QMetaType::Int,   87,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
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
    QMetaType::QString, QMetaType::Int,   80,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  114,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  117,

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
        case 31: _t->Laser_interface(); break;
        case 32: _t->Helium_interface(); break;
        case 33: _t->VLC_interface(); break;
        case 34: _t->caenoscilloscope(); break;
        case 35: _t->USB_DAQ(); break;
        case 36: _t->OPTICAL_DAQ(); break;
        case 37: _t->Enabling_Tabwidget(); break;
        case 38: _t->Enable_TabWidget_3_4_XY(); break;
        case 39: _t->Image_Export(); break;
        case 40: _t->Velocity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 41: _t->PassoX_Func((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 42: _t->Xminimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 43: _t->Xmassimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 44: _t->PassoY_Func((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 45: _t->Yminimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 46: _t->Ymassimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 47: _t->X_to((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 48: _t->Y_to((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 49: _t->movetoref_Xmotor((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 50: _t->Init_Xmotor(); break;
        case 51: _t->movetoref_Ymotor((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 52: _t->Init_Ymotor(); break;
        case 53: _t->movetoref_Zmotor((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 54: _t->StartX(); break;
        case 55: _t->StartY(); break;
        case 56: { bool _r = _t->StartXYScan();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 57: _t->ScanXY(); break;
        case 58: _t->MoveX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 59: _t->MoveY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 60: _t->Move_up(); break;
        case 61: _t->MoveX_To(); break;
        case 62: _t->MoveY_To(); break;
        case 63: _t->Move_down(); break;
        case 64: _t->Move_right(); break;
        case 65: _t->Move_left(); break;
        case 66: _t->MoveDoubleClick(); break;
        case 67: _t->Abort(); break;
        case 68: _t->stop_motorXY(); break;
        case 69: _t->CheckXOnTarget(); break;
        case 70: _t->CheckYOnTarget(); break;
        case 71: _t->timerEvent(); break;
        case 72: _t->CheckSegFault(); break;
        case 73: _t->SaveTxt(); break;
        case 74: { QString _r = _t->SetSerialXName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 75: { QString _r = _t->SetSerialYName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 76: _t->AssignY(); break;
        case 77: _t->AssignX(); break;
        case 78: _t->NameY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 79: _t->NameX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 80: _t->XrayTable(); break;
        case 81: _t->LoadSHM_SumMap(); break;
        case 82: _t->Treshold(); break;
        case 83: _t->Zminimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 84: _t->Zmassimo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 85: _t->Z_to((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 86: _t->Init_Zmotor(); break;
        case 87: _t->StartZ(); break;
        case 88: _t->Init_KeyenceLaser(); break;
        case 89: _t->MoveZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 90: _t->MoveZ_To(); break;
        case 91: _t->Move_forward(); break;
        case 92: _t->Move_backward(); break;
        case 93: _t->AbortZ(); break;
        case 94: _t->StopZ(); break;
        case 95: _t->CheckZOnTarget(); break;
        case 96: _t->Focustimer(); break;
        case 97: _t->readKeyence(); break;
        case 98: _t->TrackingON(); break;
        case 99: _t->AutoFocusRunning(); break;
        case 100: _t->Laser_switching(); break;
        case 101: _t->LaserOn(); break;
        case 102: _t->LaserOff(); break;
        case 103: { QString _r = _t->SetSerialZName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 104: _t->AssignZ(); break;
        case 105: _t->NameZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 106: _t->AssignACM(); break;
        case 107: _t->NameACM((*reinterpret_cast< int(*)>(_a[1]))); break;
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
        if (_id < 108)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 108;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 108)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 108;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
