TEMPLATE	= app
CONFIG		+= qt warn_on debug ming
unix {
	LIBS        += -L../../qcadguiqt/lib -lqcadguiqt -L../../qcadlib/lib -lqcad -L../../dxflib/lib -ldxf
}
win32 {
	LIBS        += ../../qcadguiqt/lib/qcadguiqt.lib ../../qcadlib/lib/qcad.lib ../../dxflib/lib/dxflib.lib
}
INCLUDEPATH += ../../dxflib/include ../../qcadlib/include ../../qcadguiqt/include
exists(../../ming-0.2a/libming.a) {
	LIBS        += -L../../ming-0.2a -lming
	INCLUDEPATH += ../../ming-0.2a
	DEFINES     += SWF_SUPPORT
}
HEADERS		= vec2web.h \
              graphicview.h
SOURCES		= vec2web.cpp \
              graphicview.cpp \
              main.cpp 
DESTDIR     = ..
TARGET		= vec2web
OBJECTS_DIR = obj
MOC_DIR     = moc