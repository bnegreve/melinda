CPP=g++

ifeq ($(ci), 1)
  CFLAGS+=-DCHECK_INTEGRITY
endif

ifeq ($(prof), 1)
  CFLAGS+=-pg
  LDFLAGS+=-pg
endif

ifdef ni
  CFLAGS+=-DNUM_INTERNALS_MACRO=$(ni)
else
  CFLAGS+=-DNUM_INTERNALS_MACRO=2
endif


ifeq ($(optim), 1)
  CFLAGS+=-O3 -DNDEBUG
endif

ifeq ($(debug), 1)
  CFLAGS+= -g3
else
  CFLAGS+= -O3 -DNDEBUG
endif


CFLAGS+= 
#CFLAGS= -g3 -pg

SRCS=MultiTuple.cpp Tuple.cpp 

all:libbelinda.a test-tuple

MultiTuple.o: MultiTuple.cpp MultiTuple.hpp Internal.hpp
	$(CPP) -c $(CFLAGS) -o $@ $<

TupleSpace.o: TupleSpace.cpp TupleSpace.hpp Tuple.hpp Internal.hpp
	$(CPP) -c $(CFLAGS) -o $@ $<

Tuple.o: Tuple.cpp Tuple.hpp
	$(CPP) -c $(CFLAGS) -o $@ $<

# test-tuple: MultiTuple.cpp MultiTuple.hpp Tuple.cpp Tuple.hpp Internal.hpp
# 	$(CPP) $(CFLAGS) -DTEST_TUPLE MultiTuple.cpp Tuple.cpp -lpthread -o test-tuple

test-tuple: TupleSpace.cpp TupleSpace.hpp Tuple.cpp Tuple.hpp Internal.hpp
	$(CPP) $(CFLAGS) -DTEST_TUPLE -DCHECK_INTEGRITY TupleSpace.cpp Tuple.cpp -lpthread -o test-tuple


libbelinda.a: TupleSpace.o Tuple.o
	ar -crs $@ $^

.PHONY clean:
	rm -rf *.o libbelinda.a test-tuple *~