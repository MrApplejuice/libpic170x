# Unix makefile for building

chip = 16F1705
xtal_freq ?= 8000000

xc8 ?= /opt/microchip/xc8/*/bin/xc8
xc8_opts += --chip=$(chip) --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space --mode=free -D_XTAL_FREQ=$(xtal_freq)

build_dir = build


header_files = \
	libpic170x.X/timer0.h \
	libpic170x.X/freq.h \

.PHONY: all doc

all: libpic170x.lpp

libpic170x.lpp: $(build_dir)/timer0.p1
	$(xc8) $(xc8_opts) --output=lpp -O$@ $^

$(build_dir)/%.p1: libpic170x.X/%.c $(header_files)
	mkdir -p $(build_dir)
	$(xc8) $(xc8_opts) --pass1 -O$@ $<

doc:
	doxygen

