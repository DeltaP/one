#!/usr/bin/perl
use strict;
use warnings;

my $comm_sz=16;
my $core_difference=1;
my $divisor=2;

do {
  for ( my $i=0; $i<$comm_sz; $i+=$core_difference ) {
    if ($i % $divisor == $core_difference) {
      my $to = $i - $core_difference;
      print"Process $i sending to process $to\n";
    }
    elsif ($i % $divisor == 0) {
      my $from = $i + $core_difference;
      print"Process $i receiving from process $from and performing local sum\n";
    }
    else { print"Process $i inactive\n"; }
  }
  print"\n* * * *\n\n";
  $divisor *=2;
  $core_difference *=2;
} while ($divisor <= $comm_sz)


