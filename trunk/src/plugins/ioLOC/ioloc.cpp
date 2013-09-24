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

#include "ioloc.h"

namespace PluginIOLOC {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ioLOC::ioLOC()
    {
        help = new IoLOCHelp();
        mOptions = new IoLOCOptionsFrame();
    } //ioLOC::ioLOC

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString ioLOC::getName()
    {
        return tr("Load LOC from geocaching.com");
    } //ioLOC::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon ioLOC::getIcon()
    {
        return QIcon(":/resources/loc.xpm");
    } //ioLOC::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* ioLOC::getHelp()
    {
        return help;
    } // ioLOC::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* ioLOC::getOptions()
    {
        return mOptions;
    } // ioLOC::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString ioLOC::getOpenFilter()
    {
        return tr("LOC file ")+"(*.loc)";
    } //ioLOC::getOpenFilter

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString ioLOC::getSaveFilter()
    {
        return "";
    } //ioLOC::getSaveFilter

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioLOC::on_about()
    {
        QWidget* tmp = new QWidget();
        tmp->setWindowIcon(getIcon());
        QString about = getName() + tr(" is a plugin dedicated to load and save loc file from <a href=\"http://www.geocaching.com/\">geocaching.com</a> files.");
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
    } //ioLOC::on_about

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioLOC::init( QWidget*, GPSData* )
    {
        locreader = new LocReader();
    } //ioLOC::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ioLOC::open(QString filename, GPSData* gpsdata)
    {
        //qDebug()<< "LOC - Opening file: " << filename;
        locreader->load(gpsdata, filename);
    } //ioLOC::open

#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#else
    Q_EXPORT_PLUGIN2(ioLOC, ioLOC)
#endif
}
