#!/usr/bin/perl
print "#this file is made by makefiles.pl automatically\n#Please do not edit manually\n\n";

while(<>)
{
	chomp;
	$src=$_;
	if(/html/)
	{
		if(/\.\/www\/files\// && ! /\.svn/)
		{

			my $lang="";
			my $host_alias="";
			s/\.\/www\/files\///g;
			if(/\@([a-z]*)\./g)
			{
				$host_alias=$1;
			}
			s/\@[a-z]*//g;
			if(/\.([a-z]*)\.html/)
			{
				$lang=$1;
			}
			s/\.en\.html//g;
			s/\.ru\.html//g;
			s/\.de\.html//g;
			s/\.fr\.html//g;
			s/\.html//g;
			if($host_alias ne "" && $lang ne "")
			{
				print "$host_alias\@$lang:$_=$src\n";
				next;
			}
			if($host_alias ne "")
			{
				print "$host_alias\@$_=$src\n";
				next;
			}
			if($lang ne "")
			{
				print "$lang:$_=$src\n";
				next;
			}
			
				print "$_=$src\n";
		}
	}
}