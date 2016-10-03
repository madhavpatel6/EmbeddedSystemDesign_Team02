/****************************************************************************
** Meta object code from reading C++ file 'clientsocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BasicGuiClientTCP/clientsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ClientSocket_t {
    QByteArrayData data[21];
    char stringdata0[270];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClientSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClientSocket_t qt_meta_stringdata_ClientSocket = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ClientSocket"
QT_MOC_LITERAL(1, 13, 11), // "updateError"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "val"
QT_MOC_LITERAL(4, 30, 25), // "sendCommStatTargetLocator"
QT_MOC_LITERAL(5, 56, 21), // "commStatTargetLocator"
QT_MOC_LITERAL(6, 78, 23), // "serverIsConnectedSignal"
QT_MOC_LITERAL(7, 102, 27), // "connectedToServerBoolSignal"
QT_MOC_LITERAL(8, 130, 18), // "sentCommStatSignal"
QT_MOC_LITERAL(9, 149, 12), // "sendCommStat"
QT_MOC_LITERAL(10, 162, 6), // "source"
QT_MOC_LITERAL(11, 169, 7), // "goodMsg"
QT_MOC_LITERAL(12, 177, 9), // "commError"
QT_MOC_LITERAL(13, 187, 7), // "reqRecv"
QT_MOC_LITERAL(14, 195, 7), // "resRecv"
QT_MOC_LITERAL(15, 203, 7), // "reqSent"
QT_MOC_LITERAL(16, 211, 7), // "resSent"
QT_MOC_LITERAL(17, 219, 9), // "connected"
QT_MOC_LITERAL(18, 229, 12), // "disconnected"
QT_MOC_LITERAL(19, 242, 9), // "readyRead"
QT_MOC_LITERAL(20, 252, 17) // "commStatRequested"

    },
    "ClientSocket\0updateError\0\0val\0"
    "sendCommStatTargetLocator\0"
    "commStatTargetLocator\0serverIsConnectedSignal\0"
    "connectedToServerBoolSignal\0"
    "sentCommStatSignal\0sendCommStat\0source\0"
    "goodMsg\0commError\0reqRecv\0resRecv\0"
    "reqSent\0resSent\0connected\0disconnected\0"
    "readyRead\0commStatRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientSocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       6,    1,   65,    2, 0x06 /* Public */,
       8,    0,   68,    2, 0x06 /* Public */,
       9,    7,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,   84,    2, 0x0a /* Public */,
      18,    0,   85,    2, 0x0a /* Public */,
      19,    0,   86,    2, 0x0a /* Public */,
      20,    0,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Char, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,   12,   13,   14,   15,   16,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ClientSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClientSocket *_t = static_cast<ClientSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sendCommStatTargetLocator((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->serverIsConnectedSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->sentCommStatSignal(); break;
        case 4: _t->sendCommStat((*reinterpret_cast< char(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 5: _t->connected(); break;
        case 6: _t->disconnected(); break;
        case 7: _t->readyRead(); break;
        case 8: _t->commStatRequested(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ClientSocket::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientSocket::updateError)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ClientSocket::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientSocket::sendCommStatTargetLocator)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ClientSocket::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientSocket::serverIsConnectedSignal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ClientSocket::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientSocket::sentCommStatSignal)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ClientSocket::*_t)(char , QString , QString , QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientSocket::sendCommStat)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject ClientSocket::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ClientSocket.data,
      qt_meta_data_ClientSocket,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ClientSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ClientSocket.stringdata0))
        return static_cast<void*>(const_cast< ClientSocket*>(this));
    return QObject::qt_metacast(_clname);
}

int ClientSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ClientSocket::updateError(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientSocket::sendCommStatTargetLocator(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ClientSocket::serverIsConnectedSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ClientSocket::sentCommStatSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void ClientSocket::sendCommStat(char _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6, QString _t7)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
