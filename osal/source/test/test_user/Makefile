# Create by y00271809,2015/05/20
#*******************************************************
ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

#*******************************************************
SRCS := $(wildcard *.c)
TARGET := test

LIB := ../kernel_release/libdrv.a
LIB += ../../kernel/libhi_osal.a
LIB += ../c-unit/libcunit.a

INC := -I ../kernel_release/
INC += -I ../../../include
INC += -I ../c-unit/Headers/
#*******************************************************
CFLAGS += $(INC)
CFLAGS += -DHICHIP=$(HICHIP)

LDFLAGS := $(LITEOS_LDFLAGS)
LIBDEP := --start-group $(LITEOS_LIBS) --end-group
OBJS  := $(SRCS:%.c=%.o)
BIN := $(TARGET).bin
MAP := $(TARGET).map

.PHONY : clean all drv

all: $(BIN) 

$(BIN):$(TARGET)
	@$(OBJCOPY) -O binary $(TARGET) $(BIN)

$(TARGET):$(OBJS)
	@$(LD) $(LDFLAGS) -Map=$(MAP) -o $(TARGET) $(OBJS) $(LIB) $(LIBDEP)
#@$(OBJDUMP) -d $@ > $@.asm

$(OBJS):%.o:%.c drv
	@$(CC) $(CFLAGS) -c $< -o $@

drv:
	@make -C ../test_kernel/

clean:
	@rm -f $(TARGET) $(BIN) $(MAP) *.asm
	@rm -f $(OBJS)
	@make -C ../test_kernel/ clean
