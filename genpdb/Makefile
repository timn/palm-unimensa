#
# Makefile for genUMENpdb
#
# Copyright (C) 2002 by Tim Niemueller <tim@niemueller.de>
# Created on November 28th 2002
# Website: http://www.niemueller.de/software/palm/unimensa/
#

include ../UniMensa.mk

GENPDBVERSION=0.3

CC=gcc
CFLAGS=-static
LDFLAGS=-static

PROGS=genUMENpdb getUMENpdbUniDate
OBJECTS=genUMENpdb.o getUMENpdbUniDate.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

dist: clean all parsemensa.pl getmensa.pl run.sh README $(PROGS)
	mkdir genUMENpdb-$(GENPDBVERSION)
	cp genUMENpdb genUMENpdb-$(GENPDBVERSION)
	cp getUMENpdbUniDate genUMENpdb-$(GENPDBVERSION)
	cp parsemensa.pl genUMENpdb-$(GENPDBVERSION)
	cp getmensa.pl genUMENpdb-$(GENPDBVERSION)
	cp senddb.pl genUMENpdb-$(GENPDBVERSION)
	cp run.sh genUMENpdb-$(GENPDBVERSION)
	cp README genUMENpdb-$(GENPDBVERSION)
	cp *.c genUMENpdb-$(GENPDBVERSION)
	cp *.h genUMENpdb-$(GENPDBVERSION)
	cp Makefile genUMENpdb-$(GENPDBVERSION)
	tar cvfz genUMENpdb-$(GENPDBVERSION).tar.gz genUMENpdb-$(GENPDBVERSION)/
	rm -rf genUMENpdb-$(GENPDBVERSION)

upload:
	scp genUMENpdb-$(GENPDBVERSION).tar.gz $(SSH_USER)@$(SSH_HOST):$(SSH_PATH)

all: $(PROGS)

clean:
	-rm -f $(PROGS) *.o
