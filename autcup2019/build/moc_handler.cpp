/****************************************************************************
** Meta object code from reading C++ file 'handler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../handler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Handler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,   39,   39,   39, 0x05,
      40,   39,   39,   39, 0x05,
      62,   93,   97,   39, 0x05,
     101,  119,   39,   39, 0x05,

 // slots: signature, parameters, type, tag, flags
     128,  147,   39,   39, 0x0a,
     153,   39,   39,   39, 0x0a,
     176,   39,   39,   39, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Handler[] = {
    "Handler\0_SIGNAL_detectYellowRedFrame()\0"
    "\0_SIGNAL_reduceNoise()\0"
    "_SIGNAL_Hist_And_Backproj(Mat)\0src\0"
    "Mat\0gateCallBack(int)\0movement\0"
    "imageCallback(Mat)\0frame\0"
    "detectYellowRedFrame()\0reduceNoise()\0"
};

void Handler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Handler *_t = static_cast<Handler *>(_o);
        switch (_id) {
        case 0: _t->_SIGNAL_detectYellowRedFrame(); break;
        case 1: _t->_SIGNAL_reduceNoise(); break;
        case 2: { Mat _r = _t->_SIGNAL_Hist_And_Backproj((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 3: _t->gateCallBack((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->imageCallback((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 5: _t->detectYellowRedFrame(); break;
        case 6: _t->reduceNoise(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Handler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Handler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Handler,
      qt_meta_data_Handler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Handler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Handler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Handler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Handler))
        return static_cast<void*>(const_cast< Handler*>(this));
    return QObject::qt_metacast(_clname);
}

int Handler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Handler::_SIGNAL_detectYellowRedFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Handler::_SIGNAL_reduceNoise()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
Mat Handler::_SIGNAL_Hist_And_Backproj(Mat _t1)
{
    Mat _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
    return _t0;
}

// SIGNAL 3
void Handler::gateCallBack(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
