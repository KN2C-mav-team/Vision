/****************************************************************************
** Meta object code from reading C++ file 'hdetection.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdetection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hdetection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HDetection[] = {

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
      11,   35,   40,   40, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   67,   40,   40, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_HDetection[] = {
    "HDetection\0landingPadCallBack(int)\0"
    "land\0\0hLandingCallBack(cv::Mat)\0img\0"
};

void HDetection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HDetection *_t = static_cast<HDetection *>(_o);
        switch (_id) {
        case 0: _t->landingPadCallBack((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->hLandingCallBack((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HDetection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HDetection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HDetection,
      qt_meta_data_HDetection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HDetection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HDetection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HDetection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HDetection))
        return static_cast<void*>(const_cast< HDetection*>(this));
    return QObject::qt_metacast(_clname);
}

int HDetection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HDetection::landingPadCallBack(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
