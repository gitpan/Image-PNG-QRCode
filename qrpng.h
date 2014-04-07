/* This is a Cfunctions (version 0.28) generated header file.
   Cfunctions is a free program for extracting headers from C files.
   Get Cfunctions from 'http://www.lemoda.net/cfunctions/'. */

/* This file was generated with:
'cfunctions -inc qrpng.c' */
#ifndef CFH_QRPNG_H
#define CFH_QRPNG_H

/* From 'qrpng.c': */

#line 10 "qrpng.c"
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

#line 45 "qrpng.c"
qrpng_status_t qrpng_make_png (qr_t * qr , qrpng_t * qrpng );

#line 124 "qrpng.c"
qrpng_status_t qrpng_write (qrpng_t * qrpng );

#line 144 "qrpng.c"
qrpng_status_t qrpng_free (qrpng_t * qrpng );

#endif /* CFH_QRPNG_H */
