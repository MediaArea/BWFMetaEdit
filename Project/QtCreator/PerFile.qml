import QtQuick 2.9
import QtQuick.Controls 2.2

Control {
    id: "root"
    readonly property color red: "#fc6744"
    readonly property color green: "#90ee90"
    readonly property color darkgreen: "#4ee44e"
    visible: true
    ListModel {
        id: sections
        ListElement {
            section: "MD5"
            fields: [
                ListElement{ name: "MD5Stored"; label: "MD5Stored"; fieldLength: 370; fieldHeight: 32; multiline: false },
                ListElement{ name: "MD5Generated"; label: "MD5Generated"; fieldLength: 370; fieldHeight: 32; multiline: false }
            ]
        }
        ListElement {
            section: "CORE"
            fields: [
                ListElement{ name: "LoudnessValue"; label: "LoudnessValue"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "LoudnessRange"; label: "LoudnessRange"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "MaxTruePeakLevel"; label: "MaxTruePeakLevel"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "MaxMomentaryLoudness"; label: "MaxMomentaryLoudness"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "MaxShortTermLoudness"; label: "MaxShortTermLoudness"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "CodingHistory"; label: "CodingHistory"; fieldLength: 185; fieldHeight: 64; multiline: true }
            ]
        }
        ListElement {
            section: "BEXT"
            fields: [
                ListElement{ name: "BextVersion"; label: "Version"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "Description"; label: "Description"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "Originator"; label: "Originator"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "OriginatorReference"; label: "OriginatorReference"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "OriginationDate"; label: "OriginationDate"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "TimeReference"; label: "Time Reference"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "OriginationTime"; label: "OriginationTime"; fieldLength: 185; fieldHeight: 32; multiline: false },
                ListElement{ name: "UMID"; label: "UMID"; fieldLength: 585; fieldHeight: 32; multiline: false }
            ]
        }
        ListElement {
            section: "INFO"
            fields: [
                ListElement{ name: "IARL"; label: "Archival Location (IARL)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "IART"; label: "Artist (IART)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ICMS"; label: "Commissioned (ICMS)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ICMT"; label: "Comments (ICMT)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ICOP"; label: "Copyright (ICOP)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ICRD"; label: "Creation date (ICRD)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "IENG"; label: "Engineer (IENG)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "IGNR"; label: "Genre (IGNR)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "IKEY"; label: "Keywords (IKEY)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "IMED"; label: "Medium (IMED)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "INAM"; label: "Name (Title) (INAM)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "IPRD"; label: "Product (Album) (IPRD)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ISBJ"; label: "Subject (ISBJ)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ISFT"; label: "Software (ISFT)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ISRC"; label: "Source (ISRC)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ISRF"; label: "Source Form (ISRF)"; fieldLength: 185; fieldHeight: 32; multiline: true },
                ListElement{ name: "ITCH"; label: "Technician (ITCH)"; fieldLength: 185; fieldHeight: 32; multiline: true }
            ]
        }
    }
    ScrollView {
        anchors.fill: parent
        width: parent.width
        ScrollBar.vertical.policy: contentHeight > height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
        ListView {
            // With Qt5.9 (only on macOS?) list do not fill width without this element
            header: Rectangle {
                width: parent.width
                height: 1
                color: "gray"
            }
            id: fileList
            spacing: 10
            width: parent.width
            model: Model
            delegate: Column {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                Row {
                    spacing: 5
                    width: parent.width
                    Image {
                        id: expand
                        property var expandSvg: ["data:image/svg+xml;utf8,",
                                                "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                                  "<path d='M19 13h-6v6h-2v-6H5v-2h6V5h2v6h6v2z'/>",
                                                  "<path d='M0 0h24v24H0z' fill='none'/>",
                                                "</svg>"].join('')
                        property var collapseSvg: ["data:image/svg+xml;utf8,",
                                                "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                                  "<path d='M19 13H5v-2h14v2z'/>",
                                                  "<path d='M0 0h24v24H0z' fill='none'/>",
                                                "</svg>"].join('')
                        source: expanded ? collapseSvg.arg("gray") : expandSvg.arg("gray")
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                visible: parent.containsMouse
                                timeout: 5000
                                delay: 1000
                                text: expanded ? "Collapse" : "Expand"
                            }
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                expanded = !expanded
                                parent.source = expanded ? parent.collapseSvg.arg("gray") : parent.expandSvg.arg("gray")
                            }
                            onEntered: {
                                parent.source = expanded ? parent.collapseSvg.arg("dimgray") : parent.expandSvg.arg("dimgray")
                            }
                            onExited: {
                                parent.source = expanded ? parent.collapseSvg.arg("gray") : parent.expandSvg.arg("gray")
                            }
                        }
                    }
                    Text {
                        id: title
                        width: parent.width - expand.width - edit.width - save.width - close.width - parent.spacing * 4
                        text: "<b>" + file + "</b>"
                        elide: Text.ElideMiddle
                    }
                    Image {
                        id: edit
                        property var editSvg: ["data:image/svg+xml;utf8,",
                                           "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                             "<path d='M3 17.25V21h3.75L17.81 9.94l-3.75-3.75L3 17.25zM20.71 7.04c.39-.39.39-1.02 0-1.41l-2.34-2.34c-.39-.39-1.02-.39-1.41 0l-1.83 1.83 3.75 3.75 1.83-1.83z'/>",
                                             "<path d='M0 0h24v24H0z' fill='none'/>",
                                           "</svg>"].join('')
                        property var displaySvg: ["data:image/svg+xml;utf8,",
                                           "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                             "<path d='M12 4.5C7 4.5 2.73 7.61 1 12c1.73 4.39 6 7.5 11 7.5s9.27-3.11 11-7.5c-1.73-4.39-6-7.5-11-7.5zM12 17c-2.76 0-5-2.24-5-5s2.24-5 5-5 5 2.24 5 5-2.24 5-5 5zm0-8c-1.66 0-3 1.34-3 3s1.34 3 3 3 3-1.34 3-3-1.34-3-3-3z'/>",
                                             "<path d='M0 0h24v24H0z' fill='none'/>",
                                           "</svg>"].join('')
                        source: Model.valid(file) ? (editMode ? displaySvg.arg("gray") : editSvg.arg("gray")) : editSvg.arg("whitesmoke")
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                visible: parent.containsMouse && Model.valid(file)
                                timeout: 5000
                                delay: 1000
                                text: editMode ? "Display Mode" : "Edit Mode"
                            }
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                if (Model.valid(file)) {
                                    editMode = !editMode
                                    parent.source = editMode ? parent.displaySvg.arg("gray") : parent.editSvg.arg("gray")
                                }
                            }
                            onEntered: {
                                if (Model.valid(file)) {
                                    parent.source = editMode ? parent.displaySvg.arg("dimgray") : parent.editSvg.arg("dimgray")
                                }
                            }
                            onExited: {
                                if (Model.valid(file)) {
                                    parent.source = editMode ? parent.displaySvg.arg("gray") : parent.editSvg.arg("gray")
                                }
                            }
                        }
                    }
                    Image {
                        id: save
                        property var svg: ["data:image/svg+xml;utf8,",
                                           "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                             "<path d='M17 3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2V7l-4-4zm-5 16c-1.66 0-3-1.34-3-3s1.34-3 3-3 3 1.34 3 3-1.34 3-3 3zm3-10H5V5h10v4z'/>",
                                             "<path d='M0 0h24v24H0z' fill='none'/>",
                                           "</svg>"].join('')
                        source: modified ? svg.arg("orange") : svg.arg("gray")
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                visible: hovered
                                timeout: 5000
                                delay: 1000
                                text: modified ? "Save" : "Saved"
                            }
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                if (modified) {
                                    Model.saveFile(file)
                                    parent.source = modified ? parent.svg.arg("orange") : parent.svg.arg("gray")
                                }
                            }
                            onEntered: {
                                if (modified) {
                                    parent.source = parent.svg.arg("darkorange")
                                }
                            }
                            onExited: {
                                if (modified) {
                                    parent.source = parent.svg.arg("orange")
                                }
                            }
                        }
                    }
                    Image {
                        id: close
                        property var svg: ["data:image/svg+xml;utf8,",
                                           "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                             "<path d='M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z'/>",
                                             "<path d='M0 0h24v24H0z' fill='none'/>",
                                           "</svg>"].join('')
                        source: svg.arg("gray")
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                visible: parent.containsMouse
                                timeout: 5000
                                delay: 1000
                                text: "Close"
                            }
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                Model.closeFile(file)
                            }
                            onEntered: {
                                parent.source = parent.svg.arg("dimgray")
                            }
                            onExited: {
                                parent.source = parent.svg.arg("gray")
                            }
                        }
                    }
                }
                Pane {
                    visible: expanded
                    width: parent.width
                    Column {
                        width: parent.width
                        spacing: 10
                        Row {
                            width: parent.width
                            spacing: 5
                            Text {
                                font.pointSize: 10
                                width: parent.width - xmp.width - axml.width - ixml.width - parent.spacing * 2
                                elide: Text.ElideRight
                                text: tech
                            }
                            Text {
                                id: xmp
                                text: "XMP"
                                color: Model.visible(file, "XMP") ? (Model.modified(file, "XMP") ? root.green : "gray") : "lightgray"
                                horizontalAlignment: Text.AlignHCenter
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    acceptedButtons: Qt.LeftButton
                                    onClicked: {
                                        if(Model.visible(file, "XMP")) {
                                            Model.editField(file, "XMP")
                                            parent.color = Model.modified(file, "XMP") ? root.green : "gray"
                                        }
                                    }
                                    onEntered: {
                                        if(Model.visible(file, "XMP")) {
                                            parent.color = Model.modified(file, "XMP") ? root.darkgreen : "dimgray"
                                        }
                                    }
                                    onExited: {
                                        if(Model.visible(file, "XMP")) {
                                            parent.color = Model.modified(file, "XMP") ? root.green : "gray"
                                        }
                                    }
                                }
                            }
                            Text {
                                id: axml
                                text: "aXML"
                                color: Model.visible(file, "aXML") ? (Model.modified(file, "aXML") ? root.green : "gray") : "lightgray"
                                horizontalAlignment: Text.AlignHCenter
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    acceptedButtons: Qt.LeftButton
                                    onClicked: {
                                        if(Model.visible(file, "aXML")) {
                                            Model.editField(file, "aXML")
                                            parent.color = Model.modified(file, "aXML") ? root.green : "gray"
                                        }
                                    }
                                    onEntered: {
                                        if(Model.visible(file, "aXML")) {
                                            parent.color = Model.modified(file, "aXML") ? root.darkgreen : "dimgray"
                                        }
                                    }
                                    onExited: {
                                        if(Model.visible(file, "aXML")) {
                                            parent.color = Model.modified(file, "aXML") ? root.green : "gray"
                                        }
                                    }
                                }
                            }
                            Text {
                                id: ixml
                                text: "iXML"
                                color: Model.visible(file, "iXML") ? (Model.modified(file, "iXML") ? root.green : "gray") : "lightgray"
                                horizontalAlignment: Text.AlignHCenter
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    acceptedButtons: Qt.LeftButton
                                    onClicked: {
                                        if(Model.visible(file, "iXML")) {
                                            Model.editField(file, "iXML")
                                            parent.color = Model.modified(file, "iXML") ? root.green : "gray"
                                        }
                                    }
                                    onEntered: {
                                        if(Model.visible(file, "iXML")) {
                                            parent.color = Model.modified(file, "iXML") ? root.darkgreen : "dimgray"
                                        }
                                    }
                                    onExited: {
                                        if(Model.visible(file, "iXML")) {
                                            parent.color = Model.modified(file, "iXML") ? root.green : "gray"
                                        }
                                    }
                                }
                            }
                        }
                        Text {
                           text: "<h4><font color='" + root.red + "'>Errors:</font></h4>" + "<font color='" + root.red + "'>" + Model.errors(file) + "</font>"
                           width: parent.width
                           visible: Model.errors(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Text {
                           text: "<h4>Informations:</h4>" +  Model.informations(file)
                           width: parent.width
                           visible: Model.informations(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Text {
                           text: "<h4><font color='orange'>Unsupported chunks:</font></h4>" + "<font color='orange'>" + Model.unsupportedChunks(file) + "</font>"
                           width: parent.width
                           visible: Model.unsupportedChunks(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Repeater {
                            model: sections
                            Column {
                                width: parent.width
                                visible: {
                                    for (var i = 0; i < fields.count; i++) {
                                        if (Model.visible(file, fields.get(i).name))
                                            return true
                                    }
                                    return false
                                }
                                Text {
                                    text: "<h4>" + section + "</h4>"
                                }
                                Flow {
                                    width: parent.width
                                    spacing: 5
                                    Repeater {
                                        model: fields
                                        Row {
                                            visible: Model.visible(file, name)
                                            Label {
                                                id: title
                                                height: fieldHeight
                                                width: 185
                                                horizontalAlignment: Text.AlignRight
                                                verticalAlignment: Text.AlignVCenter
                                                text: label + ": "
                                            }
                                            // Unused, metrics don't works on Qt5.9
                                            /*TextMetrics {
                                                id: ruler
                                                font: input.font
                                                text: "—".repeat(fieldLength)
                                            }*/
                                            ScrollView {
                                                id: scollarea
                                                height: fieldHeight
                                                width: fieldLength
                                                clip: true
                                                ScrollBar.vertical.policy: contentHeight > height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
                                                ScrollBar.horizontal.policy: contentWidth > width ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
                                                Component.onCompleted: {
                                                ScrollBar.vertical.position = 0
                                                ScrollBar.horizontal.position = 0
                                                }
                                                background: Rectangle {
                                                   color: Model.readonly(file, name) ? "whitesmoke" : "transparent"
                                                    border.color: {
                                                        if (parent.activeFocus) {
                                                            return "dodgerblue"
                                                        }
                                                        else if (!Model.valid(file, name, input.text)) {
                                                            return root.red
                                                        }
                                                        else if (Model.modified(file, name)) {
                                                            return root.green
                                                        }
                                                        else {
                                                            return "lightgray"
                                                        }
                                                    }
                                                    radius: 2
                                                }
                                                TextArea {
                                                    id: input
                                                    color: "black"
                                                    text: Model.value(file, name)
                                                    readOnly: Model.readonly(file, name)
                                                    selectByMouse: true
                                                    onEditingFinished: {
                                                        if (Model.valid(file, name, text)) {
                                                            Model.setValue(file, name, text)
                                                        }
                                                    }
                                                    Keys.onPressed: {
                                                        if (event.key == Qt.Key_Return && !multiline) {
                                                            event.accepted = true;
                                                        }
                                                    }
                                                    onActiveFocusChanged: {
                                                        if (activeFocus) {
                                                            Model.setSelected(file)
                                                        }
                                                    }
                                                /*MouseArea {
                                                    anchors.fill: parent
                                                    //hoverEnabled: true
                                                    acceptedButtons: Qt.LeftButton
                                                    propagateComposedEvents: true
                                                    onClicked: {
                                                        input.forceActiveFocus()
                                                        //Model.editField(file, name)
                                                        mouse.accepted = false
                                                    }
                                                    onDoubleClicked: {
                                                        input.forceActiveFocus()
                                                        Model.editField(file, name)
                                                        mouse.accepted = false
                                                    }
                                                }*/
                                                }
                                            }
                                            Image {
                                                property var message: Model.valid(file, name, input.text) ? "" : Model.lastValidationError(file)
                                                visible: !Model.valid(file, name, input.text)
                                                height: 24
                                                width: 24
                                                source: "qrc:///Image/Menu/Error.svg"
                                                anchors.verticalCenter: title.verticalCenter
                                                MouseArea {
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    ToolTip {
                                                        visible: parent.containsMouse
                                                        delay: 500
                                                        text: parent.parent.message
                                                        background: Rectangle {
                                                            border.color: root.red
                                                        }
                                                    }
                                                }
                                            }
                                            Button {
                                                height: 24
                                                width: 24
                                                anchors.verticalCenter: title.verticalCenter
                                                flat: true
                                                hoverEnabled: true
                                                onClicked: Model.editField(file, name)
                                               Image {
                                                   property var svg: ["data:image/svg+xml;utf8,",
                                                       "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                                         "<path d='M19 19H5V5h7V3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2v-7h-2v7zM14 3v2h3.59l-9.83 9.83 1.41 1.41L19 6.41V10h2V3h-7z'/>",
                                                         "<path d='M0 0h24v24H0z' fill='none'/>",
                                                       "</svg>"].join('')
                                                    source: svg.arg("gray")
                                                    fillMode: Image.PreserveAspectFit
                                                    anchors { top: parent.top; right: parent.right; topMargin: (parent.height - height) / 2 }
                                                }
                                                ToolTip {
                                                    visible: parent.hovered
                                                    delay: 1000
                                                    timeout: 5000
                                                    text: "Open in field editor"
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    width: parent.width
                    height: 2
                    color: "gainsboro"
                }
            }
        }
    }
}
