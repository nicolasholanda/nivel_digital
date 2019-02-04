# MPLAB IDE generated this makefile for use with GNU make.
# Project: teste.mcp
# Date: Wed Jun 20 09:39:04 2018

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

teste.cof : source_teste.o
	$(LD) /p18F4520 /l"C:\Program Files\Microchip\mplabc18\v3.47\lib" "source_teste.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"teste.cof" /M"teste.map" /W

source_teste.o : source_teste.c ../../../../../Program\ Files/Microchip/mplabc18/v3.47/h/delays.h ../../../../../Program\ Files/Microchip/mplabc18/v3.47/h/timers.h ../../../../../Program\ Files/Microchip/mplabc18/v3.47/h/stdlib.h lcd.h source_teste.c ../../../../../Program\ Files/Microchip/mplabc18/v3.47/h/p18f4520.h ../../../../../Program\ Files/Microchip/mplabc18/v3.47/h/p18cxxx.h ../../../../../Program\ Files/Microchip/mplabc18/v3.47/h/pconfig.h
	$(CC) -p=18F4520 "source_teste.c" -fo="source_teste.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "source_teste.o" "teste.cof" "teste.hex"

