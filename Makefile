OBJS = sam.o main.o

CC = gcc
CFLAGS = -Wall -Os
LFLAGS = 

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
