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
#include <QToolBar>
#include "displayinternetbrowserframe.h"
#include "ui_displayinternetbrowserframe.h"
#include "interface.h"
#include "displayinternetbrowser.h"

using namespace GPSBook;

namespace PluginDisplayInternetBrowser {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayInternetBrowserFrame::DisplayInternetBrowserFrame(QWidget *parent) :
    QFrame(parent), m_ui(new Ui::DisplayInternetBrowserFrame)
    {
        m_ui->setupUi(this);

        QToolBar* toolbar = new QToolBar();
        m_ui->horizontalLayout->insertWidget(0,toolbar);

        progressIndicator = new QProgressIndicator();
        m_ui->horizontalLayout->addWidget(progressIndicator);

        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Back));
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Forward));
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Reload));
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Stop));
        connect(m_ui->webView, SIGNAL(loadStarted()),progressIndicator,SLOT(startAnimation()));
        connect(m_ui->webView, SIGNAL(loadFinished(bool)),progressIndicator,SLOT(stopAnimation()));

        tmpFile = new QTemporaryFile (QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_gpx-view.gpx");

        activeSite = "qrc:///welcome.html";

        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName("127.0.0.1");
        proxy.setPort(8080);
        m_ui->webView->page()->networkAccessManager()->setProxy(proxy);
        m_ui->webView->page()->setForwardUnsupportedContent(true);
        connect(m_ui->webView->page(), SIGNAL(unsupportedContent(QNetworkReply*)),
                this, SLOT(unsupportedContent(QNetworkReply*)));
        m_ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);


    } //DisplayInternetBrowserFrame::DisplayInternetBrowserFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayInternetBrowserFrame::~DisplayInternetBrowserFrame()
    {
        if (tmpFile->exists()) {
            tmpFile->remove();
        }
        delete m_ui;
    } //DisplayInternetBrowserFrame::~DisplayInternetBrowserFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }//DisplayInternetBrowserFrame::changeEvent

    /*------------------------------------------------------------------------------*
      Action to perform when file is about to be downloaded
     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::unsupportedContent ( QNetworkReply * reply )
    {
        QString content = reply->rawHeader("Content-Disposition");
        if ( content.contains("gpx"))
        {
            int ret = QMessageBox::Cancel;
            if (mGPSData->trackList.isEmpty() &&
                mGPSData->routeList.isEmpty() &&
                mGPSData->wayPointList.isEmpty() ) {

                ret = QMessageBox::information(this,
                                             tr("Downloading Track"),
                                             qApp->applicationName()+" "+tr("is about to load the trace.") ,
                                             QMessageBox::Ok|QMessageBox::Cancel);
            }
            else {
                ret = QMessageBox::information(this,
                                               tr("Downloading Track and append to the current track"),
                                               tr("Do you want to append the loaded data to the current track ?") ,
                                               QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            }
            if (ret != QMessageBox::Cancel) {
                //Download file
                QByteArray data(reply->readAll());
                if (tmpFile->open()) {
                    //Store data into a temporary file
                    tmpFile->write(data);
                    tmpFile->close();
                    //qDebug() <<  "DisplayInternetBrowserFrame" << "::" << __FUNCTION__ << "-> File saved:" << fileName;
                }
                else {
                    //ERROR
                    QMessageBox::critical(this,
                                          tr("Downloading error"),
                                          qApp->applicationName()+" "+tr("was enabled to load the trace.") ,
                                          QMessageBox::Ok);
                }
            }

            switch (ret)
            {
                case QMessageBox::Yes :
                    //TODO: if a file is already loaded, ask if info dowloaded should be appended to the current trace
                break;
                case QMessageBox::Ok :
                case QMessageBox::No :
                        // Send signal to main application to load file
                        pluginOwner->loadFile(tmpFile->fileName());
                        //QString filename = reply->rawHeader("Content-Disposition");
                        //filename = filename.right(filename.length()-filename.indexOf("=")-1);
                        mGPSData->filename=tmpFile->fileName();
                break;
            }
        }
    } //DisplayInternetBrowserFrame::unsupportedContent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::init(GPSData* gpsdata)
    {
        mGPSData = gpsdata;
    } //DisplayInternetBrowserFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::refreshPage()
    {
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::on_commandLinkButton_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::downloadFinished()
    {
        unsupportedContent(reply);
    } //DisplayInternetBrowserFrame::downloadFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::on_webView_linkClicked(QUrl url)
    {
        if ( url.toString().contains("gpx2gpx.php")  || // gpx-view.com
             url.toString().contains("download.php")    // visugpx.com
            )
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
    } //DisplayInternetBrowserFrame::on_webView_linkClicked


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::on_buttonGpxView_toggled(bool)
    {
        activeSite = "http://www.gpx-view.com/gpxMap.php";
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::on_buttonGpxView_toggled


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::on_buttonVisuGpx_toggled(bool)
    {
        activeSite = "http://www.visugpx.com/recherche.php5";
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::on_buttonVisuGpx_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::on_buttonGeoCaching_toggled(bool)
    {
        activeSite = "http://www.geocaching.com/";
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::on_buttonVisuGpx_toggled

}

