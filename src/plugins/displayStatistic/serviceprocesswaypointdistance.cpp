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
#include "serviceprocesswaypointdistance.h"



namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ServiceProcessWaypointDistance::ServiceProcessWaypointDistance()
    {
    } //ServiceProcessWaypointDistance::ServiceProcessWaypointDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool ServiceProcessWaypointDistance::init(GPSData* gpsdata)
    {
        mGPSData = gpsdata;
        return true;
    } //ServiceProcessWaypointDistance::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ServiceProcessWaypointDistance::run()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track,  mGPSData->trackList )
        {
            foreach (TrackSeg* trackSeg, track->trackSegList )
            {
                double totalDistance = 0;
                WayPoint* waypointBegin = 0;
                WayPoint* waypointEnd = 0;
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    if ( waypointEnd == 0 )
                        waypointEnd = waypoint;
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;
                    double delta =  calculateDistance(waypointBegin->lon, waypointBegin->lat, waypointEnd->lon, waypointEnd->lat);
                    if ( isnan(delta) )
                        delta=0;
                    totalDistance += delta;
                    //qDebug() << iloop <<" - Distance: " << d << " - Total: "<< totalDistance << "km";
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","delta",delta);
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","distance",totalDistance);
                }
            }
        }
        //mGPSData->unlockGPSData();
        mGPSData->setGPXModified(true);
    } //ServiceProcessWaypointDistance::run

    /*!
      \brief Calculate distance between 2 GPS lat/lon locations

    From http://jan.ucc.nau.edu/~cvm/latlon_formula.html
    Formula and code for calculating distance based on two lat/lon locations

    The following is the formula do the calculations. Angles are in radians.

    d = arccos(cos(lat1)*cos(lon1)*cos(lat2)*cos(lon2) + cos(lat1)*sin(lon1)*cos(lat2)*sin(lon2) + sin(lat1)*sin(lat2)) * r;

    Where:
        latitude and longitude are in radians
        r = radius of the earth in whatever units you want

        The values I use for radius of the earth are:

        3963.1 statute miles
        3443.9 nautical miles
        6378 km

    */
    double ServiceProcessWaypointDistance::calculateDistance(double lon1, double lat1, double lon2, double lat2)
    {
        long r = 6378; //meters
        const double PI = 3.14159265358979323846264338328;
        double rad_lon1 = lon1 * ( PI / 180 );
        double rad_lat1 = lat1 * ( PI / 180 );
        double rad_lon2 = lon2 * ( PI / 180 );
        double rad_lat2 = lat2 * ( PI / 180 );
        return acos(cos(rad_lat1)*cos(rad_lon1)*cos(rad_lat2)*cos(rad_lon2) + cos(rad_lat1)*sin(rad_lon1)*cos(rad_lat2)*sin(rad_lon2) + sin(rad_lat1)*sin(rad_lat2)) * r;
    } //ServiceProcessWaypointDistance::calculateDistance
}
