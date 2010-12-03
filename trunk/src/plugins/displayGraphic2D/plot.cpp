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
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_series_data.h>
#include <qwt_plot_directpainter.h>
#include <qwt_color_map.h>
#include <qwt_symbol.h>
#include <QPainter>
#include "plot.h"

#include <QDebug>

namespace PluginDisplayGraphic2D {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText DurationScaleDraw::label(double v) const
    {
        QTime time;
        QString result = time.addSecs(v).toString("hh:mm");
        return result;
    } //DurationScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText TimeScaleDraw::label(double v) const
    {
        QDateTime dateTime = QDateTime::fromTime_t(v);
        QString result = dateTime.toString("hh:mm");
        return result;
    } //TimeScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText LongitudeScaleDraw::label(double v) const
    {
        QString result;
        if (v > 0)
            result = QString::number(v) + "E";
        else
            result = QString::number(-v) + "W";
        return result;
    } //LongitudeScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText LatitudeScaleDraw::label(double v) const
    {
        QString result;
        if (v > 0)
            result = QString::number(v) + "N";
        else
            result = QString::number(-v) + "S";
        return result;
    } //LatitudeScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText DistanceScaleDraw::label(double v) const
    {
        QString result = QString::number(v);
        return result + "km";
    } //DistanceScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText PointScaleDraw::label(double v) const
    {
        QString result = QString::number(round(v));
        return result;
    } //PointScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText AltitudeScaleDraw::label(double v) const
    {
        QString result = QString::number(v);
        return result + "m";
    } //AltitudeScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText SpeedScaleDraw::label(double v) const
    {
        QString result = QString::number(v);
        return result + "km/h";
    } //SpeedScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText AccelerationScaleDraw::label(double v) const
    {
        QString result = QString::number(v);
        return result + "m/s/s";
    } //AccelerationScaleDraw::label

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    PlotPicker::PlotPicker(QwtPlotCanvas *canvas): QwtPlotPicker(canvas)
    {
        setTrackerMode(AlwaysOn);
        durationScaleDraw     = new DurationScaleDraw();
        timeScaleDraw         = new TimeScaleDraw();
        longitudeScaleDraw    = new LongitudeScaleDraw();
        latitudeScaleDraw     = new LatitudeScaleDraw();
        distanceScaleDraw     = new DistanceScaleDraw();
        pointScaleDraw        = new PointScaleDraw();
        altitudeScaleDraw     = new AltitudeScaleDraw();
        speedScaleDraw        = new SpeedScaleDraw();
        accelerationScaleDraw = new AccelerationScaleDraw();
        enabled = false;
    } //PlotPicker::PlotPicker

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText PlotPicker::trackerTextT(QPointF pos)
    {
        qDebug() << __FILE__ << __FUNCTION__ << pos;
        // WARNING
        return trackerText(transform(pos));
    }

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QwtText PlotPicker::trackerText(const QPoint &pos) const
    {
        QString trackerText = "";
        if ( enabled )
        {
            // WARNING
            QPointF position = invTransform(pos);
            switch (x_axis)
            {
                case Plot::axis_x_absolute_time:
                    trackerText = timeScaleDraw->label(position.x()).text();
                break;
                case Plot::axis_x_distance:
                    trackerText = distanceScaleDraw->label(position.x()).text();
                break;
                case Plot::axis_x_duration:
                    trackerText = durationScaleDraw->label(position.x()).text();
                break;
                case Plot::axis_x_longitude:
                    trackerText = longitudeScaleDraw->label(position.x()).text();
                break;
                case Plot::axis_x_point:
                    trackerText = pointScaleDraw->label(position.x()).text();
                break;
            }
            trackerText += ", ";
            switch (y_axis)
            {
                case Plot::axis_y_latitude:
                    trackerText += latitudeScaleDraw->label(position.y()).text();
                break;
                case Plot::axis_y_distance:
                    trackerText += distanceScaleDraw->label(position.y()).text();
                break;
                case Plot::axis_y_altitude:
                    trackerText += altitudeScaleDraw->label(position.y()).text();
                break;
                case Plot::axis_y_speed:
                    trackerText += speedScaleDraw->label(position.y()).text();
                break;
                case Plot::axis_y_acceleration:
                    trackerText += accelerationScaleDraw->label(position.y()).text();
                break;
            }
        }
        QColor bg(Qt::white);
        QwtText result;
        result.setBackgroundBrush( QBrush( bg ));
        result.setText(trackerText);
        return result;
    } //QwtText PlotPicker::trackerText

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    Plot::Plot(QWidget *parent): QwtPlot(parent)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        setCanvasBackground(Qt::white);
        rescale = true;

        QwtPlotGrid *grid = new QwtPlotGrid;
        grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
        grid->attach(this);

        //____________________________________________________________________________
        canvasPicker = new CanvasPicker(this);

        //____________________________________________________________________________
        QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );

        magnifier->setAxisEnabled(QwtPlot::xBottom,true);
        magnifier->setAxisEnabled(QwtPlot::yLeft,true);

        //____________________________________________________________________________
        QwtPlotPanner *panner = new QwtPlotPanner(canvas());
        panner->setAxisEnabled(QwtPlot::yRight, false);
        panner->setMouseButton(Qt::MidButton);

        //____________________________________________________________________________
        plotPicker = new PlotPicker(canvas());
        const QColor c(Qt::darkRed);
        plotPicker->setRubberBandPen(c);
        plotPicker->setTrackerPen(c);

        //____________________________________________________________________________
        rescaler = new QwtPlotRescaler(canvas());
        rescaler->setExpandingDirection(QwtPlotRescaler::ExpandBoth);
        rescaler->aspectRatio(1);
        rescaler->setEnabled(true);
    } //Plot::Plot

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Plot::init(GPSData* gpsdata)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        mGPSData = gpsdata;
        canvasPicker->init(gpsdata);
    }


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    int Plot::drawSegment(QList<TrackSeg*> segmentList, int trackId, int displayedIndex, XAxis X_Axis, YAxis Y_Axis, int point)
    {
        QLinearGradient gradient;
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0,Qt::white);
        gradient.setStart(0, 0);
        gradient.setFinalStop(1, 1);

        int segmentIdStart=0;
        int segmentIdStop=segmentList.count();
        if (displayedIndex >= 0 )
        {
            segmentIdStart=displayedIndex;
            segmentIdStop=displayedIndex+1;
        }

        int colorRef = 360 / (segmentList.count() + 1 );
        int colorId = segmentIdStart;
        for (int segmentId = segmentIdStart; segmentId < segmentIdStop ; segmentId++ )
        {
            Curve* curve = new Curve();
            QPen* pen = new QPen(QColor(Qt::black));
            pen->setWidth(2);
            if (fillcurve) {
                gradient.setColorAt(1,QColor::fromHsv(colorRef*colorId,150,150,255));
                QBrush brush(gradient);
                curve->setBrush(brush);
            }
            else
            {
                pen->setColor(QColor::fromHsv(colorRef*colorId,200,200,255));
            }
            curve->setPen(*pen);
            colorId++;


            double x[segmentList[segmentId]->wayPointList.count()];
            double y[segmentList[segmentId]->wayPointList.count()];
            double origin = segmentList[segmentId]->wayPointList[0]->time.toTime_t();
            int i=0;
            foreach (WayPoint* waypoint, segmentList[segmentId]->wayPointList)
            {
                switch (X_Axis)
                {
                    case Plot::axis_x_absolute_time:
                        x[i] = waypoint->time.toTime_t();
                    break;
                    case Plot::axis_x_distance:
                        x[i] = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","distance").toDouble();
                    break;
                    case Plot::axis_x_duration:
                        x[i] = waypoint->time.toTime_t() - origin;
                    break;
                    case Plot::axis_x_longitude:
                        x[i] = waypoint->lon;
                    break;
                    case Plot::axis_x_point:
                        x[i] = point;
                    break;

                }
                switch (Y_Axis)
                {
                    case Plot::axis_y_latitude:
                        y[i] = waypoint->lat;
                    break;
                    case Plot::axis_y_distance:
                        y[i] = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","distance").toDouble();
                    break;
                    case Plot::axis_y_altitude:
                        y[i] = waypoint->ele;
                    break;
                    case Plot::axis_y_speed:
                        y[i] = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","speed").toDouble();
                    break;
                    case Plot::axis_y_acceleration:
                        y[i] = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","acceleration").toDouble();
                    break;
                }
                i++;
                point++;
            }
            curve->setSamples(x, y, sizeof(x) / sizeof(x[0]));
            if (trackId != -1 )
            {
                curve->setTrackId(trackId);
                curve->setSegmentId(segmentId);
                curve->setRouteId(-1);
            }
            else
            {
                curve->setTrackId(-1);
                curve->setSegmentId(-1);
                curve->setRouteId(segmentId);
            }
            curve->attach(this);
            curveList << curve;
            if ( curve->minXValue() < minXValue)
            {
                minXValue = curve->minXValue();
            }
            if ( curve->maxXValue() > maxXValue)
            {
                maxXValue = curve->maxXValue();
            }
            if ( curve->minYValue() < minYValue)
            {
                minYValue = curve->minYValue();
            }
            if ( curve->maxYValue() > maxYValue)
            {
                maxYValue = curve->maxYValue();
            }
        }
        return point;
    }

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Plot::updateTracklistCurve(GPSData* gpsdata, XAxis X_Axis, YAxis Y_Axis)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;

        minXValue = 1e300;
        maxXValue = -1e300;
        minYValue = 1e300;
        maxYValue = -1e300;
        plotPicker->x_axis = X_Axis;
        plotPicker->y_axis = Y_Axis;
        while (!curveList.isEmpty())
        {
            curveList.takeFirst()->detach();
        }

        //rescaler->setEnabled((X_Axis == Plot::axis_x_longitude));
        rescaler->setEnabled(rescale);

        gpsdata->lockGPSDataForRead();
        //ROUTE
        if (gpsdata->displayedRouteIndex != -2)
        {
            drawSegment(reinterpret_cast< QList<TrackSeg*> &>(gpsdata->routeList), -1, gpsdata->displayedRouteIndex, X_Axis, Y_Axis, 0);
        }


        //TRACK
        if (gpsdata->displayedTrackIndex != -2)
        {
            int trackIdStart=0;
            int trackIdStop=gpsdata->trackList.count();
            if (gpsdata->displayedTrackIndex >= 0)
            {
                trackIdStart=gpsdata->displayedTrackIndex;
                trackIdStop=gpsdata->displayedTrackIndex+1;
            }

            int point = 0;
            for (int trackId = trackIdStart; trackId < trackIdStop; trackId++ )
            {
                point = drawSegment(gpsdata->trackList[trackId]->trackSegList, trackId, gpsdata->displayedSegmentIndex, X_Axis, Y_Axis, point);
            }
        }
        foreach(Curve* curve, curveList){
            if ( ( Y_Axis == Plot::axis_y_speed ) ||
                 ( Y_Axis == Plot::axis_y_acceleration ) ) {
                curve->setBaseline(0);
            }
            else {
                curve->setBaseline(minYValue);
            }
        }
        gpsdata->unlockGPSData();
        switch (X_Axis)
        {
            case Plot::axis_x_absolute_time:
                setAxisScaleDraw(Plot::xBottom, new TimeScaleDraw());
            break;
            case Plot::axis_x_distance:
                setAxisScaleDraw(Plot::xBottom, new DistanceScaleDraw());
            break;
            case Plot::axis_x_duration:
                setAxisScaleDraw(Plot::xBottom, new DurationScaleDraw());
            break;
            case Plot::axis_x_longitude:
                setAxisScaleDraw(Plot::xBottom, new LongitudeScaleDraw());
            break;
            case Plot::axis_x_point:
                setAxisScaleDraw(Plot::xBottom, new PointScaleDraw());
            break;
        }
        switch (Y_Axis)
        {
            case Plot::axis_y_latitude:
                setAxisScaleDraw(Plot::yLeft, new LatitudeScaleDraw());
            break;
            case Plot::axis_y_distance:
                setAxisScaleDraw(Plot::yLeft, new DistanceScaleDraw());
            break;
            case Plot::axis_y_altitude:
                setAxisScaleDraw(Plot::yLeft, new AltitudeScaleDraw());
            break;
            case Plot::axis_y_speed:
                setAxisScaleDraw(Plot::yLeft, new SpeedScaleDraw());
            break;
            case Plot::axis_y_acceleration:
                setAxisScaleDraw(Plot::yLeft, new AccelerationScaleDraw());
            break;
        }
        if ((X_Axis == Plot::axis_x_longitude))
        {
            double deltaX = (maxXValue - minXValue);
            double deltaY = (maxYValue - minYValue);
            if (width() > height())
            {
               if (deltaX > deltaY)
               {
                    rescaler->setReferenceAxis(QwtPlot::xBottom);
               }
               else
               {
                   rescaler->setReferenceAxis(QwtPlot::yLeft);
               }
            }
            else
            {
                if (deltaX > deltaY)
                 {
                     rescaler->setReferenceAxis(QwtPlot::xBottom);
                 }
                else
                {
                    rescaler->setReferenceAxis(QwtPlot::yLeft);
                }
            }
        }

        if ( rescale ) {
            setAxisScale(QwtPlot::xBottom, minXValue, maxXValue);
            setAxisScale(QwtPlot::yLeft, minYValue, maxYValue);
            setAxisAutoScale(QwtPlot::yLeft);
            setAxisAutoScale(QwtPlot::xBottom);
            plotLayout()->setAlignCanvasToScales(true);
        }

        replot();

    } //Plot::updateTracklistCurve

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Plot::setDisplayCursorInfo(bool enabled)
    {
        plotPicker->enabled = enabled;
    } //Plot::setDisplayCursorInfo

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Plot::replot()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        QwtPlot::replot();

        foreach (Curve* curve, curveList)
        {
            qDebug() << __FILE__ << __FUNCTION__ << "\n" <<
                    "mGPSData->selectedWaypointIndex" << mGPSData->selectedWaypointIndex << "\n" <<
                    "curve->trackId()" << curve->trackId() << "==" <<
                    "mGPSData->selectedTrackIndex" << mGPSData->selectedTrackIndex << "\n && \n" <<
                    "curve->segmentId()" <<  curve->segmentId()<< "==" <<
                    "mGPSData->selectedSegmentIndex" << mGPSData->selectedSegmentIndex << "\n" <<
                    "curve->routeId()" << curve->routeId() << "==" <<
                    "mGPSData->selectedRouteIndex" << mGPSData->selectedRouteIndex;
            if (
                 (
                    mGPSData->selectedWaypointIndex >= 0
                 )
                 &&
                 (
                   (
                     ( curve->trackId() >= 0 )
                     &&
                     ( curve->trackId() == mGPSData->selectedTrackIndex )
                     &&
                     ( curve->segmentId() == mGPSData->selectedSegmentIndex )
                   )
                   ||
                   (
                     ( curve->routeId() >= 0)
                     &&
                     ( curve->routeId() == mGPSData->selectedRouteIndex )
                   )
                 )
               )
            {
                QwtSymbol *symbolCross = new QwtSymbol(QwtSymbol::Cross,
                                                       Qt::gray,
                                                       QColor(Qt::black),
                                                       QSize(10, 10));
                curve->setSymbol(symbolCross);
                QwtPlotDirectPainter directPainter;
                directPainter.drawSeries(curve,
                                         mGPSData->selectedWaypointIndex,
                                         mGPSData->selectedWaypointIndex);
                QwtSymbol *noSymbol = new QwtSymbol(QwtSymbol::NoSymbol);

                curve->setSymbol(noSymbol);
            }
        }

    } //Plot::replot

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Plot::doreplot()
    {
        emit signalReplot();
    }
}

