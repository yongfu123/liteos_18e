ifeq ($(PARAM_FILE), )
     PARAM_FILE:=../../../../mpp/Makefile.param
     include $(PARAM_FILE)
endif

CC ?= $(CROSS)gcc
AR ?= $(CROSS)ar

base_name:= cunit
lib_so:= $(addsuffix .so,$(addprefix lib,$(base_name)))
lib_static := $(addsuffix .a,$(addprefix lib,$(base_name)))

CFLAGS += -Wall -fPIC
CFLAGS += -I./Headers
CFLAGS += $(LIBS_CFLAGS)
#CFLAGS += -DCUNIT_BUILD_TESTS

STATIC_FLAGS := rcv

DYNAMIC_FLAGS := -fPIC -shared
DYNAMIC_FLAGS += $(LIBS_LD_CFLAGS)


SRC := $(wildcard Sources/Automated/*.c)
SRC += $(wildcard Sources/Basic/*.c)
SRC += $(wildcard Sources/Console/*.c)
SRC += $(wildcard Sources/Framework/*.c)
SRC += $(wildcard Sources/Curses/*.c)

OBJS := $(SRC:%.c=%.o)


.PHONY: $(lib_so) $(lib_static) clean 

all: $(lib_static)

	
$(lib_so): $(OBJS)
	@$(CC) $(DYNAMIC_FLAGS) -o $@ $^

$(lib_static):$(OBJS)	
	@$(AR) $(STATIC_FLAGS) $(lib_static) $(OBJS) 

clean:
	@rm -rf $(OBJS) *.so *.a
