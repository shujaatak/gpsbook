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
** Copyright (C) 2010, gpsbook-team
**
****************************************************************************/
#include <QMessageBox>
#include "filtercleanup.h"
#include "pixmap.h"
#include "QDebug"

namespace PluginFilterCleanup {

    FilterCleanup::FilterCleanup() {
        mOptions = new FilterCleanupOptionsFrame();
        mHelp = new FilterCleanupHelp();
    } //FilterCleanup


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString FilterCleanup::getName()
    {
        return "Cleanup";
    } //FilterCleanup::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon FilterCleanup::getIcon()
    {
        return QIcon(filterCleanup_xpm);
    } //FilterCleanup::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* FilterCleanup::getHelp()
    {
        return mHelp;
    } //FilterCleanup::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* FilterCleanup::getOptions()
    {
        return mOptions;
    } //FilterCleanup::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanup::init( QWidget*, GPSData* gpsdata )
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        this->mGPSData = gpsdata;
    } //FilterCleanup::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanup::on_showPlugin()
    {
        FilterCleanupDialog* f = new FilterCleanupDialog();
        f->setWindowTitle(getName());
        f->mGPSData = this->mGPSData;
        f->show();
    } //FilterCleanup::on_showPlugin

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void FilterCleanup::on_about()
    {
        QWidget* tmp = new QWidget();
        tmp->setWindowIcon(getIcon());
        QMessageBox::about(tmp,tr("About")+" Cleanup",
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
                            + getName() + tr(" is a plugin dedicated filter gpsdata to cleanup traces.") + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2013"+ "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //FilterCleanup::on_about

    Q_EXPORT_PLUGIN2(FilterCleanup, FilterCleanup)
}
