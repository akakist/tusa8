#!/usr/bin/perl
undef $/;
$r=<>;
@a=split /\s+/,$r;
foreach(@a)
{
    printf "m[$_]=\"$_\";\n";
}