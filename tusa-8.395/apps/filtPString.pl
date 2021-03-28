#!/usr/bin/perl

opendir(DIR,"./");
@d=readdir(DIR);
foreach $n (@d)
{
	$_=$n;
	next unless /html/;
	open(F,$n)||next;
	my @out;

	while(<F>)
	{
		s/\/ca\//\//g;
		push @out,$_;
	}
	close(F);
	open(F,">$n");
	print F @out;
	close($f);
}



