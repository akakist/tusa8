#!/usr/bin/perl
@r=<>;
my %m;
foreach(@r)
{

	if(/(\d+\.\d+\.\d+\.\d+)/g)
	{
		$m{$&}=1;
	}
}
print join("\",\"",keys %m);