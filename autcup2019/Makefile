#############################################################################
# Makefile for building: autcup2019
# Generated by qmake (2.01a) (Qt 4.8.7) on: Fri Mar 1 10:53:37 2019
# Project:  autcup2019.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -o Makefile autcup2019.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQEXTSERIALPORT_USING_SHARED -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I/usr/include/qt4/QtExtSerialPort -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/arm-linux-gnueabihf `pkg-config opencv --libs` -lzbar -lqextserialport -lQtGui -lQtNetwork -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		capture.cpp \
		programhandeler.cpp \
		inverter.cpp \
		lineEquations.cpp \
		qr_reciever.cpp \
		handler.cpp \
		colordetection.cpp \
		udetection.cpp \
		optflow.cpp \
		quad_board.cpp \
		hdetection.cpp moc_mainwindow.cpp \
		moc_capture.cpp \
		moc_programhandeler.cpp \
		moc_inverter.cpp \
		moc_lineEquations.cpp \
		moc_qr_reciever.cpp \
		moc_handler.cpp \
		moc_colordetection.cpp \
		moc_optflow.cpp \
		moc_quad_board.cpp \
		moc_hdetection.cpp
OBJECTS       = main.o \
		mainwindow.o \
		capture.o \
		programhandeler.o \
		inverter.o \
		lineEquations.o \
		qr_reciever.o \
		handler.o \
		colordetection.o \
		udetection.o \
		optflow.o \
		quad_board.o \
		hdetection.o \
		moc_mainwindow.o \
		moc_capture.o \
		moc_programhandeler.o \
		moc_inverter.o \
		moc_lineEquations.o \
		moc_qr_reciever.o \
		moc_handler.o \
		moc_colordetection.o \
		moc_optflow.o \
		moc_quad_board.o \
		moc_hdetection.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/extserialport.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		autcup2019.pro
QMAKE_TARGET  = autcup2019
DESTDIR       = 
TARGET        = autcup2019

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: autcup2019.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/extserialport.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/arm-linux-gnueabihf/libqextserialport.prl \
		/usr/lib/arm-linux-gnueabihf/libQtGui.prl \
		/usr/lib/arm-linux-gnueabihf/libQtNetwork.prl \
		/usr/lib/arm-linux-gnueabihf/libQtCore.prl
	$(QMAKE) -o Makefile autcup2019.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/extserialport.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/arm-linux-gnueabihf/libqextserialport.prl:
/usr/lib/arm-linux-gnueabihf/libQtGui.prl:
/usr/lib/arm-linux-gnueabihf/libQtNetwork.prl:
/usr/lib/arm-linux-gnueabihf/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile autcup2019.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/autcup20191.0.0 || $(MKDIR) .tmp/autcup20191.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/autcup20191.0.0/ && $(COPY_FILE) --parents mainwindow.h capture.h includes.h programhandeler.h inverter.h lineEquations.h qr_reciever.h handler.h colordetection.h udetection.h optflow.h quad_board.h hdetection.h .tmp/autcup20191.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp capture.cpp programhandeler.cpp inverter.cpp lineEquations.cpp qr_reciever.cpp handler.cpp colordetection.cpp udetection.cpp optflow.cpp quad_board.cpp hdetection.cpp .tmp/autcup20191.0.0/ && (cd `dirname .tmp/autcup20191.0.0` && $(TAR) autcup20191.0.0.tar autcup20191.0.0 && $(COMPRESS) autcup20191.0.0.tar) && $(MOVE) `dirname .tmp/autcup20191.0.0`/autcup20191.0.0.tar.gz . && $(DEL_FILE) -r .tmp/autcup20191.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_capture.cpp moc_programhandeler.cpp moc_inverter.cpp moc_lineEquations.cpp moc_qr_reciever.cpp moc_handler.cpp moc_colordetection.cpp moc_optflow.cpp moc_quad_board.cpp moc_hdetection.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_capture.cpp moc_programhandeler.cpp moc_inverter.cpp moc_lineEquations.cpp moc_qr_reciever.cpp moc_handler.cpp moc_colordetection.cpp moc_optflow.cpp moc_quad_board.cpp moc_hdetection.cpp
moc_mainwindow.cpp: capture.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		mainwindow.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_capture.cpp: includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		capture.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) capture.h -o moc_capture.cpp

moc_programhandeler.cpp: includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		programhandeler.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) programhandeler.h -o moc_programhandeler.cpp

moc_inverter.cpp: includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		inverter.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) inverter.h -o moc_inverter.cpp

moc_lineEquations.cpp: includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		lineEquations.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) lineEquations.h -o moc_lineEquations.cpp

moc_qr_reciever.cpp: includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		qr_reciever.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) qr_reciever.h -o moc_qr_reciever.cpp

moc_handler.cpp: colordetection.h \
		udetection.h \
		handler.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) handler.h -o moc_handler.cpp

moc_colordetection.cpp: colordetection.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) colordetection.h -o moc_colordetection.cpp

moc_optflow.cpp: includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h \
		optflow.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) optflow.h -o moc_optflow.cpp

moc_quad_board.cpp: quad_board.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) quad_board.h -o moc_quad_board.cpp

moc_hdetection.cpp: hdetection.h
	/usr/lib/arm-linux-gnueabihf/qt4/bin/moc $(DEFINES) $(INCPATH) hdetection.h -o moc_hdetection.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		capture.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		capture.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

capture.o: capture.cpp capture.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o capture.o capture.cpp

programhandeler.o: programhandeler.cpp programhandeler.h \
		includes.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o programhandeler.o programhandeler.cpp

inverter.o: inverter.cpp inverter.h \
		includes.h \
		programhandeler.h \
		lineEquations.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o inverter.o inverter.cpp

lineEquations.o: lineEquations.cpp lineEquations.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		qr_reciever.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o lineEquations.o lineEquations.cpp

qr_reciever.o: qr_reciever.cpp qr_reciever.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		optflow.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qr_reciever.o qr_reciever.cpp

handler.o: handler.cpp handler.h \
		colordetection.h \
		udetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o handler.o handler.cpp

colordetection.o: colordetection.cpp colordetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o colordetection.o colordetection.cpp

udetection.o: udetection.cpp udetection.h \
		colordetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o udetection.o udetection.cpp

optflow.o: optflow.cpp optflow.h \
		includes.h \
		programhandeler.h \
		inverter.h \
		lineEquations.h \
		qr_reciever.h \
		quad_board.h \
		handler.h \
		colordetection.h \
		udetection.h \
		hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o optflow.o optflow.cpp

quad_board.o: quad_board.cpp quad_board.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o quad_board.o quad_board.cpp

hdetection.o: hdetection.cpp hdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o hdetection.o hdetection.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_capture.o: moc_capture.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_capture.o moc_capture.cpp

moc_programhandeler.o: moc_programhandeler.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_programhandeler.o moc_programhandeler.cpp

moc_inverter.o: moc_inverter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_inverter.o moc_inverter.cpp

moc_lineEquations.o: moc_lineEquations.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_lineEquations.o moc_lineEquations.cpp

moc_qr_reciever.o: moc_qr_reciever.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qr_reciever.o moc_qr_reciever.cpp

moc_handler.o: moc_handler.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_handler.o moc_handler.cpp

moc_colordetection.o: moc_colordetection.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_colordetection.o moc_colordetection.cpp

moc_optflow.o: moc_optflow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_optflow.o moc_optflow.cpp

moc_quad_board.o: moc_quad_board.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_quad_board.o moc_quad_board.cpp

moc_hdetection.o: moc_hdetection.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_hdetection.o moc_hdetection.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

