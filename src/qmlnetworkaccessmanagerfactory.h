#ifndef QMLNETWORKACCESSMANAGERFACTORY_H
#define QMLNETWORKACCESSMANAGERFACTORY_H
#include <QQmlNetworkAccessManagerFactory>

class QMLNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QMLNetworkAccessManagerFactory();

    //create QNetworkAccessManager
    QNetworkAccessManager * create(QObject *parent);
};

#endif // QMLNETWORKACCESSMANAGERFACTORY_H
