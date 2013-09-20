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
#include <QDebug>
#include <QtGui>
#include <QObject>
#include <QMessageBox>
#include <QWidget>
#include <QSettings>
#include "pixmap.h"
#include "displaystatistic.h"

namespace PluginDisplayStatistic {
    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayStatistic::DisplayStatistic()
    {
        serviceProcessMinMaxAltitude        = new ServiceProcessMinMaxAltitude();
        serviceProcessMinMaxSpeed           = new ServiceProcessMinMaxSpeed();
        serviceProcessWaypointAcceleration  = new ServiceProcessWaypointAcceleration();
        serviceProcessWaypointDistance      = new ServiceProcessWaypointDistance();
        serviceProcessWaypointSpeed         = new ServiceProcessWaypointSpeed();
        displayStatisticOptionsFrame        = new DisplayStatisticOptionsFrame();
        displayStatisticFrame               = new DisplayStatisticFrame();
        mHelp                               = new DisplayStatisticHelp();
        displayStatisticFrame->owner  = this;
        processing = false;
    } //DisplayStatistic::DisplayStatistic

    /*------------------------------------------------------------------------------*
      TODO a revoir
     *------------------------------------------------------------------------------*/
    void DisplayStatistic::on_gpsdataGPXChanged()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        if (!gpsdata->trackList.isEmpty())
        {
            serviceProcessMinMaxSpeed->run();
            serviceProcessMinMaxAltitude->run();
            displayStatisticFrame->activeSegmentIdChanged();
        }
        displayStatisticFrame->autoSetEnabled();

    } //DisplayStatistic::on_gpsdataChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatistic::on_fileLoaded()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        bool modified = false;
        if (!gpsdata->trackList.isEmpty())
        {
            gpsdata->blockSignals(true);

            QSettings settings("GPSBook","GPSBook");
            if (gpsdata->isFromCatalog) {
                if ( settings.value("PluginDisplayStatistic_CatalogUpdateDistance",false).toBool() ) {
                    serviceProcessWaypointDistance->run();
                    modified = true;
                }

                if ( settings.value("PluginDisplayStatistic_CatalogUpdateSpeed",true).toBool() ) {
                    serviceProcessWaypointSpeed->run();
                    modified = true;
                }

                if ( settings.value("PluginDisplayStatistic_CatalogUpdateAcceleration",false).toBool() ) {
                    serviceProcessWaypointAcceleration->run();
                    modified = true;
                }

            }
            else
            {
                if ( settings.value("PluginDisplayStatistic_OtherUpdateDistances",true).toBool() ) {
                    serviceProcessWaypointDistance->run();
                    modified = true;
                }

                if ( settings.value("PluginDisplayStatistic_OtherUpdateSpeed",true).toBool() ) {
                    serviceProcessWaypointSpeed->run();
                    modified = true;
                }

                if ( settings.value("PluginDisplayStatistic_OtherUpdateAcceleration",true).toBool() ) {
                    serviceProcessWaypointAcceleration->run();
                    modified = true;
                }

            }

            gpsdata->blockSignals(false);

            if ( modified) {
              gpsdata->setTracksModified(true);
            }
        }
        on_showPlugin();

    } //DisplayStatistic::on_fileLoaded

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayStatistic::getName()
    {
        return tr("Statistics");
    } //DisplayStatistic::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayStatistic::getIcon()
    {
        return QIcon(displaystatistic_xpm);
    } //DisplayStatistic::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayStatistic::getHelp()
    {
        return mHelp;
    } //DisplayStatistic::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayStatistic::getOptions()
    {
        return displayStatisticOptionsFrame;
    } //DisplayStatistic::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatistic::init(QWidget* , GPSData* gpsdata)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        connect((QObject*)serviceProcessMinMaxAltitude, SIGNAL(SignalMinimumAltitudeChanged(double, double, double)),
                (QObject*)displayStatisticFrame,        SLOT  (minimumAltitudeChanged(double, double, double)));
        connect((QObject*)serviceProcessMinMaxAltitude, SIGNAL(SignalMaximumAltitudeChanged(double, double, double)),
                (QObject*)displayStatisticFrame,        SLOT  (maximumAltitudeChanged(double, double, double)));
        connect((QObject*)serviceProcessMinMaxSpeed,    SIGNAL(SignalAverageSpeedChanged(double)),
                (QObject*)displayStatisticFrame,        SLOT  (averageSpeedChanged(double)));
        connect((QObject*)serviceProcessMinMaxSpeed,    SIGNAL(SignalMaximumSpeedChanged(double, double, double)),
                (QObject*)displayStatisticFrame,        SLOT  (maximumSpeedChanged(double, double, double)));
        serviceProcessMinMaxAltitude->init(gpsdata);
        serviceProcessMinMaxSpeed->init(gpsdata);
        serviceProcessWaypointAcceleration->init(gpsdata);
        serviceProcessWaypointDistance->init(gpsdata);
        serviceProcessWaypointSpeed->init(gpsdata);
        displayStatisticFrame->init(gpsdata);
        this->gpsdata = gpsdata;
        displayStatisticFrame->autoSetEnabled();
    } //DisplayStatistic::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayStatistic::getWidget()
    {
        return displayStatisticFrame;
    } //DisplayStatistic::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatistic::on_about()
    {
        QWidget* tmp = new QWidget();
        tmp->setWindowIcon(getIcon());
        QMessageBox::about(tmp,tr("About")+" "+getName(),
                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                    "<html>"
                        "<head>"
                            "<meta name=\"qrichtext\" content=\"1\" />"
                        "</head>"
                        "<body>"
                        "<p style=\" font-family:'Sans'; font-size:14pt; font-weight:400; font-style:vold; \">"
                            + getName() +
                            " v" + QString(BUILD_VERSION) + "." + QString(SVN_VERSION) + "  <sub>(" + QString(BUILD_DATE) + ")<sub>" +
                        "</p>"
                        "<p style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:vold; \">"
                            + getName() + tr(" is a plugin displaying statistics about gpsdata.") + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2013" + "<br><br>" +
                            tr("This plugin use code from ") + "<a href=\"http://jan.ucc.nau.edu/~cvm/latlon_formula.html\">Chris Michels</a>" + "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //DisplayStatistic::on_about

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatistic::on_showPlugin()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        bool status=!gpsdata->trackList.isEmpty();
        emit signalSetTrackSelection(status,!status,!status,status,status);
        QSettings settings("GPSBook","GPSBook");
        emit signalShowCatalog(settings.value("PluginStatisticShowCatalog",true).toBool());
        on_selectionChanged();
        displayStatisticFrame->autoSetEnabled();
    } //DisplayStatistic::on_showPlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatistic::on_selectionChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        displayStatisticFrame->activeSegmentIdChanged();
        //Todo load data from extension instead of recalculating it!!!
        serviceProcessMinMaxAltitude->run();
        serviceProcessMinMaxSpeed->run();
    } //DisplayStatistic::on_selectionChanged

    Q_EXPORT_PLUGIN2(DisplayStatistic, DisplayStatistic)
}
