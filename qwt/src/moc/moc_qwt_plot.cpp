/****************************************************************************
** Meta object code from reading C++ file 'qwt_plot.h'
**
** Created: Fri Oct 4 18:11:02 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qwt_plot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_plot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       2,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,    9,    8,    8, 0x05,
      67,   53,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     116,    8,    8,    8, 0x0a,
     125,    8,    8,    8, 0x0a,
     139,   53,    8,    8, 0x08,

 // properties: name, type, flags
     195,  188, 0x42095103,
     217,  212, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_QwtPlot[] = {
    "QwtPlot\0\0plotItem,on\0"
    "itemAttached(QwtPlotItem*,bool)\0"
    "itemInfo,data\0"
    "legendDataChanged(QVariant,QList<QwtLegendData>)\0"
    "replot()\0autoRefresh()\0"
    "updateLegendItems(QVariant,QList<QwtLegendData>)\0"
    "QBrush\0canvasBackground\0bool\0autoReplot\0"
};

void QwtPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtPlot *_t = static_cast<QwtPlot *>(_o);
        switch (_id) {
        case 0: _t->itemAttached((*reinterpret_cast< QwtPlotItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->legendDataChanged((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QList<QwtLegendData>(*)>(_a[2]))); break;
        case 2: _t->replot(); break;
        case 3: _t->autoRefresh(); break;
        case 4: _t->updateLegendItems((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QList<QwtLegendData>(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtPlot::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QwtPlot,
      qt_meta_data_QwtPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtPlot))
        return static_cast<void*>(const_cast< QwtPlot*>(this));
    if (!strcmp(_clname, "QwtPlotDict"))
        return static_cast< QwtPlotDict*>(const_cast< QwtPlot*>(this));
    return QFrame::qt_metacast(_clname);
}

int QwtPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QBrush*>(_v) = canvasBackground(); break;
        case 1: *reinterpret_cast< bool*>(_v) = autoReplot(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCanvasBackground(*reinterpret_cast< QBrush*>(_v)); break;
        case 1: setAutoReplot(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QwtPlot::itemAttached(QwtPlotItem * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QwtPlot::legendDataChanged(const QVariant & _t1, const QList<QwtLegendData> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
