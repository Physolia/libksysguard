/*
    SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2019 David Edmundson <davidedmundson@kde.org>
    SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
    SPDX-FileCopyrightText: 2019 Kai Uwe Broulik <kde@broulik.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import org.kde.ksysguard.sensors as Sensors
import org.kde.ksysguard.faces as Faces

import org.kde.quickcharts as Charts
import org.kde.quickcharts.controls as ChartControls

Faces.CompactSensorFace {
    id: root

    readonly property bool horizontalOrientation: controller.faceConfiguration.horizontalBars

    readonly property real minimumBarSize: {
        if ((horizontalFormFactor && !horizontalOrientation) || (verticalFormFactor && horizontalOrientation)) {
            return Kirigami.Units.smallSpacing
        } else {
            return 1
        }
    }

    Layout.minimumWidth: Math.max(horizontalFormFactor ? minimumBarSize * barChart.barCount : 0, defaultMinimumSize)
    Layout.minimumHeight: Math.max(verticalFormFactor ? minimumBarSize * barChart.barCount : 0, defaultMinimumSize)

    contentItem: ColumnLayout {
        BarChart {
            id: barChart
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            updateRateLimit: root.controller.updateRateLimit
            controller: root.controller
            spacing: root.minimumBarSize * 0.25
        }
        QQC2.Label {
            id: label
            visible: root.formFactor == Faces.SensorFace.Planar && root.controller.showTitle && text.length > 0
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            text: root.controller.title
        }
    }
}
