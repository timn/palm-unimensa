#!/usr/bin/perl

# $Id: parsemensa.pl,v 1.1.1.1 2003/02/10 22:45:50 tim Exp $

# UniMensa -> parsemensa.pl
# Create a text file that genUMENpdb can parse and create the Palm database
#
# The format of the file you output is:
#
# block
# ---
# block
# ...
#
# Where the first block MUST be:
# University name (not more than 15 characters!)
# Number of Mensas (an int, not more than 16)
# From Date (format: YYYY/MM/DD)
# To date (format as from date)
#
# The second block MUST be:
# The names of the mensen, must be exactly as many as written in the first block
# and they may be no longer than 15 characters
#
# Further blocks:
# One block for each mensa in the same order as in the second block.
# Each line is for one day. It must ALWAYS be 6 lines per mensa, no more, no less.
# This assumption is done to minimize work in genpdb so that we can get around an
# intelligent PDB generator :-)
# MAKE SURE, THAT YOUR MENU LINES ARE NO LONGER THAN 2047 characters!


use strict;
use HTML::TreeBuilder;
use Date::Calc qw(Today Week_of_Year Monday_of_Week Add_Delta_Days);

my $filename="mensa.raw";
my $outfile="mensa.pre";

my %repl=("<br>" => "::",
          "<.*?>" => "",
          "&ouml;" => "ö",
          "&Ouml;" => "Ö",
          "&Auml;" => "Ä",
          "&auml;" => "ä",
          "&Uuml;" => "Ü",
          "&uuml;" => "ü",
          "&szlig;" => "ß",
          "&quot;" => "\""
          );

open(FILE, ">$outfile");

my $tree = HTML::TreeBuilder->new; # empty tree
$tree->parse_file($filename);

my $table = $tree->look_down("_tag", "table");
$table = $table->look_down("_tag", "td");
$table = $table->look_down("_tag", "table");
my $tdmensa = $table->look_down("_tag", "td");

my @tables = $tdmensa->look_down("_tag", "table");
my @as = $tdmensa->look_down("_tag", "a",
                             sub {
                               return 1 if ($_[0]->{'href'} =~ /#\d\d?/);
                               return 0;
                             }
                            );

my @mensen=();
foreach my $a (@as) {
  # print $a->as_text(), "\n";
  push(@mensen, $a->as_text());
}

my $num_mensen=scalar(@mensen);

# Date is written based on assumption, otherwise we would have to mess around with fseek and ftell, but
# assumption should to it since we call the plan for next week...
my ($year, $month, $day) = Today();
my ($week, $year) = Week_of_Year($year, $month, $day);
if ($week < 52) {
  $week += 1;
} else {
  $week = 1;
  $year += 1;
}
my ($beg_year, $beg_month, $beg_day) = Monday_of_Week($week, $year);
my ($end_year, $end_month, $end_day) = Add_Delta_Days($beg_year, $beg_month, $beg_day, 5);

print FILE "RWTH Aachen\n$num_mensen\n$beg_year/$beg_month/$beg_day\n$end_year/$end_month/$end_day\n---\n";

#foreach my $m (@mensen) {
#  if ($m =~ /(.+)? -/) {
#    print FILE "$m :::";
#    $m = $1;
#  }
#  $m = substr($m, 0, 15);
#  print FILE "$m\n";
#}

# For better results/look print "by hand"
print FILE "Mensa I\n",
           "Mensa II\n",
           "Mensa III, VII\n",
           "Mensa V\n",
           "Mensa M6\n",
           "Mensa Vita\n",
           "Cafeteria I\n",
           "Cafe Templgr.\n",
           "Mensa IV, Jül.\n",
           "---\n";

my $i=0;
foreach my $t (@tables) {

  #print "\n", $mensen[$i++], "\n\n";
  my @tds = $t->look_down("_tag", "td");

  foreach my $td (@tds) {
    my $br = $td->look_down("_tag", "br");
    my $text=$td->as_HTML();
    chomp $text;
    for (keys %repl) {
      $text =~  s/$_/$repl{$_}/g;
    }
    # if ($a) {
    #  my $repl=$a->as_text();
    #  $text =~ s/$repl/\t$repl/g;
    #}
    if ($text !~  /\S\S (\d\d\.\d\d\.\d\d\d\d)/) {
      print FILE $text, "\n";
      # print length($text), "\n";
    }
  }

  print FILE "Nicht geöffnet.\n---\n";
}


# Now that we're done with it, we must destroy it.
$tree = $tree->delete;

close(FILE);

