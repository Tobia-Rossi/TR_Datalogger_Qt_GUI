NAME = "TR Datalogger"
VERSION = 1.0-4
AUTHOR = "Tobia Rossi"

DEFINES += APP_NAME=\"\\\"$${NAME}\\\"\"
DEFINES += APP_VERSION=\\\"$${VERSION}\\\"
DEFINES += APP_AUTHOR=\"\\\"$${AUTHOR}\\\"\"


QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Forms/AboutWindow.cpp \
    Forms/MainWindow.cpp \
    Main.cpp \

HEADERS += \
    Forms/AboutWindow.hpp \
    Forms/MainWindow.hpp

FORMS += \
    Forms/AboutWindow.ui \
    Forms/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = Icons/logo.ico

RESOURCES += \
	Resources/Resources_01.qrc
