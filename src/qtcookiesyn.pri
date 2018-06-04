INCLUDEPATH *= \
    $$PWD/../include

QT += network qml webchannel websockets

SOURCES += \
    $$PWD/networkcookiejar.cpp \
    $$PWD/qmlnetworkaccessmanagerfactory.cpp \
    $$PWD/websockettransport.cpp \
    $$PWD/websocketclientwrapper.cpp \
    $$PWD/cookieprasetools_p.cpp

HEADERS +=  \
    $$PWD/networkcookiejar.h \
    $$PWD/qmlnetworkaccessmanagerfactory.h \
    $$PWD/websockettransport.h \
    $$PWD/websocketclientwrapper.h \
    $$PWD/cookieprasetools_p.h

DISTFILES += \
    $$PWD/../README.md

#to serial/deserial objects in xml or jason fomat
include(3rd-party/loadSaveProcessor/src/loadsaveProcessor.pri)
