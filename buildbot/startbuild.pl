#!/usr/bin/perl
# vim: set ts=2 sw=2 tw=99 noet: 

use File::Basename;

my ($myself, $path) = fileparse($0);
chdir($path);

require 'helpers.pm';

chdir('../../OUTPUT');

$ENV{'BREAKPAD_SYMBOL_SERVER'} = 'http://crash.limetech.org/submit-symbols';

if ($^O eq "linux") {
	$ENV{'PATH'} .= ':/home/builds/common/';
} elsif ($^O eq "darwin") {
	$ENV{'PATH'} .= ':/Users/builds/slaves/common/';
}

if ($^O eq "linux" || $^O eq "darwin") {
	system("python3 build.py 2>&1");
} else {
	system("C:\\Python31\\python.exe build.py 2>&1");
}

if ($? != 0)
{
	die "Build failed: $!\n";
}
else
{
	exit(0);
}

