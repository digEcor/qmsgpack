# Builds the qmsgpack package.

host=$(shell hostname)
ifeq ($(host), jdev)
    package_version := $(shell date +%Y%m%d)
else
    package_version := $(shell dige-gen-pkg-version ..)
endif

# Assumes pkg name same in NV and L control files.
package := $(shell grep ^Package ./control | cut -f2 -d\ )

ifeq ($(DIGECOR_TARGET), NV)
    ziparg :=
    control := control.NV
ifeq ($(DIGECOR_TARGET), CM)
    ziparg := 
    control := control.CM
endif
arch := $(shell grep ^Architecture $(control) | cut -f2 -d' ')

package_name := $(package)_$(package_version)_$(arch)
build := $(shell pwd)/$(package_name)
deb := $(shell pwd)/$(package_name).deb


all:
	# Clean up any previous build (staging) directory.
	rm -rf $(build)
	
	# Stage Debian-specific package files.
	mkdir -p $(build)/DEBIAN
	cp ./$(control) $(build)/DEBIAN/control
	cp ./conffiles $(build)/DEBIAN/conffiles
	cp ./postinst $(build)/DEBIAN/postinst
	cp ./prerm $(build)/DEBIAN/prerm
	chmod 755 $(build)/DEBIAN/postinst
	chmod 755 $(build)/DEBIAN/prerm

	# Stage qmsgpack files.
	mkdir -p $(build)/usr/include/qmsgpack
	mkdir -p $(build)/usr/include/qmsgpack/stream
	mkdir -p $(build)/usr/lib

	cp -m 644 -p ../src/msgpack.h $(build)/usr/include/qmsgpack/
	cp -m 644 -p ../src/endianhelper.h $(build)/usr/include/qmsgpack/
	cp -m 644 -p ../src/msgpackcommon.h $(build)/usr/include/qmsgpack/
	cp -m 644 -p ../src/msgpack_export.h $(build)/usr/include/qmsgpack/
	cp -m 644 -p ../src/stream/location.h $(build)/usr/include/qmsgpack/stream/
	cp -m 644 -p ../src/stream/tune.h $(build)/usr/include/qmsgpack/stream/
	
	cp -m 755 -p ../bin/libqmsgpack.so.1.0.0 $(build)/usr/lib/
	strip --strip-unneeded $(build)/usr/lib/libqmsgpack.so.1.0.0
	ln -f -s $(build)/usr/lib/libqmsgpack.so.1.0.0 $(build)/usr/lib/libqmsgpack.so
	ln -f -s $(build)/usr/lib/libqmsgpack.so.1.0.0 $(build)/usr/lib/libqmsgpack.so.1
	ln -f -s $(build)/usr/lib/libqmsgpack.so.1.0.0 $(build)/usr/lib/libqmsgpack.so.1.0

	# Build the .deb package.
	sed -i "s/__VERSION__/$(package_version)/g" $(build)/DEBIAN/control
	# User jenkins cannot chmod files to root, thus fakeroot.
	fakeroot dpkg-deb $(ziparg) -b $(build) $(deb) 2>&1 |\
		 grep -v 'dpkg-deb: warning:' | grep -v 'contains user-defined field'
	# chown --reference=./Makefile $(deb)
	rm -rf $(build)


clean:
	rm -rf $(build)
	rm -f $(deb)
	rm -f *.deb
	rm -f make.out
	


