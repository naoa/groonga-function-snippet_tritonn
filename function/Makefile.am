AM_CFLAGS =					\
	$(GROONGA_CFLAGS)			

AM_LDFLAGS =					\
	-avoid-version				\
	-module					\
	-no-undefined

LIBS =						\
	$(GROONGA_LIBS)				

function_plugins_LTLIBRARIES = 
function_plugins_LTLIBRARIES += snippet_tritonn.la

function_plugins_SOURCE =			\
	snippet_tritonn.c

include snippet_tritonn.am
