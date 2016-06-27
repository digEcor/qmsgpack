#!/usr/bin/env bash

# This script is intended to be executed by Jenkins.

# Bail on first error.
set -e

source /usr/local/lib/bash/jenkins.sh

if [ "$DIGECOR_TARGET" == "NV" ]; then
	enable_toolchain
fi

if [ -f Makefile ]; then
	make distclean
fi

qmake -makefile qmsgpack.pro
make
