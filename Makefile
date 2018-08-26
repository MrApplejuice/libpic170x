# Unix makefile for building

chip ?= 16F1705
xtal_freq ?= 8000000

xc8 ?= /opt/microchip/xc8/*/bin/xc8
xc8_opts += \
	--chip=$(chip) \
	--double=24 \
	--float=24 \
	--opt=+asm,+asmfile,-speed,+space \
	--mode=free \
	-D_XTAL_FREQ=$(xtal_freq) \
	-DPIC$(chip)


main_target = install/libpic170x_$(chip)_$(xtal_freq).lpp
build_dir = build/$(chip)/$(xtal_freq)

header_files := \
	libpic170x.X/libpic170x/timer0.h \
	libpic170x.X/libpic170x/freq.h \
	libpic170x.X/libpic170x/io_control.h

install_header_dir := install/include/libpic170x/
install_header_files = \
	$(addprefix $(install_header_dir),$(notdir $(header_files)))

.PHONY: all doc clean

all: $(main_target) $(install_header_files)

clean:
	rm -rf build
	rm -rf doc/html/ doc/latex/

$(main_target): $(build_dir)/timer0.p1 $(build_dir)/io_control.p1 
	mkdir -p install/
	$(xc8) $(xc8_opts) --output=lpp -O$@ $^

$(install_header_dir):
	mkdir -p $(install_header_dir)

$(install_header_dir)%.h: libpic170x.X/libpic170x/%.h $(install_header_dir)
	cp -v $< $@

$(build_dir)/%.p1: libpic170x.X/%.c $(header_files)
	mkdir -p $(build_dir)
	$(xc8) $(xc8_opts) --pass1 -O$@ $<

doc:
	doxygen

