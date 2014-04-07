/* This is a Cfunctions (version 0.28) generated header file.
   Cfunctions is a free program for extracting headers from C files.
   Get Cfunctions from 'http://www.lemoda.net/cfunctions/'. */

/* This file was generated with:
'cfunctions -inc qrencode.c' */
#ifndef CFH_QRENCODE_H
#define CFH_QRENCODE_H

/* From 'qrencode.c': */

#line 8 "qrencode.c"
typedef struct qr {
    char * input;
    int input_length;
    unsigned level;
    unsigned version;
    unsigned char * strinbuf;
    unsigned char * qrframe;
    unsigned char *framebase;
    unsigned char *framask;
    unsigned char *rlens;
    unsigned char  WD;
    unsigned char WDB;

    unsigned initialized : 1;
}
qr_t;
#define QRBIT(f,x,y) ( ( qr->f[((x)>>3) + (y) * qr->WDB] >> (7-((x) & 7 ))) & 1 )

#line 161 "qrencode.c"
void initframe (qr_t * qr );

#line 270 "qrencode.c"
unsigned initecc (qr_t * qr );

#line 304 "qrencode.c"
unsigned initeccsize (qr_t * qr );

#line 774 "qrencode.c"
void qrencode (qr_t * qr );

#line 807 "qrencode.c"
void qrfree (qr_t * qr );

#endif /* CFH_QRENCODE_H */
