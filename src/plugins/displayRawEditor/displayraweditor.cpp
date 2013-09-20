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
#include <QDebug>
#include <QSettings>
#include "pixmap.h"
#include "displayraweditor.h"

namespace PluginDisplayRawEditor {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayRawEditor::DisplayRawEditor()
    {
        mView    = new DisplayRawEditorFrame();
        mHelp    = new DisplayRawEditorHelp();
        mOptions = new DisplayRawEditorOptionsFrame();
    } //DisplayRawEditor::DisplayRawEditor

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayRawEditor::getName()
    {
        return tr("Raw editor");
    } //DisplayRawEditor::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayRawEditor::getHelp()
    {
        return mHelp;
    } //DisplayRawEditor::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayRawEditor::getOptions()
    {
        return mOptions;
    } //DisplayRawEditor::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayRawEditor::getWidget()
    {
        return mView;
    } //DisplayRawEditor::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayRawEditor::getIcon()
    {
        return QIcon(raweditor_xpm);
    } //DisplayRawEditor::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayRawEditor::init(QWidget*, GPSData* gpsdata)
    {
        mGPSData = gpsdata;
        mView->init(gpsdata);
    }// DisplayRawEditor::init(QWidget*, GPSData* )

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayRawEditor::on_fileLoaded()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mView->loadfile(mGPSData->filename);
    }// DisplayRawEditor::on_fileLoaded()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayRawEditor::on_showPlugin()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mView->loadfile(mGPSData->filename);
        QSettings settings("GPSBook","GPSBook");
        emit signalShowCatalog(settings.value("PluginRawEditorShowCatalog",false).toBool());
    }// DisplayRawEditor::on_showPlugin()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayRawEditor::on_about()
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
                            "gpsbook-team &copy;2013" + "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //DisplayRawEditor::on_about

    Q_EXPORT_PLUGIN2(DisplayRawEditor, DisplayRawEditor)
}
