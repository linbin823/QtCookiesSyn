TEMPLATE = app

CONFIG += c++11

QT += qml quick webview

SOURCES += main.cpp

RESOURCES += qml.qrc

include(../../src/qtcookiesyn.pri)


android*{
    message("android")

    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat \
        android/assets/html/qwebchannel.js \
        android/assets/html/index.html

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    deployment.path = /assets/html/
    deployment.files += qwebchannel.js  \
                        index.html
    INSTALLS += deployment
}
