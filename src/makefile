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

pcb.o :
	$(COMPILE) pcb.c

asl.o :
	$(COMPILE) asl.c

utils.o:
	$(COMPILE) utils.c

p1.5test_rikaya_v0.o : pcb.o asl.o
	$(COMPILE) p1.5test_rikaya_v0.c

init.o: utils.o pcb.o
	$(COMPILE) init.c

scheduler.o: pcb.o init.c
	$(COMPILE) scheduler.c

interrupt.o: scheduler.o
	$(COMPILE) interrupt.c

syscall.o: scheduler.o
	$(COMPILE) syscall.c

handler.o: interrupt.o syscall.o utils.o
	$(COMPILE) handler.c

main.o: init.o pcb.o scheduler.o
	$(COMPILE) main.c


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

