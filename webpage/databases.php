<?php

/* $Id: databases.php,v 1.1 2003/02/10 22:45:50 tim Exp $ */

$title="niemueller.de - palm software - UniMensa Databases";
$meta_desc="A software keep mensa plans on your Palm.";
$meta_keyw="Palm, PalmOS, university, planer, mensa, beaming, semester, food, Mensaplan";
include("$DOCUMENT_ROOT/header.inc.php");
include("$DOCUMENT_ROOT/left.inc.php");

$unis = array("rwth" => "RWTH Aachen (generated every friday for next week)"
              );

$pdbs=array( "rwth" => "UniMensaDB-RWTH.pdb"
           );

$links=array( "rwth" => "rwth.php"
             );

$genpdbver="0.1";

box_begin("Uni Mensa Databases", "Last modified ".date ("F d Y H:i:s.", getlastmod()));
?>
<br/>

<table border="0" cellpadding="4">
 <tr>
  <td rowspan="2" valign="top"><a href="index.php"><img src="unimensa_big.png" border="0" alt="Uni Mensa"></a></td>
  <td><h3>Uni Mensa Databases</h3>
 </tr>
 <tr>
  <td><h4>Databases for UniMensa</h4></td>
 </tr>
</table>

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
 <tr>
  <td><?=$desc?></td>
  <td>&nbsp; &nbsp;</td>
  <td><a href="<?=$pdbs[$uni]?>"><?=$pdbs[$uni]?></a></td>
  <td>&nbsp; &nbsp;</td>
  <td><a href="<?=$links[$uni]?>">More Info</a></td>
 </tr>
<? } ?>
</table>

<h5>Developer kit:</h5><br/>
If you want to generate UniMensa databases for your university the following kit is for you!
It contains the scripts that I use to generate the database for RWTH Aachen. That are:<br/>
<li>getmensa.pl: Gets webpage with next weeks plan</li>
<li>parsemensa.pl: Uses HTML::TreeBuilder to extract the real menu information from the file leeched with getmensa.pl</li>
<li>genUMENpdb: a statically linked version of the PDB generator that creates the real database. Right now I do not publish
the source code. That may come with a later release when everything has stabilized. This binary was compiled on a pretty
clean RedHat 8 machine with GCC 3. It is plain C so there shouldn't be many problems. I have plans to make the generator
available as a web or SOAP app. If you really have plans to generate databases and you <i>can code</i> and are willing to
do some work just <a href="mailto:tim@niemueller.de?Subject=UniMensa PDB Generator">mail</a> me and we will find a
solution.</li>

Here is the file: <a href="genUMENpdb-<?=$genpdbver?>.tar.gz">genUMENpdb-<?=$genpdbver?>.tar.gz</a><br/><br/>

</div>

<?php
box_end();

?>

<? include("$DOCUMENT_ROOT/right.inc.php"); ?>
<? include("$DOCUMENT_ROOT/footer.inc.php"); ?>
</body>
</html>
