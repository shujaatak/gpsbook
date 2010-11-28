#ifndef QTRACKSLISTWIDGET_H
#define QTRACKSLISTWIDGET_H

#include <QListWidget>
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
