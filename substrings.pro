######################################################################
# pro file for complexity plot Wed Oct 19 14:59:16 2011
######################################################################

TEMPLATE = app
TARGET   = substrings

CONFIG   += console warn_on release
QT       -= gui
QT       += sql



DEFINES      += _SQL_    

!win32{

DEFINES                  += _APPNAME=\\\"$$TARGET\\\"
#QMAKE_CXXFLAGS           += -Werror 
#-std=c++0x

OBJECTS_DIR              = GeneratedFiles
UI_DIR                   = GeneratedFiles
MOC_DIR                  = GeneratedFiles
RCC_DIR                  = GeneratedFiles

}

win32{

DEFINES        += _APPNAME=\"$$TARGET\"

}

macx{

QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
CONFIG -= app_bundle
               
}


DEPENDPATH  +=  . \
                ./src 

INCLUDEPATH +=  . \
                ./src



HEADERS     +=  src/substrings.hpp \
		        src/Arguments.hpp
               
               
SOURCES     +=  src/main.cpp \
                src/substrings.cpp \
		        src/Arguments.cpp




QMAKE_CLEAN += $$TARGET logfile*.log *~

