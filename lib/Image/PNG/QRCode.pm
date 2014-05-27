package Image::PNG::QRCode;
require Exporter;
@ISA = qw(Exporter);
@EXPORT_OK = qw/qrpng/;
%EXPORT_TAGS = (
    all => \@EXPORT_OK,
);
use warnings;
use strict;
use Carp;
our $VERSION = '0.03';
require XSLoader;
XSLoader::load ('Image::PNG::QRCode', $VERSION);

sub qrpng
{
    my (%options) = @_;
    if ($options{in}) {
	if ($options{text}) {
	    carp "Overwriting input text '$options{text}' with contents of file $options{in}";
	}
	$options{text} = '';
	open my $in, "<:raw", $options{in} or die $!;
	while (<$in>) {
	    $options{text} .= $_;
	}
	close $in or die $!;
    }
    if (! $options{text}) {
	croak "No input";
    }
    if ($options{quiet}) {
	if ($options{quiet} < 0) {
	    croak "quiet zone cannot be negative";
	}
	if ($options{quiet} > 100) {
	    croak "requested quiet zone, $options{quiet}, exceeds arbitrary maximum of 100";
	}
    }
    if ($options{scale}) {
	if ($options{scale} < 1) {
	    croak "negative or zero scale $options{scale}";
	}
	if ($options{scale} != int $options{scale}) {
	    croak "scale option needs to be an integer";
	}
	if ($options{scale} > 100) {
	    croak "requested scale, $options{scale}, exceeds arbitrary maximum of 100";
	}
    }
    if ($options{version}) {
	if ($options{version} < 1 || $options{version} > 40 ||
	    $options{version} != int ($options{version})) {
	    croak "Bad version number $options{version}: use integer between one and forty";
	}
    }
    if ($options{level}) {
	if ($options{level} < 1 || $options{level} > 4 ||
	    $options{level} != int ($options{level})) {
	    croak "Bad level number $options{level}: use integer between one and four";
	}
    }
    my $r = defined wantarray () && ! wantarray ();
    my $s = $options{out} && ref $options{out} eq 'SCALAR';
    if ($r || $s) {
	$options{out_sv} = 1;
    }
    qrpng_internal (\%options);
    if ($r) {
	return $options{png_data};
    }
    elsif ($s) {
	${$options{out}} = $options{png_data};
    }
}

1;
