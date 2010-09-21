/****************************************************************************
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA  02110-1301, USA.
**
** ---
** Copyright (C) 2009, gpsbook-team
**
****************************************************************************/
#ifndef VIEWGRAPHFRAME_H
#define VIEWGRAPHFRAME_H

#include <QtGui/QFrame>
#include <QMenu>
#include "plot.h"
#include "gpsdata.h"
#include "curve.h"

namespace Ui {
    class DisplayGraphic2DFrame;
}

namespace PluginDisplayGraphic2D {

    class DisplayGraphic2DFrame : public QFrame {
        Q_OBJECT
        Q_DISABLE_COPY(DisplayGraphic2DFrame)
    public:
        explicit DisplayGraphic2DFrame(QWidget *parent = 0);
        virtual ~DisplayGraphic2DFrame();
        QLayout* getVerticalLayout();
        Plot* getPlot();
        Plot* getGeoPlot();
        void autoSetEnabled();
        void init(GPSData* gpsdata);


    protected:
        //geoPlot displays the graph with longitude and latidute
        Plot* geoPlot;
        //plot displays other graphs
        Plot* plot;
        virtual void changeEvent(QEvent *e);

    private:
        GPSData* trackList;
        Ui::DisplayGraphic2DFrame *m_ui;
        QMenu *menuX;
        QMenu *menuY;
        QMenu *menuEdition;
        Plot::XAxis xAxis;
        Plot::YAxis yAxis;
        QActionGroup* xActionGroup;
        QActionGroup* yActionGroup;

    private slots:
        void on_actionSplitSegment_triggered();
        void on_toolButtonGeoPlot_toggled(bool checked);
        void on_actionYLatitude_triggered();
        void on_actionYSpeed_triggered();
        void on_actionYDistance_triggered();
        void on_actionYAltitude_triggered();
        void on_actionYAcceleration_triggered();
        void on_actionXPoints_triggered();
        void on_actionXLongitude_triggered();
        void on_actionXDuration_triggered();
        void on_actionXDistance_triggered();
        void on_actionXAbsoluteTime_triggered();

    public slots:
        void on_update();

    };
}
#endif // VIEWGRAPHFRAME_H
