#include <QUrl>
#include <QDateTime>
#include <QNetworkCookieJar>
#include "cookieprasetools_p.h"

CookiePraseToolsPrivate::CookiePraseToolsPrivate()
{
}

QString CookiePraseToolsPrivate::escape(const QString& unescaped){
    int i;
    QChar j;
    QString tmp;
    tmp.resize(unescaped.length() * 6);
    for (i = 0; i < unescaped.length(); i++) {
        j = unescaped.at(i);
        if (j.isNumber() || j.isLower() || j.isUpper())
            tmp.append(j);
        else if (j < 256) {
            tmp.append("%");
            if (j < 16)
                tmp.append("0");
            tmp= tmp + QString::number(j.row() ,16);
        } else {
            tmp.append("%u");
            tmp= tmp + QString::number(j.row() ,16);
        }
    }
    return tmp;
}

QString CookiePraseToolsPrivate::unescape(const QString& escaped){
    Q_UNUSED(escaped);
    return QString::null;
}

QList<QNetworkCookie> CookiePraseToolsPrivate::parseHtmlCookies(const QString& rawStr){
    QList<QNetworkCookie> ret;
    if( rawStr.isEmpty() || rawStr.isNull() )
        return ret;
    QStringList rawStrList = rawStr.split(";");
    if(rawStrList.size() == 0)
        return ret;
    int index;
    QNetworkCookie temp;
    QString exp,title,value;
    for(int i=0; i<rawStrList.length(); i++){
        exp = rawStrList.at(i).trimmed();
        index = exp.indexOf("=");
        if(index == -1 ){
            if(exp.toLower() == "secure"){
                //is secured!
                temp.setSecure(true);
                continue;
            }else{
                //can't prase
                continue;
            }
        }
        title = exp.left(index);
        value = exp.mid(index + 1);
        if(title.toLower().trimmed() == "expires"){
            //expires date
            temp.setExpirationDate( QDateTime::fromString(value, "ddd, dd MMM yyyy HH:mm:ss") );
            continue;
        }else if(title.toLower().trimmed() == "path"){
            //path
            temp.setPath( value );
            continue;
        }else if(title.toLower().trimmed() == "domain"){
            //domain
            temp.setDomain( value );
            continue;
        }else{
            //new name and value
            if(!temp.name().isEmpty() || !temp.name().isNull() )
                ret.append( temp );
            temp.setName( title.toLocal8Bit() );
            temp.setValue( value.toLocal8Bit() );
            temp.setDomain( QString() );
            temp.setExpirationDate( QDateTime() );
            temp.setPath( QString() );
            temp.setSecure( false );
            temp.setHttpOnly( false );
            continue;
        }
    }
    if(!temp.name().isEmpty() || !temp.name().isNull() )
        ret.append( temp );
    return ret;
}


QString CookiePraseToolsPrivate::getHtmlCookieStr( const QNetworkCookie& cookie ){
    QString ret;
    ret  = cookie.name();
    ret += "="+cookie.value()+"; ";
    if(!cookie.path().isEmpty() && !cookie.path().isNull()) ret+="path="+cookie.path()+"; ";
    if(cookie.expirationDate().isValid()) ret+="expires="+cookie.expirationDate().toString("ddd, dd MMM yyyy HH:mm:ss")+" GMT; ";
    if(!cookie.domain().isEmpty()) ret+="domain="+cookie.domain()+"; ";
    if(cookie.isSecure()) ret+="secure; ";
    return ret;
}

