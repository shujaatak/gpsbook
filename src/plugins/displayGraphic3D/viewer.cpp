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
#include "viewer.h"
#include <QDebug>
#include <QSettings>

namespace PluginDisplayGraphic3D {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    Viewer::Viewer(QWidget* parent){
        qDebug() << __FILE__ << __FUNCTION__ ;
        this->setParent(parent);
        //setAxisIsDrawn(true);
        setGridIsDrawn(true);
        drawWall = false;
        drawBox = true;
        drawFloor = true;
        light=true;
        mLineSize = 1;
        mAltitudeScaleRatio = 1.5;
        QSettings settings("GPSBook","GPSBook");
        QString storagePath = settings.value("StorageLocation","").toString();
        srtmDownloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/",
                                            storagePath+"srtm");
    } //Viewer::Viewer

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Viewer::draw()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        if (light)
        {
            glDisable(GL_LIGHTING);
        }
        if (drawBox)
        {
            glColor3f(1, 1, 1);
            glLineWidth(1.0);
            glBegin(GL_LINE_STRIP);
              glVertex3i(-1,-1,0);glVertex3i(1,-1,0);glVertex3i(1,1,0);glVertex3i(-1,1,0);
              glVertex3i(-1,-1,0);glVertex3i(-1,-1,1);glVertex3i(-1,1,1);glVertex3i(1,1,1);
              glVertex3i(1,-1,1);glVertex3i(-1,-1,1);
            glEnd();
            glBegin(GL_LINES);
              glVertex3i(-1,1,0);glVertex3i(-1,1,1);
              glVertex3i(1,1,0);glVertex3i(1,1,1);
              glVertex3i(1,-1,0);glVertex3i(1,-1,1);
            glEnd();
        }
        if (drawFloor)
        {
            drawTerrain();
        }
        if ( !gpsdata->routeList.isEmpty() ||
             !gpsdata->trackList.isEmpty() )
        {
            glLineWidth(mLineSize);
            gpsdata->lockGPSDataForRead();

            //ROUTE
            if (gpsdata->displayedRouteIndex != -2) {
                int routeIdStart=0;
                int routeIdStop=gpsdata->routeList.count();
                if (gpsdata->displayedRouteIndex >= 0 )
                {
                    routeIdStart=gpsdata->displayedRouteIndex;
                    routeIdStop=gpsdata->displayedRouteIndex+1;
                }

                for (int routeId = routeIdStart; routeId < routeIdStop ; routeId++ )
                {
                    if (drawWall)
                        glBegin(GL_QUAD_STRIP);
                    else
                        glBegin(GL_LINE_STRIP);
                        foreach (WayPoint* waypoint, gpsdata->routeList[routeId]->wayPointList)
                        {
                            //Recalculate position
                            double lon=(2/(maxlon-minlon))*(waypoint->lon+loncenter)+(-1-((2*minlon)/(maxlon-minlon)));
                            double lat=(2/(maxlat-minlat))*(waypoint->lat+latcenter)+(-1-((2*minlat)/(maxlat-minlat)));
                            //double ele=(2/(maxele-minele))*waypoint->ele+(-1-((2*minele)/(maxele-minele)));
                            double ele=convertAlt(waypoint->ele);

                            glColor3f(1.0-(0.5*ele), 0.2f , ele);
                            if (drawWall)
                                glVertex3f(lon, lat, 0);
                            glVertex3f(lon, lat, ele);
                            //qDebug() << waypoint->lon << ","  << waypoint->lat << ","  <<  waypoint->ele;
                        }
                    glEnd();
                }
            }

            //TRACK
            if (gpsdata->displayedTrackIndex != -2) {
                int trackIdStart=0;
                int trackIdStop=gpsdata->trackList.count();
                if (gpsdata->displayedTrackIndex >= 0)
                {
                    trackIdStart=gpsdata->displayedTrackIndex;
                    trackIdStop=gpsdata->displayedTrackIndex+1;
                }
                WayPoint* firstWaypoint = 0 ;
                for (int trackId = trackIdStart; trackId < trackIdStop; trackId++ )
                {
                    int segmentIdStart=0;
                    int segmentIdStop=gpsdata->trackList[trackId]->trackSegList.count();
                    if (gpsdata->displayedSegmentIndex >= 0)
                    {
                        segmentIdStart=gpsdata->displayedSegmentIndex;
                        segmentIdStop=gpsdata->displayedSegmentIndex+1;
                    }
                    for (int trackSegId = segmentIdStart; trackSegId < segmentIdStop ; trackSegId++ )
                    {
                        if (drawWall)
                            glBegin(GL_QUAD_STRIP);
                        else
                            glBegin(GL_LINE_STRIP);
                                foreach (WayPoint* waypoint, gpsdata->trackList[trackId]->trackSegList[trackSegId]->wayPointList)
                                {
                                    if (!firstWaypoint)
                                    {
                                        //qDebug() << waypoint->lon << ","  << waypoint->lat << ","  <<  waypoint->ele;
                                        firstWaypoint = waypoint;
                                    }
                                    //Recalculate position
                                    double lon=(2/(maxlon-minlon))*(waypoint->lon+loncenter)+(-1-((2*minlon)/(maxlon-minlon)));
                                    double lat=(2/(maxlat-minlat))*(waypoint->lat+latcenter)+(-1-((2*minlat)/(maxlat-minlat)));
                                    //double ele=(2/(maxele-minele))*waypoint->ele+(-1-((2*minele)/(maxele-minele)));
                                    double ele=convertAlt(waypoint->ele);

                                    glColor3f(1.0-(0.5*ele), 0.2f , ele);
                                    if (drawWall)
                                        glVertex3f(lon, lat, 0);
                                    glVertex3f(lon, lat, ele);
                                    //qDebug() << waypoint->lon << ","  << waypoint->lat << ","  <<  waypoint->ele;
                                }
                            glEnd();
                    }
                }
            }
            gpsdata->unlockGPSData();
        }
    }  //Viewer::draw

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Viewer::updateData()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        draw();
        showEntireScene();
    } //Viewer::updateData

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    int Viewer::lineSize()
    {
        return mLineSize;
    } //Viewer::lineSize

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Viewer::setLineSize(int value)
    {
        mLineSize =  value;
        updateGL();
    } //Viewer::setLineSize

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    double Viewer::altitudeScaleRatio()
    {
        return mAltitudeScaleRatio;
    } //Viewer::altitudeScaleRatio

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Viewer::setAltutudeScaleRatio(double value)
    {
        mAltitudeScaleRatio =  value;
        updateGL();
    } //Viewer::setAltutudeScaleRatio


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Viewer::drawTerrain(){
        qDebug() << __FILE__ << __FUNCTION__ ;

        int deltaMax = 2;
        double Precision = 0.025;
        double X_Buffer, Y_Buffer;
        for (X_Buffer = -deltaMax/2 ; X_Buffer < deltaMax/2 ; X_Buffer = X_Buffer + Precision){
            for (Y_Buffer = -deltaMax/2 ; Y_Buffer < deltaMax/2 ; Y_Buffer = Y_Buffer + Precision){
                glBegin(GL_TRIANGLE_STRIP);
                    // define first triangle
                    glColor3f(sin(altitude( X_Buffer , Y_Buffer )), altitude( X_Buffer , Y_Buffer ), cos(altitude( X_Buffer , Y_Buffer )));
                    glVertex3d(X_Buffer, Y_Buffer, altitude(X_Buffer, Y_Buffer));
                    glVertex3d(X_Buffer + Precision, Y_Buffer, altitude(X_Buffer +Precision, Y_Buffer));
                    glVertex3d(X_Buffer, Y_Buffer + Precision, altitude(X_Buffer, Y_Buffer + Precision) );

                    // Define second triangle
                    glColor3f(sin(altitude( X_Buffer , Y_Buffer )), cos(altitude( X_Buffer , Y_Buffer )), altitude( X_Buffer , Y_Buffer ));
                    glVertex3d(X_Buffer + Precision, Y_Buffer + Precision, altitude( X_Buffer + Precision , Y_Buffer + Precision));
                glEnd();
            }
        }
    }

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    double Viewer::convertAlt(double altitude){
        //mAltitudeScaleRatio
        return (1/(maxele/mAltitudeScaleRatio-minele))*altitude+(-((1*minele)/(maxele/mAltitudeScaleRatio-minele)));

    }

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    double Viewer::altitude(double x, double y){
        //Convert x,y in lon,lat
        double lat=(y-(-1-((2*minlat)/(maxlat-minlat))))/(2/(maxlat-minlat))-latcenter;
        double lon=(x-(-1-((2*minlon)/(maxlon-minlon))))/(2/(maxlon-minlon))-loncenter;
        double alt = 0;
        alt = convertAlt(srtmDownloader->getAltitudeFromLatLon(lat,lon));
        return alt;
    }


}
