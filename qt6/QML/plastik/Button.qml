import QtQuick
import QtQuick.Templates as T

T.Button {
    id: control

    implicitwidth: implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    flat: false

    hoverEnabled: Qt.styleHints.useHoverEffect

    padding: 4
    spacing: 6

    iconWidth: 16
    iconHeight: 16

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.palette.buttonText

    }

     background: ButtonPanel {
        implicitWidth: 80
        implicitHeight: 24

        control: control
        visible: !control.flat || control.down || control.checked || control.highlighted || control.visualFocus
            || (enabled && control.hovered)
    }
}
