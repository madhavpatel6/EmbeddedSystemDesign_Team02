/****************************************************************************
** Meta object code from reading C++ file 'req_respform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../completePF/req_respform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'req_respform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_req_respForm_t {
    QByteArrayData data[6];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_req_respForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_req_respForm_t qt_meta_stringdata_req_respForm = {
    {
QT_MOC_LITERAL(0, 0, 12), // "req_respForm"
QT_MOC_LITERAL(1, 13, 16), // "updateCurReqSlot"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 6), // "curReq"
QT_MOC_LITERAL(4, 38, 17), // "updateCurRespSlot"
QT_MOC_LITERAL(5, 56, 7) // "curResp"

    },
    "req_respForm\0updateCurReqSlot\0\0curReq\0"
    "updateCurRespSlot\0curResp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_req_respForm[] = {

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
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void req_respForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        req_respForm *_t = static_cast<req_respForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateCurReqSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateCurRespSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject req_respForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_req_respForm.data,
      qt_meta_data_req_respForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *req_respForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *req_respForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_req_respForm.stringdata0))
        return static_cast<void*>(const_cast< req_respForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int req_respForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
