
CC := g++
AR := ar
LD := ld
CFLAGS := -std=c++11 -Wall -fPIC -Wextra -pedantic
LDFLAGS := 

SOURCES=$(wildcard *.cc)
SOURCES += $(wildcard bHiVEModels/*.cc)
OBJECTS=$(patsubst %.cc, %.o, $(SOURCES))
STATIC_LIBS= libsoftinj.a
SHARED_LIBS= libsoftinj.so

.PHONY: all clean

all: $(OBJECTS) $(STATIC_LIBS) $(SHARED_LIBS)

%.o: %.cc %.hh
	$(CC) $(CFLAGS) -c  $< -o $@ $(LDFLAGS)

libsoftinj.a: $(OBJECTS)
	$(AR) -cvq $@ $(OBJECTS)

libsoftinj.so: $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $@


clean:
	rm $(OBJECTS) $(STATIC_LIBS) $(SHARED_LIBS) *~
