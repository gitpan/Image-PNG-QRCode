#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PNG_SKIP_SETJMP_CHECK
#include <png.h>

#include "qrencode.h"
#include "qrpng.h"

#ifdef HEADER

typedef struct qrpng {
    unsigned int scale;
    unsigned int quietzone;
    char * filename;
    png_structp png;
    png_infop info; 
    png_byte ** row_pointers;
    int size;
    png_byte * quiet;
}
qrpng_t;

typedef enum qrpng_status {
    qrpng_ok,
    qrpng_bad_scale,
    qrpng_bad_quietzone,
    qrpng_bad_filename,
}
qrpng_status_t;

#define QUIETZONE 4

#endif /* def HEADER */

/* Set bit "x" in "f", which is "unsigned char *". */

#define SETBIT(f,x) f[((x)>>3)] |= 0x80 >> ((x) & 7)

#define MAX_SCALE 100
#define MAX_QUIETZONE 100

qrpng_status_t
qrpng_make_png (qr_t * qr, qrpng_t * qrpng)
{
    int width;
    size_t x;
    size_t y;
    int size;

    if (qrpng->scale < 1 || qrpng->scale > MAX_SCALE) {
	return qrpng_bad_scale;
    }
    if (qrpng->quietzone > MAX_QUIETZONE) {
	return qrpng_bad_quietzone;
    }
    qrpng->size = qr->WD;

    size = qr->WD + qrpng->quietzone * 2;
    width = size * qrpng->scale;

    qrpng->png = png_create_write_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (! qrpng->png) {
	fprintf (stderr, "png_create_write_struct failed\n");
	exit (1);
    }
#ifdef USESETJMP
    if (setjmp (png_jmpbuf (png))) {
        fprintf (stderr, "libpng borked\n");
	exit (1);
    }
#endif /* def USESETJMP */
    qrpng->info = png_create_info_struct (qrpng->png);
    png_set_IHDR (qrpng->png, qrpng->info, width, width, 1,
		  PNG_COLOR_TYPE_GRAY,
		  PNG_INTERLACE_NONE,
		  PNG_COMPRESSION_TYPE_DEFAULT,
		  PNG_FILTER_TYPE_DEFAULT);

    qrpng->row_pointers = png_malloc (qrpng->png,
				      width * sizeof (png_byte *));
    /* Fill top and bottom quiet zones. */
    qrpng->quiet = calloc (width, sizeof (png_byte));
    for (y = 0; y < qrpng->quietzone * qrpng->scale; y++) {
	/* Counting from the top. */
	int from_top;

	from_top = (qr->WD + qrpng->quietzone * 2)*qrpng->scale - y - 1;
	qrpng->row_pointers[y] = qrpng->quiet;
	qrpng->row_pointers[from_top] = qrpng->quiet;
    }
    for (y = 0; y < qr->WD; y++) {
	int repeat;
	png_byte * line;
	int bits[qr->WD];
	line = calloc (width / 8 + 1, sizeof (png_byte));
	/* Get the bits from qr->qrframe. */
	for (x = 0; x < qr->WD; x++) {
	    bits[x] = QRBIT (qrframe, x, y);
	}
	/* Put the bits in, repeatedly, to "line", then repeatedly use
	   "line" in "qrpng->row_pointers". */
	for (x = 0; x < qr->WD; x++) {
	    if (bits[x]) {
		int r;
		int o;
		o = (x + qrpng->quietzone) * qrpng->scale;
		for (r = 0; r < qrpng->scale; r++) {
		    int p;
		    p = o + r;
		    //		    printf ("%d %d %d %d %d\n", x, width, r, p, qrpng->quietzone * qrpng->scale);
		    assert (p <= width - qrpng->quietzone * qrpng->scale);
		    SETBIT(line, p);
		}
	    }
	}
	for (repeat = 0; repeat < qrpng->scale; repeat++) {
	    qrpng->row_pointers[(y+qrpng->quietzone)*qrpng->scale + repeat]
		= line;
	}
    }
    png_set_rows (qrpng->png, qrpng->info, qrpng->row_pointers);
    return qrpng_ok;
}


qrpng_status_t
qrpng_write (qrpng_t * qrpng)
{
    FILE * f;
    if (! qrpng->filename) {
	return qrpng_bad_filename;
    }

    f = fopen (qrpng->filename, "wb");
    if (! f) {
	fprintf (stderr, "fopen failed\n");
	exit (1);
    }
    png_init_io (qrpng->png, f);
    png_write_png (qrpng->png, qrpng->info, PNG_TRANSFORM_INVERT_MONO, NULL);
    fclose (f);
    return qrpng_ok;
}

qrpng_status_t
qrpng_free (qrpng_t * qrpng)
{
    int y;

    png_destroy_write_struct (& qrpng->png, & qrpng->info);
    free (qrpng->quiet);
    for (y = 0; y < qrpng->size; y++) {
	free (qrpng->row_pointers[(y+qrpng->quietzone)*qrpng->scale]);
    }
    free (qrpng->row_pointers);
    return qrpng_ok;
}
