QT -= gui

CONFIG -= app_bundle
CONFIG += c++0x console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.

HEADERS += \
    nmea/nmea_packet.h \
    nmea/nmea_zda.h \
    nmea/t_nmea.h
