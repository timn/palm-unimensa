#!/usr/bin/perl
#
# $Id: getmensa.pl,v 1.1 2003/02/10 22:45:50 tim Exp $
#
# Retrieve Mensa plan from website


use strict;
use HTTP::Lite;

my $filename="mensa.raw";

my %postvars = ( "elec_Wann" => "NaechsteWoche", # DieseWoche für diese Woche...
                 "newcoys" => "::",
                 "newspdfs" => "::",
                 "allec2" => "1",
                 "elec_Mensa1" => "1",
                 "elec_Mensa2" => "1",
                 "elec_MensaM6" => "1",
                 "elec_Mensa5" => "1",
                 "elec_Cafeteria1" => "1",
                 "elec_CafeteriaTemplergraben" => "1",
                 "elec_MensaVita" => "1",
                 "elec_Mensa3" => "1",
                 "elec_MensaJuelich" => "1"
               );

my %subst = ( "&ouml;" => "ö",
              "&Ouml;" => "Ö",
              "&auml;" => "ä",
              "&Auml;" => "Ä",
              "&uuml;" => "ü",
              "&Uuml;" => "Ü",
              "&szlig;" => "ß",
              "&euro;" => "EUR"
             );

my $http = new HTTP::Lite;
$http->prepare_post(\%postvars);

my $req = $http->request("http://www.studentenwerk-aachen.de/essen/speiseplan.asp");

if (! $req) {
  print "Cannot get file\n";
  exit;
}

my $body=$http->body();

for (keys %subst) {
  $body =~ s/$_/$subst{$_}/g;
}

open(FILE, ">$filename");
print FILE "Req: $req\n";
print FILE $body;
close(FILE);


