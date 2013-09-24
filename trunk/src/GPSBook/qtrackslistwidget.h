#ifndef QTRACKSLISTWIDGET_H
#define QTRACKSLISTWIDGET_H

#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QListWidget>
#else
#include <QListWidget>
#endif
#include <QDropEvent>


namespace GPSBook {
    class QTracksListWidget : public QListWidget
    {
        Q_OBJECT
    public:
        explicit QTracksListWidget(QWidget *parent = 0);

    protected:
        void dropEvent(QDropEvent *event);

    signals:

    public slots:

    };
}

#endif // QTRACKSLISTWIDGET_H
