/*
 Copyright (c) 2017-2018 (c) Project "DeM Labs Inc" https://github.com/demlabsinc
  All rights reserved.

 This file is part of DAP (Deus Applications Prototypes) the open source project

    DAP (Deus Applicaions Prototypes) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DAP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef DapConnectClient_H
#define DapConnectClient_H

#include <QObject>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QString>
#include <QList>
#include <QVector>
#include <QBuffer>

using HttpRequestHeader = QPair<const QString, const QString>;

class DapConnectClient final : public QObject
{
    Q_OBJECT
private:
    bool _buildRequest(QNetworkRequest& req, const QString& host,  quint16 port, const QString & urlPath,
                       const QVector<HttpRequestHeader>& headers);

    void _rebuildNetworkManager();
public:
    DapConnectClient(QObject *parent = Q_NULLPTR);
    virtual ~DapConnectClient();

    /* Example call: request_GET("google.com", {
     *                                           HttpRequestHeader("User-Agent", "Mozilla/5.0"),
     *                                           HttpRequestHeader("Accept", "text/html")
     *                                         }); */
    QNetworkReply* request_GET(const QString& host,  quint16 port, const QString & urlPath,
                               const QVector<HttpRequestHeader>& headers);

    // Sets Content-Type Header by default "text/plain"
    QNetworkReply* request_GET(const QString& host,  quint16 port,const QString & urlPath);

    QNetworkReply* request_POST(const QString& host,  quint16 port,
                                const QString & urlPath, const QByteArray& data,
                                const QVector<HttpRequestHeader>& headers);

    // Sets Content-Type Header by default "text/plain"
    QNetworkReply* request_POST(const QString& host,  quint16 port,
                                const QString & urlPath, const QByteArray& data);

private:
    QNetworkAccessManager * m_httpClient;
    // Network setting before upping DAP network interface
    const QNetworkConfiguration * m_defaultNetworkConfig = Q_NULLPTR;

public slots:
    // Restore network configuration before upping DAP network interface
    void restoreDefaultNetConf();
    // Saving network configuration before upping DAP network interface
    void saveCurrentNetConf();
};

#endif // DapConnectClient_H
