PROGRAMMER 	 = usbasp
PORT       	 = usb
PROG_MCU   	 = m328p	
AVRDUDE    	 = avrdude -c $(PROGRAMMER) -p $(PROG_MCU) -P $(PORT)

F_CPU        = 8000000
MCU          = atmega328p

ELF_IMAGE    = image.elf
TARGET_IMAGE = image.bin

SRC_DIR      = src/
INC_DIR      = include/
OBJ_DIR      = obj/

SRC_SOURCES  = $(shell ls $(SRC_DIR)*.c)
SRC_OBJS     = $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SRC_SOURCES:.c=.o))

OBJS 		 = $(SRC_OBJS)

DFLAGS  	 = -gdwarf-2
WFLAGS  	 = -Wall

CFLAGS 		 = $(WFLAGS)
CFLAGS 		+= -mmcu=$(MCU)
CFLAGS 		+= -DF_CPU=$(F_CPU)UL
CFLAGS 		+= -Os

INC_FLAGS 	 = -I $(INC_DIR) -I $(SRC_DIR)


all : $(TARGET_IMAGE) size

$(TARGET_IMAGE) : $(ELF_IMAGE)
	avr-objcopy -j .text -j .data -O ihex $(ELF_IMAGE) $(TARGET_IMAGE)

$(ELF_IMAGE) : $(OBJS)
	avr-gcc $(CFLAGS) -o $(ELF_IMAGE) $(OBJS)
	@echo "Created: $(ELF_IMAGE)"

$(OBJ_DIR)%.o : $(SRC_DIR)%.c | $(OBJ_DIR)
	avr-gcc -c $(CFLAGS) $(INC_FLAGS) $< -o $@

$(OBJ_DIR) :
	mkdir -p $@

size:
	@echo "Created: $(TARGET_IMAGE)"
	avr-size -C --mcu=$(MCU) $(ELF_IMAGE)

clean :
	rm -rf $(OBJ_DIR) $(ELF_IMAGE) $(TARGET_IMAGE)

flash: $(TARGET_IMAGE)
	$(AVRDUDE) -U flash:w:$(TARGET_IMAGE):i

fuses:
	$(AVRDUDE) -U lfuse:w:0xE2:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m

program: fuses flash


.PHONY : all size clean flash fuses program