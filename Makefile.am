lib_LIBRARIES = libmelinda.a

AM_CFLAGS = -std=c99
AM_CPPFLAGS = 

if DEBUG
AM_CPPFLAGS +=  
else 
AM_CPPFLAGS += -DNDEBUG
endif 

libmelinda_a_SOURCES = melinda.c tuplespace.c internal.c thread.c\
	  tuplespace.h internal.h thread.h melinda.h defines.h



