import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQuick.Shapes 1.0

import Qt.labs.settings 1.0

import OpenHD 1.0



BaseWidget {   
    id: vsiWidget
    width: 50
    height: 50
    defaultAlignment: 1
    defaultXOffset: -20
    defaultHCenter: false
    defaultVCenter: true

    visible: settings.show_vsi

    widgetIdentifier: "vsi_widget"

    hasWidgetDetail: true
    widgetDetailComponent: Column {
        Item {
            width: parent.width
            height: 32
            Text {
                text: qsTr("Transparency")
                color: "white"
                height: parent.height
                font.bold: true
                font.pixelSize: detailPanelFontPixels
                anchors.left: parent.left
                verticalAlignment: Text.AlignVCenter
            }
            Slider {
                id: vsi_opacity_Slider
                orientation: Qt.Horizontal
                from: .1
                value: settings.vsi_opacity
                to: 1
                stepSize: .1
                height: parent.height
                anchors.rightMargin: 0
                anchors.right: parent.right
                width: parent.width - 96
                // @disable-check M223
                onValueChanged: {
                    settings.vsi_opacity = vsi_opacity_Slider.value
                }
            }
        }

        Item {
            width: parent.width
            height: 32
            Text {
                text: qsTr("Range")
                color: "white"
                height: parent.height
                font.bold: true
                font.pixelSize: detailPanelFontPixels
                anchors.left: parent.left
                verticalAlignment: Text.AlignVCenter
            }
            Slider {
                id: vsi_max_Slider
                orientation: Qt.Horizontal
                from: 5
                value: settings.vsi_max
                to: 50
                stepSize: 5
                height: parent.height
                anchors.rightMargin: 0
                anchors.right: parent.right
                width: parent.width - 96
                // @disable-check M223
                onValueChanged: {
                    settings.vsi_max = vsi_max_Slider.value
                }
            }
        }
    }

    Glow {
        anchors.fill: widgetInner
        radius: 3
        samples: 17
        color: settings.color_glow
        opacity: settings.vsi_opacity
        source: widgetInner
    }

    Item {
        id: widgetInner
        anchors.fill: parent
        opacity: settings.vsi_opacity


        CircularGauge {
            id: gauge
            anchors.fill: parent
            antialiasing: true

            rotation: 270

            minimumValue: settings.vsi_max*-1
            maximumValue: settings.vsi_max

            value: OpenHD.vsi

            style: CircularGaugeStyle {
                labelInset: outerRadius * -.3
                minorTickmarkCount : 0
                // @disable-check M223
                labelStepSize : {settings.vsi_max/5}
                maximumValueAngle : 135
                minimumValueAngle : -135

                tickmark: Rectangle {
                    visible: styleData.value
                    implicitWidth: outerRadius * 0.05
                    antialiasing: true
                    implicitHeight: outerRadius * 0.09
                    color: settings.color_shape
                }

                tickmarkLabel:  Text {
                    rotation: 90
                    //font.pixelSize: Math.max(6, outerRadius * 0.1)
                    font.pixelSize: 9
                    text: styleData.value
                    color: settings.color_text
                    antialiasing: true
                }

                needle: Rectangle {
                    y: outerRadius * -.01 //this is the needle base closest to axis
                    implicitWidth: outerRadius * 0.05
                    implicitHeight: outerRadius
                    antialiasing: true
                    color: settings.color_text
                }

                function degreesToRadians(degrees) {
                    return degrees * (Math.PI / 180);
                }

                background: Canvas {
                    // @disable-check M223
                    onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();

                    ctx.beginPath();
                    ctx.strokeStyle = settings.color_shape
                    ctx.lineWidth = outerRadius * 0.02;

                    ctx.arc(outerRadius, outerRadius, outerRadius - ctx.lineWidth / 2,
                        degreesToRadians(valueToAngle(settings.vsi_max*-1) - 90),
                        degreesToRadians(valueToAngle(settings.vsi_max) - 90));
                    ctx.stroke();
                    }
                }
            }
        }
    }
}
