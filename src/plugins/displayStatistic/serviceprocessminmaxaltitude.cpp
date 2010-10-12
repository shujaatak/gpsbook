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
#include <QObject>
#include <QDebug>
#include "serviceprocessminmaxaltitude.h"

namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ServiceProcessMinMaxAltitude::ServiceProcessMinMaxAltitude()
    {
    } //ServiceProcessMinMaxAltitude::ServiceProcessAltitude

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool ServiceProcessMinMaxAltitude::init(GPSData* aTracklist)
    {
        minimumAltitudeZ =   9999999;
        maximumAltitudeZ = - 9999999;
        gpsdata = aTracklist;
        return true;
    } //ServiceProcessMinMaxAltitude::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ServiceProcessMinMaxAltitude::run()
    {
        qDebug() <<__FILE__ << __FUNCTION__;
        minTmpValueX = 0;
        minTmpValueY = 0;
        minTmpValueZ = 9999999;
        maxTmpValueX = 0;
        maxTmpValueY = 0;
        maxTmpValueZ = - 9999999;

        gpsdata->lockGPSDataForRead();
        //If calculation has to be done for all tracks
        if (gpsdata->displayedTrackIndex < 0 )
        {
            foreach(Track* track, gpsdata->trackList)
            {
                foreach(TrackSeg* trackseg, track->trackSegList)
                {
                    getAltitudesOfSegment(trackseg);
                }
            }
        }
        else
        {
            //If altitude has to be calculated for all segment of a track
            if (gpsdata->displayedSegmentIndex < 0)
            {
                foreach(TrackSeg* trackseg, gpsdata->trackList[gpsdata->displayedTrackIndex]->trackSegList)
                {
                    getAltitudesOfSegment(trackseg);
                }
            }
            else
            {
                //If altitude has to be calculated for a specific segment
                getAltitudesOfSegment(gpsdata->trackList[gpsdata->displayedTrackIndex]->trackSegList[gpsdata->displayedSegmentIndex]);
            }
        }
        gpsdata->unlockGPSData();

        //If lowest altidude has changed
        if ( minTmpValueZ != minimumAltitudeZ )
        {
            minimumAltitudeX = minTmpValueX;
            minimumAltitudeY = minTmpValueY;
            minimumAltitudeZ = minTmpValueZ;
            emit SignalMinimumAltitudeChanged(minimumAltitudeZ,minimumAltitudeY,minimumAltitudeX);
        }
        //If highest altitude has changed
        if ( maxTmpValueZ != maximumAltitudeZ )
        {
            maximumAltitudeX = maxTmpValueX;
            maximumAltitudeY = maxTmpValueY;
            maximumAltitudeZ = maxTmpValueZ;
            emit SignalMaximumAltitudeChanged(maximumAltitudeZ,maximumAltitudeY,maximumAltitudeX);
        }
    } //ServiceProcessAltitude::run

    /*------------------------------------------------------------------------------*
      Calculate the min and max altitude of a segment
     *------------------------------------------------------------------------------*/
    void ServiceProcessMinMaxAltitude::getAltitudesOfSegment(TrackSeg* trackseg)
    {
        foreach(WayPoint* wp, trackseg->wayPointList)
        {
            //Find Minimum Altitude
            if ( wp->ele < minTmpValueZ)
            {
                minTmpValueX = wp->lat;
                minTmpValueY = wp->lon;
                minTmpValueZ = wp->ele;
            }
            //Find Maximum Altidude
            if ( wp->ele > maxTmpValueZ)
            {
                maxTmpValueX = wp->lat;
                maxTmpValueY = wp->lon;
                maxTmpValueZ = wp->ele;
            }
        }
    } //ServiceProcessMinMaxAltitude::getAltitudesOfSegment
}
