#!/usr/bin/perl

	open(F,$ARGV[0]);
	my @out;

	while(<F>)
	{
		s/\x0d//g;
		push @out,$_;
	}
	close(F);
	open(F,">".$ARGV[0]);
	print F @out;
	close($f);
