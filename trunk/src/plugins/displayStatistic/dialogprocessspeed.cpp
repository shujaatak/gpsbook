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
#include "dialogprocessspeed.h"
#include "ui_dialogprocessspeed.h"
#include <QDebug>

namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessSpeed::DialogProcessSpeed(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DialogProcessSpeed)
    {
        m_ui->setupUi(this);
    } //DialogProcessSpeed::DialogProcessSpeed

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessSpeed::~DialogProcessSpeed()
    {
        delete m_ui;
    } //DialogProcessSpeed::~DialogProcessSpeed

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessSpeed::changeEvent(QEvent *e)
    {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DialogProcessSpeed::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessSpeed::init(GPSData* gpsdata)
    {
        this->mGPSData = gpsdata;
    } //DialogProcessSpeed::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessSpeed::updateSpeed()
    {
        //TODO: use the service instead of this (duplicated) code
        WayPoint* waypointBegin = 0;
        WayPoint* waypointEnd = 0;
        double speed = 0;

        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track, mGPSData->trackList)
        {
            foreach(TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                m_ui->progressBar->setRange(1,trackSeg->wayPointList.count());
                mGPSData->setExtensionData(trackSeg->wayPointList[0]->extensions,"gpsbook:WayPointExtension","gpsbook:speed",0);
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    m_ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    if ( waypointBegin == 0 )
                        waypointEnd = waypoint;
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;
                    uint timeBegin = waypointBegin->time.toTime_t();
                    uint timeEnd = waypointEnd->time.toTime_t();
                    double distanceBegin = mGPSData->getExtensionData(waypointBegin->extensions,"gpsbook:WayPointExtension","gpsbook:distance").toDouble();
                    double distanceEnd = mGPSData->getExtensionData(waypointEnd->extensions,"gpsbook:WayPointExtension","gpsbook:distance").toDouble();
                    double distance = distanceEnd - distanceBegin;
                    uint time = timeEnd - timeBegin;
                    //qDebug() << "Distance=" << distance << "; time=" << time;
                    speed=0;
                    if ( time != 0 )
                    {
                        //speed in km/h
                        speed = ( 3600 * distance ) / time ;
                        //double delta = prevSpeed - speed;
                        //qDebug() << "Speed=" << speed;

                        //If speed difference between 2 point is too high, we filter it!
                        //if (abs(delta) < 33)
                        //{
                        //    mGPSData->setExtensionData(waypointEnd->extensions,"gpsbook:WayPointExtension","gpsbook:speed",speed);
                        //    prevSpeed = speed;
                        //}
                        //else
                        //    mGPSData->setExtensionData(waypointEnd->extensions,"gpsbook:WayPointExtension","gpsbook:speed",prevSpeed);
                    }
                    //else
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"gpsbook:WayPointExtension","gpsbook:speed",speed);
                }
            }
        }
        mGPSData->unlockGPSData();
        mGPSData->setGPXModified(true);
    } //DialogProcessSpeed::updateSpeed

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessSpeed::on_buttonBox_accepted()
    {
        updateSpeed();
        close();
    } //DialogProcessSpeed::on_buttonBox_accepted

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessSpeed::on_buttonBox_rejected()
    {
        close();
    } //DialogProcessSpeed::on_buttonBox_rejected
}
