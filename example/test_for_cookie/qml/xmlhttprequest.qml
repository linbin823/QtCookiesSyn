/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.7
import QtWebView 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
//import QtWebEngine 1.4

ApplicationWindow{

    id: window
    title: qsTr("Test for cookie")
    width: 800
    height: 640
    visible: true

    WebView{
        //This is for android pageView
        width: parent.width / 2
        height: parent.height
        url: "file:///android_asset/html/index.html"
    }

    Rectangle {
        //This is for XMLHttpRequest()
        id: content
        width: parent.width / 2
        height: parent.height
        x:parent.width / 2
        function showRequestInfo(text) {
            log.text = log.text + "\n" + text
            //console.log(text)
        }
        Flickable{
            anchors.fill: parent
            contentHeight: log.height
            contentWidth:  log.width
            flickableDirection: Flickable.VerticalFlick
            Text {
                id: log;
                width: parent.width
                height: log.contentHeight
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 10
                wrapMode: Text.Wrap
            }

        }

        Rectangle {
            id: button
            anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: parent.bottom; anchors.margins: 10
            width: buttonText.width + 10; height: buttonText.height + 10
            border.width: mouseArea.pressed ? 2 : 1
            radius : 5; antialiasing: true

            Text { id: buttonText; anchors.centerIn: parent; text: "Request data.xml" }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    log.text = ""
                    var doc = new XMLHttpRequest();
                    doc.onreadystatechange = function() {
                        if (doc.readyState == XMLHttpRequest.DONE) {
                            content.showRequestInfo("response headers -->");
                            content.showRequestInfo(doc.getAllResponseHeaders ());
                            content.showRequestInfo("responseText");
                            content.showRequestInfo(doc.responseText)
                            content.showRequestInfo("responseXML");
                            content.showRequestInfo(doc.responseXML)
                        }
                    }

                    doc.open("GET", "http://www.baidu.com/");
                    doc.send();

                }
            }
        }
    }
}
