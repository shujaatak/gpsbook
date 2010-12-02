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
#include "displaygooglemap.h"

using namespace GPSBook;

namespace PluginDisplayGoogleMap {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGoogleMap::DisplayGoogleMap()
    {
        //qDebug() << "DisplayGoogleMap::DisplayGoogleMap()";
        mView    = new DisplayGoogleMapFrame();
        mHelp    = new DisplayGoogleMapHelp();
        mOptions = new DisplayGoogleMapOptionsFrame();
        firstDisplay = true;
    } //DisplayGoogleMap::DisplayGoogleMap

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayGoogleMap::getName()
    {
        return tr("Google Map");
    } //DisplayGoogleMap::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGoogleMap::getHelp()
    {
        return mHelp;
    } //DisplayGoogleMap::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGoogleMap::getOptions()
    {
        return mOptions;
    } //DisplayGoogleMap::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMap::init(QWidget* , GPSData* gpsdata)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->init(gpsdata);
    } //DisplayGoogleMap::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGoogleMap::getWidget()
    {
        return mView;
    } //DisplayGoogleMap::getWidge

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayGoogleMap::getIcon()
    {
        return QIcon(google_xpm);
    } //DisplayGoogleMap::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMap::on_about()
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
                            + getName() + tr(" is a plugin displaying GPS trace using <a href=\"http://maps.google.com/\">Google maps</a> service.") + "<br><br>" +
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
    } //DisplayGoogleMap::on_about

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMap::on_showPlugin()
    {
        if (firstDisplay)
        {
            mView->run();
            firstDisplay = false;
        }
        emit signalSetTrackSelection(false,false,false,false,false);
    } //DisplayGoogleMap::on_showPlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMap::on_fileLoaded()
    {
        emit signalSetTrackSelection(false,false,false,false,false);
    } //DisplayGoogleMap::on_fileLoaded

    Q_EXPORT_PLUGIN2(DisplayGoogleMap, DisplayGoogleMap)

}



