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
#include "dialogprocessacceleration.h"
#include "ui_dialogprocessacceleration.h"
#include <QDebug>

namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessAcceleration::DialogProcessAcceleration(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogProcessAcceleration)
    {
        ui->setupUi(this);
    } //DialogProcessAcceleration::DialogProcessAcceleration

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessAcceleration::~DialogProcessAcceleration()
    {
        delete ui;
    } //DialogProcessAcceleration::~DialogProcessAcceleration

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAcceleration::changeEvent(QEvent *e)
    {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DialogProcessAcceleration::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAcceleration::init(GPSData* gpsdata)
    {
        this->mGPSData = gpsdata;
    } //DialogProcessAcceleration::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAcceleration::updateAcceleration()
    {
        //TODO: use the service instead of this (duplicated) code
        WayPoint* waypointBegin = 0;
        WayPoint* waypointEnd = 0;
        double acceleration = 0;
        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track, mGPSData->trackList)
        {
            foreach(TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                ui->progressBar->setRange(1,trackSeg->wayPointList.count());
                mGPSData->setExtensionData(trackSeg->wayPointList[0]->extensions,"GPSBookWayPointExtension","acceleration",0);
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    if ( waypointBegin == 0 )
                        waypointEnd = waypoint;
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;
                    uint timeBegin = waypointBegin->time.toTime_t();
                    uint timeEnd = waypointEnd->time.toTime_t();
                    double speedBegin = mGPSData->getExtensionData(waypointBegin->extensions,"GPSBookWayPointExtension","speed").toDouble();
                    double speedEnd = mGPSData->getExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","speed").toDouble();
                    double speedDelta = speedEnd - speedBegin;
                    uint time = timeEnd - timeBegin;
                    if ( time != 0 )
                    {
                        //Acceleration in m/s²
                        acceleration = 3600 * speedDelta / 1000 / time;
                        //qDebug() << "acceleration=" << acceleration;

                    }
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","acceleration",acceleration);
                }
            }
        }
        mGPSData->unlockGPSData();
        mGPSData->setGPXModified(true);
    } //DialogProcessAcceleration::updateAcceleration

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAcceleration::on_buttonBox_accepted()
    {
        updateAcceleration();
        close();
    } //DialogProcessAcceleration::on_buttonBox_accepted

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAcceleration::on_buttonBox_rejected()
    {
        close();
    } //DialogProcessAcceleration::on_buttonBox_rejected
}
