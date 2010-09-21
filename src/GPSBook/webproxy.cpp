/*
    This file is part of the OfiLabs X2 project.

    Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>
                  2010 gpsbook-team

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Modified and adapted by gpsbook-team.
      - Creation of the object
      - Creation of the web server (for local requests)
*/
#include <QtNetwork>
#include <QFileInfo>
#include "webproxy.h"

namespace GPSBook {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    WebProxy::WebProxy()
    {
        QTcpServer *proxyServer = new QTcpServer(this);
        proxyServer->listen(QHostAddress::Any, 8080);
        connect(proxyServer, SIGNAL(newConnection()), this, SLOT(manageQuery()));
        qDebug( )  << __FILE__ << __FUNCTION__ << "Proxy server running at port" << proxyServer->serverPort();
    } //WebProxy::WebProxy

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void WebProxy::manageQuery() {
        QTcpServer *proxyServer = qobject_cast<QTcpServer*>(sender());
        QTcpSocket *socket = proxyServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(processQuery()));
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    } //WebProxy::manageQuery

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void WebProxy::processQuery() {
        QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
        QByteArray requestData = socket->readAll();

        int pos = requestData.indexOf("\r\n");
        QByteArray requestLine = requestData.left(pos);
        requestData.remove(0, pos + 2);

        QList<QByteArray> entries = requestLine.split(' ');
        QByteArray method = entries.value(0);
        QByteArray address = entries.value(1);
        QByteArray version = entries.value(2);

        //qDebug( )  << __FILE__ << __FUNCTION__ << "Processing " << address;

        QUrl url = QUrl::fromEncoded(address);
        if (!url.isValid()) {
            //qWarning() << "Invalid URL:" << url;
            socket->disconnectFromHost();
            return;
        }

        //Act as server is request are for local server
        if ((url.host() == "") && (QFile(address).exists())) {
            //qDebug( )  << __FILE__ << __FUNCTION__ << "Sending " << address;
            QByteArray header;
            QTextStream headerStream(&header, QIODevice::WriteOnly);
            //Construct response header
            headerStream << "HTTP/1.0 200 OK" << endl;
            headerStream << "Server: gpsbook/" << qApp->applicationVersion() << endl;
            headerStream << "Date: " << QDateTime::currentDateTime().toUTC().toString("ddd, dd MMM yyyy hh:mm:ss") << "GMT" << endl;
            headerStream << "Content-Type: text/html; charset=utf-8" << endl;
            headerStream << "Connection: close" << endl;
            headerStream << "Pragma: no-cache" << endl;
            headerStream << "Cache-Control: no-cache" << endl;
            QFile file(address);
            if (!file.open(QFile::ReadOnly | QFile::Text))
            {
                 qWarning() << "Cannot open:" << address;
                 socket->disconnectFromHost();
                 return ;
            }

            QByteArray content;
            QTextStream contentStream(&content, QIODevice::WriteOnly);

            while (!file.atEnd()) {
                contentStream << file.readLine() << endl;
            }

            headerStream << "Content-Length:" << content.size() << endl;
            headerStream << "" << endl;

            socket->write(header);
            socket->write(content);
            //qDebug( )  << __FILE__ << __FUNCTION__ << "File sent (" << content.size() << "bytes) :-)";
            socket->disconnectFromHost();
        return;
        }

        //Remove advert to speedup development ;-)
        if (url.toString().contains("googlesyndication")) {
            //qDebug( )  << __FILE__ << __FUNCTION__ << "Remove Advert URL:" << url;
            socket->disconnectFromHost();
            return;
        }

        QString host = url.host();
        int port = (url.port() < 0) ? 80 : url.port();
        QByteArray req = url.encodedPath();
        if (url.hasQuery())
            req.append('?').append(url.encodedQuery());
        requestLine = method + " " + req + " " + version + "\r\n";
        requestData.prepend(requestLine);

        QString key = host + ':' + QString::number(port);
        QTcpSocket *proxySocket = socket->findChild<QTcpSocket*>(key);
        if (proxySocket) {
            proxySocket->setObjectName(key);
            proxySocket->setProperty("url", url);
            proxySocket->setProperty("requestData", requestData);
            proxySocket->write(requestData);
        } else {
            proxySocket = new QTcpSocket(socket);
            proxySocket->setObjectName(key);
            proxySocket->setProperty("url", url);
            proxySocket->setProperty("requestData", requestData);
            connect(proxySocket, SIGNAL(connected()), this, SLOT(sendRequest()));
            connect(proxySocket, SIGNAL(readyRead()), this, SLOT(transferData()));
            connect(proxySocket, SIGNAL(disconnected()), this, SLOT(closeConnection()));
            connect(proxySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(closeConnection()));
            proxySocket->connectToHost(host, port);
        }
    } //WebProxy::processQuery

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void WebProxy::sendRequest() {
        QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
        QByteArray requestData = proxySocket->property("requestData").toByteArray();
        proxySocket->write(requestData);
    } //WebProxy::sendRequest

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void WebProxy::transferData() {
        QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
        QTcpSocket *socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
        socket->write(proxySocket->readAll());
    } //WebProxy::transferData

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void WebProxy::closeConnection() {
        QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
        if (proxySocket) {
            QTcpSocket *socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
            if (socket)
                socket->disconnectFromHost();
            if (proxySocket->error() != QTcpSocket::RemoteHostClosedError)
                qWarning() << "Error for:" << proxySocket->property("url").toUrl() << proxySocket->errorString();
            proxySocket->deleteLater();;
        }
    } //WebProxy::closeConnection
}



