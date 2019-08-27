QT          += widgets multimedia

HEADERS     = \
              helper.h \
              widget.h \
              window.h \
    mos6502.h \
    mos6502disassembler.h \
    mainwindow.h \
    dvg.h \
    inputkeys.h
SOURCES     = \
              helper.cpp \
              main.cpp \
              widget.cpp \
              window.cpp \
    mos6502.cpp \
    mos6502disassembler.cpp \
    mainwindow.cpp \
    dvg.cpp \
    inputkeys.cpp

# install
target.path = /media/jmg/New450/src/asteroids
INSTALLS += target

FORMS += \
    mainwindow.ui
