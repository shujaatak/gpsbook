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
#include <QMessageBox>
#include <QNetworkReply>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include "interface.h"
#include "displaygpxviewdotcom.h"
#include "displaygpxviewdotcomframe.h"
#include "ui_displaygpxviewdotcomframe.h"

using namespace GPSBook;

namespace PluginDisplayGpxViewDotCom {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGpxViewDotComFrame::DisplayGpxViewDotComFrame(QWidget *parent) :
    QFrame(parent), m_ui(new Ui::DisplayGpxViewDotComFrame)
    {
        m_ui->setupUi(this);

        tmpFile = new QTemporaryFile (QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_gpx-view.gpx");

        m_ui->progressBar->hide();
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName("127.0.0.1");
        proxy.setPort(8080);
        m_ui->webView->page()->networkAccessManager()->setProxy(proxy);
        m_ui->webView->page()->setForwardUnsupportedContent(true);
        connect(m_ui->webView->page(), SIGNAL(unsupportedContent(QNetworkReply*)),
                this, SLOT(unsupportedContent(QNetworkReply*)));
        m_ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        menu = new QMenu;
        QAction* action1 = new QAction(this);
        action1->setText("gpx-view.com");
        action1->setData("http://www.gpx-view.com/gpxMap.php");
        activeSite = "http://www.gpx-view.com/gpxMap.php";
        action1->setCheckable(true);
        action1->setChecked(true);
        connect(action1,SIGNAL(triggered()),this,SLOT(siteSelection()));
        menu->addAction(action1);

        QAction* action2 = new QAction(this);
        action2->setText("skitour.fr");
        action2->setData("http://www.skitour.fr/gpx/recherche.php");
        action2->setCheckable(true);
        connect(action2,SIGNAL(triggered()),this,SLOT(siteSelection()));
        menu->addAction(action2);

    } //DisplayGpxViewDotComFrame::DisplayGpxViewDotComFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayGpxViewDotComFrame::~DisplayGpxViewDotComFrame()
    {
        tmpFile->remove();
        delete m_ui;
    } //DisplayGpxViewDotComFrame::~DisplayGpxViewDotComFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }//DisplayGpxViewDotComFrame::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::siteSelection()
    {
        foreach (QAction* menuItem, menu->actions())
            menuItem->setChecked(false);
        QAction *action = qobject_cast<QAction *>(sender());
        activeSite = action->data().toString();
        action->setChecked(true);
    } //DisplayGpxViewDotComFrame::siteSelection

    /*------------------------------------------------------------------------------*
      Action to perform when file is about to be downloaded
     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::unsupportedContent ( QNetworkReply * reply )
    {
        QString content = reply->rawHeader("Content-Disposition");
        if ( content.contains("gpx"))
        {
            //Ask confirmation of download
            if (QMessageBox::information(this,
                                         tr("Downloading Track"),
                                         qApp->applicationName()+" "+tr("is about to load the trace.") ,
                                         QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
            {
                //Download file
                QByteArray data(reply->readAll());

                if (tmpFile->open()) {
                    //Store data into a temporary file
                    QString fileName = tmpFile->fileName();
                    tmpFile->write(data);
                    tmpFile->close();
                    //qDebug() <<  "DisplayGpxViewDotComFrame" << "::" << __FUNCTION__ << "-> File saved:" << fileName;

                    // Check if type is supported

                    // Send signal to main application to load file
                    pluginOwner->loadFile(fileName);
                    //QString filename = reply->rawHeader("Content-Disposition");
                    //filename = filename.right(filename.length()-filename.indexOf("=")-1);
                    mGPSData->filename=fileName;

                    //Delete temporary file
                    //tmpFile.remove();
                }
            }
        }
        m_ui->webView->reload();
    } //DisplayGpxViewDotComFrame::unsupportedContent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::init(GPSData* gpsdata)
    {
        mGPSData = gpsdata;
    } //DisplayGpxViewDotComFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::on_commandLinkButton_clicked(bool )
    {
        m_ui->webView->setUrl(QUrl(activeSite));
        m_ui->commandLinkButton->setText(tr("Home"));
    } //DisplayGpxViewDotComFrame::on_commandLinkButton_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::on_commandLinkButton_customContextMenuRequested(QPoint pos)
    {
        //qDebug() << "DisplayGpxViewDotComFrame::" << __FUNCTION__;
        menu->exec(mapToGlobal(pos));
    } //DisplayGpxViewDotComFrame::on_commandLinkButton_customContextMenuRequested

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::downloadFinished()
    {
        unsupportedContent(reply);
    } //DisplayGpxViewDotComFrame::downloadFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayGpxViewDotComFrame::on_webView_linkClicked(QUrl url)
    {
        if (url.toString().contains("download"))
        {
            qDebug() << __FILE__ << __FUNCTION__ << "download(" << url << ")";
            request.setUrl(url);
            reply = m_ui->webView->page()->networkAccessManager()->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
        }
        else
        {
            qDebug() << __FILE__ << __FUNCTION__ << "followlink(" << url << ")";
            m_ui->webView->setUrl(url);
        }
    } //DisplayGpxViewDotComFrame::on_webView_linkClicked
}


