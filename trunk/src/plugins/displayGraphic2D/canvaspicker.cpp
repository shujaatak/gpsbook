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
** Copyright (c) 2010, gpsbook-team
**
****************************************************************************/
#include <qapplication.h>
#include <qdebug.h>
#include <qevent.h>
#include <qwhatsthis.h>
#include <qpainter.h>
#include <qwt_scale_map.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_directpainter.h>
#include "canvaspicker.h"
#include "plot.h"

namespace PluginDisplayGraphic2D {


    CanvasPicker::CanvasPicker(Plot *plot):
        QObject(plot),
        d_selectedCurve(NULL),
        d_selectedPoint(-1)
    {
        qDebug() << __FILE__ << __FUNCTION__;

        QwtPlotCanvas *canvas = (QwtPlotCanvas*) plot->canvas();

        canvas->installEventFilter(this);

        // We want the focus, but no focus rect. The
        // selected point will be highlighted instead.

        canvas->setFocusPolicy(Qt::StrongFocus);
    #ifndef QT_NO_CURSOR
        canvas->setCursor(Qt::PointingHandCursor);
    #endif
        canvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
        canvas->setFocus();

        mousePressed = false;

        accelerate = 0;

    }

    void CanvasPicker::init(GPSData* gpsdata)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        tracklist = gpsdata;
    }

    bool CanvasPicker::event(QEvent *e)
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if ( e->type() == QEvent::User )
        {
            showCursor();
            return true;
        }
        return QObject::event(e);
    }

    bool CanvasPicker::eventFilter(QObject *object, QEvent *e)
    {
        //qDebug() << __FILE__ << __FUNCTION__;
        if ( object != (QObject *)plot()->canvas() )
            return false;

        switch(e->type())
        {
            case QEvent::MouseButtonPress:
               mousePressed = true;
               select(((QMouseEvent *)e)->pos());
               break;
            case QEvent::MouseMove:
               if (mousePressed)
                 select(((QMouseEvent *)e)->pos());
               break;
            case QEvent::MouseButtonRelease:
               mousePressed = false;
               break;
            case QEvent::KeyPress:
               {
                   if (((QKeyEvent*)e)->isAutoRepeat()) {
                       if ( accelerate < 20 ) {
                           accelerate++;
                       }
                   }
                   else {
                       accelerate = 1;
                   }
                   switch(((const QKeyEvent *)e)->key())
                   {
                       case Qt::Key_Right:
                            if (d_selectedPoint < (d_selectedCurve->data()->size() - accelerate - 1)){
                                d_selectedPoint += accelerate;
                            }
                            else {
                                int idx = plot()->curveList.indexOf(d_selectedCurve);
                                if ((d_selectedPoint == (d_selectedCurve->data()->size()  - 1)) && (idx != (plot()->curveList.count()-1) ) ) {
                                    d_selectedCurve = plot()->curveList[++idx];
                                    d_selectedPoint = 0;
                                    accelerate = 0;
                                }
                                else {
                                    d_selectedPoint =  d_selectedCurve->data()->size()  - 1;
                                }
                            }
                            showCursor();
                       break;
                       case Qt::Key_Left:
                            if (d_selectedPoint > accelerate) {
                                d_selectedPoint -= accelerate;
                            }
                            else {
                                int idx = plot()->curveList.indexOf(d_selectedCurve);
                                if ((d_selectedPoint == 0) && (idx != 0) ) {
                                    d_selectedCurve = plot()->curveList[--idx];
                                    d_selectedPoint = d_selectedCurve->data()->size()  - 1;
                                    accelerate = 0;
                                }
                                else {
                                    d_selectedPoint = 0;
                                }
                            }
                            showCursor();
                       break;
                   }

               }
            break;
            default:
            break;
        }
        return QObject::eventFilter(object, e);
    }

    // Select the point at a position. If there is no point
    // deselect the selected point
    void CanvasPicker::select(const QPoint &pos)
    {
        //qDebug() << __FILE__ << __FUNCTION__;
        Curve *curve = NULL;
        double dist = 10e10;
        int index = -1;

        const QwtPlotItemList& itmList = plot()->itemList();
        for ( QwtPlotItemIterator it = itmList.begin();
            it != itmList.end(); ++it )
        {
            if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve )
            {
                Curve *c = (Curve*)(*it);

                double d;
                int idx = c->closestPoint(pos, &d);
                if ( d < dist )
                {
                    curve = c;
                    index = idx;
                    dist = d;
                }
            }
        }

        showCursor();
        d_selectedCurve = NULL;
        d_selectedPoint = -1;

        if ( curve && dist < 10 ) // 10 pixels tolerance
        {
            d_selectedCurve = curve;
            d_selectedPoint = index;
            showCursor();
        }

    }

    // Hightlight the selected point
    void CanvasPicker::showCursor()
    {
        //qDebug() << __FILE__ << __FUNCTION__ ;
        if ( !d_selectedCurve )
            return;

        tracklist->selectedRouteIndex    = d_selectedCurve->routeId();
        tracklist->selectedTrackIndex    = d_selectedCurve->trackId();
        tracklist->selectedSegmentIndex  = d_selectedCurve->segmentId();
        tracklist->selectedWaypointIndex = d_selectedPoint;

        //Update current trace
        plot()->replot();

        //Send signal to replot other traces
        plot()->doreplot();
    };
}
