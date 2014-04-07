typedef struct {
    SV * png_image;
}
scalar_as_image_t;

static void
perl_png_scalar_write (png_structp png, png_bytep bytes_to_write,
                       png_size_t byte_count_to_write)
{
    scalar_as_image_t * si;

    si = png_get_io_ptr (png);
    if (si->png_image == 0) {
        si->png_image = newSVpv ((char *) bytes_to_write, byte_count_to_write);
    }
    else {
        sv_catpvn (si->png_image, (char *) bytes_to_write, byte_count_to_write);
    }
}


void
qrpng_internal (HV * options)
{
    char * text;
    unsigned text_length;
    qr_t qr = {0};
    qrpng_t qrpng = {0};
    //    unsigned char v = 0;
    unsigned char l = 1;
    SV ** sv_ptr;
    qrpng_status_t qrpng_status;
    
    HASH_FETCH_PV (options, text);
    //    printf ("text = '%s', length = %d\n", text, text_length);
    //if v is the version:
    //initecc(qr, l, v);

    qr.input = text;
    qr.input_length = text_length;

    initeccsize (& qr, l);
    initframe(& qr);

    qrencode (& qr);
    
    sv_ptr = hv_fetch (options, "quiet", strlen ("quiet"), 0);
    if (sv_ptr) {
	SV * quiet_sv;
	quiet_sv = * sv_ptr;
	qrpng.quietzone = SvUV (quiet_sv);
    }
    else {
	qrpng.quietzone = QUIETZONE;
    }

    sv_ptr = hv_fetch (options, "scale", strlen ("scale"), 0);
    if (sv_ptr) {
	SV * scale_sv;
	scale_sv = * sv_ptr;
	qrpng.scale = SvUV (scale_sv);
    }
    else {
	qrpng.scale = 3;
    }

    qrpng_status = qrpng_make_png (& qr, & qrpng);

    if (qrpng_status != qrpng_ok) {
	croak ("bad status %d from qrpng_make_png", qrpng_status);
    }
    sv_ptr = hv_fetch (options, "out_sv", strlen ("out_sv"), 0);
    if (sv_ptr) {

	/* Write it as a scalar. The code is copied out of
	   Image::PNG::Libpng, but we don't depend on that. */

	scalar_as_image_t * si;

	si = malloc (sizeof (scalar_as_image_t));
	assert (si);
	png_set_write_fn (qrpng.png, si, perl_png_scalar_write,
			  0 /* No flush function */);
	png_write_png (qrpng.png, qrpng.info,
		       PNG_TRANSFORM_INVERT_MONO, NULL);
	(void) hv_store (options, "png_data", strlen ("png_data"),
			 si->png_image, 0);
	free (si);
    }
    else {
	char * out;
	unsigned int out_length;

	HASH_FETCH_PV (options, out);
	qrpng.filename = out;
	qrpng_write (& qrpng);
    }
    qrfree (& qr);
    qrpng_free (& qrpng);
}
