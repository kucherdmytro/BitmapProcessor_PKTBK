QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/
INCLUDEPATH += src/models
INCLUDEPATH += src/gui/misc/

SOURCES += \
        src/appengine.cpp \
        src/imageprocessor.cpp \
        src/main.cpp \
        src/models/filelistmodel.cpp

RESOURCES += qml.qrc \
    img.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/appengine.h \
    src/imageprocessor.h \
    src/models/filelistmodel.h

unix:!macx: LIBS += -L$$OUT_PWD/../bmpproc/ -lbmpproc

INCLUDEPATH += $$PWD/../bmpproc
DEPENDPATH += $$PWD/../bmpproc
