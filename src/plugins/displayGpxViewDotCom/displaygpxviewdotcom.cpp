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
#include <QtGui>
#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include "pixmap.h"
#include "displaygpxviewdotcom.h"

using namespace GPSBook;

namespace PluginDisplayGpxViewDotCom {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGpxViewDotCom::DisplayGpxViewDotCom()
    {
        //qDebug() << "DisplayGpxViewDotCom::DisplayGpxViewDotCom()";
        mOptions = new DisplayGpxViewDotComOptionsFrame();
        mHelp    = new DisplayGpxViewDotComHelp();
        mView    = new DisplayGpxViewDotComFrame();
        mView->pluginOwner = this;
        firstDisplay = true;
    } //DisplayGpxViewDotCom::DisplayGpxViewDotCom

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotCom::on_gpsdataChanged()
    {
        update();
    } //DisplayGpxViewDotCom::on_gpsdataChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayGpxViewDotCom::getName()
    {
        return tr("GPX-View.com");
    } //DisplayGpxViewDotCom::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGpxViewDotCom::getHelp()
    {
        return mHelp;
    } //DisplayGpxViewDotCom::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGpxViewDotCom::getOptions()
    {
        return mOptions;
    } //DisplayGpxViewDotCom::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotCom::init( QWidget* , GPSData* gpsdata )
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        connect((QObject*)gpsdata, SIGNAL(signalGPSDataUpdated()),
                (QObject*)this,    SLOT  (on_gpsdataChanged   ()));
        mView->init(gpsdata);
    } //DisplayGpxViewDotCom::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGpxViewDotCom::getWidget()
    {
        //qDebug() << "DisplayGpxViewDotCom::getWidget()";
        return mView;
    } //DisplayGpxViewDotCom::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayGpxViewDotCom::getIcon()
    {
        return QIcon(gpsview_xpm);
    } //DisplayGpxViewDotCom::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotCom::loadFile(QString filename)
    {
        emit signalLoadFile(filename,false);
    } //DisplayGpxViewDotCom::loadFile

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotCom::on_showPlugin()
    {
        if (firstDisplay)
        {
            mView->on_buttonRefresh_clicked(true);
            firstDisplay = false;
        }
        emit signalSetTrackSelection(false,false,false,false,false);
    } //DisplayGpxViewDotCom::on_showPlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotCom::on_fileLoaded()
    {
        emit signalSetTrackSelection(false,false,false,false,false);
    } //DisplayGpxViewDotCom::on_fileLoaded

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotCom::on_about()
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
                            + getName() + tr(" is a plugin allowing to use <a href=\"http://gpx-view.com/\">gpx-view</a> service.") + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2009" + "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //DisplayGpxViewDotCom::about

    Q_EXPORT_PLUGIN2(DisplayGpxViewDotCom, DisplayGpxViewDotCom)
}
