<?php

/* $Id: rwth.php,v 1.4 2003/10/30 13:29:20 tim Exp $ */

$title="niemueller.de - palm software - UniMensa Database for RWTH Aachen";
$meta_desc="A software to keep mensa plans on your Palm.";
$meta_keyw="Palm, PalmOS, university, planer, mensa, beaming, semester, food, Mensaplan, RWTH Aachen, RWTH, Aachen";
include("$DOCUMENT_ROOT/header.inc.php");
include("$DOCUMENT_ROOT/left.inc.php");

include("$DOCUMENT_ROOT/software/palm/unimensa/rwth_db_info.inc.php");

$dbs=array( "RWTH Aachen (generated every friday for next week)" => "UniMensaDB-RWTH.pdb"
           );

$version="0.3";
$basename="UniMensa";

box_begin("Uni Mensa Databases", "Last modified ".date ("F d Y H:i:s.", getlastmod()));
?>

<h5>This page is available in German language only!</h5><br/><br/>

<table border="0" cellpadding="4">
 <tr>
  <td rowspan="2" valign="top"><a href="index.php"><img src="unimensa_big.png" border="0" alt="Uni Mensa"></a></td>
  <td><h3>Uni Mensa Datenbank f&uuml;r die RWTH Aachen</h3>
 </tr>
 <tr>
  <td><h4>W&ouml;chentliche Datenbank (Mensaplan)</h4></td>
 </tr>
</table>

<div class="maintext">
<h5>Was ist UniMensa?</h5>
UniMensa ist ein Programm um Mensa-Pläne auf einen PalmOS-Handheld zu syncen und mitzunehmen.

<h5>Was gibt's hier?</h5>
Hier gibt's jede Woche die aktuelle Datenbank. Sie wird stets freitags um 23:00 generiert.
Die Datenbank wird von der <a href="http://www.studentenwerk-aachen.de" target="_new">Seite des Studentenwerks Aachen</a>
generiert.
Das Programm wird offiziell <a href="http://www.studentenwerk-aachen.de/essen/speiseplaene.asp" target="_new">unterstützt</a> :-)

<h5>Was wird benötigt?</h5>
<li>PalmOS Handheld mit PalmOS Version 3.5 oder besser (z.B. Palm III(x|xe|c...), m5xx, m1xx)</li>
<li>UniMensa (gibt's <a href="index.php">hier</a>)</li>

<h5>Kurzanleitung</h5>
Zuerst UniMensa <a href="<?=$basename?>-<?=$version?>_dist/<?=$basename?>-<?=$version?>-de.prc">hier</a> herunterladen.
Diese Datei mit der Endung .prc ist das eigentliche Programm für den Palm. Dieses Muss dann auf
dem Palm Handheld installiert werden (etwa über den Palm Desktop, pilot-tools etc.)
Eine Anleitung dazu gibt's auf der <a href="index.php">UniMensa Seite</a> (Englisch).
Danach einfach wöchentlich die Datenbank auf dem PalmOS installieren. Dazu einfach
die <a href="UniMensaDB-RWTH.pdb">Palm Datenbank</a> herunterladen und mit
dem Palm Desktop, den pilot-tools oder ähnlichem installieren.
Die bestehende Datenbank wird automatisch überschrieben und muss nicht explizit gelöscht werden.
Nun kann das Programm auf dem Palm Handheld gestartet werden und zeigt den aktuellen Mensaplan an.

<h5>Mailingliste</h5>
Das <a href="http://www.rz.rwth-aachen.de" target="_new">Rechenzentrum</a> der RWTH Aachen hat freundlicherweise eine
Mailingliste eingerichtet. Zu dieser Mailingliste schickt der Datenbankgenerator wöchentlich die aktuelle Datenbank.
Das mag für manche doch einfacher sein, als jedes Mal auf die Website zu müssen. Auch Announcements über neue Versionen
von UniMensa wird es dort geben.<br/>
Interessierte können sich <a href="http://mailman.rwth-aachen.de/mailman/listinfo/unimensa" target="_new">hier</a> anmelden,
abmelden etc.

<h5>Datenbank</h5>
Die Datenbank ist eine PDB Datei die einfach auf dem PalmOS Handheld installiert wird. Siehe auch Kurzanleitung oben.
<br/><br/>
Datenbank: <a href="UniMensaDB-RWTH.pdb">UniMensaDB-RWTH.pdb</a> (<?=$DB_DATE?>, <?=filesize("UniMensaDB-RWTH.pdb")?> Bytes)

</div>

<?php
box_end();

?>

<? include("$DOCUMENT_ROOT/right.inc.php"); ?>
<? include("$DOCUMENT_ROOT/footer.inc.php"); ?>
</body>
</html>
