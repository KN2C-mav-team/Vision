/****************************************************************************
** Meta object code from reading C++ file 'quad_board.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "quad_board.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'quad_board.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Quad_Board[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   22,   22,   22, 0x0a,
      23,   52,   22,   22, 0x0a,
      63,   82,   22,   22, 0x0a,
      91,  116,   22,   22, 0x0a,
     121,  141,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Quad_Board[] = {
    "Quad_Board\0readData()\0\0"
    "lineSerial_CB(double,double)\0dist,angle\0"
    "gateSerial_CB(int)\0movement\0"
    "landingPadSerial_CB(int)\0land\0"
    "optFlow_CB(int,int)\0delta_x,delta_y\0"
};

void Quad_Board::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Quad_Board *_t = static_cast<Quad_Board *>(_o);
        switch (_id) {
        case 0: _t->readData(); break;
        case 1: _t->lineSerial_CB((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->gateSerial_CB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->landingPadSerial_CB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->optFlow_CB((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Quad_Board::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Quad_Board::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Quad_Board,
      qt_meta_data_Quad_Board, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Quad_Board::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Quad_Board::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Quad_Board::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Quad_Board))
        return static_cast<void*>(const_cast< Quad_Board*>(this));
    return QObject::qt_metacast(_clname);
}

int Quad_Board::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
