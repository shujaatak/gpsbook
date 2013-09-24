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
#ifndef CURVE_H
#define CURVE_H

#include "qwt/qwt_plot_curve.h"

namespace PluginDisplayGraphic2D {

    class Curve: public QwtPlotCurve
    {
    private:
        int mTrackId;
        int mSegmentId;
        int mRouteId;

    public:
        Curve();
        void setTrackId(int trackId);
        int trackId();
        void setSegmentId(int segmentId);
        int segmentId();
        void setRouteId(int routeId);
        int routeId();
    };

}

#endif // CURVE_H
