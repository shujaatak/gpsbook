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
#include <QDebug>
#include "displaygraphic3dframe.h"
#include "ui_displaygraphic3dframe.h"

namespace PluginDisplayGraphic3D {
    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGraphic3DFrame::DisplayGraphic3DFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::DisplayGraphic3DFrame)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        ui->setupUi(this);
        viewer = new Viewer(this);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
        viewer->setSizePolicy(sizePolicy);
        ui->horizontalLayout->addWidget(viewer);
    } //DisplayGraphic3DFrame::DisplayGraphic3DFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGraphic3DFrame::~DisplayGraphic3DFrame()
    {
        delete ui;
    } //DisplayGraphic3DFrame::~DisplayGraphic3DFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::changeEvent(QEvent *e)
    {
        QFrame::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DisplayGraphic3DFrame::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::init(GPSData* gpsdata){
        qDebug() << __FILE__ << __FUNCTION__ ;
        viewer->gpsdata = gpsdata;
    } //DisplayGraphic3DFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::update(){
        qDebug() << __FILE__ << __FUNCTION__;
        viewer->minlat = viewer->minlon = viewer->minele = 9999999;
        viewer->maxlat = viewer->maxlon = viewer->maxele = -9999999;

        //Calculate the bounding area.
        viewer->gpsdata->lockGPSDataForRead();

        //ROUTE
        if (viewer->gpsdata->displayedRouteIndex != -2)
        {
            int routeIdStart=0;
            int routeIdStop=viewer->gpsdata->routeList.count();
            if (viewer->gpsdata->displayedRouteIndex >= 0 )
            {
                routeIdStart=viewer->gpsdata->displayedRouteIndex;
                routeIdStop=viewer->gpsdata->displayedRouteIndex+1;
            }
            for (int routeId = routeIdStart; routeId < routeIdStop; routeId++ )
            {
                foreach (WayPoint* waypoint, viewer->gpsdata->routeList[routeId]->wayPointList)
                {
                    if (waypoint->lat < viewer->minlat) { viewer->minlat = waypoint->lat; }
                    if (waypoint->lat > viewer->maxlat) { viewer->maxlat = waypoint->lat; }
                    if (waypoint->lon < viewer->minlon) { viewer->minlon = waypoint->lon; }
                    if (waypoint->lon > viewer->maxlon) { viewer->maxlon = waypoint->lon; }
                    if (waypoint->ele < viewer->minele) { viewer->minele = waypoint->ele; }
                    if (waypoint->ele > viewer->maxele) { viewer->maxele = waypoint->ele; }
                }
            }
        }

        //TRACK
        if (viewer->gpsdata->displayedTrackIndex != -2)
        {
            int trackIdStart=0;
            int trackIdStop=viewer->gpsdata->trackList.count();
            if (viewer->gpsdata->displayedTrackIndex >= 0 )
            {
                trackIdStart=viewer->gpsdata->displayedTrackIndex;
                trackIdStop=viewer->gpsdata->displayedTrackIndex+1;
            }
            for (int trackId = trackIdStart; trackId < trackIdStop; trackId++ )
            {
                int segmentIdStart=0;
                int segmentIdStop=viewer->gpsdata->trackList[trackId]->trackSegList.count();
                if (viewer->gpsdata->displayedSegmentIndex  >= 0 )
                {
                    segmentIdStart=viewer->gpsdata->displayedSegmentIndex;
                    segmentIdStop=viewer->gpsdata->displayedSegmentIndex+1;
                }
                for (int trackSegId = segmentIdStart; trackSegId < segmentIdStop ; trackSegId++ )
                {
                    foreach (WayPoint* waypoint, viewer->gpsdata->trackList[trackId]->trackSegList[trackSegId]->wayPointList)
                    {
                        if (waypoint->lat < viewer->minlat) { viewer->minlat = waypoint->lat; }
                        if (waypoint->lat > viewer->maxlat) { viewer->maxlat = waypoint->lat; }
                        if (waypoint->lon < viewer->minlon) { viewer->minlon = waypoint->lon; }
                        if (waypoint->lon > viewer->maxlon) { viewer->maxlon = waypoint->lon; }
                        if (waypoint->ele < viewer->minele) { viewer->minele = waypoint->ele; }
                        if (waypoint->ele > viewer->maxele) { viewer->maxele = waypoint->ele; }
                    }
                }
            }
        }

        viewer->latcenter=0;
        viewer->loncenter=0;
        if ((viewer->maxlon-viewer->minlon) > (viewer->maxlat-viewer->minlat))
        {
            viewer->latcenter=((viewer->maxlon-viewer->minlon)-(viewer->maxlat-viewer->minlat))/2;
            viewer->maxlat=viewer->minlat + (viewer->maxlon-viewer->minlon) ;            
        }
        else
        {
            viewer->loncenter=((viewer->maxlat-viewer->minlat)-(viewer->maxlon-viewer->minlon))/2;
            viewer->maxlon=viewer->minlon + (viewer->maxlat-viewer->minlat);
        }
        //Define max elevation base on scale defined for lon and lat
            //TODO: use the service instead of this (duplicated) code
            long r = 6378; //meters
            const double PI = 3.14159265358979323846264338328;
            double rad_lon1 = 0 * ( PI / 180 );
            double rad_lat1 = viewer->latcenter * ( PI / 180 );
            double rad_lon2 = 0 * ( PI / 180 );
            double rad_lat2 = viewer->maxlat * ( PI / 180 );
            viewer->maxele=viewer->minele+acos(cos(rad_lat1)*cos(rad_lon1)*cos(rad_lat2)*cos(rad_lon2) + cos(rad_lat1)*sin(rad_lon1)*cos(rad_lat2)*sin(rad_lon2) + sin(rad_lat1)*sin(rad_lat2)) * r;
        //qDebug() << viewer->minlat << viewer->minlon << viewer->minele << viewer->maxlat << viewer->maxlon << viewer->maxele;
        viewer->gpsdata->unlockGPSData();

        viewer->updateData();

    } //DisplayGraphic3DFrame::update

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_clicked(bool checked)
    {
        viewer->setGridIsDrawn(checked);
    } //DisplayGraphic3DFrame::on_toolButton_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_4_clicked(bool checked)
    {
        viewer->setAxisIsDrawn(checked);
    } //DisplayGraphic3DFrame::on_toolButton_4_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_2_toggled(bool checked)
    {
        viewer->drawWall = checked;
        viewer->showEntireScene();
    } //DisplayGraphic3DFrame::on_toolButton_2_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_5_toggled(bool checked)
    {
        viewer->drawBox = checked;
        viewer->showEntireScene();
    } //DisplayGraphic3DFrame::on_toolButton_5_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_terrain_toggled(bool checked)
    {
        viewer->drawFloor = checked;
        viewer->showEntireScene();
    }

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_light_toggled(bool checked)
    {
        viewer->light=checked;
        viewer->showEntireScene();
    }

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_Plus_clicked()
    {
        viewer->setLineSize( viewer->lineSize() + 1);
        ui->toolButton_Moins->setEnabled(viewer->lineSize()>1);
        ui->toolButton_Plus->setEnabled(viewer->lineSize()<20);
    } //DisplayGraphic3DFrame::on_toolButton_Plus_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3DFrame::on_toolButton_Moins_clicked()
    {
        viewer->setLineSize( viewer->lineSize() - 1);
        ui->toolButton_Moins->setEnabled(viewer->lineSize()>1);
        ui->toolButton_Plus->setEnabled(viewer->lineSize()<20);
    } //DisplayGraphic3DFrame::on_toolButton_Moins_clicked


    void DisplayGraphic3DFrame::on_toolButton_AltitudeScalePlus_clicked()
    {
        viewer->setAltutudeScaleRatio( viewer->altitudeScaleRatio() + 0.1);
        ui->toolButton_AltitudeScaleMoins->setEnabled(viewer->altitudeScaleRatio()>0.1);
        ui->toolButton_AltitudeScalePlus->setEnabled(viewer->altitudeScaleRatio()<5);
    }

    void DisplayGraphic3DFrame::on_toolButton_AltitudeScaleMoins_clicked()
    {
        viewer->setAltutudeScaleRatio( viewer->altitudeScaleRatio() - 0.1);
        ui->toolButton_AltitudeScaleMoins->setEnabled(viewer->altitudeScaleRatio()>0.1);
        ui->toolButton_AltitudeScalePlus->setEnabled(viewer->altitudeScaleRatio()<5);
    }
}

void PluginDisplayGraphic3D::DisplayGraphic3DFrame::on_toolButton_AltitudeScaleMean_clicked()
{
    viewer->setAltutudeScaleRatio( 1) ;
    ui->toolButton_AltitudeScaleMoins->setEnabled(true);
    ui->toolButton_AltitudeScalePlus->setEnabled(true);

}
