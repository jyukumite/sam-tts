OBJS = sam.o main.o

CC = gcc
STMCC = "c:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-gcc"
STMOBJCOPY = "c:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-objcopy"
STMOBJDUMP = "c:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-objdump"

CFLAGS = -Wall -O3 -ffunction-sections -fdata-sections
LFLAGS = 

STMCFLAGS = -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -nostartfiles -Wl,--gc-sections -T stm32g070.ld -lc -lnosys -specs=nosys.specs -specs=nano.specs

stm32g: $(OBJS)
	$(STMCC) $(STMCFLAGS) -o firmware.elf emain.c sam.c
	$(STMOBJCOPY) -O binary firmware.elf firmware.bin
	$(STMOBJDUMP) -xsSt firmware.elf >firmware.dump


sam: $(OBJS)
	$(CC) -o sam $(OBJS) $(LFLAGS)

test: sam
	./sam.exe "hello world, I'm talking to you." >test1.wav && md5sum test1.wav 
	./sam.exe "1234567890." >test2.wav&& md5sum test2.wav
	./sam.exe -phonetic "AY5 AEM EY TAO4LXKIHNX KAX4MPYUX4TAH" >testp5.wav&& md5sum testp5.wav
	./sam.exe -phonetic " /HEH3LOW2, /HAW AH YUX2 TUXDEY. AY /HOH3P YUX AH FIYLIHNX OW4 KEY." >testp6.wav&& md5sum testp6.wav
	./sam.exe -phonetic " /HEY2, DHIHS IH3Z GREY2T. /HAH /HAH /HAH.AYL BIY5 BAEK." >testp7.wav&& md5sum testp7.wav
	./sam.exe -phonetic " /HAH /HAH /HAH" >testp8.wav&& md5sum testp8.wav
	./sam.exe -phonetic " /HAH /HAH /HAH." >testp9.wav&& md5sum testp9.wav
	./sam.exe -phonetic ".TUW BIY5Y3,, OHR NAA3T - TUW BIY5IYIY., DHAE4T IHZ DHAH KWEH4SCHAHN." >testp10.wav&& md5sum testp10.wav
	./sam.exe -phonetic " IYIHEHAEAAAHAOOHUHUXERAXIX" >testp12.wav&& md5sum testp12.wav
	./sam.exe -phonetic " EYAYOYAWOWUW ULUMUNQ YXWXRXLX/XDX" >testp15.wav&& md5sum testp15.wav
	./sam.exe -speed 90 -pitch 50 -mouth 100 -phonetic " /HAALAOAO." >testp16.wav&& md5sum testp16.wav

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
