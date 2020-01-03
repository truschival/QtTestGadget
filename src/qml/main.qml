import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtMultimedia 5.9
import "Jsutil.js" as Util


ApplicationWindow {
    id: applicationWindow
    visible: true
    width: Style.canvasWidth;
    height: Style.canvasHeight;


    FontLoader {
        id: materialdesignIconsFont;
        source: "materialdesignicons-webfont.ttf"
    }

    menuBar: ToolBar {
        height: Style.toolbarHeight;
            Label {
                id: titleLabel
                text: "HALLO";
                elide: Label.ElideRight
                Layout.fillWidth: true
            }
        }

    Rectangle{
        id: aRectangle;
        color:"green";
        width: 50;
        height: 30;
    }

    Clock{
        id: currentTime
    }

    /**** global connections ****/
    Component.onCompleted: {
        console.log("main.qml completed")
    }
} // application window
