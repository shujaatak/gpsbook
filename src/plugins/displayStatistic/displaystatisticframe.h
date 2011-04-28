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
#ifndef VIEWSTATFRAME_H
#define VIEWSTATFRAME_H

#include <QtGui/QFrame>
#include "gpsdata.h"
#include "interface.h"
#include "ui_displaystatisticframe.h"

using namespace GPSBook;

namespace Ui {
    class DisplayStatisticFrame;
}

namespace PluginDisplayStatistic {

    class DisplayStatisticFrame : public QFrame {
        Q_OBJECT
        Q_DISABLE_COPY(DisplayStatisticFrame)
    public:
        explicit DisplayStatisticFrame(QWidget *parent = 0);
        virtual ~DisplayStatisticFrame();
        void init(GPSData* gpsdata);
        void setTotalDistance(QString value);
        void setNumberOfPoints(QString value);
        void setTotalDuration(QString value);
        void setElevationGain(QString value);
        void setElevationDrop(QString value);
        void autoSetEnabled();
        DisplayPluginInterface* owner;

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DisplayStatisticFrame *m_ui;
        GPSData* mGPSData;
        int getDuration(TrackSeg* trackSeg);
        double getDistance(TrackSeg* trackSeg);
        double getElevationGain(TrackSeg* trackSeg);
        double getElevationDrop(TrackSeg* trackSeg);

    public slots:
        void maximumAltitudeChanged(double altitude, double longitude, double latitude);
        void minimumAltitudeChanged(double altitude, double longitude, double latitude);
        void averageSpeedChanged(double speed);
        void maximumSpeedChanged(double speed, double longitude, double latitude);
        void activeSegmentIdChanged();

    private slots:
        void on_toolButtonAltitude_clicked();
        void on_toolButtonAcceleration_clicked();
        void on_toolButtonSpeed_clicked();
        void on_toolButtonDistance_clicked();
    };
}

#endif // VIEWSTATFRAME_H
