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
** Copyright (c) 2009, gpsbook-team
**
****************************************************************************/
#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QDir>
#include <QIcon>
#include <QIconEngine>
#else
#include <QMessageBox>
#include <QWidget>
#include <QtGui>
#endif

#include <QObject>
#include <QSettings>
#include "displaysrtm.h"
#include "srtm.h"

namespace PluginDisplaySRTM {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplaySRTM::DisplaySRTM()
    {
        mView    = new DisplaySRTMFrame();
        mHelp    = new DisplaySRTMHelp();
        mOptions = new DisplaySRTMOptionsFrame();

        QSettings settings("GPSBook","GPSBook");
        QString storagePath = settings.value("StorageLocation","").toString();
        QDir* dir = new QDir();
        if (  !dir->exists(storagePath+"srtm")  )
        {
            if ( !dir->mkpath(storagePath+"srtm") )
            {
                QMessageBox::critical(0, qApp->applicationName(),qApp->tr("An error occurs during srtm directory creation.\n"
                                                                          "Altitude feature may not work properly."));
            }
        }


        /*SrtmDownloader* srtm = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/",
                                                  storagePath+"srtm");
        int alt = srtm->getAltitudeFromLatLon(45.226183,6.743932);
        qDebug() << "Altitude a Aussois :" << alt;
        alt = srtm->getAltitudeFromLatLon(45.189296,5.720444);
        qDebug() << "Altitude a Grenoble :" << alt;
        alt = srtm->getAltitudeFromLatLon(45.255314,5.73452);
        qDebug() << "Altitude a la bastille :" << alt;*/


    } //DisplaySRTM::DisplaySRTM

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplaySRTM::getName()
    {
        return tr("SRTM");
    } //DisplaySRTM::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplaySRTM::getHelp()
    {
        return mHelp;
    } //DisplaySRTM::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplaySRTM::getOptions()
    {
        return mOptions;
    } //DisplaySRTM::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplaySRTM::getWidget()
    {
        return NULL;
    } //DisplaySRTM::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplaySRTM::getIcon()
    {
        return QIcon(":/resources/srtm.xpm");
    } //DisplaySRTM::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplaySRTM::on_about()
    {
        QWidget* tmp = new QWidget();
        tmp->setWindowIcon(getIcon());
        QString about = getName() + tr(" as been developped on top of the SRTM code writen by Hermann Kraus.") + "<br>" +
                                    tr("Ref. SRTM_COPYING for the details of license related to this part of code.");

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
                            + about + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2013"+ "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );

    } //DisplaySRTM::on_about

#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#else
     Q_EXPORT_PLUGIN2(DisplaySRTM, DisplaySRTM)
#endif

}
