#!/usr/bin/perl

# Input format:
# enwraps enwrap V 3sg PRES

use strict;
use IO::Handle;
my $N;

while (<>) {
  print STDERR "\r",$N if (++$N % 10 == 0);
  chomp;

  my($w,$l,@f) = split;
  print "<Stem>",match($l,$w);
  foreach $l (@f) {
    print "<$l>";
  }
  print "\n";
}


######################################################################
# alignment functions

sub match {
  my $lemma = shift;
  my $word = shift;
  my @w = mysplit($word);
  my @l = mysplit($lemma);
  unshift(@w, undef);
  unshift(@l, undef);
  my($i,$k,$s,@score,@action,$result);

  $score[0][0] = 0.0;
  
  for( $i=0; $i<=$#w; $i++ ) {
    for( $k=0; $k<=$#l; $k++ ) {
      next if $i==0 && $k == 0;
      $score[$i][$k] = 10000000000000;
      # matching
      $s = $score[$i-1][$k-1] + cost($w[$i], $l[$k]);
      if ($score[$i][$k] >= $s) {
	$score[$i][$k] = $s;
	$action[$i][$k] = "m";
      }
      
      # delete character in word
      $s = $score[$i-1][$k] + cost($w[$i], '');
      if ($score[$i][$k] >= $s) {
	$score[$i][$k] = $s;
	$action[$i][$k] = "w";
      }
      
      # delete character in lemma
      $s = $score[$i][$k-1] + cost('', $l[$k]);
      if ($score[$i][$k] >= $s) {
	$score[$i][$k] = $s;
	$action[$i][$k] = "l";
      }
    }
  }

  $i = $#w;
  $k = $#l;
  while (defined $action[$i][$k]) {
    if ($action[$i][$k] eq "m") {
      if ($w[$i] eq $l[$k]) {
	$result = quote($w[$i]).$result;
      } else {
	$result = quote($l[$k]).":".quote($w[$i]).$result;
      }
      $i--; $k--;
    }
    elsif ($action[$i][$k] eq "w") {
      $result = "<>:".quote($w[$i]).$result;
      $i--;
    }
    else {
      $result = quote($l[$k]).":<>".$result;
      $k--;
    }
  }
  return $result;
}

sub mysplit {
  my $s = shift;
  $s =~ s/(<[A-za-z0-9-]*>|.)/$1 /g;
  $s =~ s/ $//;
  return split(/ /,$s);
}

sub quote {
  my $s = shift;
  $s =~ s/^([\!\"\'\(\)\,\-\.\:\?\~\<])$/\\$1/;
  return $s;
}

sub cost {
  my($w,$l) = @_;
  return 1e30 unless (defined $w && defined $l);
  return 0 if ($w eq $l);
  return 0.2 if (lc($w) eq lc($l));
  return 0.5 if ($w eq '�' && $l eq 'a');
  return 0.5 if ($w eq '�' && $l eq 'o');
  return 0.5 if ($w eq '�' && $l eq 'u');
  return 0.5 if ($w eq 's' && $l eq '�');
  return 0.7 if ($w eq 'c' && $l eq 's');
  return 0.7 if ($w eq 's' && $l eq 'c');
  return 0.7 if ($w eq '');
  return 0.8 if ($l eq '');
  return 0.95 if ($w =~ /[aeiou���]/ && $l =~ /[aeiou���]/);
  return 1;
}
