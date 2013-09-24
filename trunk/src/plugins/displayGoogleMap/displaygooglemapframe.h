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
#ifndef DISPLAYGOOGLEMAPFRAME_H
#define DISPLAYGOOGLEMAPFRAME_H

#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QFrame>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>
#else
#include <QFrame>
#include <QWebFrame>
#include <QWebPage>
#endif
#include <QUrl>
#include <QFile>
#include <QThread>

#include <QNetworkProxy>
#include <QTemporaryFile>

#include <QDir>
#include "gpsdata.h"
#include "QProgressIndicator.h"

namespace Ui {
    class DisplayGoogleMapFrame;
}

using namespace GPSBook;
using namespace ProgressIndicator;

namespace PluginDisplayGoogleMap {
    //Define fake user agent as google chrome to make visible the earth plugin
    class CustomWebPage : public QWebPage
    {
    protected:
       QString userAgentForUrl ( const QUrl & ) const
       {
#if defined(Q_OS_LINUX)
    return "Mozilla/5.0 (X11; U; Linux i686; en-US) AppleWebKit/533.4 (KHTML, like Gecko) Chrome/6.0.472.63 Safari/534.3";
#elif defined(Q_OS_WIN)
    return "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/533.4 (KHTML, like Gecko) Chrome/6.0.472.63 Safari/534.3";
#elif defined(Q_OS_MAC)
    return "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_3; en-US) AppleWebKit/533.4 (KHTML, like Gecko) Chrome/6.0.472.63 Safari/534.3";
#endif
       }
    };


    class DisplayGoogleMapFrame : public QFrame{ //, public QThread {
        Q_OBJECT
        Q_DISABLE_COPY(DisplayGoogleMapFrame)

    public:
        explicit DisplayGoogleMapFrame(QWidget *parent = 0);
        virtual ~DisplayGoogleMapFrame();
        void on_gpsdataChanged(){}
        void init(GPSData* gpsdata);
        void run();

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DisplayGoogleMapFrame *m_ui;
        GPSData* trackList;
        QTemporaryFile* tempPage;
        QTemporaryFile* tempGPX;
        QProgressIndicator* progressIndicator;

    private slots:
        void on_commandLinkButton_clicked();
    };

}

#endif // DISPLAYGOOGLEMAPFRAME_H
