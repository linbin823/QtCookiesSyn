#ifndef COOKIEPRASETOOLSPRIVATE_H
#define COOKIEPRASETOOLSPRIVATE_H
#include <QObject>
#include <QString>
#include <QNetworkCookie>

class CookiePraseToolsPrivate
{
public:
    CookiePraseToolsPrivate();
    static QString escape(const QString& unescaped);
    static QString unescape(const QString& escaped);
    static QList<QNetworkCookie> parseHtmlCookies(const QString& rawStr);
    static QString getHtmlCookieStr( const QNetworkCookie& cookie );
};

#endif // COOKIEPRASETOOLSPRIVATE_H
