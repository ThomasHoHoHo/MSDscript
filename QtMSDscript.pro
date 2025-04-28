QT += widgets
CONFIG += c++17

TARGET = Assignment1QT
TEMPLATE = app

HEADERS += \
    mainwidget.h \
    expr.h \
    val.h \
    env.h \
    parse.hpp \
    pointer.h

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    expr.cpp \
    val.cpp \
    env.cpp \
    parse.cpp
