///
/// Class used to load TGA image as texture.
///

#ifndef tga_h
#define tga_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

// Error codes
#define TGA_FILE_NOT_FOUND          13 /* file was not found */
#define TGA_BAD_IMAGE_TYPE          14 /* color mapped image or image is not uncompressed */
#define TGA_BAD_DIMENSION			15 /* dimension is not a power of 2 */
#define TGA_BAD_BITS				16 /* image bits is not 8, 24 or 32 */
#define TGA_BAD_DATA				17 /* image data could not be loaded */

class TGA {
public:
	static GLenum texFormat;
	static int checkSize (int x);
	static unsigned char *getRGBA (FILE *s, int size);
	static unsigned char *getRGB (FILE *s, int size);
	static unsigned char *getGray (FILE *s, int size);
	static char *getData (FILE *s, int sz, int iBits);
	static int returnError (FILE* s, int error);
	static int loadTGA (char *name, int id);
};

#endif // tga_h