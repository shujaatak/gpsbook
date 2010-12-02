#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QObject>
#include <QtNetwork>

namespace PluginDisplayInternetBrowser {
    class CookieJar : public QNetworkCookieJar
    {
        Q_OBJECT

        public:
        CookieJar (QString path, QObject *parent = 0);
        ~CookieJar ();
        QList<QNetworkCookie> cookiesForUrl ( const QUrl & url ) const;
        bool setCookiesFromUrl ( const QList<QNetworkCookie> & cookieList, const QUrl & url );

        private:
        QString filename;
    };
}

#endif // COOKIESJAR_H
