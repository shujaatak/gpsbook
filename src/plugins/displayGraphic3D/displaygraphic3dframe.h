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
#ifndef DISPLAYGRAPHIC3DFRAME_H
#define DISPLAYGRAPHIC3DFRAME_H

#include <QFrame>
#include "viewer.h"
#include "gpsdata.h"

namespace Ui {
    class DisplayGraphic3DFrame;
}

namespace PluginDisplayGraphic3D {
    class DisplayGraphic3DFrame : public QFrame
    {
        Q_OBJECT

        public:
            explicit DisplayGraphic3DFrame(QWidget *parent = 0);
            ~DisplayGraphic3DFrame();

            void init(GPSData* gpsdata);
            void update();

        protected:
            void changeEvent(QEvent *e);

        private:
            Ui::DisplayGraphic3DFrame *ui;
            Viewer* viewer;
            double minlat, maxlat;
            double minlon, maxlon;
            double minele, maxele;


        private slots:
            void on_toolButton_AltitudeScaleMean_clicked();
            void on_toolButton_AltitudeScaleMoins_clicked();
            void on_toolButton_AltitudeScalePlus_clicked();
            void on_toolButton_light_toggled(bool checked);
            void on_toolButton_terrain_toggled(bool checked);
            void on_toolButton_Moins_clicked();
            void on_toolButton_Plus_clicked();
            void on_toolButton_5_toggled(bool checked);
            void on_toolButton_2_toggled(bool checked);
            void on_toolButton_clicked(bool checked);
            void on_toolButton_4_clicked(bool checked);
    };
}
#endif // DISPLAYGRAPHIC3DFRAME_H
