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
#include <QObject>
#include <QMessageBox>
#include <QWidget>
#include "pixmap.h"
#include "displayinformation.h"

namespace PluginDisplayInformation {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayInformation::DisplayInformation()
    {
        mView    = new DisplayInformationFrame();
        mHelp    = new DisplayInformationHelp();
        mOptions = new DisplayInformationOptionsFrame();
    } //DisplayInformation::DisplayInformation

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::on_gpsdataGPXChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        mView->updateDisplay();
        mView->autoSetEnabled();
    } //DisplayInformation::on_gpsdataChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::on_fileLoaded()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        on_showPlugin();
    } //DisplayInformation::on_fileLoaded

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayInformation::getName()
    {
        return tr("Informations");
    } //DisplayInformation::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayInformation::getIcon()
    {
        return QIcon(displayinformation_xpm);
    } //DisplayInformation::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayInformation::getHelp()
    {
        return mHelp;
    } //DisplayInformation::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayInformation::getOptions()
    {
        return mOptions;
    } //DisplayInformation::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::init(QWidget* , GPSData* gpsdata)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        mGPSData = gpsdata;
        mView->init(gpsdata);
    } //DisplayInformation::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayInformation::getWidget()
    {
        return mView;
    } //DisplayInformation::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::on_about()
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
                            + getName() + tr(" is a plugin designed to display and edit metadata of GPX files.") + "<br><br>" +
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
    } //DisplayInformation::about

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::on_showPlugin()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        on_gpsdataGPXChanged();
        emit signalSetTrackSelection(true,true,true,true,false);
    } //DisplayInformation::on_showPlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::on_hidePlugin()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
    } //DisplayInformation::on_hidePlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformation::on_selectionChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        mView->updateDisplay();
    }


    Q_EXPORT_PLUGIN2(DisplayInformation, DisplayInformation)
}
