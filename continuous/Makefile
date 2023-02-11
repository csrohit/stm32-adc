TARGET := adc

# debug build?Release
BUILD_TYPE = Release
BUILD_DIR:= build

TRIPLE  = 	arm-none-eabi
CC 		=	${TRIPLE}-gcc
LD 		= 	${TRIPLE}-ld
AS 		= 	${TRIPLE}-as
GDB 	= 	${TRIPLE}-gdb
OBJCOPY =  	${TRIPLE}-objcopy

INCFLAGS 	:= -Iinclude
CFLAGS 		:= -mcpu=cortex-m3 -mfloat-abi=soft -mthumb -nostdlib $(INCFLAGS) -std=gnu11 -Os -Wall -fstack-usage  -fdata-sections -ffunction-sections -DSTM32F103xB
LDFLAGS 	:= -mcpu=cortex-m3 -mfloat-abi=soft -mthumb -nostdlib $(INCFLAGS)

# add debug flags if build type is debug
ifeq ($(BUILD_TYPE), Debug)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP

SRC_DIR := src
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o) 

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -R .stack -O binary $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).elf: $(OBJS) stm32f1.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -T"stm32f1.ld" -Wl,-Map="$(BUILD_DIR)/$(TARGET).map" -Wl,--gc-sections
	arm-none-eabi-size $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC " $< " ==> " $@

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

all: $(BUILD_DIR)/$(TARGET).bin

clean:
	rm -rf $(BUILD_DIR)
