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

#include "QDebug"
#include "displaygraphic2Dframe.h"
#include "ui_displaygraphic2Dframe.h"
#include "gpsdata.h"

namespace PluginDisplayGraphic2D {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGraphic2DFrame::DisplayGraphic2DFrame(QWidget *parent) :
        QFrame(parent),
        m_ui(new Ui::DisplayGraphic2DFrame)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->setupUi(this);

        //
        geoPlot = new Plot(this);
        QSizePolicy geoPlotSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        geoPlotSizePolicy.setHorizontalStretch(0);
        geoPlotSizePolicy.setVerticalStretch(0);
        geoPlotSizePolicy.setHeightForWidth(geoPlot->sizePolicy().hasHeightForWidth());
        geoPlot->setSizePolicy(geoPlotSizePolicy);
        m_ui->frameGeoPlot->layout()->addWidget(geoPlot);

        //
        plot = new Plot(this);
        QSizePolicy plotSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        plotSizePolicy.setHorizontalStretch(0);
        plotSizePolicy.setVerticalStretch(0);
        plotSizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(plotSizePolicy);
        m_ui->framePlot->layout()->addWidget(plot);

        connect(plot,SIGNAL(signalReplot()),geoPlot,SLOT(replot()));
        connect(geoPlot,SIGNAL(signalReplot()),plot,SLOT(replot()));

        //_________________________________________________________________________
        xAxis = Plot::axis_x_point;
        yAxis = Plot::axis_y_altitude;

        menuX = new QMenu();
        xActionGroup = new QActionGroup(this);
        xActionGroup->setExclusive(true);

        m_ui->actionXAbsoluteTime->setActionGroup(xActionGroup);
        menuX->addAction(m_ui->actionXAbsoluteTime);

        m_ui->actionXDistance->setActionGroup(xActionGroup);
        menuX->addAction(m_ui->actionXDistance);

        m_ui->actionXDuration->setActionGroup(xActionGroup);
        menuX->addAction(m_ui->actionXDuration);

        m_ui->actionXLongitude->setActionGroup(xActionGroup);
        menuX->addAction(m_ui->actionXLongitude);

        m_ui->actionXPoints->setActionGroup(xActionGroup);
        menuX->addAction(m_ui->actionXPoints);

        m_ui->toolButtonX->setMenu(menuX);


        menuY = new QMenu();
        yActionGroup = new QActionGroup(this);
        yActionGroup->setExclusive(true);

        m_ui->actionYAltitude->setActionGroup(yActionGroup);
        menuY->addAction(m_ui->actionYAltitude);

        m_ui->actionYDistance->setActionGroup(yActionGroup);
        menuY->addAction(m_ui->actionYDistance);

        m_ui->actionYSpeed->setActionGroup(yActionGroup);
        menuY->addAction(m_ui->actionYSpeed);

        m_ui->actionYAcceleration->setActionGroup(yActionGroup);
        menuY->addAction(m_ui->actionYAcceleration);

        m_ui->actionYLatitude->setActionGroup(yActionGroup);

        m_ui->toolButtonY->setMenu(menuY);

        menuEdition = new QMenu();
        menuEdition->addAction(m_ui->actionSplitSegment);
        m_ui->toolButtonEdit->setMenu(menuEdition);


    } //DisplayGraphic2DFrame::DisplayGraphic2DFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGraphic2DFrame::~DisplayGraphic2DFrame()
    {
        delete m_ui;
    } //DisplayGraphic2DFrame::~DisplayGraphic2DFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QLayout* DisplayGraphic2DFrame::getVerticalLayout()
    {
        return m_ui->verticalLayout;
    } //DisplayGraphic2DFrame::getVerticalLayout

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    Plot* DisplayGraphic2DFrame::getPlot()
    {
        return plot;
    } //DisplayGraphic2DFrame::getPlot

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    Plot* DisplayGraphic2DFrame::getGeoPlot()
    {
        return geoPlot;
    } //DisplayGraphic2DFrame::getGeoPlot

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DisplayGraphic2DFrame::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::init(GPSData* gpsdata)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        trackList = gpsdata;
        geoPlot->init(gpsdata);
        plot->init(gpsdata);
    } //DisplayGraphic2DFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::autoSetEnabled()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        bool enabled = ( !trackList->trackList.isEmpty() || !trackList->routeList.isEmpty() );
        setEnabled(enabled);
        plot->setDisplayCursorInfo(enabled);
        geoPlot->setDisplayCursorInfo(enabled);
    } //DisplayGraphic2DFrame::autoSetEnabled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionXAbsoluteTime_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        menuY->clear();
        menuY->addAction(m_ui->actionYAltitude);
        m_ui->actionYAltitude->setChecked(true);
        menuY->addAction(m_ui->actionYDistance);
        menuY->addAction(m_ui->actionYSpeed);
        menuY->addAction(m_ui->actionYAcceleration);
        m_ui->toolButtonX->setIcon(m_ui->actionXAbsoluteTime->icon());
        m_ui->toolButtonY->setIcon(m_ui->actionYAltitude->icon());
        xAxis = Plot::axis_x_absolute_time;
        yAxis = Plot::axis_y_altitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionXAbsoluteTime_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionXDistance_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        menuY->clear();
        menuY->addAction(m_ui->actionYAltitude);
        m_ui->actionYAltitude->setChecked(true);
        menuY->addAction(m_ui->actionYSpeed);
        menuY->addAction(m_ui->actionYAcceleration);
        m_ui->toolButtonX->setIcon(m_ui->actionXDistance->icon());
        m_ui->toolButtonY->setIcon(m_ui->actionYAltitude->icon());
        xAxis = Plot::axis_x_distance;
        yAxis = Plot::axis_y_altitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionXDistance_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionXDuration_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        menuY->clear();
        menuY->addAction(m_ui->actionYAltitude);
        m_ui->actionYAltitude->setChecked(true);
        menuY->addAction(m_ui->actionYDistance);
        menuY->addAction(m_ui->actionYSpeed);
        menuY->addAction(m_ui->actionYAcceleration);
        m_ui->toolButtonX->setIcon(m_ui->actionXDuration->icon());
        m_ui->toolButtonY->setIcon(m_ui->actionYAltitude->icon());
        xAxis = Plot::axis_x_duration;
        yAxis = Plot::axis_y_altitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionXDuration_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionXLongitude_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        menuY->clear();
        menuY->addAction(m_ui->actionYLatitude);
        m_ui->actionYLatitude->setChecked(true);
        m_ui->toolButtonX->setIcon(m_ui->actionXLongitude->icon());
        m_ui->toolButtonY->setIcon(m_ui->actionYLatitude->icon());
        xAxis = Plot::axis_x_longitude;
        yAxis = Plot::axis_y_latitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionXLongitude_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionXPoints_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        menuY->clear();
        menuY->addAction(m_ui->actionYAltitude);
        m_ui->actionYAltitude->setChecked(true);
        menuY->addAction(m_ui->actionYDistance);
        menuY->addAction(m_ui->actionYSpeed);
        menuY->addAction(m_ui->actionYAcceleration);
        m_ui->toolButtonX->setIcon(m_ui->actionXPoints->icon());
        m_ui->toolButtonY->setIcon(m_ui->actionYAltitude->icon());
        xAxis = Plot::axis_x_point;
        yAxis = Plot::axis_y_altitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionXPoints_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionYAcceleration_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->toolButtonY->setIcon(m_ui->actionYAcceleration->icon());
        yAxis = Plot::axis_y_acceleration;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionYAcceleration_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionYAltitude_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->toolButtonY->setIcon(m_ui->actionYAltitude->icon());
        yAxis = Plot::axis_y_altitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionYAltitude_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionYDistance_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->toolButtonY->setIcon(m_ui->actionYDistance->icon());
        yAxis = Plot::axis_y_distance;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionYDistance_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionYSpeed_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->toolButtonY->setIcon(m_ui->actionYSpeed->icon());
        yAxis = Plot::axis_y_speed;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionYSpeed_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionYLatitude_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->toolButtonY->setIcon(m_ui->actionYLatitude->icon());
        yAxis = Plot::axis_y_latitude;
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_actionYLatitude_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_toolButtonGeoPlot_toggled(bool checked)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        m_ui->frameGeoPlot->setVisible(checked);
    } //DisplayGraphic2DFrame::on_toolButtonGeoPlot_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_update()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        getGeoPlot()->updateTracklistCurve(trackList,Plot::axis_x_longitude,Plot::axis_y_latitude);
        plot->updateTracklistCurve(trackList,xAxis,yAxis);
    } //DisplayGraphic2DFrame::on_update

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2DFrame::on_actionSplitSegment_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        trackList->lockGPSDataForRead();
        TrackSeg* trackSeg = trackList->trackList[trackList->selectedTrackIndex]->trackSegList[trackList->selectedSegmentIndex];
        TrackSeg* newTrackSeg = new TrackSeg();
        trackList->trackList[trackList->selectedTrackIndex]->trackSegList << newTrackSeg;

        while ( trackList->selectedWaypointIndex < trackSeg->trackPointList.count() )
        {
            newTrackSeg->trackPointList << trackSeg->trackPointList.at(trackList->selectedWaypointIndex);
            trackSeg->trackPointList.removeAt(trackList->selectedWaypointIndex);
        }

        trackList->setModified(true);
        trackList->unlockGPSData();
    } //DisplayGraphic2DFrame::on_actionSplitSegment_triggered
}
