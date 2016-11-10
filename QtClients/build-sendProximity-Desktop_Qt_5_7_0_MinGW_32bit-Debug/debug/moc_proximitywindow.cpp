/****************************************************************************
** Meta object code from reading C++ file 'proximitywindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sendProximity/proximitywindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'proximitywindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_proximityWindow_t {
    QByteArrayData data[21];
    char stringdata0[381];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_proximityWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_proximityWindow_t qt_meta_stringdata_proximityWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "proximityWindow"
QT_MOC_LITERAL(1, 16, 15), // "serverConnected"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "connected"
QT_MOC_LITERAL(4, 43, 19), // "updateAlignmentData"
QT_MOC_LITERAL(5, 63, 6), // "source"
QT_MOC_LITERAL(6, 70, 14), // "alignmentData0"
QT_MOC_LITERAL(7, 85, 14), // "alignmentData1"
QT_MOC_LITERAL(8, 100, 14), // "alignmentData2"
QT_MOC_LITERAL(9, 115, 15), // "alignmentBool_0"
QT_MOC_LITERAL(10, 131, 15), // "alignmentBool_1"
QT_MOC_LITERAL(11, 147, 15), // "alignmentBool_2"
QT_MOC_LITERAL(12, 163, 20), // "alignmentGrabberBool"
QT_MOC_LITERAL(13, 184, 13), // "alignmentInfo"
QT_MOC_LITERAL(14, 198, 32), // "on_connectToServerButton_clicked"
QT_MOC_LITERAL(15, 231, 37), // "on_disconnectFromServerButton..."
QT_MOC_LITERAL(16, 269, 29), // "on_startSensingButton_clicked"
QT_MOC_LITERAL(17, 299, 31), // "on_proximitySlider_valueChanged"
QT_MOC_LITERAL(18, 331, 5), // "value"
QT_MOC_LITERAL(19, 337, 30), // "on_proximityPushButton_clicked"
QT_MOC_LITERAL(20, 368, 12) // "sendProxSlot"

    },
    "proximityWindow\0serverConnected\0\0"
    "connected\0updateAlignmentData\0source\0"
    "alignmentData0\0alignmentData1\0"
    "alignmentData2\0alignmentBool_0\0"
    "alignmentBool_1\0alignmentBool_2\0"
    "alignmentGrabberBool\0alignmentInfo\0"
    "on_connectToServerButton_clicked\0"
    "on_disconnectFromServerButton_clicked\0"
    "on_startSensingButton_clicked\0"
    "on_proximitySlider_valueChanged\0value\0"
    "on_proximityPushButton_clicked\0"
    "sendProxSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_proximityWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    9,   57,    2, 0x0a /* Public */,
      14,    0,   76,    2, 0x08 /* Private */,
      15,    0,   77,    2, 0x08 /* Private */,
      16,    0,   78,    2, 0x08 /* Private */,
      17,    1,   79,    2, 0x08 /* Private */,
      19,    0,   82,    2, 0x08 /* Private */,
      20,    0,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Char, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,    8,    9,   10,   11,   12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void proximityWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        proximityWindow *_t = static_cast<proximityWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serverConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->updateAlignmentData((*reinterpret_cast< char(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9]))); break;
        case 2: _t->on_connectToServerButton_clicked(); break;
        case 3: _t->on_disconnectFromServerButton_clicked(); break;
        case 4: _t->on_startSensingButton_clicked(); break;
        case 5: _t->on_proximitySlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_proximityPushButton_clicked(); break;
        case 7: _t->sendProxSlot(); break;
        default: ;
        }
    }
}

const QMetaObject proximityWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_proximityWindow.data,
      qt_meta_data_proximityWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *proximityWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *proximityWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_proximityWindow.stringdata0))
        return static_cast<void*>(const_cast< proximityWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int proximityWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
