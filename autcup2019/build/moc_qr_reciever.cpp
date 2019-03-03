/****************************************************************************
** Meta object code from reading C++ file 'qr_reciever.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qr_reciever.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qr_reciever.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qr_reciever[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   64,   68,   68, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   92,   68,   68, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qr_reciever[] = {
    "qr_reciever\0"
    "QR_signal(string,string,std::vector<cv::Point>,int)\0"
    ",,,\0\0imageCallback(cv::Mat)\0raw_image\0"
};

void qr_reciever::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qr_reciever *_t = static_cast<qr_reciever *>(_o);
        switch (_id) {
        case 0: _t->QR_signal((*reinterpret_cast< const string(*)>(_a[1])),(*reinterpret_cast< const string(*)>(_a[2])),(*reinterpret_cast< const std::vector<cv::Point>(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->imageCallback((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qr_reciever::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qr_reciever::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_qr_reciever,
      qt_meta_data_qr_reciever, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qr_reciever::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qr_reciever::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qr_reciever::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qr_reciever))
        return static_cast<void*>(const_cast< qr_reciever*>(this));
    return QObject::qt_metacast(_clname);
}

int qr_reciever::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void qr_reciever::QR_signal(const string _t1, const string _t2, const std::vector<cv::Point> _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
