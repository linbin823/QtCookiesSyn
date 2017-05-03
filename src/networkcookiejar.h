#ifndef NETWORKCOOKIEJAR_H
#define NETWORKCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QMutex>
#include <QDataStream>
#include <QDateTime>

#define COOKIES_FILE "cookies"

class QNetworkCookie;
class NetworkCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    ~NetworkCookieJar();
    static NetworkCookieJar* Instance();
    //due to QNetworkCookieJar::allCookies() is protected, create a public to fufill same
    QList<QNetworkCookie> getAllCookies()const;
    //HTML slot
    Q_INVOKABLE bool setHtmlCookiesFromUrl(const QString& rawString, const QUrl &url);
public slots:
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) override;

signals:
    //HTML signal
    void synHtmlCookie( const QString& cookieStr );

private:
    explicit NetworkCookieJar(QObject *parent = 0);
    static NetworkCookieJar* _singleton;
    QMutex _lock;
    QString cookiesDirectory();
    bool    _setHtmlCookies;
private slots:
    void save();
    void load();

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
