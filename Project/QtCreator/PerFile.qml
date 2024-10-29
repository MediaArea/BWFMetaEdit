import QtQuick 2.9
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.3

Control {
    id: root
    readonly property color red: "#f44336"
    readonly property color green: "#4caf50"
    function alternate(color, mode) {
        if (!mode) {
            return color
        }
        else if (root.Material.theme === Material.Dark) {
            return Qt.lighter(color, 1.25)
        }
        return Qt.darker(color, 1.25)
    }
    visible: true
    Material.theme: Material.System
    Material.accent: "dodgerblue"
    Rectangle
    {
        id: background
        anchors.fill: parent
        color: parent.Material.background
        border.color: parent.Material.background
    }
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
                ListElement{ name: "IDIT"; label: "Digitization date (IDIT)"; fieldLength: 185; fieldHeight: 32; multiline: true },
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
                    Button {
                        implicitHeight: expand_icon.height
                        implicitWidth: expand_icon.width
                        bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                        bottomPadding: 0; rightPadding: 0; leftPadding: 0; topPadding: 0
                        flat: true
                        hoverEnabled: true
                        onClicked: {
                            expanded = !expanded
                        }
                        ToolTip {
                            visible: (parent.hovered || parent.activeFocus)
                            timeout: 5000
                            delay: 1000
                            text: expanded ? "Collapse" : "Expand"
                        }
                        Image {
                            id: expand_icon
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
                            source: {
                                return expanded ? collapseSvg.arg(root.alternate("gray", parent.hovered || parent.activeFocus)) : expandSvg.arg(root.alternate("gray", parent.hovered || parent.activeFocus))
                            }
                            anchors { top: parent.top; right: parent.right; topMargin: (parent.height - height) / 2 }
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    Label {
                        id: filename
                        text: "<b>" + file + "</b>"
                        elide: Text.ElideMiddle
                        Layout.fillWidth: true
                    }
                    Button {
                        implicitHeight: editmode_icon.height
                        implicitWidth: editmode_icon.width
                        bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                        bottomPadding: 0; rightPadding: 0; leftPadding: 0; topPadding: 0
                        flat: true
                        hoverEnabled: true
                        onClicked: {
                            if (Model.isWritable(file)) {
                                editMode = !editMode
                            }
                        }
                        ToolTip {
                            visible: (parent.hovered || parent.activeFocus) && Model.valid(file)
                            timeout: 5000
                            delay: 1000
                            text: Model.readOnly(file) ? "Edit mode disabled, file is read only!" : (editMode ? "Display Mode" : "Edit Mode")
                        }
                        Image {
                            id: editmode_icon
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
                            source: {
                                return Model.isWritable(file) ? (editMode ? displaySvg.arg(root.alternate("gray", parent.hovered || parent.activeFocus)) : editSvg.arg(root.alternate("gray", parent.hovered || parent.activeFocus))) : editSvg.arg("lightgray")
                            }
                            anchors { top: parent.top; right: parent.right; topMargin: (parent.height - height) / 2 }
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    Button {
                        implicitHeight: save_icon.height
                        implicitWidth: save_icon.width
                        bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                        bottomPadding: 0; rightPadding: 0; leftPadding: 0; topPadding: 0
                        flat: true
                        hoverEnabled: true
                        onClicked: {
                            if (modified) {
                                Model.saveFile(file)
                            }
                        }
                        ToolTip {
                            visible: (parent.hovered || parent.activeFocus) && Model.valid(file)
                            timeout: 5000
                            delay: 1000
                            text: modified ? "Save" : "Saved"
                        }
                        Image {
                            id: save_icon
                            property var svg: ["data:image/svg+xml;utf8,",
                                               "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                                 "<path d='M17 3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2V7l-4-4zm-5 16c-1.66 0-3-1.34-3-3s1.34-3 3-3 3 1.34 3 3-1.34 3-3 3zm3-10H5V5h10v4z'/>",
                                                 "<path d='M0 0h24v24H0z' fill='none'/>",
                                               "</svg>"].join('')
                            source: {
                                if (modified) {
                                    return svg.arg(root.alternate("orangered", parent.hovered || parent.activeFocus))
                                }
                                return svg.arg("lightgray")
                            }
                            anchors { top: parent.top; right: parent.right; topMargin: (parent.height - height) / 2 }
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    Button {
                        implicitHeight: close_icon.height
                        implicitWidth: close_icon.width
                        bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                        bottomPadding: 0; rightPadding: 0; leftPadding: 0; topPadding: 0
                        flat: true
                        hoverEnabled: true
                        onClicked: {
                            Model.closeFile(file)
                        }
                        ToolTip {
                            visible: (parent.hovered || parent.activeFocus) && Model.valid(file)
                            timeout: 5000
                            delay: 1000
                            text: "Close"
                        }
                        Image {
                            id: close_icon
                            property var svg: ["data:image/svg+xml;utf8,",
                                               "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                                 "<path d='M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z'/>",
                                                 "<path d='M0 0h24v24H0z' fill='none'/>",
                                               "</svg>"].join('')
                            source: {
                                return svg.arg(root.alternate("gray", parent.hovered || parent.activeFocus))
                            }
                            anchors { top: parent.top; right: parent.right; topMargin: (parent.height - height) / 2 }
                            fillMode: Image.PreserveAspectFit
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
                        Label {
                            font.pointSize: 10
                            elide: Text.ElideRight
                            text: tech
                        }
                        Label {
                            font.pointSize: 10
                            elide: Text.ElideRight
                            text: "Encoding:"
                        }
                        ComboBox {
                            id: encoding
                            font.pointSize: 10
                            flat: true
                            bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                            bottomPadding: 4; rightPadding: 4; leftPadding: 4; topPadding: 4
                            currentIndex: {
                                var value = Model.value(file, "Encoding")
                                if (value === "UTF-8")
                                    return 0;
                                else if (value === "CP437")
                                    return 1;
                                else if (value === "CP850")
                                    return 2;
                                else if (value === "CP858")
                                    return 3;
                                else if (value === "CP1252")
                                    return 4;
                                else if (value === "ISO-8859-1")
                                    return 5;
                                else if (value === "ISO-8859-2")
                                    return 6;
                                else if (value === "Local")
                                    return 7;
                                return -1;
                            }
                            textRole: "text"
                            model: [
                                { text: "UTF-8", value: "UTF-8", enabled: true},
                                { text: "IBM CP437", value: "CP437", enabled: true},
                                { text: "IBM CP850", value: "CP850", enabled: true},
                                { text: "IBM CP858", value: "CP858", enabled: true},
                                { text: "Windows 1252", value: "CP1252", enabled: true},
                                { text: "ISO-8859-1", value: "8859-1", enabled: true},
                                { text: "ISO-8859-2", value: "8859-2", enabled: true},
                                { text: "Local", value: "Local", enabled: false}
                            ]
                            delegate: ItemDelegate {
                                width: encoding.width
                                text: modelData.text
                                background: Rectangle {
                                    color: highlighted ? root.Material.accent : root.Material.background
                                }
                                font.weight: encoding.currentIndex === index ? Font.DemiBold : Font.Normal
                                highlighted: ListView.isCurrentItem
                                enabled: modelData.enabled
                            }
                            onActivated: {
                                Model.setValue(file, "Encoding", encoding.model[encoding.currentIndex].value)
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Repeater {
                            model: ["Cue", "XMP", "aXML", "iXML"]
                            delegate: Button {
                                function buttonColor(hovered) {
                                    var field = modelData==="Cue"?"cuexml":modelData
                                    if (!Model.valid(file, field, Model.value(file, field)))
                                        return root.alternate("red", hovered)
                                    else if (Model.lastValidationWarning(file).length > 0)
                                        return root.alternate("orangered", hovered)
                                    else if (Model.modified(file, field))
                                        return root.alternate(root.green, hovered)
                                    else if (Model.visible(file, field))
                                        return root.alternate("gray", hovered)
                                    else
                                        return "lightgray"
                                }
                                implicitHeight: icon.height
                                implicitWidth: text.width + icon.width
                                bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                                bottomPadding: 0; rightPadding: 0; leftPadding: 0; topPadding: 0
                                flat: true
                                hoverEnabled: true
                                onClicked: {
                                    if(Model.visible(file, modelData)) {
                                        Model.editField(file, modelData)
                                    }
                                }
                                ToolTip {
                                    property var valid: Model.valid(file, modelData=="Cue"?"cuexml":modelData, Model.value(file, modelData=="Cue"?"cuexml":modelData))
                                    property var message: valid ? Model.lastValidationWarning(file) : Model.lastValidationError(file)
                                    visible: message.length > 0 && (parent.hovered || parent.activeFocus)
                                    delay: 500
                                    // Basic wrapping of text since QML ToolTip Doesn't have option for that
                                    text: message.replace(/(?![^\n]{1,40}$)([^\n]{1,40})\s/g, '$1\n')
                                    background: Rectangle {
                                        color: root.Material.background
                                        border.color: parent.valid ? "orangered" : root.red
                                    }
                                }
                                Label {
                                    id: text
                                    text: modelData
                                    color: parent.buttonColor(parent.hovered || parent.activeFocus)
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
                                    source: svg.arg(parent.buttonColor(parent.hovered || parent.activeFocus))
                                    fillMode: Image.PreserveAspectFit
                                    anchors { top: text.top; left: text.right; topMargin: (text.height - height) / 2 }
                                }
                            }
                        }
                    }
                    Column {
                        id: messages
                        anchors { left: parent.left; right: parent.right }
                        spacing: 5
                        Label {
                           id:errors
                           text: "<h4><font color='" + root.red + "'>Errors:</font></h4>" + "<font color='" + root.red + "'>" + Model.errors(file) + "</font>"
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                  // use widget width instead and give room for the scroll bar
                           visible: Model.errors(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Label {
                           id:warnings
                           text: "<h4><font color='darkorange'>Warnings:</font></h4>" + "<font color='darkorange'>" + Model.warnings(file) + "</font>"
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                  // use widget width instead and give room for the scroll bar
                           visible: Model.warnings(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Label {
                           id: informations
                           text: "<h4>Informations:</h4>" +  Model.informations(file)
                           width: root.width - 20 // Can't refer to parent.width dues to bug,
                                                  // use widget width instead and give room for the scroll bar
                           visible: Model.informations(file).length > 0
                           wrapMode: Text.Wrap
                        }
                        Label {
                           id: unsupported
                           text: {
                               var chunks = Model.unsupportedChunks(file);
                               var message = "<font color='orangered'>";
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
                            Label {
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
                                                color: root.alternate(root.Material.background, input.readOnly)
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
                                                background: Rectangle {
                                                    color: "transparent"
                                                }
                                                bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                                                bottomPadding: 4; rightPadding: 4; leftPadding: 4; topPadding: 4
                                                color: root.alternate(root.Material.foreground, readOnly)
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
                                                    else if (event.key === Qt.Key_Alt) {
                                                        forceActiveFocus();
                                                        if (!Model.readOnly(file, name) && Model.valid(file, name, parent.text)) {
                                                            Model.showCoreMenu(mapToGlobal(x, y), file, name);
                                                        }
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
                                                        if (!Model.readOnly(file, name) && Model.valid(file, name, parent.text)) {
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
                                                    delay: 500
                                                    // Basic wrapping of text since QML ToolTip Doesn't have option for that
                                                    text: status.message.replace(/(?![^\n]{1,40}$)([^\n]{1,40})\s/g, '$1\n')
                                                    visible: text.length && (parent.containsMouse || parent.activeFocus || input.activeFocus)
                                                    background: Rectangle {
                                                        color: root.Material.background
                                                        border.color: status.valid ? "orangered" : root.red
                                                    }
                                                }
                                            }
                                        }
                                        Button {
                                            height: 24
                                            width: 24
                                            anchors.verticalCenter: title.verticalCenter
                                            bottomInset: 0; rightInset: 0; leftInset: 0; topInset: 0
                                            bottomPadding: 0; rightPadding: 0; leftPadding: 0; topPadding: 0
                                            flat: true
                                            hoverEnabled: true
                                            onClicked: Model.editField(file, name)
                                            Image {
                                                property var svg: ["data:image/svg+xml;utf8,",
                                                    "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' height='24' width='24' fill='%1'>",
                                                    "<path d='M19 19H5V5h7V3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2v-7h-2v7zM14 3v2h3.59l-9.83 9.83 1.41 1.41L19 6.41V10h2V3h-7z'/>",
                                                    "<path d='M0 0h24v24H0z' fill='none'/>",
                                                    "</svg>"].join('')
                                                source: {
                                                    return svg.arg(root.alternate("gray", parent.hovered || parent.activeFocus))
                                                }
                                                fillMode: Image.PreserveAspectFit
                                                anchors { top: parent.top; right: parent.right; topMargin: (parent.height - height) / 2 }
                                            }
                                            ToolTip {
                                                visible: (parent.hovered || parent.activeFocus)
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
