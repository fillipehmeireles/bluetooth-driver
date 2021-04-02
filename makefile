# Makefile

all: bluetooth-driver

bluetooth-driver: main.c
	gcc main.c -o bluetooth-driver -lbluetooth

