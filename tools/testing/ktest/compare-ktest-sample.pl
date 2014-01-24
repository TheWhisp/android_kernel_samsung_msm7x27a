#!/usr/bin/perl

open (IN,"ktest.pl");
while (<IN>) {
<<<<<<< HEAD
    if (/\$opt\{"?([A-Z].*?)(\[.*\])?"?\}/ ||
=======
    # hashes are now used
    if (/\$opt\{"?([A-Z].*?)(\[.*\])?"?\}/ ||
	/^\s*"?([A-Z].*?)"?\s*=>\s*/ ||
>>>>>>> refs/remotes/origin/cm-10.0
	/set_test_option\("(.*?)"/) {
	$opt{$1} = 1;
    }
}
close IN;

open (IN, "sample.conf");
while (<IN>) {
<<<<<<< HEAD
    if (/^\s*#?\s*(\S+)\s*=/) {
=======
    if (/^\s*#?\s*([A-Z]\S*)\s*=/) {
>>>>>>> refs/remotes/origin/cm-10.0
	$samp{$1} = 1;
    }
}
close IN;

foreach $opt (keys %opt) {
    if (!defined($samp{$opt})) {
	print "opt = $opt\n";
    }
}

foreach $samp (keys %samp) {
    if (!defined($opt{$samp})) {
	print "samp = $samp\n";
    }
}
