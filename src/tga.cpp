///
/// Class used to load TGA image as texture.
///

#include "tga.h"

GLenum TGA::texFormat;

/// Check if both dimensions of the image are power of 2.
int TGA::checkSize (int x) {
    if (x == 2	 || x == 4 || 
        x == 8	 || x == 16 || 
        x == 32  || x == 64 ||
        x == 128 || x == 256 || x == 512)
        return 1;
    else return 0;
}

/// Read RGBA data of 32 bit image.
unsigned char* TGA::getRGBA (FILE *s, int size) {
    unsigned char *rgba;
    unsigned char temp;
    int bread;
    int i;

    rgba = (unsigned char*)malloc (size * 4); 

    if (rgba == NULL)
        return 0;

    bread = fread (rgba, sizeof (unsigned char), size * 4, s); 

    // TGA is stored in BGRA, make it RGBA
    if (bread != size * 4) {
        free (rgba);
        return 0;
    }

    for (i = 0; i < size * 4; i += 4 ) {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }

    texFormat = GL_RGBA;
    return rgba;
}

/// Reads RGB data of 24 bit image.
unsigned char* TGA::getRGB (FILE *s, int size) {
    unsigned char *rgb;
    unsigned char temp;
    int bread;
    int i;

    rgb = (unsigned char*)malloc (size * 3); 

    if (rgb == NULL)
        return 0;

    bread = fread (rgb, sizeof (unsigned char), size * 3, s);

    if (bread != size * 3) {
        free (rgb);
        return 0;
    }

    /* TGA is stored in BGR, make it RGB */
    for (i = 0; i < size * 3; i += 3) {
        temp = rgb[i];
        rgb[i] = rgb[i + 2];
        rgb[i + 2] = temp;
    }

    texFormat = GL_RGB;

    return rgb;
}

/// Get grayscale image data.
unsigned char* TGA::getGray (FILE *s, int size) {
    unsigned char *grayData;
    int bread;

    grayData = (unsigned char*) malloc (size);

    if (grayData == NULL)
        return 0;

    bread = fread (grayData, sizeof (unsigned char), size, s);

    if (bread != size) {
        free (grayData);
        return 0;
    }

    texFormat = GL_ALPHA;

    return grayData;
}

/// Get image data for specified bit depth.
char* TGA::getData (FILE *s, int sz, int iBits) {
    if (iBits == 32)
        return (char*)getRGBA (s, sz);
    else if (iBits == 24)
        return (char*)getRGB (s, sz);	
    else if (iBits == 8)
        return (char*)getGray (s, sz);
}

/// Called when there is an error loading file.
int TGA::returnError (FILE* s, int error) {
    fclose (s);
    return error;
}

/// Loads targa file. Supports 8, 24 and 32bit uncompressed images. Parameter id is texture ID to bind.
int TGA::loadTGA (char *name, int id) {
    unsigned char type[4];
    unsigned char info[7];
    unsigned char *imageData = NULL;
    int imageWidth, imageHeight;
    int imageBits, size;
    FILE *s;

    if (!(s = fopen (name, "r+bt")))
        return TGA_FILE_NOT_FOUND;

    fread (&type, sizeof (char), 3, s); // read in colormap info and image type, byte 0 ignored
    fseek (s, 12, SEEK_SET);			// seek past the header and useless info
    fread (&info, sizeof (char), 6, s);

    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
        returnError (s, TGA_BAD_IMAGE_TYPE);

    imageWidth = info[0] + info[1] * 256; 
    imageHeight = info[2] + info[3] * 256;
    imageBits =	info[4]; 
    
    size = imageWidth * imageHeight; 

    /* make sure dimension is a power of 2 */
    if (!checkSize (imageWidth) || !checkSize (imageHeight))
        returnError (s, TGA_BAD_DIMENSION);

    /* make sure we are loading a supported type */
    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
        returnError (s, TGA_BAD_BITS);

    imageData = (unsigned char*) getData (s, size, imageBits);

    /* no image data */
    if (imageData == NULL)
        returnError (s, TGA_BAD_DATA);

    fclose (s);

    glBindTexture (GL_TEXTURE_2D, id);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D (GL_TEXTURE_2D, 0, texFormat, imageWidth, imageHeight, 0, texFormat, GL_UNSIGNED_BYTE, imageData);

    /* release data, its been uploaded */
    free (imageData);

    return 1;
}
