# Cross toolchain variables
# If these are not in your path, you can make them absolute.
XT_PRG_PREFIX = mipsel-linux-gnu-
CC = $(XT_PRG_PREFIX)gcc
LD = $(XT_PRG_PREFIX)ld

# uMPS2-related paths

# Simplistic search for the umps2 install. prefix. If you have umps2
# installed on some weird location, set UMPS2_DIR_PREFIX by hand.
ifneq ($(wildcard /usr/bin/umps2),)
    UMPS2_DIR_PREFIX = /usr
else
    UMPS2_DIR_PREFIX = /usr/local
endif

UMPS2_DATA_DIR = $(UMPS2_DIR_PREFIX)/share/umps2
UMPS2_INCLUDE_DIR = $(UMPS2_DIR_PREFIX)/include/umps2

# Compiler options
CFLAGS_LANG = -ffreestanding
CFLAGS_MIPS = -c -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32 -std=gnu11 -nostdlib
# rimosso -I/usr/local/include/umps2
CFLAGS = $(CFLAGS_LANG) $(CFLAGS_MIPS) -I$(UMPS2_INCLUDE_DIR) -Wall -O0

# Linker options
LDFLAGS = -G 0 -nostdlib -T $(UMPS2_DATA_DIR)/umpscore.ldscript

# Add the location of crt*.S to the search path
VPATH = $(UMPS2_DATA_DIR)

.PHONY : all clean

all : kernel.core.umps

kernel.core.umps : kernel
	umps2-elf2umps -k $<

# gli header non vanno compilati vanno sono inclusi!
#inserisco tutti i flag nella "macro" CFLAGS_MIPS" e chiamo la compilazione cono solo
# mipsel-linux-gnu-gcc $(GFLAGS_MIPS) nomefile.c

#per compilare
COMPILE = $(CC) $(CFLAGS)
#per trovare la cartella degli headers
H = ./header/
#per trovare la cartella dei source
S = ./src/

pcb.o : $(S)pcb.c $(H)const.h $(H)listx.h $(H)pcb.h $(H)types_rikaya.h
	$(COMPILE) $(S)pcb.c

asl.o : $(S)asl.c $(H)const.h $(H)listx.h $(H)pcb.h $(H)asl.h $(H)types_rikaya.h
	$(COMPILE) $(S)asl.c

utils.o: $(S)utils.c $(H)utils.h $(H)pcb.h
	$(COMPILE) $(S)utils.c

p1.5test_rikaya_v0.o : $(S)p1.5test_rikaya_v0.c $(H)const.h $(H)listx.h $(H)pcb.h
	$(COMPILE) $(S)p1.5test_rikaya_v0.c

init.o: $(S)init.c $(H)init.h $(H)types_rikaya.h $(H)utils.h $(H)pcb.h $(H)handler.h
	$(COMPILE) $(S)init.c

scheduler.o: $(S)scheduler.c $(H)listx.h $(H)utils.h $(H)pcb.h $(H)init.h
	$(COMPILE) $(S)scheduler.c

interrupt.o: $(S)interrupt.c $(H)interrupt.h $(H)scheduler.h
	$(COMPILE) $(S)interrupt.c

syscall.o: $(S)syscall.c $(H)syscall.h $(H)pcb.h $(H)scheduler.h
	$(COMPILE) $(S)syscall.c

handler.o: $(S)handler.c $(H)handler.h $(H)interrupt.h $(H)syscall.h $(H)types_rikaya.h
	$(COMPILE) $(S)handler.c

main.o: $(S)main.c $(H)init.h $(H)pcb.h $(H)scheduler.h
	$(COMPILE) $(S)main.c


#$(CFLAGS_MIPS)

kernel : pcb.o asl.o utils.o init.o scheduler.o interrupt.o syscall.o handler.o main.o p1.5test_rikaya_v0.o crtso.o libumps.o
#kernel : main.o p1.5test_rikaya_v0.o crtso.o libumps.o
	$(LD) -o $@ $^ $(LDFLAGS)

# linking command:
#	mipsel-linux-gnu-ld -o
#	kernel LISTEPCB.o
#	/usr/local/lib/umps2/crtso.o
#	/usr/local/lib/umps2/libumps.o -nostdlib -T /
#	usr/local/share/umps2/umpscore.ldscript

clean :
	-rm -f *.o kernel kernel.*.umps

# Pattern rule for assembly modules
%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<

