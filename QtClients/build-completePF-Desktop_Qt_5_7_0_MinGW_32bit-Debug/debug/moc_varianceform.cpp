/****************************************************************************
** Meta object code from reading C++ file 'varianceform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../completePF/varianceform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'varianceform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_varianceForm_t {
    QByteArrayData data[10];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_varianceForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_varianceForm_t qt_meta_stringdata_varianceForm = {
    {
QT_MOC_LITERAL(0, 0, 12), // "varianceForm"
QT_MOC_LITERAL(1, 13, 18), // "updateVarianceData"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 6), // "source"
QT_MOC_LITERAL(4, 40, 14), // "alignmentData0"
QT_MOC_LITERAL(5, 55, 14), // "alignmentData1"
QT_MOC_LITERAL(6, 70, 14), // "alignmentData2"
QT_MOC_LITERAL(7, 85, 14), // "alignmentData3"
QT_MOC_LITERAL(8, 100, 14), // "alignmentData4"
QT_MOC_LITERAL(9, 115, 34) // "on_calculateVarianceButton_cl..."

    },
    "varianceForm\0updateVarianceData\0\0"
    "source\0alignmentData0\0alignmentData1\0"
    "alignmentData2\0alignmentData3\0"
    "alignmentData4\0on_calculateVarianceButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_varianceForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    6,   24,    2, 0x0a /* Public */,
       9,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Char, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,    7,    8,
    QMetaType::Void,

       0        // eod
};

void varianceForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        varianceForm *_t = static_cast<varianceForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateVarianceData((*reinterpret_cast< char(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6]))); break;
        case 1: _t->on_calculateVarianceButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject varianceForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_varianceForm.data,
      qt_meta_data_varianceForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *varianceForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *varianceForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_varianceForm.stringdata0))
        return static_cast<void*>(const_cast< varianceForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int varianceForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
