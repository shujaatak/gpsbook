#include "cookiejar.h"

namespace PluginDisplayInternetBrowser {
    CookieJar::CookieJar (QString path, QObject *parent) : QNetworkCookieJar(parent)
    {
        filename=path;

        QFile cookieFile(filename);
        if (cookieFile.exists() && cookieFile.open(QIODevice::ReadOnly) )
        {
            QList<QNetworkCookie> list;
            QByteArray line;
            while(!(line = cookieFile.readLine()).isNull())
            {
                list.append(QNetworkCookie::parseCookies(line));
            }
            setAllCookies(list);
        }
        else
        {
            qWarning() << "Cannot open "<< filename ;
        }

    }

    CookieJar::~CookieJar () //destructeur; sauvegarde les cookies
    {
        QList <QNetworkCookie> cookieList;
        cookieList = allCookies();

        QFile file(filename);

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qWarning() << "Cannot open "<< filename;
            return;
        }

        QTextStream out(&file);

        for(int i = 0 ; i < cookieList.size() ; i++)
        {
            QByteArray line =  cookieList.at(i).toRawForm(QNetworkCookie::Full);
            out << line << "\n";
        }

        file.close();
    }

    QList<QNetworkCookie> CookieJar::cookiesForUrl ( const QUrl & url ) const
    {
        return QNetworkCookieJar::cookiesForUrl(url);
    }

    bool CookieJar::setCookiesFromUrl ( const QList<QNetworkCookie> & cookieList, const QUrl & url )
    {
        return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
    }
}
