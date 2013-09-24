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
#include <QtGlobal>
#include <QDebug>
#include <QtGui>
#include "displaystatisticframe.h"

#include "dialogprocessacceleration.h"
#include "dialogprocessaltitude.h"
#include "dialogprocessdistance.h"
#include "dialogprocessspeed.h"

#include "ui_displaystatisticframe.h"

namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayStatisticFrame::DisplayStatisticFrame(QWidget *parent) :
        QFrame(parent),
        m_ui(new Ui::DisplayStatisticFrame)
    {
        m_ui->setupUi(this);
#if not defined(Q_OS_LINUX)
        m_ui->toolButtonAltitude->setDisabled(true);
#endif
     } //DisplayStatisticFrame::DisplayStatisticFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayStatisticFrame::~DisplayStatisticFrame()
    {
        delete m_ui;
    } //DisplayStatisticFrame::~DisplayStatisticFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DisplayStatisticFrame::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::maximumAltitudeChanged(double altitude, double longitude, double latitude)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        QString direction = "";
        m_ui->labelAltitudeMaximumValue->setText(QString::number(altitude)+"m");
        double d_tmp = longitude;
        if ( d_tmp > 0 )
            direction = tr("E");
        else
            direction = tr("W");
        m_ui->labelAltitudeMaximumLongitudeValue->setText(QString::number(d_tmp) + direction);
        d_tmp = latitude;
        if ( d_tmp > 0 )
            direction = tr("N");
        else
            direction = tr("S");
        m_ui->labelAltitudeMaximumLatitudeValue->setText(QString::number(d_tmp) + direction);
    } //DisplayStatisticFrame::maximumAltitudeChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::minimumAltitudeChanged(double altitude, double longitude, double latitude)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        QString direction = "";
        m_ui->labelAltitudeMinimumValue->setText(QString::number(altitude)+"m");
        double d_tmp = longitude;
        if ( d_tmp > 0 )
            direction = tr("E");
        else
            direction = tr("W");
        m_ui->labelAltitudeMinimumLongitudeValue->setText(QString::number(d_tmp) + direction);
        d_tmp = latitude;
        if ( d_tmp > 0 )
            direction = tr("N");
        else
            direction = tr("S");
        m_ui->labelAltitudeMinimumLatitudeValue->setText(QString::number(d_tmp) + direction);
    } //DisplayStatisticFrame::minimumAltitudeChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::averageSpeedChanged(double speed)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->labelSpeedAverageValue->setText(QString::number(speed)+"km/h");
    } //DisplayStatisticFrame::averageSpeedChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::maximumSpeedChanged(double speed, double longitude, double latitude)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        QString direction = "";
        m_ui->labelSpeedMaximumValue->setText(QString::number(speed)+"km/h");
        double d_tmp = longitude;
        if ( d_tmp > 0 )
            direction = tr("E");
        else
            direction = tr("W");
        m_ui->labelSpeedMaximumLongitudeValue->setText(QString::number(d_tmp) + direction);
        d_tmp = latitude;
        if ( d_tmp > 0 )
            direction = tr("N");
        else
            direction = tr("S");
        m_ui->labelSpeedMaximumLatitudeValue->setText(QString::number(d_tmp) + direction);
    } //DisplayStatisticFrame::maximumSpeedChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::init(GPSData* mGPSData)
    {
        this->mGPSData = mGPSData;
    } //DisplayStatisticFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::setTotalDistance(QString value)
    {
        m_ui->labelTotalDistanceValue->setText(value);
    } //DisplayStatisticFrame::setTotalDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::setNumberOfPoints(QString value)
    {
        m_ui->labelNumberOfPointValue->setText(value);
    } //DisplayStatisticFrame::setNumberOfPoints

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::setTotalDuration(QString value)
    {
        m_ui->labelDurationValue->setText(value);
    } //DisplayStatisticFrame::setTotalDuration

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::setElevationGain(QString value)
    {
        m_ui->labelElevationGainTotalValue->setText(value);
    } //DisplayStatisticFrame::setElevationGain

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::setElevationDrop(QString value)
    {
        m_ui->labelElevationDropTotalValue->setText(value);
    } //DisplayStatisticFrame::setElevationDrop

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::autoSetEnabled()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        bool enabled = (!mGPSData->trackList.isEmpty());
        if (!enabled)
        {
            m_ui->labelAltitudeMaximumValue->setText("");
            m_ui->labelAltitudeMaximumLongitudeValue->setText("");
            m_ui->labelAltitudeMaximumLatitudeValue->setText("");
            m_ui->labelAltitudeMinimumValue->setText("");
            m_ui->labelAltitudeMinimumLongitudeValue->setText("");
            m_ui->labelAltitudeMinimumLatitudeValue->setText("");
            m_ui->labelSpeedMaximumValue->setText("");
            m_ui->labelSpeedAverageValue->setText("");
            m_ui->labelSpeedMaximumLatitudeValue->setText("");
            m_ui->labelSpeedMaximumLongitudeValue->setText("");
            m_ui->labelElevationGainTotalValue->setText("");
            m_ui->labelElevationDropTotalValue->setText("");
            m_ui->labelElevationGainDistanceValue->setText("");
            m_ui->labelElevationDropDistanceValue->setText("");
            m_ui->labelElevationGainSpeedMaximumValue->setText("");
            m_ui->labelElevationGainSpeedAverageValue->setText("");
            m_ui->labelElevationGainLinearSpeedValue->setText("");
            m_ui->labelElevationDropSpeedAverageValue->setText("");
            m_ui->labelElevationDropSpeedMaximumValue->setText("");
            m_ui->labelElevationDropLinearSpeedValue->setText("");
            m_ui->labelElevationGainDurationValue->setText("");
            m_ui->labelElevationDropDurationValue->setText("");
            m_ui->labelTotalDistanceValue->setText("");
            m_ui->labelDurationValue->setText("");
            m_ui->labelNumberOfPointValue->setText("");
        }
        setEnabled(enabled);
    } //DisplayStatisticFrame::autoSetEnabled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::activeSegmentIdChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if (!mGPSData->trackList.isEmpty())
        {
            double distance      = 0;
            double points        = 0;
            int    duration      = 0;
            double elevationGain = 0;
            double elevationDrop = 0;

            mGPSData->lockGPSDataForRead();

            //qDebug() << "DisplayStatisticFrame:: " << __FUNCTION__ << "index: " << mGPSData->activeTrackIndex << "/" << mGPSData->activeSegmentIndex;
            //If information to display is related to all segments of the track
            if (mGPSData->displayedSegmentIndex < 0)
            {
                //Data do display are related to all segment of all tracks
                if (mGPSData->displayedTrackIndex < 0)
                {
                    foreach(Track* track, mGPSData->trackList)
                    {
                        foreach(TrackSeg* trackSeg, track->trackSegList)
                        {
                            distance      += getDistance(trackSeg);
                            duration      += getDuration(trackSeg);
                            points        += trackSeg->wayPointList.count();
                            elevationGain += getElevationGain(trackSeg);
                            elevationDrop += getElevationDrop(trackSeg);
                        }
                    }
                }
                //Data to display is related to one track
                else
                {
                    foreach(TrackSeg* trackSeg, mGPSData->trackList[mGPSData->displayedTrackIndex]->trackSegList)
                    {
                        distance += getDistance(trackSeg);
                        duration += getDuration(trackSeg);
                        points   += trackSeg->wayPointList.count();
                        elevationGain += getElevationGain(trackSeg);
                        elevationDrop += getElevationDrop(trackSeg);
                    }

                }
            }
            //If information to display is related to one specific segment
            else
            {
                TrackSeg* trackSeg = mGPSData->trackList[mGPSData->displayedTrackIndex]->trackSegList[mGPSData->displayedSegmentIndex];
                distance = getDistance(trackSeg);
                duration = getDuration(trackSeg);
                points   = trackSeg->wayPointList.count();
                elevationGain = getElevationGain(trackSeg);
                elevationDrop = getElevationDrop(trackSeg);
            }
            setTotalDistance(QString::number(distance)+tr("km"));
            QDateTime dateTime = QDateTime::fromTime_t(duration).toUTC();
            QString durationText = dateTime.toString("hh:mm:ss");
            setTotalDuration(durationText);
            setNumberOfPoints(QString::number(points));
            setElevationGain(QString::number(elevationGain));
            setElevationDrop(QString::number(elevationDrop));

            mGPSData->unlockGPSData();
        }
        //qDebug() << "on_comboBoxSegments_currentIndexChanged(int index)";
    } //DisplayStatisticFrame::activeSegmentIdChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    int DisplayStatisticFrame::getDuration(TrackSeg* trackSeg)
    {
        return trackSeg->wayPointList.last()->time.toTime_t() - trackSeg->wayPointList.first()->time.toTime_t();
    } //DisplayStatisticFrame::getDuration

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    double DisplayStatisticFrame::getDistance(TrackSeg* trackSeg)
    {
        return mGPSData->getExtensionData(trackSeg->wayPointList.last()->extensions,"gpsbook:WayPointExtension","gpsbook:distance").toDouble();
    } //DisplayStatisticFrame::getDistance

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    double DisplayStatisticFrame::getElevationGain(TrackSeg* trackSeg)
    {
        return mGPSData->getExtensionData(trackSeg->extensions,"gpsbook:SegmentExtension","gpsbook:elevationGain").toDouble();
    } //DisplayStatisticFrame::getElevationGain

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    double DisplayStatisticFrame::getElevationDrop(TrackSeg* trackSeg)
    {
        return mGPSData->getExtensionData(trackSeg->extensions,"gpsbook:SegmentExtension","gpsbook:elevationDrop").toDouble();
    } //DisplayStatisticFrame::getElevationDrop


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::on_toolButtonDistance_clicked()
    {
        DialogProcessDistance* dialogProcessDistance = new DialogProcessDistance();
        dialogProcessDistance->init(mGPSData);
        dialogProcessDistance->show();
        //Call on_selectionChanged to update displayed data
        owner->on_selectionChanged();
    } //DisplayStatisticFrame::on_toolButtonDistance_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::on_toolButtonSpeed_clicked()
    {
        DialogProcessSpeed* dialogProcessSpeed = new DialogProcessSpeed();
        dialogProcessSpeed->init(mGPSData);
        dialogProcessSpeed->show();
        //Call on_selectionChanged to update displayed data
        owner->on_selectionChanged();
    } //DisplayStatisticFrame::on_toolButtonSpeed_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::on_toolButtonAcceleration_clicked()
    {
        DialogProcessAcceleration* dialogProcessAcceleration = new DialogProcessAcceleration();
        dialogProcessAcceleration->init(mGPSData);
        dialogProcessAcceleration->show();
        //Call on_selectionChanged to update displayed data
        owner->on_selectionChanged();

    } //DisplayStatisticFrame::on_toolButtonAcceleration_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticFrame::on_toolButtonAltitude_clicked()
    {
        DialogProcessAltitude* dialogProcessAltitude= new DialogProcessAltitude();
        dialogProcessAltitude->init(mGPSData);
        dialogProcessAltitude->show();
        //Call on_selectionChanged to update displayed data
        owner->on_selectionChanged();
    }//DisplayStatisticFrame::on_toolButtonAltitude_clicked
}
