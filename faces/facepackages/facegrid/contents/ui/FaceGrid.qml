/*
    SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import org.kde.ksysguard.sensors as Sensors
import org.kde.ksysguard.faces as Faces
import org.kde.ksysguard.formatter as Formatter

GridLayout {
    id: grid

    property int columnCount
    property int autoColumnCount

    readonly property real preferredWidth: titleMetrics.width

    columns: columnCount > 0 ? columnCount : autoColumnCount

    columnSpacing: Kirigami.Units.largeSpacing
    rowSpacing: Kirigami.Units.largeSpacing

    Kirigami.Heading {
        id: heading
        Layout.fillWidth: true
        Layout.columnSpan: parent.columns

        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        text: root.controller.title
        visible: root.controller.showTitle && text.length > 0
        level: 2

        TextMetrics {
            id: titleMetrics
            font: heading.font
            text: heading.text
        }
    }

    Repeater {
        id: gridRepeater

        model: root.controller.highPrioritySensorIds

        FaceControl {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            Layout.preferredHeight: 0

            controller: root.controller
            sensors: [modelData]
            faceId: root.controller.faceConfiguration.faceId
        }
    }
}
