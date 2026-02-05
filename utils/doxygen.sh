#!/usr/bin/env bash

doxygen Doxyfile
cd latex/doxygen
make

cp refman.pdf ../../docs