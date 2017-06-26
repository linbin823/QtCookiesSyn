#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QtDebug>
#include <QtWebView/QtWebView>
#include <QtCookieSyn>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtWebView::initialize();
    QQmlApplicationEngine engine;
    START_COOKIE_SYN(engine)
    engine.load(QUrl(QStringLiteral("qrc:/qml/xmlhttprequest.qml")));
    return app.exec();
}
