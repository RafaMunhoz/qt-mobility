QT += testlib
TEMPLATE=app
TARGET=tst_qversitdocument
CONFIG+=testcase

include(../../../common.pri)
DEFINES += BUILD_QTVERSIT QT_ASCII_CAST_WARNINGS

DEPENDPATH += .
INCLUDEPATH += \
    . \
    ../../ \
    ../../../src/versit \
    ../../../src/contacts \
    ../../../src/contacts/details \
    ../../../src/contacts/requests \
    ../../../src/contacts/filters

HEADERS += ut_qversitdocument.h
SOURCES += ut_qversitdocument.cpp

qtAddLibrary(QtContacts)
qtAddLibrary(QtVersit)

symbian: { 
    TARGET.CAPABILITY = ALL \
        -TCB
    LIBS += -lws32 \
        -lbafl
}