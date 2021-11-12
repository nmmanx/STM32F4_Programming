PROJECT := P00_SWO

STARTUP_FILE := ./Libs/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f411xe.s
LINKER_SCRIPT := ./STM32F411VETX_FLASH.ld

PREFIX := arm-none-eabi

AS := $(PREFIX)-gcc -x assembler-with-cpp
CC := $(PREFIX)-gcc
LD := $(PREFIX)-gcc
OBJCOPY := $(PREFIX)-objcopy

SOURCES := $(STARTUP_FILE)
INCLUDES :=

include $(PROJECT)/Build.mk

OBJS := $(patsubst %.c, %.o, $(filter %.c, $(SOURCES)))
OBJS += $(patsubst %.s, %.o, $(filter %.s, $(SOURCES)))

CPPFLAGS := -DDEBUG $(INCLUDES)
ASFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
CFLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=gnu11 -specs=nano.specs -g -O0 -nostartfiles
LDFLAGS := -T "$(LINKER_SCRIPT)" -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -specs=nano.specs
LIBS := -lc -lm -lnosys

%.o:%.s
	$(AS) -c $(CPPFLAGS) $(ASFLAGS) $< -o $@

%.o:%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

all: $(PROJECT).bin

$(PROJECT).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(LIBS) $^ -o $(PROJECT).elf

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary -S $(PROJECT).elf $@

.PHONY: all clean
clean:
	rm -rf $(OBJS) *.bin *.elf