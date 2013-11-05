/*
 * File: bmp.h
 * Author: Wayne O. Cochran  (wcochran@vancouver.wsu.edu) with
 *   code heavily lifted from the appendix of
 *           Computer Graphics Using Open GL, Second Edition
 *           by F.S. Hill, Jr
 *   http://cwx.prenhall.com/bookbind/pubbooks/hill4/chapter0/deluxe.html
 * Purpose:
 *   Interface for the routine bmpGetImage() which reads a Windows BMP
 *   file. The stored image is particulary suited for using as
 *   a texture map.
 * Modification History:
 *   11/19/2001 (WOC) ............... created.
 *   10/15/2013 (WOC) ............... got rid of non-power-of-2 crud
 */

#ifndef BMP_H
#define BMP_H

#include <cstdlib>

#include "texture.hpp"

typedef struct {            /* Image read from BMP image */
  int W, H;                 /* width and height of image */
  unsigned char (*rgba)[4]; /* red/green/blue/alpha pixel values */
} BmpImage;

/*
 * bmpGetImage();
 * Purpose: Read 24-bit Windows BMP file into image buffer with 4 bytes
 *   per pixel (RGBA). Any added RGBA values are 0.
 * Parameters:
 *   fname: file name of 24-bit BMP file;
 *   image: ptr to structure to store image data;
 * Note:
 *   The first pixel stored in a BMP file represents the
 *   lower left-hand portion of the image. This is nice, since
 *   the image (i,j) coordinate system has the same orientation
 *   as the texture map (s,t) coordinate system.
 */
void bmpGetImage(const char *fname, BmpImage *image);

class Bmp : public image
{
public:
    Bmp(const char *fname)
    {
        BmpImage img;
        bmpGetImage(fname, &img);
        width = img.W;
        height = img.H;
        data = (color *)img.rgba;
    }
    ~Bmp()
    {
        free(data);
    }
};

#endif /* BMP_H */
