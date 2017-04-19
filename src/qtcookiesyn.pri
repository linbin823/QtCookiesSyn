INCLUDEPATH *= \
    $$PWD

QT += network qml
SOURCES += \
    $$PWD/networkcookiejar.cpp \
    $$PWD/qmlnetworkaccessmanagerfactory.cpp

HEADERS +=  \
    $$PWD/networkcookiejar.h \
    $$PWD/qmlnetworkaccessmanagerfactory.h

android*{
    message("android")
    QT += webview
}else{
    message("not android")
    QT += webengine webenginewidgets
}
