<?php

/* $Id: databases.php,v 1.5 2004/01/18 21:06:58 tim Exp $ */

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

$genpdbver="0.3";

heading_lastmod();
heading("UniMensa", "Menu Databases for UniMensa",
        "/software/palm/unimensa/unimensa_big.png", "index.php");
?>
<div class="maintext">
<h5>What do you find here</h5>
Here will be a list of databases available for <a href="index.php">UniMensa</a>. Right that is only the RWTH Aachen but maybe
there are some other coders around the globe who would like to adapt my software to their university.


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

<h5>Developer kit:</h5><br/>
If you want to generate UniMensa databases for your university the following kit is for you!
It contains the scripts that I use to generate the database for RWTH Aachen. That are:<br/>
<ul>
<li>getmensa.pl: Gets webpage with next weeks plan</li>
<li>parsemensa.pl: Uses HTML::TreeBuilder to extract the real menu information from the file leeched with getmensa.pl</li>
<li>genUMENpdb: a statically linked version of the PDB generator that creates the real database. Right now I do not publish
the source code. That may come with a later release when everything has stabilized. This binary was compiled on a pretty
clean RedHat 8 machine with GCC 3. It is plain C so there shouldn't be many problems. I have plans to make the generator
available as a web or SOAP app. If you really have plans to generate databases and you <i>can code</i> and are willing to
do some work just <a href="mailto:tim@niemueller.de?Subject=UniMensa PDB Generator">mail</a> me and we will find a
solution.</li>
</ul>

Here is the file: <a href="genUMENpdb-<?=$genpdbver?>.tar.gz">genUMENpdb-<?=$genpdbver?>.tar.gz</a><br/><br/>

</div>

<? include("$DOCUMENT_ROOT/right.inc.php"); ?>
<? include("$DOCUMENT_ROOT/footer.inc.php"); ?>
