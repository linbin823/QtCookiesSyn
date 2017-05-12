#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QtDebug>
#include <QTextStream>
#include <QNetworkCookie>
#include <QSettings>
#include <QMutexLocker>
#include "networkcookiejar.h"
#include "cookieprasetools_p.h"

NetworkCookieJar* NetworkCookieJar::_singleton = nullptr;

NetworkCookieJar::NetworkCookieJar(QObject *parent):QNetworkCookieJar(parent){
    _setHtmlCookies = false;
    load();
}

NetworkCookieJar::~NetworkCookieJar(){
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

bool NetworkCookieJar::setHtmlCookiesFromUrl(const QString& rawString, const QUrl &url){
    QList<QNetworkCookie> ret = CookiePraseToolsPrivate::parseHtmlCookies(rawString);
//    qDebug()<<Q_FUNC_INFO<<rawString<<ret;
    if(ret.length() != 0){
        //set cookies to current cookieJar
        _setHtmlCookies = true;
        bool res = setCookiesFromUrl(ret, url);
        _setHtmlCookies = false;
        return res;
    }
    return false;
}

bool NetworkCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    //mutex
    QMutexLocker locker(&_lock);
    if( QNetworkCookieJar::setCookiesFromUrl(cookieList, url) ){
        //to tell the Html to synchronize cookies
        if(!_setHtmlCookies){
            qDebug()<<Q_FUNC_INFO<<"from QML cookies:"<<cookieList.length()<<url;
            //avoid loop
            foreach(QNetworkCookie it, cookieList){
                if( !it.name().isEmpty() || !it.name().isNull() ){
                    emit synHtmlCookie( CookiePraseToolsPrivate::getHtmlCookieStr(it) );
                }
            }
        }
        else{
            qDebug()<<Q_FUNC_INFO<<"from Html cookies:"<<cookieList.length()<<url;
        }
        //save to loacl storage
        save();
        return true;
    }
    return false;
}

void NetworkCookieJar::save(){
    QString directory = cookiesDirectory();
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }
    const QString location = directory + QDir::separator() + COOKIES_FILE;
    QSettings cookieSettings(location, QSettings::IniFormat);
    cookieSettings.setValue(QLatin1String("cookies"), QVariant::fromValue(getAllCookies()));
    cookieSettings.sync();
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
    qDebug()<<Q_FUNC_INFO<<cookies;
    setAllCookies(cookies);
}

QString NetworkCookieJar::cookiesDirectory(){
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "cookies";
}

inline QDataStream& operator<<(QDataStream& out, const QNetworkCookie cookie){
    out<<cookie.domain()<<cookie.expirationDate()<<cookie.isHttpOnly()<<cookie.isSecure()
      <<cookie.name()<<cookie.path()<<cookie.value();
    return out;
}
inline QDataStream& operator>>(QDataStream& in, QNetworkCookie& cookie){
    QString tempStr;
    QDateTime tempDT;
    bool tempBool;
    QByteArray tempBA;

    in>>tempStr;
    cookie.setDomain(tempStr);
    in>>tempDT;
    cookie.setExpirationDate(tempDT);
    in>>tempBool;
    cookie.setHttpOnly(tempBool);
    in>>tempBool;
    cookie.setSecure(tempBool);
    in>>tempBA;
    cookie.setName(tempBA);
    in>>tempStr;
    cookie.setPath(tempStr);
    in>>tempBA;
    cookie.setValue(tempBA);
    return in;
}

