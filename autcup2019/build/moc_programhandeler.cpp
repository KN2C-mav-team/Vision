/****************************************************************************
** Meta object code from reading C++ file 'programhandeler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../programhandeler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'programhandeler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProgramHandeler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   44,   46,   46, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   66,   46,   46, 0x0a,
      76,  118,   46,   46, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ProgramHandeler[] = {
    "ProgramHandeler\0connectorPkg(double,double)\0"
    ",\0\0imageCallBack(Mat)\0raw_image\0"
    "Callback(string,string,vector<Point>,int)\0"
    ",,,\0"
};

void ProgramHandeler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ProgramHandeler *_t = static_cast<ProgramHandeler *>(_o);
        switch (_id) {
        case 0: _t->connectorPkg((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->imageCallBack((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 2: _t->Callback((*reinterpret_cast< const string(*)>(_a[1])),(*reinterpret_cast< const string(*)>(_a[2])),(*reinterpret_cast< const vector<Point>(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ProgramHandeler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ProgramHandeler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProgramHandeler,
      qt_meta_data_ProgramHandeler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProgramHandeler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProgramHandeler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProgramHandeler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProgramHandeler))
        return static_cast<void*>(const_cast< ProgramHandeler*>(this));
    return QObject::qt_metacast(_clname);
}

int ProgramHandeler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ProgramHandeler::connectorPkg(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
