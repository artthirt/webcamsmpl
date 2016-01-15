TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
	utils/datastream.cpp \
	utils/asf_stream.cpp \
    pool_send.cpp \
    utils.cpp \
    opj_encode.cpp \
    jpeg_encode.cpp \
	utils/common.cpp

INCLUDEPATH += utils

DEFINES += _CV _JPEG

CXX = $$find(QMAKE_CXX, arm-unknown.*)

message($$CXX)

isEmpty(CXX){
	CONFIG += qt
	QT += core gui widgets

	UI_DIR += $$OUT_PWD/tmp/ui
	MOC_DIR += $$OUT_PWD/tmp/moc
	OBJECTS_DIR += $$OUT_PWD/tmp/obj
	RCC_DIR += $$OUT_PWD/tmp/rcc

	message("desktop")
}else{
	message("arm")
	CONFIG -= qt
	DEFINES += ARM
	INCLUDEPATH += "/opt/crosstools/boost/include" \
					/opt/crosstools/rpi_usr/include \
					"/opt/crosstools/opencv2/include" \
					/opt/crosstools/openjpeg/include
	LIBS +=	-L/opt/crosstools/boost/lib \
			-L/opt/crosstools/opencv2/lib \
			-L/opt/crosstools/openjpeg/lib \
			-L/oprt/crosstools/rpi_usr/lib
	LIBS += -lpthread -lrt
}

LIBS += -lopenjp2 -lboost_thread -lboost_system

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -ljpeg

HEADERS += \
	utils/datastream.h \
	utils/asf_stream.h \
	utils/common.h \
    utils.h \
    pool_send.h \
    widget.h \
    opj_encode.h \
    jpeg_encode.h
