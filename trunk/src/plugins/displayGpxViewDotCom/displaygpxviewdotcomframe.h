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
#ifndef DISPLAYGPXVIEWDOTCOMFRAME_H
#define DISPLAYGPXVIEWDOTCOMFRAME_H

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

namespace Ui {
    class DisplayGpxViewDotComFrame;
}

using namespace GPSBook;
using namespace ProgressIndicator;

namespace PluginDisplayGpxViewDotCom {

    class DisplayGpxViewDotCom;

    class DisplayGpxViewDotComFrame : public QFrame{ //, public QThread {
        Q_OBJECT
        Q_DISABLE_COPY(DisplayGpxViewDotComFrame)

    public:
        explicit DisplayGpxViewDotComFrame(QWidget *parent = 0);
        virtual ~DisplayGpxViewDotComFrame();
        void on_gpsdataChanged();
        void init(GPSData* gpsdata);
        void run() {}
        DisplayGpxViewDotCom* pluginOwner;

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DisplayGpxViewDotComFrame *m_ui;
        GPSData* mGPSData;
        QString activeSite;
        QMenu* menu;
        QNetworkRequest request;
        QNetworkReply* reply;
        QNetworkProxy proxy;
        QTemporaryFile* tmpFile;
        QProgressIndicator* progressIndicator;

    public slots:
        void unsupportedContent ( QNetworkReply* reply );
        void on_buttonRefresh_clicked( bool );

    private slots:
        void downloadFinished();
        void on_webView_linkClicked( QUrl url );
        void on_buttonGpxView_toggled(bool);
        void on_buttonVisuGpx_toggled(bool);

  };
}

#endif // DISPLAYGPXVIEWDOTCOMFRAME_H
