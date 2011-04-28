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
#include "dialogprocessdistance.h"
#include "ui_dialogprocessdistance.h"
#include <QDebug>

namespace PluginDisplayStatistic {
    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessDistance::DialogProcessDistance(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DialogProcessDistance)
    {
        m_ui->setupUi(this);
    } //DialogProcessDistance::DialogProcessDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessDistance::~DialogProcessDistance()
    {
        delete m_ui;
    } //DialogProcessDistance::~DialogProcessDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessDistance::changeEvent(QEvent *e)
    {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DialogProcessDistance::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessDistance::init(GPSData* gpsdata)
    {
        this->mGPSData = gpsdata;
    } //DialogProcessDistance::init

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
    double DialogProcessDistance::calculateDistance(double lon1, double lat1, double lon2, double lat2)
    {
        //TODO: use the service instead of this (duplicated) code
        long r = 6378; //meters
        const double PI = 3.14159265358979323846264338328;
        double rad_lon1 = lon1 * ( PI / 180 );
        double rad_lat1 = lat1 * ( PI / 180 );
        double rad_lon2 = lon2 * ( PI / 180 );
        double rad_lat2 = lat2 * ( PI / 180 );
        return acos(cos(rad_lat1)*cos(rad_lon1)*cos(rad_lat2)*cos(rad_lon2) + cos(rad_lat1)*sin(rad_lon1)*cos(rad_lat2)*sin(rad_lon2) + sin(rad_lat1)*sin(rad_lat2)) * r;
    } //DialogProcessDistance::calculateDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessDistance::updateDistance()
    {
        //TODO: use the service instead of this (duplicated) code
        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track,  mGPSData->trackList )
        {
            foreach (TrackSeg* trackSeg, track->trackSegList )
            {
                double totalDistance = 0;
                int iloop = 1;
                WayPoint* waypointBegin = 0;
                WayPoint* waypointEnd = 0;
                m_ui->progressBar->setRange(1,trackSeg->wayPointList.count());
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    m_ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    if ( waypointEnd == 0 )
                        waypointEnd = waypoint;
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;
                    double delta =  calculateDistance(waypointBegin->lon, waypointBegin->lat, waypointEnd->lon, waypointEnd->lat);
                    if ( isnan(delta) )
                        delta=0;
                    totalDistance += delta;
                    //qDebug() << iloop <<" - Distance: " << d << " - Total: "<< totalDistance << "km";
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"gpsbook:WayPointExtension","gpsbook:delta",delta);
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"gpsbook:WayPointExtension","gpsbook:distance",totalDistance);
                }
            }
        }
        mGPSData->unlockGPSData();
        mGPSData->setGPXModified(true);
    } //DialogProcessDistance::updateDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessDistance::on_buttonBox_accepted()
    {
        updateDistance();
        close();
    } //DialogProcessDistance::on_buttonBox_accepted

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessDistance::on_buttonBox_rejected()
    {
        close();
    } //DialogProcessDistance::on_buttonBox_rejected
}
