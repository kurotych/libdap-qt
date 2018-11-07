include (libdap/libdap.pri)
INCLUDEPATH += libdap/ libdap/common

SOURCES += $$PWD/DapConnectBase.cpp \
    $$PWD/DapSession.cpp \
    $$PWD/DapCrypt.cpp \
    $$PWD/DapKeyMsrln.cpp \
    $$PWD/DapLogger.cpp \
    $$PWD/DapKeyIaes.cpp


HEADERS += \
    $$PWD/DapConnectBase.h \
    $$PWD/DapSession.h \
    $$PWD/DapCrypt.h \
    $$PWD/InterfaceDapKey.h \
    $$PWD/DapKeyMsrln.h \
    $$PWD/DapLogger.h \
    $$PWD/DapKeyIaes.h


INCLUDEPATH += $$PWD


