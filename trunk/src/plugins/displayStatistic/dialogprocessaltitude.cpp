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
#include "dialogprocessaltitude.h"
#include "ui_dialogprocessaltitude.h"
#include <QDebug>
#include <QSettings>


namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessAltitude::DialogProcessAltitude(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogProcessAltitude)
    {
        ui->setupUi(this);
        QSettings settings("GPSBook","GPSBook");
        QString storagePath = settings.value("StorageLocation","").toString();
        srtmDownloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/",
                                            storagePath+"srtm");


    } //DialogProcessAltitude::DialogProcessAltitude

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogProcessAltitude::~DialogProcessAltitude()
    {
        delete ui;
    } //DialogProcessAltitude::~DialogProcessAltitude

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAltitude::changeEvent(QEvent *e)
    {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DialogProcessAltitude::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAltitude::init(GPSData* gpsdata)
    {
        this->mGPSData = gpsdata;
    } //DialogProcessAltitude::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAltitude::updateAltitude()
    {
        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track, mGPSData->trackList)
        {
            foreach(TrackSeg* trackSeg, track->trackSegList)
            {
                int iloop = 0;
                ui->progressBar->setRange(1,trackSeg->wayPointList.count());
                WayPoint* waypointBegin = 0;
                WayPoint* waypointEnd = 0;
                double elevationGain = 0;
                double elevationDrop = 0;
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    ui->progressBar->setValue(iloop++);
                    qApp->processEvents();
                    if ( waypointEnd == 0 )
                    {
                        waypointEnd = waypoint;
                        continue;
                    }
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;

                    waypoint->ele = srtmDownloader->getAltitudeFromLatLon(waypoint->lat,waypoint->lon);
                    /*double elevation = waypointEnd->ele - waypointBegin->ele;
                    if( elevation < 0 )
                    {
                       elevationDrop += -elevation;
                    }
                    else
                    {
                       elevationGain += elevation;
                    }
                    //qDebug() << "elevation=" << elevation << " -- elevationDrop=" << elevationDrop << " -- elevationGain=" << elevationGain;
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","elevation",elevation);*/
                }
                trackSeg->extensions = mGPSData->setExtensionData(trackSeg->extensions,"GPSBookSegmentExtension","elevationGain",elevationGain);
                trackSeg->extensions = mGPSData->setExtensionData(trackSeg->extensions,"GPSBookSegmentExtension","elevationDrop",elevationDrop);
            }
        }
        mGPSData->unlockGPSData();
        mGPSData->setGPXModified(true);
    } //DialogProcessAltitude::updateAltitude

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAltitude::on_buttonBox_accepted()
    {
        updateAltitude();
        close();
    } //DialogProcessAltitude::on_buttonBox_accepted

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogProcessAltitude::on_buttonBox_rejected()
    {
        close();
    } //DialogProcessAltitude::on_buttonBox_rejected
}
