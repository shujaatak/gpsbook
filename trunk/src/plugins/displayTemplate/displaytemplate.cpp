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
#include "displaytemplate.h"

namespace PluginDisplayTemplate {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayTemplate::DisplayTemplate()
    {
        mView    = new DisplayTemplateFrame();
        mHelp    = new DisplayTemplateHelp();
        mOptions = new DisplayTemplateOptionsFrame();
    } //DisplayTemplate::DisplayTemplate

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DisplayTemplate::getName()
    {
        return tr("Template");
    } //DisplayTemplate::getName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayTemplate::getHelp()
    {
        return mHelp;
    } //DisplayTemplate::getHelp

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayTemplate::getOptions()
    {
        return mOptions;
    } //DisplayTemplate::getOptions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QWidget* DisplayTemplate::getWidget()
    {
        return mView;
    } //DisplayTemplate::getWidget

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QIcon DisplayTemplate::getIcon()
    {
        return QIcon(template_xpm);
    } //DisplayTemplate::getIcon

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayTemplate::on_about()
    {
        QWidget* tmp = new QWidget();
        tmp->setWindowIcon(getIcon());
        QString about = getName() + tr(" describe plugin here.");
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
                            "gpsbook-team &copy;2009"+ "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"
                    );
    } //DisplayTemplate::on_about

    Q_EXPORT_PLUGIN2(DisplayTemplate, DisplayTemplate)
}
