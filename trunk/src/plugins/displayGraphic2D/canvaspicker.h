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
#ifndef CANVASPICKER_H
#define CANVASPICKER_H

#include <qobject.h>
#include <qwt_symbol.h>
#include <plot.h>
#include <curve.h>
#include "gpsdata.h"

class QPoint;
class QCustomEvent;

using namespace GPSBook;

namespace PluginDisplayGraphic2D {



    class Plot;

    class CanvasPicker: public QObject
    {
        Q_OBJECT
    public:
        CanvasPicker(Plot *plot);
        virtual bool eventFilter(QObject *, QEvent *);
        virtual bool event(QEvent *);
        void init(GPSData* gpsdata);


    private:
        void select(const QPoint &);
        void showCursor();
        bool mousePressed;
        GPSData* tracklist;
        Curve *d_selectedCurve;
        int d_selectedPoint;

        Plot *plot() { return (Plot *)parent(); }
        const Plot *plot() const { return (Plot *)parent(); }



    };
}

#endif // CANVAPICKER_H
