#!/bin/bash
g++	-c	lab7_1.cpp
g++	-o	lab7_1	lab7_1.o		-lrt	-lpthread
sudo setcap cap_sys_resource=eip ./lab7_1
