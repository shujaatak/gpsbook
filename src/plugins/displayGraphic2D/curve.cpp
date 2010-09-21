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
#include "curve.h"

namespace PluginDisplayGraphic2D {

    Curve::Curve() : QwtPlotCurve()
    {
        mTrackId   = -1;
        mSegmentId = -1;
        mRouteId   = -1;
    }

    void Curve::setTrackId(int trackId)
    {
        mTrackId = trackId;
    }

    int Curve::trackId()
    {
        return mTrackId;
    }

    void Curve::setSegmentId(int segmentId)
    {
        mSegmentId = segmentId;
    }

    int Curve::segmentId()
    {
        return mSegmentId;
    }

    void Curve::setRouteId(int routeId)
    {
        mRouteId = routeId;
    }

    int Curve::routeId()
    {
        return mRouteId;
    }
}

