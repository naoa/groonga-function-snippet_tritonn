AM_CFLAGS =					\
	$(GROONGA_CFLAGS)			

AM_LDFLAGS =					\
	-avoid-version				\
	-module					\
	-no-undefined

LIBS =						\
	$(GROONGA_LIBS)				

functions_plugins_LTLIBRARIES = 
functions_plugins_LTLIBRARIES += snippet_tritonn.la

functions_plugins_SOURCE =			\
	snippet_tritonn.c

include snippet_tritonn.am
