#ifndef QMLNETWORKACCESSMANAGERFACTORY_H
#define QMLNETWORKACCESSMANAGERFACTORY_H
#include <QQmlNetworkAccessManagerFactory>


//QQmlNetworkAccessManagerFactory for QQmlEngine
//QNetworkAccessManager with cookie function!
class QMLNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QMLNetworkAccessManagerFactory();

    //create QNetworkAccessManager
    QNetworkAccessManager * create(QObject *parent);
};

#endif // QMLNETWORKACCESSMANAGERFACTORY_H
