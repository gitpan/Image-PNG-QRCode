# This is a test for module Image::PNG::QRCode.

use warnings;
use strict;
use Test::More;
use Image::PNG::QRCode 'qrpng';
use FindBin;
use File::Compare;

my $pngfile = "$FindBin::Bin/test1.png";
qrpng (text => "this is my png", out => $pngfile);
ok (-f $pngfile);

my $pngfile2 = "$FindBin::Bin/test2.png";
# A scalar to write to
my $s;
qrpng (text => "this is my png", out => \$s);
ok ($s);
open my $o, ">:raw", $pngfile2 or die $!;
print $o $s;
close $o or die $!;
is (compare ($pngfile, $pngfile2), 0);

my $pngfile3 = "$FindBin::Bin/test3.png";
qrpng (text => "ballroom blitz", scale => 6, out => $pngfile3);

ok (-f $pngfile3);
ok (-s $pngfile3 > 0);

my $pngfile4 = "$FindBin::Bin/test4.png";
qrpng (text => "monster mash", scale => 6, quiet => 10, out => $pngfile4);

ok (-f $pngfile4);
ok (-s $pngfile4 > 0);

TODO: {
    local $TODO = 'not implemented yet';
};

# Remove the output files.

for my $file ($pngfile, $pngfile2, $pngfile3, $pngfile4) {
    if (-f $file) {
	unlink $file;
    }
}

done_testing ();

# Local variables:
# mode: perl
# End:
