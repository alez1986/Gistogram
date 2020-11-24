QT += charts qml quick core concurrent

CONFIG += c++11

SOURCES += \
        main.cpp \
        SearchController.cpp

HEADERS += \
        SearchController.h

RESOURCES += qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin

!isEmpty(target.path): INSTALLS += target
