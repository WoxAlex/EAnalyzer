# -------------------------------------------------
# Project created by QtCreator
# -------------------------------------------------
QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Analyzer
DESTDIR = ../../BIN/Analyzer
OBJECTS_DIR = ../../BIN/Analyzer/.obj
MOC_DIR = ../../BIN/Analyzer/.moc
RCC_DIR = ../../BIN/Analyzer/.rcc
UI_DIR = ../../BIN/Analyzer/.ui

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

SOURCES += ../../SRC/Analyzer/*.cpp \
    ../../SRC/Analyzer/Forms/*.cpp \
    ../../SRC/Analyzer/DataTypes/*.cpp \
    ../../SRC/mainAnalyzer.cpp

HEADERS += ../../SRC/Analyzer/*.h \
    ../../SRC/Analyzer/Forms/*.h \
    ../../SRC/Analyzer/DataTypes/*.h

FORMS += ../../SRC/Analyzer/Forms/*.ui
