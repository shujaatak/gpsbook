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
#include "serviceprocessminmaxspeed.h"

namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ServiceProcessMinMaxSpeed::ServiceProcessMinMaxSpeed()
    {
    } //ServiceProcessMinMaxSpeed::ServiceProcessSpeed

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool ServiceProcessMinMaxSpeed::init(GPSData* aTracklist)
    {
        maximumSpeed =   9999999;
        averageSpeed = - 9999999;
        mGPSData = aTracklist;
        return true;
    } //ServiceProcessMinMaxSpeed::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ServiceProcessMinMaxSpeed::run()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        average   = 0;
        maxValueX = 0;
        maxValueY = 0;
        maxValue  = - 9999999;
        nbpoints    = 0;
        mGPSData->lockGPSDataForRead();
        //If calculation has to be done for all tracks
        if (mGPSData->displayedTrackIndex < 0)
        {
            foreach(Track* track, mGPSData->trackList)
            {
                foreach(TrackSeg* trackseg, track->trackSegList)
                {
                    getSpeedOfSegment(trackseg);
                }
            }
        }
        else
        {
            //If speed has to be calculated for all segment of a track
            if (mGPSData->displayedSegmentIndex < 0)
            {
                foreach(TrackSeg* trackseg, mGPSData->trackList[mGPSData->displayedTrackIndex]->trackSegList)
                {
                    getSpeedOfSegment(trackseg);
                }
            }
            else
            {
                //If speed has to be calculated for a specific segment
                getSpeedOfSegment(mGPSData->trackList[mGPSData->displayedTrackIndex]->trackSegList[mGPSData->displayedSegmentIndex]);
            }
        }
        mGPSData->unlockGPSData();

        if (maxValue != maximumSpeed)
        {
            maximumSpeed  = maxValue;
            maximumSpeedX = maxValueX;
            maximumSpeedY = maxValueY;
            emit SignalMaximumSpeedChanged(maxValue, maxValueX, maxValueY);
        }

        // TODO - to be updated: add averages into trackseg extension
        average = average / nbpoints;
        if (average != averageSpeed)
        {
            averageSpeed = average;
            emit SignalAverageSpeedChanged(average);
        }
    } //ServiceProcessMinMaxSpeed::run

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ServiceProcessMinMaxSpeed::getSpeedOfSegment(TrackSeg* trackseg)
    {

        foreach(WayPoint* waypoint, trackseg->wayPointList)
        {
            double speed = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","speed").toDouble();
            if (  speed > maxValue )
            {
                maxValue  = speed;
                maxValueY = waypoint->lat;
                maxValueX = waypoint->lon;
            }
            average += speed;
            nbpoints++;
        }
    } //ServiceProcessMinMaxSpeed::getSpeedOfSegment
}
