# Create by y00271809,2015/05/20
#*******************************************************
ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

#*******************************************************
SRCS := test_dev.c
TARGET := drv
REL_PATH := ../kernel_release/

INC := -I../../../include
#*******************************************************
CFLAGS += $(INC)
OBJS := $(SRCS:%c=%o)

LIB := lib$(TARGET).a

.PHONY: all clean

all: $(OBJS) $(LIB)
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'...\e[0;36;1m" 
	@echo -e "\e[0m" 
	@cp $(LIB) $(REL_PATH)
	@cp test_api.h $(REL_PATH)
	
$(OBJS): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	@$(RM) $(OBJS) $(LIB) $(REL_PATH)/$(LIB) $(REL_PATH)/test_api.h
