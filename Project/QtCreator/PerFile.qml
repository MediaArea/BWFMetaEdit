import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Control {
    id: root
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
        ScrollBar.vertical.policy: contentHeight > height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
        ListView {

            // With Qt5.9 (only on macOS?) list do not fill width without this element
            header: Rectangle {
                anchors { left: parent.left; right: parent.right }
                height: 1
                color: "gray"
            }
            id: fileList
            spacing: 10
            anchors { left: parent.left; right: parent.right }
            model: Model
            delegate: Column {
                anchors { left: parent.left; right: parent.right; leftMargin: 10; rightMargin: 10 }
                // Element title
                RowLayout {
                    spacing: 5
                    anchors { left: parent.left; right: parent.right }
                    Image {
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
                        text: "<b>" + file + "</b>"
                        elide: Text.ElideMiddle
                        Layout.fillWidth: true
                    }
                    Image {
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
                        source: Model.isWritable(file) ? (editMode ? displaySvg.arg("gray") : editSvg.arg("gray")) : editSvg.arg("lightgray")
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                visible: parent.containsMouse && Model.valid(file)
                                timeout: 5000
                                delay: 1000
                                text: Model.readOnly(file) ? "Edit mode disabled, file is read only!" : (editMode ? "Display Mode" : "Edit Mode")
                            }
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                if (Model.isWritable(file)) {
                                    editMode = !editMode
                                    parent.source = editMode ? parent.displaySvg.arg("gray") : parent.editSvg.arg("gray")
                                }
                            }
                            onEntered: {
                                if (Model.isWritable(file)) {
                                    parent.source = editMode ? parent.displaySvg.arg("dimgray") : parent.editSvg.arg("dimgray")
                                }
                            }
                            onExited: {
                                if (Model.isWritable(file)) {
                                    parent.source = editMode ? parent.displaySvg.arg("gray") : parent.editSvg.arg("gray")
                                }
                            }
                        }
                    }
                    Image {
                        property var svg: ["data:image/svg+xml;utf8,",
                                           "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                             "<path d='M17 3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2V7l-4-4zm-5 16c-1.66 0-3-1.34-3-3s1.34-3 3-3 3 1.34 3 3-1.34 3-3 3zm3-10H5V5h10v4z'/>",
                                             "<path d='M0 0h24v24H0z' fill='none'/>",
                                           "</svg>"].join('')
                        source: modified ? svg.arg("orange") : svg.arg("lightgray")
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
                                    parent.source = parent.svg.arg("orange")
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
                Column {
                    visible: expanded
                    anchors { left: parent.left; right: parent.right }
                    height: {
                        return details.height + messages.height + content._height
                    }
                    spacing: 10
                    // Details text
                    RowLayout {
                        id: details
                        anchors { left: parent.left; right: parent.right }
                        spacing: 10
                        Text {
                            font.pointSize: 10
                            elide: Text.ElideRight
                            text: tech
                        }

                        //TODO: Use spacer
                        Text {
                            font.pointSize: 10
                            elide: Text.ElideRight
                            text: "Encoding:"
                        }

                        ComboBox {
                            id: encoding
                            font.pointSize: 10
                            flat: true
                            currentIndex: {
                                var value = Model.value(file, "Encoding")
                                if (value === "UTF-8")
                                    return 0;
                                else if (value === "ISO-8859-1")
                                    return 1;
                                else if (value === "ISO-8859-2")
                                    return 2;
                                else if (value === "Local")
                                    return 3;

                                return -1;
                            }
                            textRole: "text"
                            model: [
                                { text: "UTF-8", value: "UTF-8", enabled: true},
                                { text: "ISO-8859-1", value: "8859-1", enabled: true},
                                { text: "ISO-8859-2", value: "8859-2", enabled: true},
                                { text: "Local", value: "Local", enabled: false}
                            ]

                            delegate: ItemDelegate {
                                width: encoding.width
                                text: modelData.text
                                font.weight: encoding.currentIndex === index ? Font.DemiBold : Font.Normal
                                highlighted: ListView.isCurrentItem
                                enabled: modelData.enabled
                            }

                            onActivated: {
                                Model.setValue(file, "Encoding", encoding.model[encoding.currentIndex].value)
                            }
                        }

                        //TODO: Use spacer
                        Text {
                            font.pointSize: 10
                            elide: Text.ElideRight
                            text: " "
                            Layout.fillWidth: true
                        }
                        Repeater {
                            model: ["Cue", "XMP", "aXML", "iXML"]
                            delegate: Item {
                                function buttonColor(hovered) {
                                    var field = modelData==="Cue"?"cuexml":modelData
                                    if (!Model.valid(file, field, Model.value(file, field)))
                                       return hovered?"darkred":root.red
                                    else if (Model.lastValidationWarning(file).length > 0)
                                        return hovered?"darkorange":"orange"
                                    else if (Model.modified(file, field))
                                        return hovered?root.darkgreen:root.green
                                    else if (Model.visible(file, field))
                                        return hovered?"dimgray":"gray"
                                    else
                                        return "lightgray"
                                }
                                width: text.width + icon.width
                                height: text.height
                                Text {
                                    id: text
                                    text: modelData
                                    color: parent.buttonColor(false)
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                Image {
                                    property var svg: ["data:image/svg+xml;utf8,",
                                        "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                        "<path d='M19 19H5V5h7V3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2v-7h-2v7zM14 3v2h3.59l-9.83 9.83 1.41 1.41L19 6.41V10h2V3h-7z'/>",
                                        "<path d='M0 0h24v24H0z' fill='none'/>",
                                        "</svg>"].join('')
                                    id: icon
                                    width: 16
                                    height: 16
                                    source: svg.arg(parent.buttonColor(false))
                                    fillMode: Image.PreserveAspectFit
                                    anchors { top: text.top; left: text.right; topMargin: (text.height - height) / 2 }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    acceptedButtons: Qt.LeftButton
                                    onClicked: {
                                        if(Model.visible(file, modelData)) {
                                            Model.editField(file, modelData)
                                            text.color = parent.buttonColor(false)
                                            icon.source = icon.svg.arg(parent.buttonColor(false))
                                        }
                                    }
                                    onEntered: {
                                        if(Model.visible(file, modelData)) {
                                            text.color = parent.buttonColor(true)
                                            icon.source = icon.svg.arg(parent.buttonColor(true))
                                        }
                                    }
                                    onExited: {
                                        if(Model.visible(file, modelData)) {
                                            text.color = parent.buttonColor(false)
                                            icon.source = icon.svg.arg(parent.buttonColor(false))
                                        }
                                    }
                                    ToolTip {
                                        property var valid: Model.valid(file, modelData=="Cue"?"cuexml":modelData, Model.value(file, modelData=="Cue"?"cuexml":modelData))
                                        property var message: valid ? Model.lastValidationWarning(file) : Model.lastValidationError(file)
                                        visible: message.length > 0 && parent.containsMouse
                                        delay: 500
                                        // Basic wrapping of text since QML ToolTip Doesn't have option for that
                                        text: message.replace(/(?![^\n]{1,40}$)([^\n]{1,40})\s/g, '$1\n')
                                        background: Rectangle {
                                            border.color: parent.valid ? "orange" : root.red
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        id: messages
                        anchors { left: parent.left; right: parent.right }
                        spacing: 5
                        Text {
                           id:errors
                           text: "<h4><font color='" + root.red + "'>Errors:</font></h4>" + "<font color='" + root.red + "'>" + Model.errors(file) + "</font>"
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                  // use widget width instead and give room for the scroll bar
                           visible: Model.errors(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Text {
                           id:warnings
                           text: "<h4><font color='darkorange'>Warnings:</font></h4>" + "<font color='darkorange'>" + Model.warnings(file) + "</font>"
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                  // use widget width instead and give room for the scroll bar
                           visible: Model.warnings(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Text {
                           id: informations
                           text: "<h4>Informations:</h4>" +  Model.informations(file)
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                  // use widget width instead and give room for the scroll bar
                           visible: Model.informations(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Text {
                           id: unsupported
                           text: {
                               var chunks = Model.unsupportedChunks(file);
                               var message = "<font color='orange'>";
                               if (chunks.split(' ').length > 1) {
                                   message += "Unsupported chunks are found (" + chunks + "), they are ignored but kept when saving the file."
                               }
                               else {
                                   message += "Unsupported chunk found (" + chunks + "), It's ignored but kept when saving the file."
                               }
                               message+="</font>"
                               return message;
                           }
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                              // use widget width instead and give room for the scroll bar
                           visible: Model.unsupportedChunks(file).length > 0
                           wrapMode: Text.Wrap
                        }
                    }
                    Repeater {
                        id: content
                        property real _height: {
                            var h = 47; // Add missing line in computed height (bug?) + 10px margin
                            for (var i = 0; i < count; i++) {
                                if (itemAt(i).visible)
                                    h += itemAt(i).height;
                            }
                            return h;
                        }
                        model: sections
                        Column {
                            anchors { left: parent.left; right: parent.right }
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
                                width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                       // use widget width instead and give room for the scroll bar
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
                                                color: Model.readOnly(file, name) ? "whitesmoke" : "transparent"
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
                                                readOnly: Model.readOnly(file, name)
                                                selectByMouse: true
                                                onTextChanged: {
                                                    if (focus && Model.valid(file, name, text)) {
                                                        Model.setValue(file, name, text)
                                                    }
                                                }
                                                Keys.onPressed: {
                                                    if (event.key === Qt.Key_Return && !multiline) {
                                                        event.accepted = true;
                                                    }
                                                }
                                                onActiveFocusChanged: {
                                                    if (activeFocus) {
                                                        Model.setSelected(file, name)
                                                    }
                                                    else {
                                                        Model.deselect(file, name)
                                                    }
                                                }
                                                MouseArea {
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    propagateComposedEvents: true
                                                    acceptedButtons: Qt.RightButton
                                                    onClicked: {
                                                        parent.forceActiveFocus();
                                                        if (Model.valid(file, name, parent.text)) {
                                                            Model.showCoreMenu(mapToGlobal(mouse.x, mouse.y), file, name);
                                                        }
                                                    }
                                               }
                                            }
                                        }
                                        Image {
                                            id: status
                                            property var valid: Model.valid(file, name, input.text)
                                            property var message: valid ? Model.lastValidationWarning(file) : Model.lastValidationError(file)
                                            visible: message.length > 0
                                            height: 24
                                            width: 24
                                            source: valid ? "qrc:///Image/Menu/Warning.svg" : "qrc:///Image/Menu/Error.svg"
                                            anchors.verticalCenter: title.verticalCenter
                                            MouseArea {
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                ToolTip {
                                                    visible: parent.containsMouse
                                                    delay: 500
                                                    // Basic wrapping of text since QML ToolTip Doesn't have option for that
                                                    text: status.message.replace(/(?![^\n]{1,40}$)([^\n]{1,40})\s/g, '$1\n')
                                                    background: Rectangle {
                                                        border.color: status.valid ? "orange" : root.red
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
                Rectangle {
                    anchors { left: parent.left; right: parent.right }
                    height: 2
                    color: "gainsboro"
                }
            }
        }
    }
}
