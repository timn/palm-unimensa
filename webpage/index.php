<?php

/* $Id: index.php,v 1.5 2003/11/04 12:36:05 tim Exp $ */

$title="niemueller.de - palm software - UniMensa";
$meta_desc="A software to keep mensa plans on your Palm.";
$meta_keyw="Palm, PalmOS, university, planer, mensa, beaming, semester, food, Mensaplan";
include("$DOCUMENT_ROOT/header.inc.php");
include("$DOCUMENT_ROOT/left.inc.php");

$version="0.3";
$basename="UniMensa";
$languages=array( "de" => "German/Deutsch",
                  "en" => "English"
                );

$genpdbver = "0.1";

heading_lastmod();
heading("Uni Mensa",
        "Software to take the current mensa food plan with you",
        "unimensa_big.png");
?>
<div class="maintext">
<h5>Introduction</h5>
This PalmOS applications allows you to take the current mensa food plan with you.
You just need to write a software that will create a specially formatted text file that
genUMENpdb can parse (see package "UMENpdbGen" in the download section below).
<br/>
Some features:
<ul>
 <li>Multiple mensas: Up to 16 mensa can be taken.</li>
 <li>Beaming Support: Beam the current database to your friends</li>
</ul>

<br/>
Just to be sure that you heard: You will need a PalmOS device to run this
application. You can also use the Palm OS Emulator
<a href="http://www.palmos.com/dev/tools/emulator/" rel="external">POSE</a>.
Linux binaries can be found
<a href="http://sourceforge.net/project/showfiles.php?group_id=2189" rel="external">here</a>.
<br/>
<i>Do not ask for ROM files</i>. I cannot and I do not want to share
my ROMs with you. Go to the mentioned POSE page, there is a description
on how to obtain ROM files from Palm Source directly and legally
without owning a Palm handheld.
<br/><br/>
UniChat is available in multiple languages. Supported languages are right now (randomly ordered):
<ul>
 <li>German (Deutsch)</li>
 <li>English</li>
</ul>
If you want to help and translate UniMensa to another language please
<a href="mailto:tim@niemueller.de?Subject=Translate UniMensa">contact</a> me and I will
send you the language file.

<h5>Databases</h5>
My databases and later also databases created for other universities may be found <a href="databases.php">here</a>.

<?include("$DOCUMENT_ROOT/software/palm/sponsoring.inc.php");?>                                                                                

<h5>News</h5>
<b>2002/11/28</b><br/>
First public release.
If you find any bugs please send me a <a href="mailto:unimensa-bug@niemueller.de">mail</a> describing the
problem (please note OS and hardware you use).
<br/><b>2003/01/30</b><br/>
Fixed simple but annoying bug. It showed the wrong menu on startup when not monday.
<br/><b>2003/10/08</b><br/>
Several small improvements. Preferences for startup behavior (show last seen mensa,
set a default or show database default) and database info that shows for which
university the database belongs to.

<h5>Thanks</h5>
A special thanks to my friends Tobias "The Bugfinder" Weyand and Sumedha Ananda Markus Widyadharma (just had to write
whole name :-) who greatly helped to reduce my Bug rate in this
software (and others...). All remaining bugs are my fault. I probably inserted them after their checks...


<h5>Screenshots</h5>
Some screenshots will give you an impression of the software.
<br/><br/>
<table border="0" width="100%" cellpadding="0">
 <tr>
  <td align="center" style="width:33%;"><img src="screens/main.png" style="border:1px solid #c0c0c0;" alt="Main screen" /></td>
  <td align="center" style="width:34%;"><img src="screens/categories.png" style="border:1px solid #c0c0c0;" alt="Edit course" /></td>
  <td align="center" style="width:33%;"><img src="screens/about.png" style="border:1px solid #c0c0c0;" alt="Edit Time" /></td>
 </tr>
 <tr>
  <th>Main screen</th>
  <th>Multiple Mensas</th>
  <th>About Screen</th>
 </tr>
</table>

<h5>Supported devices</h5>
Any PalmOS handheld device running PalmOS 3.5 or higher should work for UniMensa.
<br/><br/>I have tested UniMensa on:<br/>
Palm m515 (PalmOS 4.1, color, tested on real device, works perfectly)<br/>
Palm IIIc (PalmOS 3.5, color, tested on real device, works perfectly)<br/>
Palm IIIx (PalmOS 4.0, gray, tested on real device, works perfectly)<br/>
Palm IIIx (PalmOS 3.5, gray, tested on emulator, works perfectly)<br/>

<h5>Download</h5>
UniMensa is released and distributed under the terms of a proprietary
license. You may use the software for free and beam it to your friend.
You may not distribute it under your name. You may distribute it on
so called "Shareware or Freeware CD-ROMs". You must then inform the author
<i>before</i> the CDs are published. The author has the right to deny distribution
of the software without further need of explanation.
<br/>
<span style="color: red;">By downloading and/or using the software you agree
to this license!</span>
<br/><br/>
You may download the Palm install file provided as a .zip and as a .tar.gz file.
<br/>Latest <b>stable</b> version is <span style="color:red;"><?=$version?></span>.
<br/><br/>
<table border="0" cellpadding="0" cellspacing="0">
<? foreach ($languages as $key => $value) { ?>
<?   if (file_exists("{$basename}-{$version}_dist/{$basename}-{$version}-{$key}.tar.gz")) { ?>
 <tr>
  <td><?=$basename?> <?=$version?> (<?=$value?>)</td>
  <td>&nbsp; &nbsp;</td>
  <td><a href="<?=$basename?>-<?=$version?>_dist/<?=$basename?>-<?=$version?>-<?=$key?>.tar.gz">.tar.gz</a> (<?=filesize("{$basename}-{$version}_dist/{$basename}-{$version}-{$key}.tar.gz")?> Bytes)</td>
  <td>&nbsp;</td>
  <td><a href="<?=$basename?>-<?=$version?>_dist/<?=$basename?>-<?=$version?>-<?=$key?>.zip">.zip</a> (<?=filesize("{$basename}-{$version}_dist/{$basename}-{$version}-{$key}.zip")?> Bytes)</td>
 </tr>
<?   } ?>
<? } ?>
</table>
<br/>
You can get the databases <a href="databases.php">here</a>.

<h5>Developer kit:</h5>
If you want to generate UniMensa databases for your university the following kit is for you!
It contains the scripts that I use to generate the database for RWTH Aachen. That are:
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
