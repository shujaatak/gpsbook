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
#ifndef PLOT_H
#define PLOT_H

#include <qobject.h>
#include <qwt_plot.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_rescaler.h>
#include "canvaspicker.h"
#include "curve.h"
#include "gpsdata.h"

using namespace GPSBook;

namespace PluginDisplayGraphic2D {

    //______________________________________________________________________________________
    class DurationScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class TimeScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class LongitudeScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class LatitudeScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class DistanceScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class PointScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class AltitudeScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class SpeedScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class AccelerationScaleDraw: public QwtScaleDraw
    {
    public:
        virtual QwtText label(double v) const;
    };

    //______________________________________________________________________________________
    class PlotPicker;
    class CanvasPicker;

    //______________________________________________________________________________________
    class Plot: public QwtPlot
    {
        Q_OBJECT
    public:
        enum XAxis {axis_x_absolute_time,
                    axis_x_distance,
                    axis_x_duration,
                    axis_x_longitude,
                    axis_x_point
                   };
        enum YAxis {axis_y_altitude,
                    axis_y_distance,
                    axis_y_latitude,
                    axis_y_speed,
                    axis_y_acceleration
                   };
        //inline XAxis xaxis() const { return static_cast<XAxis>(x); }
        //inline YAxis yaxis() const { return static_cast<YAxis>(y); }

        Plot(QWidget *parent = NULL);
        void setDisplayCursorInfo(bool enabled);
        void doreplot();
        void init(GPSData* gpsdata);
        QPointF position;
        bool fillcurve;


    public slots:
        void updateTracklistCurve(GPSData* gpsdata, XAxis X_Axis = axis_x_longitude, YAxis Y_Axis = axis_y_latitude);
        void replot();

    private:
        QList<Curve*> curveList;
        PlotPicker* plotPicker;
        QwtPlotRescaler *rescaler;
        CanvasPicker *canvasPicker;

        ushort x; //X Axis
        ushort y; //Y Axis

        double minXValue;
        double maxXValue;
        double minYValue;
        double maxYValue;

        GPSData* mGPSData;

        int drawSegment(QList<TrackSeg*> segmentList, int trackId, int displayedIndex, XAxis X_Axis, YAxis Y_Axis, int point);

    signals:
        void signalReplot();

    };

    //______________________________________________________________________________________
    class PlotPicker: public QwtPlotPicker
    {
        Q_OBJECT
    public:
        Plot::XAxis x_axis;
        Plot::YAxis y_axis;
        DurationScaleDraw*     durationScaleDraw;
        TimeScaleDraw*         timeScaleDraw;
        LongitudeScaleDraw*    longitudeScaleDraw;
        LatitudeScaleDraw*     latitudeScaleDraw;
        DistanceScaleDraw*     distanceScaleDraw;
        PointScaleDraw*        pointScaleDraw;
        AltitudeScaleDraw*     altitudeScaleDraw;
        SpeedScaleDraw*        speedScaleDraw;
        AccelerationScaleDraw* accelerationScaleDraw;
        bool enabled;

        PlotPicker(QwtPlotCanvas *canvas);

        virtual QwtText trackerText(const QPoint &pos) const;
        QwtText trackerTextT(QPointF pos);

        void paintEvent ( QPaintEvent * );


    };

}
#endif //PLOT_H
