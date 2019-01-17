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


#include "DapConnectClient.h"
#include "DapSession.h"
#include <QNetworkProxy>

#define HTTP_ADDRESS_URL_TEMPLATE(host, port, url) \
    QString("http://%1:%2%3").arg(host).arg(port).arg(url)

DapConnectClient::DapConnectClient(QObject *parent) :
    QObject(parent)
{
    m_netConfManager = new QNetworkConfigurationManager();

    m_httpClient = new QNetworkAccessManager(this);
    m_httpClient->setProxy(QNetworkProxy::NoProxy);

    connect(m_httpClient, &QNetworkAccessManager::finished, this, &DapConnectClient::finished);
    connect(m_httpClient, &QNetworkAccessManager::networkAccessibleChanged,
            [=](QNetworkAccessManager::NetworkAccessibility accessible) {
        qDebug() << "Network accessible changed to" << accessible;
        if(accessible != QNetworkAccessManager::NetworkAccessibility::Accessible) {
            _rebuildNetworkManager();
        }
    });

    connect(m_netConfManager, &QNetworkConfigurationManager::configurationChanged,
            [=](const QNetworkConfiguration & config){
        qDebug() << "Configuration changed to" << config.name();
        _rebuildNetworkManager();
    });
}

void DapConnectClient::_rebuildNetworkManager()
{
    qDebug() << "rebuildNetworkManager";
    emit sigNetworkManagerRebuild();
    delete m_httpClient;
    m_httpClient = new QNetworkAccessManager(this);
    m_httpClient->setProxy(QNetworkProxy::NoProxy);
}

bool DapConnectClient::_buildRequest(QNetworkRequest& req, const QString& host,
                                     quint16 port, const QString & urlPath,
                                     const QVector<HttpRequestHeader>& headers)
{
    if(m_httpClient->networkAccessible() == QNetworkAccessManager::NotAccessible) {
        _rebuildNetworkManager();
    }

    QString httpAddress = HTTP_ADDRESS_URL_TEMPLATE(host, port, urlPath);
    qDebug()<< "Requests httpAddress + url " << httpAddress;

    req.setUrl(httpAddress);
    if(!req.url().isValid()) {
        qCritical() << "Bad URL";
        return false;
    }

    for(const auto& header : headers) {
        req.setRawHeader(header.first.toLatin1(), header.second.toLatin1());
    }

    return true;
}

QNetworkReply* DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath)
{
    return request_GET(host, port, urlPath, {
                            HttpRequestHeader("Content-Type","text/plain")
                        });
}

QNetworkReply* DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           const QVector<HttpRequestHeader>& headers)
{
    QNetworkRequest req;
    if(!_buildRequest(req, host, port, urlPath, headers)) {
        return Q_NULLPTR;
    }
    auto netReply = m_httpClient->get(req);
    connect(this, &DapConnectClient::sigNetworkManagerRebuild, netReply, &QNetworkReply::abort);
    return netReply;
}

QNetworkReply* DapConnectClient::request_POST(const QString& host,  quint16 port,
                                              const QString & urlPath, const QByteArray& data)
{
    return request_POST(host, port, urlPath, data, {
                            HttpRequestHeader("Content-Type","text/plain")
                        });
}

QNetworkReply* DapConnectClient::request_POST(const QString& host,  quint16 port,
                            const QString & urlPath, const QByteArray& data,
                            const QVector<HttpRequestHeader>& headers)
{
    QNetworkRequest req;
    if(!_buildRequest(req, host, port, urlPath, headers)) {
        return Q_NULLPTR;
    }
    auto netReply = m_httpClient->post(req, data);
    connect(this, &DapConnectClient::sigNetworkManagerRebuild, netReply, &QNetworkReply::abort);
    return netReply;
}
