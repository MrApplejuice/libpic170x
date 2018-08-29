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


main_target = install/lib/libpic170x_$(chip)_$(xtal_freq).lpp
build_dir := build/$(chip)/$(xtal_freq)/
header_src_dir := libpic170x.X/libpic170x/

source_files := freq.c timer0.c io_control.c serial.c
header_files := \
	$(addprefix $(header_src_dir), \
		$(addsuffix .h,$(basename $(source_files))))

install_header_dir := install/include/libpic170x/
install_header_files = \
	$(addprefix $(install_header_dir),$(notdir $(header_files)))

library_version := $(shell cat VERSION)

.PHONY: all doc clean release check_version library

all: library $(install_header_files)

library: $(main_target)

clean:
	rm -rf build
	rm -rf install/doc/
	rm -rf install/include/
	rm -rf install/lib/

$(main_target): $(addprefix $(build_dir),$(addsuffix .p1,$(basename $(source_files))))
	mkdir -p $(dir $@)
	$(xc8) $(xc8_opts) --output=lpp -O$@ $^

$(install_header_dir):
	mkdir -p $(install_header_dir)

$(install_header_dir)%.h: $(header_src_dir)%.h $(install_header_dir)
	cp -v $< $@

$(build_dir)%.p1: libpic170x.X/%.c $(header_files)
	mkdir -p $(build_dir)
	$(xc8) $(xc8_opts) --pass1 -O$@ $<

doc:
	VERSION_NUMBER=$(library_version) doxygen

check_version:
	bash ./check_version.sh $(library_version)

release: check_version clean
	bash build_all.sh
	rm -rf install/release/*
	mkdir -p install/release/libpic170x
	cp -rv install/doc/html install/release/libpic170x/doc
	cp -rv install/lib install/release/libpic170x/lib
	cp -rv install/include install/release/libpic170x/include
	cd install/release && zip -r9T libpic170x_v$(library_version).zip libpic170x

