TARGET		= $(CURDIR)
OS		= $(shell cd ../..; pwd)
include target.cfg

TEST		= test-hid
TESTS		+= test-debug.bin test-task.bin test-timer.bin test-uart.bin
TESTS		+= philosophers.bin test-ram.bin
TESTS       += test-latency.bin test-tswitch.bin #test-fpu.bin
TESTS		+= test-dcache.bin #test-tcl.bin
#TESTS		+= dhrystone.bin linpack.bin whetstone.bin #livermore.bin
#TESTS		+= coremark.bin
#TESTS       += test-flash.bin test-sdhc-fast.bin print-fat32.bin test-spi.bin
#TESTS		+= test-usbdev.bin test-hid.bin

ifneq (,$(findstring NVCOM,$(CFLAGS)))
TESTS       += test-mac.bin test-eth.bin test-eth-mdb.bin
TESTS		+= test-tcp-serv.bin test-tcp-client.bin
TESTS		+= test-udp-serv.bin test-udp-client.bin
TESTS       += test-i2c.bin test-i2c-ad7416.bin test-uda1380.bin
TESTS		+= test-tlv320aic32.bin
endif

#ifneq (,$(findstring MCB03,$(CFLAGS)))
#TESTS		+= test-mcb-eth.bin test-tcp-serv.bin test-tcp-client.bin
#TESTS		+= test-spw.bin eth-to-swic.bin
#endif

ifneq (,$(findstring MC30SF6,$(CFLAGS)))
#TESTS		+= test-tcp-serv.bin test-tcp-client.bin
TESTS		+= test-usbic.bin
endif

OUTS		= $(TESTS:%.bin=%.elf)
SREC		= $(TESTS:%.bin=%.srec)
DIS		    = $(TESTS:%.bin=%.dis)

all:		$(OUTS) $(TESTS) $(SREC) $(DIS)
		$(SIZE) *.elf

$(OUTS) $(TESTS): startup.o libuos.a

clean:
		rm -rf *~ *.[oasi] *.elf *.bin *.srec *.lst *.dis *.map .deps $(MODULES)

load:		$(TEST).srec
		$(STRIP) $(TEST).elf
		mcprog -e2 $(TEST).elf 0x1fc10000

proxy:
		mcremote &

gdb:		$(TEST).elf
		mipsel-elf32-gdb -x gdbinit $(TEST).elf

insight:	$(TEST).elf
		mipsel-elf32-insight -x gdbinit $(TEST).elf &

include $(OS)/sources/rules.mak

###
