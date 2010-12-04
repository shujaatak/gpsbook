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
#ifndef VIEWER_H
#define VIEWER_H

#include "qglviewer.h"
#include "gpsdata.h"
#if defined(Q_OS_LINUX)
#include "../displaySRTM/srtm.h"
#endif

using namespace GPSBook;

namespace PluginDisplayGraphic3D {
    class Viewer : public QGLViewer
    {
        private:
            int mLineSize;
            double mAltitudeScaleRatio;
            void drawTerrain();
            double altitude(double x, double y);
            double convertAlt(double altitude);
#if defined(Q_OS_LINUX)
            SrtmDownloader* srtmDownloader;
#endif

        public :
            double minlat, maxlat;
            double minlon, maxlon;
            double minele, maxele;
            double latcenter, loncenter;
            bool drawWall;
            bool drawBox;
            bool drawFloor;
            bool light;
            GPSData* gpsdata;
            Viewer(QWidget* parent);
            int lineSize();
            void setLineSize(int value);
            double altitudeScaleRatio();
            void setAltutudeScaleRatio(double value);
            void updateData();

        protected :
            virtual void draw();

    };
}

#endif // VIEWER_H
