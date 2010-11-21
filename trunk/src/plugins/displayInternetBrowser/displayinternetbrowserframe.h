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
#ifndef DISPLAYINTERNETBROWSERFRAME_H
#define DISPLAYINTERNETBROWSERFRAME_H

#include <QtGui/QFrame>
#include <QUrl>
#include <QFile>
#include <QMenu>
#include <QThread>
#include <QWebFrame>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QTemporaryFile>
#include "interface.h"
#include "gpsdata.h"
#include "QProgressIndicator.h"
#include "cookiejar.h"

namespace Ui {
    class DisplayInternetBrowserFrame;
}

using namespace GPSBook;
using namespace ProgressIndicator;

namespace PluginDisplayInternetBrowser {

    class DisplayInternetBrowser;

    class DisplayInternetBrowserFrame : public QFrame{ //, public QThread {
        Q_OBJECT
        Q_DISABLE_COPY(DisplayInternetBrowserFrame)

    public:
        explicit DisplayInternetBrowserFrame(QWidget *parent = 0);
        virtual ~DisplayInternetBrowserFrame();
        void on_gpsdataChanged();
        void init(GPSData* gpsdata);
        void run() {}
        DisplayInternetBrowser* pluginOwner;

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DisplayInternetBrowserFrame *m_ui;
        GPSData* mGPSData;
        CookieJar* cookieJar;
        QString activeSite;
        QMenu* menu;
        QNetworkRequest request;
        QNetworkReply* reply;
        QNetworkProxy proxy;
        QTemporaryFile* tmpFile;
        QProgressIndicator* progressIndicator;

    public slots:
        void unsupportedContent ( QNetworkReply* reply );
        void refreshPage();

    private slots:
        void on_buttonGPSVizualizer_toggled(bool);
        void downloadFinished();
        void on_webView_linkClicked( QUrl url );
        void on_buttonGpxView_toggled(bool);
        void on_buttonVisuGpx_toggled(bool);
        void on_buttonGeoCaching_toggled(bool);

  };
}

#endif // DISPLAYINTERNETBROWSERFRAME_H
