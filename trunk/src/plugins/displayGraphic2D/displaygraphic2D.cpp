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
#include <QtGui>
#include <QMessageBox>
#include <QWidget>
#include "pixmap.h"
#include "displaygraphic2D.h"
#include "plot.h"

namespace PluginDisplayGraphic2D {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGraphic2D::DisplayGraphic2D()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        mView    = new DisplayGraphic2DFrame();
        mOptions = new DisplayGraphic2DOptionsFrame();
        mHelp    = new DisplayGraphic2DHelp();

    } //DisplayGraphic2D::DisplayGraphic2D

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_gpsdataGPXChanged()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->getPlot()->rescale = true;
        mView->getGeoPlot()->rescale = true;
        mView->on_update();
        mView->getPlot()->replot();
        mView->getGeoPlot()->replot();
        mView->autoSetEnabled();
        //qDebug() << "<- DisplayGraphic2D::on_gpsdataChanged()";
    } //DisplayGraphic2D::on_gpsdataChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_gpsdataTracksChanged()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->getPlot()->rescale = false;
        mView->getGeoPlot()->rescale = false;
        mView->on_update();
        mView->getPlot()->replot();
        mView->getGeoPlot()->replot();
        mView->autoSetEnabled();
        //qDebug() << "<- DisplayGraphic2D::on_gpsdataChanged()";
    } //DisplayGraphic2D::on_gpsdataTracksChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_gpsdataRoutesChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        on_gpsdataTracksChanged();
    } // DisplayGraphic2D::on_gpsdataTracksChanged()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_selectionChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        on_gpsdataGPXChanged();
    } //DisplayGraphic2D::on_selectionChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_fileLoaded()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        on_showPlugin();
    } //DisplayGraphic2D::on_fileLoaded

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayGraphic2D::getName()
    {
        return tr("Graphic 2D");
    } //DisplayGraphic2D::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGraphic2D::getHelp()
    {
        return mHelp;
    } //DisplayGraphic2D::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGraphic2D::getOptions()
    {
        return mOptions;
    } //DisplayGraphic2D::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::init(QWidget* , GPSData* gpsdata)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mGPSData = gpsdata;
        mView->init(gpsdata);
        mView->autoSetEnabled();
    } //DisplayGraphic2D::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGraphic2D::getWidget()
    {
        return mView;
    } //DisplayGraphic2D::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayGraphic2D::getIcon()
    {
        return QIcon(displaygraphic2D_xpm);
    } //DisplayGraphic2D::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_about()
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
                            + getName() + tr(" is a plugin dedicated to display 2D graphs based on gpsdata information.") + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2009" + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "This plugin is using <a href=\"http://qwt.sourceforge.net/\">Qwt</a> libraries" + "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //DisplayGraphic2D::about

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic2D::on_showPlugin()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        on_gpsdataGPXChanged();
        bool status= ( !mGPSData->trackList.isEmpty() || !mGPSData->routeList.isEmpty() );
        emit signalSetTrackSelection(status,!status,status,status,status);
    } //DisplayGraphic2D::on_showPlugin

    Q_EXPORT_PLUGIN2(DisplayGraphic2D, DisplayGraphic2D)
}

