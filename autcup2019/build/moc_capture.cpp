/****************************************************************************
** Meta object code from reading C++ file 'capture.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../capture.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'capture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_capture[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,   29,   29,   29, 0x05,
      30,   29,   29,   29, 0x05,
      55,   29,   29,   29, 0x05,
      75,   29,   29,   29, 0x05,
     100,   29,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
     123,   29,   29,   29, 0x0a,
     142,   29,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_capture[] = {
    "capture\0front_image(cv::Mat)\0\0"
    "down_image_land(cv::Mat)\0down_image(cv::Mat)\0"
    "down_image_line(cv::Mat)\0"
    "down_image_qr(cv::Mat)\0frontFrame_ready()\0"
    "downFrame_ready()\0"
};

void capture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        capture *_t = static_cast<capture *>(_o);
        switch (_id) {
        case 0: _t->front_image((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 1: _t->down_image_land((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 2: _t->down_image((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 3: _t->down_image_line((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 4: _t->down_image_qr((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 5: _t->frontFrame_ready(); break;
        case 6: _t->downFrame_ready(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData capture::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject capture::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_capture,
      qt_meta_data_capture, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &capture::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *capture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *capture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_capture))
        return static_cast<void*>(const_cast< capture*>(this));
    return QObject::qt_metacast(_clname);
}

int capture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void capture::front_image(cv::Mat _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void capture::down_image_land(cv::Mat _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void capture::down_image(cv::Mat _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void capture::down_image_line(cv::Mat _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void capture::down_image_qr(cv::Mat _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
