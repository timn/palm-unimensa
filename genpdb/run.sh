#!/bin/sh

cd /home/tim/niemueller.de/scripts
./getmensa.pl
./parsemensa.pl
./genUMENpdb
./senddb.pl

cp -f unimensa.pdb ../htdocs/software/palm/unimensa/UniMensaDB-RWTH.pdb
./getUMENpdbUniDate | awk -F "::" '{ printf("<?php\n$DB_UNI=\"%s\"\n$DB_DATE=\"%s\"\n?>", $1, $2); }' > ../htdocs/software/palm/unimensa/rwth_db_info.inc.php

# ls -l unimensa.pdb

