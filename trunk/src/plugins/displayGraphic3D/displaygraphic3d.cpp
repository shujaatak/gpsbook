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
#include <QWidget>
#include "pixmap.h"
#include "displaygraphic3d.h"

namespace PluginDisplayGraphic3D {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGraphic3D::DisplayGraphic3D()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView    = new DisplayGraphic3DFrame();
        mOptions = new DisplayGraphic3DOptionsFrame();
        mHelp    = new DisplayGraphic3DHelp();
    } //DisplayGraphic3D::DisplayGraphic3D

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3D::on_gpsdataGPXChanged()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->update();
    } //DisplayGraphic3D::on_gpsdataChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayGraphic3D::getName()
    {
        return tr("Graphic 3D");
    } //DisplayGraphic3D::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGraphic3D::getHelp()
    {
        return mHelp;
    } //DisplayGraphic3D::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGraphic3D::getOptions()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        return mOptions;
    } //DisplayGraphic3D::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3D::init(QWidget* , GPSData* gpsdata)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mGPSData= gpsdata;
        mView->init(gpsdata);
    } //DisplayGraphic3D::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayGraphic3D::getWidget()
    {
        //qDebug() << "DisplayGraphic3D::" << __FUNCTION__;
        return mView;
    } //DisplayGraphic3D::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayGraphic3D::getIcon()
    {
        //qDebug() << "DisplayGraphic3D::" << __FUNCTION__;
        return QIcon(graph3d_xpm);
    } //DisplayGraphic3D::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3D::on_about()
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
                            + getName() + tr(" is a plugin to be developped.") + "<br><br>" +
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
    } //DisplayGraphic3D::on_about


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3D::on_selectionChanged()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->update();
    } //DisplayGraphic3D::on_selectionChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3D::on_fileLoaded()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        on_showPlugin();
    } //DisplayGraphic3D::on_fileLoaded

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGraphic3D::on_showPlugin()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mView->update();
        bool status=( !mGPSData->trackList.isEmpty() || !mGPSData->routeList.isEmpty() );
        emit signalSetTrackSelection(status,!status,status,status,status);
    } //DisplayGraphic3D::on_showPlugin


    Q_EXPORT_PLUGIN2(DisplayGraphic3D, DisplayGraphic3D)

}
