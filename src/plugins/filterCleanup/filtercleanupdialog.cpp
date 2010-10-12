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
** Copyright (C) 2010, gpsbook-team
**
****************************************************************************/
#include "filtercleanupdialog.h"
#include "ui_filtercleanupdialog.h"
#include "QDebug"

namespace PluginFilterCleanup {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    FilterCleanupDialog::FilterCleanupDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::FilterCleanupDialog)
    {
        m_ui->setupUi(this);
    } //FilterCleanupDialog::FilterCleanupDialog

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    FilterCleanupDialog::~FilterCleanupDialog()
    {
        delete m_ui;
    } //FilterCleanupDialog::~FilterCleanupDialog

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanupDialog::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //FilterCleanupDialog::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanupDialog::on_toolButtonRemoveDesc_clicked()
    {
        //mGPSData->lockGPSDataForWrite();
        setEnabled(false);
        foreach (Track* track,mGPSData->trackList)
        {
            foreach (TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                m_ui->progressBar->setRange(0,trackSeg->wayPointList.count());
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    m_ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    waypoint->desc = "";
                }
            }
        }
        m_ui->progressBar->setValue(0);
        mGPSData->unlockGPSData();
        setEnabled(true);
        mGPSData->setModified(true);
    } //FilterCleanupDialog::on_toolButtonRemoveDesc_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanupDialog::on_toolButtonRemoveHighSpeed_clicked()
    {
        //mGPSData->lockGPSDataForWrite();
        setEnabled(false);
        foreach (Track* track,mGPSData->trackList)
        {
            foreach (TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                m_ui->progressBar->setRange(0,trackSeg->wayPointList.count());
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    m_ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    double speed = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","speed").toDouble();
                    if (speed > m_ui->doubleSpinBox->value())
                    {
                        trackSeg->wayPointList.removeAt(trackSeg->wayPointList.indexOf(waypoint,0));
                    }
                }
            }
        }
        m_ui->progressBar->setValue(0);
        mGPSData->unlockGPSData();
        setEnabled(true);
        mGPSData->setModified(true);
    } //FilterCleanupDialog::on_toolButtonRemoveHighSpeed_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanupDialog::on_toolButtonRemoveHighSpeedRemoveHighAcceleration_clicked()
    {
        //mGPSData->lockGPSDataForWrite();
        setEnabled(false);
        foreach (Track* track,mGPSData->trackList)
        {
            foreach (TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                m_ui->progressBar->setRange(0,trackSeg->wayPointList.count());
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    m_ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    double acceleration = mGPSData->getExtensionData(waypoint->extensions,"GPSBookWayPointExtension","acceleration").toDouble();
                    if (abs(acceleration) > m_ui->doubleSpinBoxAcceleration->value())
                    {
                        trackSeg->wayPointList.removeAt(trackSeg->wayPointList.indexOf(waypoint,0));
                    }
                }
            }
        }
        m_ui->progressBar->setValue(0);
        mGPSData->unlockGPSData();
        setEnabled(true);
        mGPSData->setModified(true);
    } //FilterCleanupDialog::on_toolButtonRemoveHighSpeedRemoveHighAcceleration_clicked

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
    double FilterCleanupDialog::calculateDistance(double lon1, double lat1, double lon2, double lat2)
    {
        long r = 6378; //meters
        const double PI = 3.14159265358979323846264338328;
        double rad_lon1 = lon1 * ( PI / 180 );
        double rad_lat1 = lat1 * ( PI / 180 );
        double rad_lon2 = lon2 * ( PI / 180 );
        double rad_lat2 = lat2 * ( PI / 180 );
        return acos(cos(rad_lat1)*cos(rad_lon1)*cos(rad_lat2)*cos(rad_lon2) + cos(rad_lat1)*sin(rad_lon1)*cos(rad_lat2)*sin(rad_lon2) + sin(rad_lat1)*sin(rad_lat2)) * r;
    } //FilterCleanupDialog::calculateDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanupDialog::on_toolButtonRemoveHighSpeedRemoveRadius_clicked()
    {
        WayPoint* waypointOrigin = 0;
        setEnabled(false);
        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track,mGPSData->trackList)
        {
            foreach (TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                m_ui->progressBar->setRange(0,trackSeg->wayPointList.count());
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    m_ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    if ( waypointOrigin==0 )
                    {
                        waypointOrigin=waypoint;
                        continue;
                    }
                    double delta = calculateDistance(waypointOrigin->lon,waypointOrigin->lat,waypoint->lon,waypoint->lat);
                    if (delta < (m_ui->doubleSpinBoxRadius->value()/1000))
                    {
                        trackSeg->wayPointList.removeAt(trackSeg->wayPointList.indexOf(waypoint,0));
                    }
                    else
                    {
                        waypoint->extensions = mGPSData->setExtensionData(waypoint->extensions,"GPSBookWayPointExtension","delta",delta);
                        waypointOrigin = waypoint;
                    }
                }
            }
        }
        m_ui->progressBar->setValue(0);
        mGPSData->unlockGPSData();
        setEnabled(true);
        mGPSData->setModified(true);
    } //FilterCleanupDialog::on_toolButtonRemoveHighSpeedRemoveRadius_clicked
}
