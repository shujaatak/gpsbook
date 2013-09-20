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
#include <QSettings>
#include "pixmap.h"
#include "displayinternetbrowser.h"

using namespace GPSBook;

namespace PluginDisplayInternetBrowser {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayInternetBrowser::DisplayInternetBrowser()
    {
        //qDebug() << "DisplayInternetBrowser::DisplayInternetBrowser()";
        mOptions = new DisplayInternetBrowserOptionsFrame();
        mHelp    = new DisplayInternetBrowserHelp();
        mView    = new DisplayInternetBrowserFrame();
        mView->pluginOwner = this;
        firstDisplay = true;
    } //DisplayInternetBrowser::DisplayInternetBrowser

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayInternetBrowser::getName()
    {
        return tr("Internet Browser");
    } //DisplayInternetBrowser::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayInternetBrowser::getHelp()
    {
        return mHelp;
    } //DisplayInternetBrowser::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayInternetBrowser::getOptions()
    {
        return mOptions;
    } //DisplayInternetBrowser::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowser::init( QWidget* , GPSData* gpsdata )
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->init(gpsdata);
    } //DisplayInternetBrowser::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayInternetBrowser::getWidget()
    {
        //qDebug() << "DisplayInternetBrowser::getWidget()";
        return mView;
    } //DisplayInternetBrowser::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayInternetBrowser::getIcon()
    {
        return QIcon(gpsview_xpm);
    } //DisplayInternetBrowser::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowser::loadFile(QString filename)
    {
        emit signalLoadFile(filename,false);
    } //DisplayInternetBrowser::loadFile

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowser::on_showPlugin()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if (firstDisplay)
        {
            mView->refreshPage();
            firstDisplay = false;
        }
        emit signalSetTrackSelection(false,false,false,false,false);
        QSettings settings("GPSBook","GPSBook");
        emit signalShowCatalog(settings.value("PluginInternetBrowserShowCatalog",false).toBool());
    } //DisplayInternetBrowser::on_showPlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowser::on_fileLoaded()
    {
        emit signalSetTrackSelection(false,false,false,false,false);
    } //DisplayInternetBrowser::on_fileLoaded

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowser::on_about()
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
                            + getName() + tr(" is a plugin allowing to use on line services.") + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2013" + "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //DisplayInternetBrowser::about

    Q_EXPORT_PLUGIN2(DisplayInternetBrowser, DisplayInternetBrowser)
}
