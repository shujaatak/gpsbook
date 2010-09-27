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
#include <QWidget>
#include <QTextStream>
#include "displaygooglemapframe.h"
#include "ui_displaygooglemapframe.h"

using namespace GPSBook;

namespace PluginDisplayGoogleMap {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGoogleMapFrame::DisplayGoogleMapFrame(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::DisplayGoogleMapFrame)
    {
        QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
        tempPage = new QTemporaryFile(QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_googlemap.html");
        m_ui->setupUi(this);
        m_ui->webView->setPage(new CustomWebPage);
    } //DisplayGoogleMapFrame::DisplayGoogleMapFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGoogleMapFrame::~DisplayGoogleMapFrame()
    {
        tempPage->remove();
        delete m_ui;
    } //DisplayGoogleMapFrame::~DisplayGoogleMapFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMapFrame::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DisplayGoogleMapFrame::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMapFrame::init(GPSData* gpsdata)
    {
        trackList = gpsdata;
    } //DisplayGoogleMapFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMapFrame::run()
    {
        //Extract googlemap.html from resources parse it and store it into a temporary file
        if (tempPage->open()) {
            QByteArray content;
            QTextStream contentStream(&content,QIODevice::WriteOnly);
            QFile file(":/resources/index.html");
            if (file.open(QFile::ReadOnly))
            {
                 while (!file.atEnd()) {
                    //Insert the filename
                    QString line = file.readLine();
                    if (line.contains("{{GPXFILE}}"))
                    {
                        QString filename = trackList->filename;
#if defined(Q_OS_WIN)
                        //qDebug() << filename;
                        filename = filename.replace("\\","/");
                        filename = filename.replace("Documents and Settings","Docume~1");
                        filename = filename.right(filename.length()-filename.indexOf(":")-1);
                        //qDebug() << filename;
#endif
                        line.replace("{{GPXFILE}}",filename !="" ? filename : "nofileselected");
                    }
                    contentStream << line;
                }
                 file.close();
            }
            contentStream << endl << endl;

            //Save the temp file
            //qDebug() << content;
            tempPage->write(content);
            tempPage->close();

            //Load the page
            QString filename = tempPage->fileName();
#if defined(Q_OS_WIN)
            //qDebug() << filename;
            filename = filename.replace("\\","/");
            filename = filename.right(filename.length()-filename.indexOf(":")-1);
            //qDebug() << filename;
#endif
            m_ui->webView->load(QUrl("http://127.0.0.1:8080"+filename));
            //m_ui->webView->load(QUrl("http://earth-api-samples.googlecode.com/svn/trunk/demos/helloearth/index.html"));
        }
    } //DisplayGoogleMapFrame::run


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMapFrame::on_commandLinkButton_clicked()
    {
        m_ui->commandLinkButton->setText(tr("Refresh"));
        run();
    } //DisplayGoogleMapFrame::on_commandLinkButton_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGoogleMapFrame::on_webView_loadFinished(bool )
    {
        //qDebug() << __FUNCTION__ << success;
        //Page has been served, deleting temporary file.
        //tempPage->remove();
    } //DisplayGoogleMapFrame::on_webView_loadFinished
}
