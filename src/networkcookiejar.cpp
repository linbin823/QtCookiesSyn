#include "networkcookiejar.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QtDebug>
#include <QTextStream>
#include <QNetworkCookie>
#include <QSettings>
#include <QMutexLocker>

#ifndef Q_OS_ANDROID
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#endif

NetworkCookieJar* NetworkCookieJar::_singleton = nullptr;

NetworkCookieJar::NetworkCookieJar(QObject *parent):QNetworkCookieJar(parent){
    loadStore();
    load();
}

NetworkCookieJar::~NetworkCookieJar(){
    save();
}

NetworkCookieJar* NetworkCookieJar::Instance(){
    if(_singleton == nullptr){
        _singleton = new NetworkCookieJar();
    }
    return _singleton;
}

QList<QNetworkCookie> NetworkCookieJar::getAllCookies() const
{
    return this->allCookies();
}

void NetworkCookieJar::loadStore(){
#ifndef Q_OS_ANDROID
    // Save cookies profile shared
    QWebEngineProfile::defaultProfile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    _store = QWebEngineProfile::defaultProfile()->cookieStore();
    connect(_store, &QWebEngineCookieStore::cookieAdded, this, &NetworkCookieJar::insertCookie);
    connect(_store, &QWebEngineCookieStore::cookieRemoved,this,&NetworkCookieJar::deleteCookie);
    _store->loadAllCookies();
#endif
}

bool NetworkCookieJar::deleteCookie(const QNetworkCookie &cookie) {
    qDebug()<<Q_FUNC_INFO<<cookie.name()<<cookie.value();
    return QNetworkCookieJar::deleteCookie(cookie);
}

bool NetworkCookieJar::insertCookie(const QNetworkCookie &cookie) {
    qDebug()<<Q_FUNC_INFO<<cookie.name()<<cookie.value();
    return QNetworkCookieJar::insertCookie(cookie);
}

bool NetworkCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    qDebug()<<Q_FUNC_INFO<<cookieList.length()<<url;
    if( QNetworkCookieJar::setCookiesFromUrl(cookieList, url) ){
        save();
        return true;
    }
    return false;
}

bool NetworkCookieJar::updateCookie(const QNetworkCookie &cookie) {
    qDebug()<<Q_FUNC_INFO<<cookie.name()<<cookie.value();
    if( QNetworkCookieJar::updateCookie(cookie) ){
        save();
        return true;
    }
    return false;
}


void NetworkCookieJar::save(){
    //mutex
    QMutexLocker locker(&_lock);
    QString directory = cookiesDirectory();
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }
    const QString location = directory + QDir::separator() + COOKIES_FILE;
    QSettings cookieSettings(location, QSettings::IniFormat);
    cookieSettings.setValue(QLatin1String("cookies"), QVariant::fromValue(getAllCookies()));
    cookieSettings.sync();
    qDebug()<<Q_FUNC_INFO<<QVariant::fromValue(getAllCookies());
}

void NetworkCookieJar::load(){
    //mutex
    QMutexLocker locker(&_lock);
    // load cookies and exceptions
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    const QString location = cookiesDirectory() + QDir::separator() + COOKIES_FILE;
    QSettings cookieSettings(location, QSettings::IniFormat);
    cookieSettings.sync();
    QVariant data = cookieSettings.value(QLatin1String("cookies"));
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >( data );
    cookieSettings.sync();
    qDebug()<<Q_FUNC_INFO<<cookies.size();
    setAllCookies(cookies);
    // Now user iterate and add it to chromium
#ifndef Q_OS_ANDROID
    for (auto cookie : cookies) {
       _store->setCookie(cookie);
    }
#endif
}

QString NetworkCookieJar::cookiesDirectory(){
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "cookies";
}
