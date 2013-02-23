# -------------------------------------------------
# Project created by QtCreator
# -------------------------------------------------
QT += core
QT -= gui

TARGET = Tests
DESTDIR = ../../BIN/Tests
OBJECTS_DIR = ../../BIN/Tests/.obj
MOC_DIR = ../../BIN/Tests/.moc
RCC_DIR = ../../BIN/Tests/.rcc
UI_DIR = ../../BIN/Tests/.ui


CONFIG -= console
CONFIG -= app_bundle
TEMPLATE = app


########################### Warning options #####################################

QMAKE_CXXFLAGS_WARN_ON += -Werror
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
#################################################################################

INCLUDEPATH += "../../"

LIBS += -L"/usr/externallib/lib/"
INCLUDEPATH += "/usr/externallib/include/"
QMAKE_CXXFLAGS += -isystem "/usr/externallib/include/"

LIBS += -lboost_serialization
LIBS += -lboost_iostreams
LIBS += -lboost_graph
LIBS += -lboost_chrono
LIBS += -lboost_date_time
LIBS += -lboost_exception
LIBS += -lboost_filesystem
LIBS += -lboost_locale
LIBS += -lboost_math_c99
LIBS += -lboost_math_tr1
LIBS += -lboost_random
LIBS += -lboost_system
LIBS += -lboost_timer
LIBS += -lboost_thread

SOURCES += ../../SRC/Tests/*.cpp
SOURCES += ../../SRC/mainTests.cpp
HEADERS += ../../SRC/Tests/*.h
