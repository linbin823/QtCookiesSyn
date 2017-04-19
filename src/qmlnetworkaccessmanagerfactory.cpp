#include <QNetworkAccessManager>
#include "qmlnetworkaccessmanagerfactory.h"
#include "networkcookiejar.h"

QMLNetworkAccessManagerFactory::QMLNetworkAccessManagerFactory()
{

}

QNetworkAccessManager *QMLNetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);

    nam->setCookieJar( NetworkCookieJar::Instance() );

    return nam;
}
