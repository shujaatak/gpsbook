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
#include <QSettings>
#include <QDir>
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
        QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
        QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
        QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);

        QSettings settings("GPSBook","GPSBook");
        QString storagePath = settings.value("StorageLocation","").toString();
        QDir *dir = new QDir();
        if ( !dir->mkpath(storagePath+"webcache") )
        {
            QMessageBox::warning(0, qApp->applicationName(),qApp->tr("Cannot create webcache dir."));
        }
        cookieJar = new CookieJar(storagePath+"webcache/cookies.txt");

        m_ui->setupUi(this);

        QToolBar* toolbar = new QToolBar();
        m_ui->horizontalLayout->insertWidget(0,toolbar);

        progressIndicator = new QProgressIndicator();
        m_ui->horizontalLayout->addWidget(progressIndicator);

        m_ui->webView->page()->networkAccessManager()->setCookieJar(cookieJar);
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Back));
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Forward));
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Reload));
        toolbar->addAction(m_ui->webView->pageAction(QWebPage::Stop));
        connect(m_ui->webView, SIGNAL(loadStarted()),progressIndicator,SLOT(startAnimation()));
        connect(m_ui->webView, SIGNAL(loadFinished(bool)),progressIndicator,SLOT(stopAnimation()));

        tmpFile = new QTemporaryFile ();

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
        qDebug() << __FILE__ << __FUNCTION__;

        QString content = reply->rawHeader("Content-Disposition");
        QString filename = content.right(content.length()-content.indexOf("=")-1);

        if (QMessageBox::information(this,
                                     tr("Downloading Track"),
                                     qApp->applicationName()+" "+tr("is about to load the trace.") ,
                                     QMessageBox::Ok|QMessageBox::Cancel) != QMessageBox::Cancel)
        {
            qDebug() << __FILE__ << __FUNCTION__ << "Download file.";
            //Download file
            QByteArray data(reply->readAll());
            if (tmpFile->open()) {
                //Store data into a temporary file
                tmpFile->write(data);
                tmpFile->close();
                qDebug() <<  __FILE__ << __FUNCTION__ << "-> File saved:" << tmpFile->fileName();
                tmpFile->rename(QDir::tempPath() + QDir::separator() + "ibtmp_"+filename);
                // Send signal to main application to load file
                pluginOwner->loadFile(tmpFile->fileName());
                mGPSData->filename=tmpFile->fileName();
            }
            else {
                //ERROR
                QMessageBox::critical(this,
                                      tr("Downloading error"),
                                      qApp->applicationName()+" "+tr("was enabled to load the trace.") ,
                                      QMessageBox::Ok);
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
        qDebug() << __FILE__ << __FUNCTION__ << "(" << activeSite << ")";
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::refreshPage

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::downloadFinished()
    {
        unsupportedContent(reply);
    } //DisplayInternetBrowserFrame::downloadFinished

    /*------------------------------------------------------------------------------*
      http://www.gpx-view.com/gpx2gpx.php?f=Footing_28_12_2008.gpx
     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::on_webView_linkClicked(QUrl url)
    {
        qDebug() << __FILE__ << __FUNCTION__ << url.toString();
        if ( url.toString().contains("gpx2gpx.php")  || // gpx-view.com
             url.toString().contains("download.php") || // visugpx.com
             url.toString().contains("download")        // gpsvisualizer.com
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
        activeSite = "http://www.geocaching.com/seek/cache_details.aspx?guid=d10abb4d-55bd-4005-b630-bbc1958a80ed";
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::on_buttonVisuGpx_toggled


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInternetBrowserFrame::on_buttonGPSVizualizer_toggled(bool)
    {
        activeSite = "http://www.gpsvisualizer.com//";
        m_ui->webView->setUrl(QUrl(activeSite));
    } //DisplayInternetBrowserFrame::on_buttonGPSVizualizer_toggled

}



