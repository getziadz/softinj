CC := g++

CFLAGS := -O3 -std=c++11

#SOFTINJ_DIR := /files/PhD/projects/elasticFidelity/code/softinj
SOFTINJ_DIR := ../

INCLUDES := $(SOFTINJ_DIR)/src/
LIB := $(SOFTINJ_DIR)/src/libsoftinj.a

APP := example
EXECUTABLES := 	$(APP)_vanilla \
		$(APP)_marked_v12 \
		$(APP)_marked_v11 \
		$(APP)_marked_v10 \
		$(APP)_marked_v9 \
		$(APP)_marked_v8 \
		$(APP)_marked_v7 \
		$(APP)_marked_v6 \
		$(APP)_marked_v5


all: $(EXECUTABLES)

##
## Compile
##

$(APP)_marked_v%: $(APP).cpp $(LIB) 
	$(CC) $^ -DSOFTINJ_STATUS=1 \
		 -DSOFTINJ_VOLTAGE=$* \
		 -DSOFTINJ_START_BIT=0 \
		 -DSOFTINJ_END_BIT=32 \
		 $(CFLAGS) \
		 -I$(INCLUDES) -o $@ $(LDFLAGS)

$(APP)_vanilla: $(APP).cpp $(LIB)
	$(CC) $^  $(CFLAGS) -I$(INCLUDES) -o $@ $(LDFLAGS)

##
## Execute
##

run_%: $(APP)_% 
	./$<

##
## Clean
##
clean:
	rm -rf *.o *~ $(EXECUTABLES)

