EXTRA_DIST=gipslitemediaengine.cc gipslitemediaengine.h
if GIPS
nodist_libcricketsessionphone_la_SOURCES=gipsmediaengine.cc \
					 gipsstatsmonitor.cc
else
dist_libcricketsessionphone_la_SOURCES=linphonemediaengine.cc
endif

libcricketsessionphone_la_SOURCES = audiomonitor.cc \
				    channelmanager.cc \
				    voicechannel.cc \
				    call.cc \
				    phonesessionclient.cc 

noinst_HEADERS =                    audiomonitor.h \
				    channelmanager.h \
				    linphonemediaengine.h \
				    mediaengine.h \
				    phonesessionclient.h \
				    voicechannel.h \
                                    call.h \
				    mediachannel.h  \
				    codec.h

AM_CPPFLAGS = -DPOSIX $(ORTP_CFLAGS) $(ILBC_CFLAGS) $(MEDIASTREAMER_CFLAGS) $(GLIB_CFLAGS)
noinst_LTLIBRARIES = libcricketsessionphone.la
				    
