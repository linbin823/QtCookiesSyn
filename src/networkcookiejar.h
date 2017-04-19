#ifndef NETWORKCOOKIEJAR_H
#define NETWORKCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QMutex>
#include <QDataStream>
#include <QDateTime>

#define COOKIES_FILE "cookies"

class QWebEngineCookieStore;
class QNetworkCookie;
class NetworkCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    ~NetworkCookieJar();
    static NetworkCookieJar* Instance();
    //due to QNetworkCookieJar::allCookies() is protected, create a public to fufill same
    QList<QNetworkCookie> getAllCookies()const;
    void loadStore();
public slots:
    bool deleteCookie(const QNetworkCookie &cookie) override;
    bool insertCookie(const QNetworkCookie &cookie) override;
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) override;
    bool updateCookie(const QNetworkCookie &cookie) override;

private slots:
    void save();
    void load();

private:
    explicit NetworkCookieJar(QObject *parent = 0);
    static NetworkCookieJar* _singleton;
    //dynamic get webEngine default profile's cookie store, to save/remove Chromium Cookies
    QWebEngineCookieStore *_store;
    QMutex _lock;

    QString cookiesDirectory();
};

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

#endif // NETWORKCOOKIEJAR_H
