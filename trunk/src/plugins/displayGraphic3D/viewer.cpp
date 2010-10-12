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
        mLineSize = 2;
    } //Viewer::Viewer

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Viewer::draw()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        glDisable(GL_LIGHTING);
        if (drawBox){
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
        //qDebug() << __FUNCTION__;
        if (gpsdata)
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
                            double ele=(1/(maxele-minele))*waypoint->ele+(-((1*minele)/(maxele-minele)));

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
                                double ele=(1/(maxele-minele))*waypoint->ele+(-((1*minele)/(maxele-minele)));

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
    } //Viewer::updateData

}
