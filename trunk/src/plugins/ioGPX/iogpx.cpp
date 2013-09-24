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
#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>
#else
#include <QMessageBox>
#include <QLabel>
#endif
#include <QIcon>

#include "iogpx.h"

namespace PluginIOGPX {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ioGPX::ioGPX()
    {
        help = new IoGPXHelp();
        mOptions = new IoGPXOptionsFrame();
        mServicesList << "saveGPX" << "loadGPX";
    } //ioGPX::ioGPX

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString ioGPX::getName()
    {
        return tr("Load and save GPX");
    } //ioGPX::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon ioGPX::getIcon()
    {
        return QIcon(":/resources/gpx.xpm");
    } //ioGPX::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* ioGPX::getHelp()
    {
        return help;
    } // ioGPX::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* ioGPX::getOptions()
    {
        return mOptions;
    } // ioGPX::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString ioGPX::getOpenFilter()
    {
        return tr("GPX file ")+"(*.gpx)";
    } //ioGPX::getOpenFilter

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString ioGPX::getSaveFilter()
    {
        return tr("GPX file ")+"(*.gpx)";
    } //ioGPX::getSaveFilter

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioGPX::on_about()
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
                            + getName() + tr(" is a plugin dedicated to load and save <a href=\"http://www.topografix.com/gpx.asp\">GPX</a> files. This plugin is mandatory for application GPX file is the format of files use in the catalog.") + "<br><br>" +
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
    } //ioGPX::on_about

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioGPX::init( QWidget*, GPSData* )
    {
        gpxreader = new GpxReader();
        gpxwriter = new GpxWriter();
    } //ioGPX::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioGPX::open(QString filename, GPSData* gpsdata)
    {
        //qDebug()<< "GPX - Opening file: " << filename;
        gpxreader->load(gpsdata, filename);
    } //ioGPX::open

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioGPX::save(QString filename, GPSData* gpsdata)
    {
        //qDebug()<< "GPX - Saving file: " << filename;
        gpxwriter->save(gpsdata, filename);
    } //ioGPX::save

#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#else
     Q_EXPORT_PLUGIN2(ioGPX, ioGPX)
#endif
}
