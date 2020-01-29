
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

import QtQuick 2.12
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami
import Utils 1.0

ListView {
    property var _dirmodel: DirModel { id: dirmodel }
    property alias path: dirmodel.path
    property alias mode: dirmodel.mode
    property alias dirName: dirmodel.dirName
    id: listView
    signal selected(string path)
    model: dirmodel.filteredModel
    onPathChanged: if(headerItem && headerItem.text) headerItem.text = ""
    header: Controls.TextField {
        onActiveFocusChanged: {
            if (activeFocus) {
                listView.currentIndex = -1
            }
        }
        id: search
        placeholderText: qsTr("Search...")
        onTextChanged: dirmodel.filter = text
        width: listView.width
        leftPadding: Kirigami.Units.largeSpacing

        Row {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            rightPadding: contextDrawer.dragMargin
            Item {
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: height

                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "edit-clear"
                    height: parent.height / 2
                    width: height
                    smooth: true
                }
                TapHandler {
                    onTapped: {
                        search.text = ""
                    }
                }
            }
            Item {
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: height

                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "go-parent-folder"
                    height: parent.height / 2
                    width: height
                    smooth: true
                }
                TapHandler {
                    onTapped: {
                        dirmodel.cdUp()
                    }
                }
            }
        }
        z: 2
    }
    headerPositioning: ListView.PullBackHeader
    currentIndex: -1

    delegate: Kirigami.BasicListItem {
        label: display
        icon: model.icon
        highlighted: focus && ListView.isCurrentItem
        onClicked: {
            if(ListView.isCurrentItem) {
                if (type === "folder") {
                    dirmodel.cd(display)
                } else {
                    selected(fullpath)
                }
            } else {
                currentIndex = index;
            }
        }
        Rectangle {
            width: height
            height: parent.height * 3
            radius: height / 2
            color: Kirigami.Theme.backgroundColor

            Kirigami.Icon {
                source: type === "folder" ? "folder-open" : "document-open"
                anchors.fill: parent
                anchors.margins: parent.height / 6
            }
            MouseArea {
                anchors.fill: parent
                onClicked: selected(fullpath)
            }
        }
    }

    Controls.Label {
        anchors.fill: parent
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        visible: parent.count == 0
        text: qsTr("Nothing to show")
        opacity: 0.6
    }
}
