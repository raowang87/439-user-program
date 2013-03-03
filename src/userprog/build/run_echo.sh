#!/bin/bash

make clean; make all
rm filesys.dsk
pintos-mkdisk filesys.dsk --filesys-size=2; pintos -f -q
pintos -p ../../examples/echo -a echo -- -q
