#!/usr/bin/perl

# $Id: senddb.pl,v 1.1 2003/02/10 22:45:50 tim Exp $
#
# Send current database to mailinglist

use strict;
use MIME::Lite;
use Net::SMTP;

### CONSTANTS
my $to='unimensa@lists.rwth-aachen.de';
my $to_name="UniMensa DB Mailinglist";
my $from='unimensa-rwth-db@niemueller.de';
my $from_name="UniMensa DB Generator";
my $domain='niemueller.de';
my $mail_host='localhost';
my $db_path="./unimensa.pdb";

my $version="0.2";

## program

my $uni_n_date = qx(./getUMENpdbUniDate);
my ($uni, $date) = split(/::/, $uni_n_date);

my $mail_text="UniMensa Datenbank\n
Sie erhalten diese email, da Sie die UniMensa Mailingliste für die RWTH Aachen abonniert haben.\n
Datenbank: $uni ($date)\n\n
Eine angenehme Woche.\n
============================================================================
Mailingliste: http://mailman.rwth-aachen.de/mailman/listinfo/unimensa
Website: http://www.niemueller.de/software/palm/unimensa/
UniMensa DB Generator v$version
";


### Start with a simple text message:
my $msg = MIME::Lite->new(
           From    => "$from_name <$from>",
           To      => "$to_name <$to>",
           Subject => "UniMensa DB - $uni ($date)",
           Type    => 'TEXT',
           Data    => $mail_text
          );  

$uni =~ s/ /_/g;
$date =~ s/ //g;
$date =~ /(\d+)\.(\d+)\.(\d+)-(\d+)\.(\d+)\.(\d+)/;

### Attach a part... the make the message a multipart automatically:
$msg->attach(Type     => 'application/palm-database',
             Path     => $db_path,
             Filename => "UniMensa.$uni.$3-$2-$1.pdb"
             );


my $smtp=Net::SMTP->new($mail_host, Hello => $domain, Timeout => 20, Debug => 0);
$smtp->mail($from);
$smtp->to($to);
$smtp->data();
$smtp->datasend($msg->as_string());
$smtp->dataend();
$smtp->quit();


