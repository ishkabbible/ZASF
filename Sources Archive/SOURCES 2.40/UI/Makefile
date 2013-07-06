include ../Makefile.inc

%.c  : %.fl
	fluid -c $<

objects=WidgetPDial.o PresetsUI.o EnvelopeUI.o LFOUI.o FilterUI.o VirKeyboard.o ConfigUI.o\
	SUBnoteUI.o ResonanceUI.o OscilGenUI.o ADnoteUI.o PADnoteUI.o EffUI.o BankUI.o \
	PartUI.o MicrotonalUI.o SeqUI.o MasterUI.o 
ifeq ($(DISABLE_GUI),YES)
    objects=
endif

all: $(objects)

WidgetPDial.o: WidgetPDial.fl WidgetPDial.c WidgetPDial.h
PresetsUI.o: PresetsUI.fl PresetsUI.c PresetsUI.h
EnvelopeUI.o: EnvelopeUI.fl EnvelopeUI.c EnvelopeUI.h
LFOUI.o: LFOUI.fl LFOUI.c LFOUI.h
FilterUI.o: FilterUI.fl FilterUI.c FilterUI.h

ResonanceUI.o:ResonanceUI.fl ResonanceUI.c ResonanceUI.h
OscilGenUI.o:OscilGenUI.fl OscilGenUI.c OscilGenUI.h
ADnoteUI.o:ADnoteUI.fl ADnoteUI.c ADnoteUI.h
SUBnoteUI.o:SUBnoteUI.fl SUBnoteUI.c SUBnoteUI.h
PADnoteUI.o:PADnoteUI.fl PADnoteUI.c PADnoteUI.h

EffUI.o: EffUI.fl EffUI.c EffUI.h
BankUI.o: BankUI.fl BankUI.c BankUI.h
PartUI.o: PartUI.fl PartUI.c PartUI.h

VirKeyboard.o:  VirKeyboard.fl VirKeyboard.c VirKeyboard.h
ConfigUI.o: ConfigUI.fl ConfigUI.c ConfigUI.h
MicrotonalUI.o: MicrotonalUI.fl MicrotonalUI.c MicrotonalUI.h
SeqUI.o: SeqUI.fl SeqUI.c SeqUI.h
MasterUI.o:  MasterUI.fl MasterUI.c MasterUI.h

.PHONY : clean
clean: 
	rm -f $(objects)
	rm -f makeinclude.deps
	rm -f *.h *.c
