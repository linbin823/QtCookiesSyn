#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QStandardPaths>
#include <QtDebug>
#include <QMLNetworkAccessManagerFactory>
#include <NetworkCookieJar>
#include <QtWebView/QtWebView>

#ifndef Q_OS_ANDROID
#include <QtWebEngine>
#endif


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtWebView::initialize();
    NetworkCookieJar::Instance();
#ifndef Q_OS_ANDROID
    QtWebEngine::initialize();
#endif

    QQmlApplicationEngine engine;
    engine.setNetworkAccessManagerFactory(new QMLNetworkAccessManagerFactory());
    engine.load(QUrl(QStringLiteral("qrc:/qml/xmlhttprequest.qml")));

    return app.exec();
}
