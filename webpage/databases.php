<?php

/* $Id: databases.php,v 1.7 2004/01/18 21:15:12 tim Exp $ */

$title="niemueller.de - palm software - UniMensa Databases";
$meta_desc="A software keep mensa plans on your Palm.";
$meta_keyw="Palm, PalmOS, university, planer, mensa, beaming, semester, food, Mensaplan";
include("$DOCUMENT_ROOT/header.inc.php");
include("$DOCUMENT_ROOT/left.inc.php");

$unis = array("rwth"   => "RWTH Aachen (generated every friday for next week)",
              "unido"  => "Universit&auml;t Dortmund",
              "tuda"   => "TU Darmstadt",
              );

$pdbs=array( "rwth"    => "UniMensaDB-RWTH.pdb",
             "unido"   => "http://home.arcor.de/thzide/unimensa/UniMensaDB-UniDo.pdb",
             "tuda"    => "http://www.jakewalk.de/unimensa/UniMensaDB-TUD.pdb",
           );

$links=array( "rwth"   => "rwth.php",
              "unido"  => "http://mensaplan.de.vu/",
              "tuda"   => "http://www.jakewalk.de/unimensa.html",
             );

heading_lastmod();
heading("UniMensa", "Menu Databases for UniMensa",
        "/software/palm/unimensa/unimensa_big.png", "index.php");
?>
<div class="maintext">
<h5>What do you find here</h5>
Here is a list of databases available for <a href="index.php">UniMensa</a>.
Read <a href="#deploy">below</a> how to deploy UniMensa to your university.

<h5>Databases</h5>
<table border="0" cellpadding="0" cellspacing="0" width="100%">
 <tr>
  <td><b>University/Description</b></td>
  <td></td>
  <td><b>Database</b></td>
  <td></td>
  <td><b>More</b></td>
 </tr>
<? foreach ($unis as $uni => $desc) { ?>
<? preg_match("/\/?([^\/]*)$/", $pdbs[$uni], $matches) ?>
 <tr>
  <td><?=$desc?></td>
  <td>&nbsp; &nbsp;</td>
  <td><a href="<?=$pdbs[$uni]?>"><?=$matches[1]?></a></td>
  <td>&nbsp; &nbsp;</td>
  <td><a href="<?=$links[$uni]?>"<? if ($uni != "rwth") { echo " rel=\"external\""; }?>>More Info</a></td>
 </tr>
<? } ?>
</table>

<h5 id="deploy">How to deploy UniMensa to your University</h5><br/>
<a href="index.php">UniMensa</a> is part of the
<a href="/uni/mensa/">Mensa Framework</a>. It describes a simple format
to make the menu plans of your university digitally public.
Right now there are several frontends available that users may use
to view the digital menu. For more information about the architecture,
the available clients (frontends for the user) and how to get your
university online please read about the
<a href="/uni/mensa/">Mensa Framework</a>.

</div>

<? include("$DOCUMENT_ROOT/right.inc.php"); ?>
<? include("$DOCUMENT_ROOT/footer.inc.php"); ?>
