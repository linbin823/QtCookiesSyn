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
    START_COOKIE_SYN_WITH_HTML(engine,12345)
    engine.load(QUrl(QStringLiteral("qrc:/qml/xmlhttprequest.qml")));
    return app.exec();
}
