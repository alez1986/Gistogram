import QtQuick 2.12
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtCharts 2.3


Item {
  width: 1000
  height: 500

  FileDialog {
    id: loadFileDialog
    title: "Please choose a text file"
    folder: shortcuts.home

    onAccepted: {
      console.log("File URL: " + loadFileDialog.fileUrl)
      searchController.readFile(loadFileDialog.fileUrl)
      loadProgressBar.value = 0
      close()
    }
  }

  Connections {
    target: searchController

    function onReadFinished() {
      //console.log("Counting finished");
      loadButton.enabled = true
    }

    function onReadProgress(value) {
      //console.log("Progress changed", value);
      loadProgressBar.value = value
    }
  }

  RowLayout {
    anchors.fill: parent
    spacing: 10
    anchors.margins: 10

    ColumnLayout {
      spacing: 20

      Button {
        id: loadButton
        text: "Load file"

        Layout.alignment: Qt.AlignCenter

        onClicked: {
          loadButton.enabled = false
          loadFileDialog.open()
        }
      }

      ProgressBar {
        id: loadProgressBar
        from: 0
        to: 100
      }

      Text {
        id: textProgress
        text: loadProgressBar.value + " %"

        Layout.alignment: Qt.AlignRight
      }
    }

    ChartView {
      Layout.fillWidth: true
      Layout.fillHeight: true
      title: "Top 15 of the words frequency"
      legend.alignment: Qt.AlignBottom
      antialiasing: true

      BarSeries {
        id: wordSeries
        axisX: BarCategoryAxis {
          categories: searchController.category
        }

        axisY: ValueAxis {
          min: 0
          max: searchController.maximum
        }

        BarSet {
          label: loadFileDialog.fileUrl.toString() !== "" ?
                   loadFileDialog.fileUrl.toString().substring(1 + loadFileDialog.fileUrl.toString().lastIndexOf("/"))
                 : "Please choose a file"

          values: searchController.value
        }
      }
    }
  }
}
