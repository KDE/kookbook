/*
 * Copyright (c) 2018 Sune Vuorela <sune@vuorela.dk>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami
import Kookbook 1.0

Kirigami.ApplicationWindow {
    id: root
    property var scanner: Scanner {
        id: scanner
    }

    visible: true
    width: 480
    height: 640
    title: "Kookbook"
    header: Kirigami.ApplicationHeader {}
    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: "Kookbook"
        titleIcon: ":/appicon.svg"
        actions: [
            Kirigami.Action {
                text: "Refresh"
                iconName: "view-refresh"
                onTriggered: {
                    scanner.refresh()
                    showPassiveNotification("Refreshing")
                }
            },
            Kirigami.Action {
                text: "Open cookbook"
                iconName: "document-open-folder"
                onTriggered: {
                    root.pageStack.pop(null)
                    var newItem = root.pageStack.push(Qt.resolvedUrl("SelectFolder.qml"))
                    newItem.selected.connect(function (path) {
                        scanner.rootPath = path
                        root.pageStack.pop(null)
                        showPassiveNotification("Scanning")
                        })
                }

            },
            Kirigami.Action {
                text: "Home"
                iconName: "go-home"
                onTriggered: {
                    root.pageStack.pop(null)
                }
            },
            Kirigami.Action {
                text: "Help"
                iconName: "help-contents"
                onTriggered: root.pageStack.push(Qt.resolvedUrl("Help.qml"))
            }
        ]
    }

    pageStack.initialPage: MainPage{
        id: mainPage
        scanner: root.scanner
    }

}
