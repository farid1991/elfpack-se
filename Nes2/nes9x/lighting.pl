#!/usr/bin/perl

use feature ":5.10";

if(@ARGV < 1)
{
  say "Specify file";
  exit 1;
}

open IN, $ARGV[0] or die "Can\'t open input!\n";


if( @ARGV == 1 )
{
  open OUT, ">".$ARGV[0].".new" or die "Can\'t open output!\n";
}
else
{
  open OUT, ">".$ARGV[1] or die "Can\'t open output!\n";
}

my $f = 3.5;
while(my $line = <IN>)
{
  if( $line !~ /(.*)\{[\s\t]*0x([0-9a-fA-F]{2})[\s\t]*,[\s\t]*0x([0-9a-fA-F]{2})[\s\t]*,[\s\t]*0x([0-9a-fA-F]{2})[\s\t]*\}(.*)/ )
  {
    print OUT $line;
    next;
  }

  print OUT "$1\{ ".int(hex($4)*$f).", ".int(hex($3)*$f).", ".int(hex($2)*$f).", 0 \},$6\n";
}