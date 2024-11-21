/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
// tr_image.c
#include "tr_local.h"

#include "tr_dsa.h"

static byte			 s_intensitytable[256];
static unsigned char s_gammatable[256];

int		gl_filter_min = GL_LINEAR_MIPMAP_NEAREST;
int		gl_filter_max = GL_LINEAR;

#define FILE_HASH_SIZE		1024
static	image_t*		hashTable[FILE_HASH_SIZE];

/*
** R_GammaCorrect
*/
void R_GammaCorrect( byte *buffer, int bufSize ) {
	int i;

	for ( i = 0; i < bufSize; i++ ) {
		buffer[i] = s_gammatable[buffer[i]];
	}
}

typedef struct {
	char *name;
	int	minimize, maximize;
} textureMode_t;

textureMode_t modes[] = {
	{"GL_NEAREST", GL_NEAREST, GL_NEAREST},
	{"GL_LINEAR", GL_LINEAR, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST},
	{"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST},
	{"GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR}
};

/*
================
return a hash value for the filename
================
*/
static long generateHashValue( const char *fname ) {
	int		i;
	long	hash;
	char	letter;

	hash = 0;
	i = 0;
	while (fname[i] != '\0') {
		letter = tolower(fname[i]);
		if (letter =='.') break;				// don't include extension
		if (letter =='\\') letter = '/';		// damn path names
		hash+=(long)(letter)*(i+119);
		i++;
	}
	hash &= (FILE_HASH_SIZE-1);
	return hash;
}

/*
===============
GL_TextureMode
===============
*/
void GL_TextureMode( const char *string ) {
	int		i;
	image_t	*glt;

	for ( i=0 ; i< 6 ; i++ ) {
		if ( !Q_stricmp( modes[i].name, string ) ) {
			break;
		}
	}

	if ( i == 6 ) {
		ri.Printf (PRINT_ALL, "bad filter name\n");
		return;
	}

	gl_filter_min = modes[i].minimize;
	gl_filter_max = modes[i].maximize;

	// change all the existing mipmap texture objects
	for ( i = 0 ; i < tr.numImages ; i++ ) {
		glt = tr.images[ i ];
		if ( glt->flags & IMGFLAG_MIPMAP && !(glt->flags & IMGFLAG_CUBEMAP)) {
			qglTextureParameterfEXT(glt->texnum, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_min);
			qglTextureParameterfEXT(glt->texnum, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_max);
		}
	}
}

/*
===============
R_SumOfUsedImages
===============
*/
int R_SumOfUsedImages( void ) {
	int	total;
	int i;

	total = 0;
	for ( i = 0; i < tr.numImages; i++ ) {
		if ( tr.images[i]->frameUsed == tr.frameCount ) {
			total += tr.images[i]->uploadWidth * tr.images[i]->uploadHeight;
		}
	}

	return total;
}

/*
===============
R_ImageList_f
===============
*/
void R_ImageList_f( void ) {
	int i;
	int estTotalSize = 0;

	ri.Printf(PRINT_ALL, "\n      -w-- -h-- -type-- -size- --name-------\n");

	for ( i = 0 ; i < tr.numImages ; i++ )
	{
		image_t *image = tr.images[i];
		char *format = "????   ";
		char *sizeSuffix;
		int estSize;
		int displaySize;

		estSize = image->uploadHeight * image->uploadWidth;

		switch(image->internalFormat)
		{
			case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT:
				format = "sDXT1  ";
				// 64 bits per 16 pixels, so 4 bits per pixel
				estSize /= 2;
				break;
			case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT:
				format = "sDXT5  ";
				// 128 bits per 16 pixels, so 1 byte per pixel
				break;
			case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
				format = "sBPTC  ";
				// 128 bits per 16 pixels, so 1 byte per pixel
				break;
			case GL_COMPRESSED_RG_RGTC2:
				format = "RGTC2  ";
				// 128 bits per 16 pixels, so 1 byte per pixel
				break;
			case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
				format = "DXT1   ";
				// 64 bits per 16 pixels, so 4 bits per pixel
				estSize /= 2;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				format = "DXT1a  ";
				// 64 bits per 16 pixels, so 4 bits per pixel
				estSize /= 2;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
				format = "DXT5   ";
				// 128 bits per 16 pixels, so 1 byte per pixel
				break;
			case GL_COMPRESSED_RGBA_BPTC_UNORM_ARB:
				format = "BPTC   ";
				// 128 bits per 16 pixels, so 1 byte per pixel
				break;
			case GL_RGB4_S3TC:
				format = "S3TC   ";
				// same as DXT1?
				estSize /= 2;
				break;
			case GL_RGBA16F:
				format = "RGBA16F";
				// 8 bytes per pixel
				estSize *= 8;
				break;
			case GL_RGBA16:
				format = "RGBA16 ";
				// 8 bytes per pixel
				estSize *= 8;
				break;
			case GL_RGBA4:
			case GL_RGBA8:
			case GL_RGBA:
				format = "RGBA   ";
				// 4 bytes per pixel
				estSize *= 4;
				break;
			case GL_LUMINANCE8:
			case GL_LUMINANCE:
				format = "L      ";
				// 1 byte per pixel?
				break;
			case GL_RGB5:
			case GL_RGB8:
			case GL_RGB:
				format = "RGB    ";
				// 3 bytes per pixel?
				estSize *= 3;
				break;
			case GL_LUMINANCE8_ALPHA8:
			case GL_LUMINANCE_ALPHA:
				format = "LA     ";
				// 2 bytes per pixel?
				estSize *= 2;
				break;
			case GL_SRGB_EXT:
			case GL_SRGB8_EXT:
				format = "sRGB   ";
				// 3 bytes per pixel?
				estSize *= 3;
				break;
			case GL_SRGB_ALPHA_EXT:
			case GL_SRGB8_ALPHA8_EXT:
				format = "sRGBA  ";
				// 4 bytes per pixel?
				estSize *= 4;
				break;
			case GL_SLUMINANCE_EXT:
			case GL_SLUMINANCE8_EXT:
				format = "sL     ";
				// 1 byte per pixel?
				break;
			case GL_SLUMINANCE_ALPHA_EXT:
			case GL_SLUMINANCE8_ALPHA8_EXT:
				format = "sLA    ";
				// 2 byte per pixel?
				estSize *= 2;
				break;
			case GL_DEPTH_COMPONENT16:
				format = "Depth16";
				// 2 bytes per pixel
				estSize *= 2;
				break;
			case GL_DEPTH_COMPONENT24:
				format = "Depth24";
				// 3 bytes per pixel
				estSize *= 3;
				break;
			case GL_DEPTH_COMPONENT:
			case GL_DEPTH_COMPONENT32:
				format = "Depth32";
				// 4 bytes per pixel
				estSize *= 4;
				break;
		}

		// mipmap adds about 50%
		if (image->flags & IMGFLAG_MIPMAP)
			estSize += estSize / 2;

		sizeSuffix = "b ";
		displaySize = estSize;

		if (displaySize > 1024)
		{
			displaySize /= 1024;
			sizeSuffix = "kb";
		}

		if (displaySize > 1024)
		{
			displaySize /= 1024;
			sizeSuffix = "Mb";
		}

		if (displaySize > 1024)
		{
			displaySize /= 1024;
			sizeSuffix = "Gb";
		}

		ri.Printf(PRINT_ALL, "%4i: %4ix%4i %s %4i%s %s\n", i, image->uploadWidth, image->uploadHeight, format, displaySize, sizeSuffix, image->imgName);
		estTotalSize += estSize;
	}

	ri.Printf (PRINT_ALL, " ---------\n");
	ri.Printf (PRINT_ALL, " approx %i bytes\n", estTotalSize);
	ri.Printf (PRINT_ALL, " %i total images\n\n", tr.numImages );
}

//=======================================================================

/*
================
ResampleTexture

Used to resample images in a more general than quartering fashion.

This will only be filtered properly if the resampled size
is greater than half the original size.

If a larger shrinking is needed, use the mipmap function 
before or after.
================
*/
static void ResampleTexture( byte *in, int inwidth, int inheight, byte *out,  
							int outwidth, int outheight ) {
	int		i, j;
	byte	*inrow, *inrow2;
	int		frac, fracstep;
	int		p1[2048], p2[2048];
	byte	*pix1, *pix2, *pix3, *pix4;

	if (outwidth>2048)
		ri.Error(ERR_DROP, "ResampleTexture: max width");
								
	fracstep = inwidth*0x10000/outwidth;

	frac = fracstep>>2;
	for ( i=0 ; i<outwidth ; i++ ) {
		p1[i] = 4*(frac>>16);
		frac += fracstep;
	}
	frac = 3*(fracstep>>2);
	for ( i=0 ; i<outwidth ; i++ ) {
		p2[i] = 4*(frac>>16);
		frac += fracstep;
	}

	for (i=0 ; i<outheight ; i++) {
		inrow = in + 4*inwidth*(int)((i+0.25)*inheight/outheight);
		inrow2 = in + 4*inwidth*(int)((i+0.75)*inheight/outheight);
		for (j=0 ; j<outwidth ; j++) {
			pix1 = inrow + p1[j];
			pix2 = inrow + p2[j];
			pix3 = inrow2 + p1[j];
			pix4 = inrow2 + p2[j];
			*out++ = (pix1[0] + pix2[0] + pix3[0] + pix4[0])>>2;
			*out++ = (pix1[1] + pix2[1] + pix3[1] + pix4[1])>>2;
			*out++ = (pix1[2] + pix2[2] + pix3[2] + pix4[2])>>2;
			*out++ = (pix1[3] + pix2[3] + pix3[3] + pix4[3])>>2;
		}
	}
}

static void RGBAtoYCoCgA(const byte *in, byte *out, int width, int height)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		const byte *inbyte  = in  + y * width * 4;
		byte       *outbyte = out + y * width * 4;

		for (x = 0; x < width; x++)
		{
			byte r, g, b, a, rb2;

			r = *inbyte++;
			g = *inbyte++;
			b = *inbyte++;
			a = *inbyte++;
			rb2 = (r + b) >> 1;

			*outbyte++ = (g + rb2) >> 1;       // Y  =  R/4 + G/2 + B/4
			*outbyte++ = (r - b + 256) >> 1;   // Co =  R/2       - B/2
			*outbyte++ = (g - rb2 + 256) >> 1; // Cg = -R/4 + G/2 - B/4
			*outbyte++ = a;
		}
	}
}

static void YCoCgAtoRGBA(const byte *in, byte *out, int width, int height)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		const byte *inbyte  = in  + y * width * 4;
		byte       *outbyte = out + y * width * 4;

		for (x = 0; x < width; x++)
		{
			byte _Y, Co, Cg, a;

			_Y = *inbyte++;
			Co = *inbyte++;
			Cg = *inbyte++;
			a  = *inbyte++;

			*outbyte++ = CLAMP(_Y + Co - Cg,       0, 255); // R = Y + Co - Cg
			*outbyte++ = CLAMP(_Y      + Cg - 128, 0, 255); // G = Y + Cg
			*outbyte++ = CLAMP(_Y - Co - Cg + 256, 0, 255); // B = Y - Co - Cg
			*outbyte++ = a;
		}
	}
}


// uses a sobel filter to change a texture to a normal map
static void RGBAtoNormal(const byte *in, byte *out, int width, int height, qboolean clampToEdge)
{
	int x, y, max;

	// convert to heightmap, storing in alpha
	// same as converting to Y in YCoCg
	max = 1;
	for (y = 0; y < height; y++)
	{
		const byte *inbyte  = in  + y * width * 4;
		byte       *outbyte = out + y * width * 4 + 3;

		for (x = 0; x < width; x++)
		{
			byte result = (inbyte[0] >> 2) + (inbyte[1] >> 1) + (inbyte[2] >> 2);
			result = result * result / 255; // Make linear
			*outbyte = result;
			max = MAX(max, *outbyte);
			outbyte += 4;
			inbyte  += 4;
		}
	}

	// level out heights
	if (max < 255)
	{
		for (y = 0; y < height; y++)
		{
			byte *outbyte = out + y * width * 4 + 3;

			for (x = 0; x < width; x++)
			{
				*outbyte = *outbyte + (255 - max);
				outbyte += 4;
			}
		}
	}


	// now run sobel filter over height values to generate X and Y
	// then normalize
	for (y = 0; y < height; y++)
	{
		byte *outbyte = out + y * width * 4;

		for (x = 0; x < width; x++)
		{
			// 0 1 2
			// 3 4 5
			// 6 7 8

			byte s[9];
			int x2, y2, i;
			vec3_t normal;

			i = 0;
			for (y2 = -1; y2 <= 1; y2++)
			{
				int src_y = y + y2;

				if (clampToEdge)
				{
					src_y = CLAMP(src_y, 0, height - 1);
				}
				else
				{
					src_y = (src_y + height) % height;
				}


				for (x2 = -1; x2 <= 1; x2++)
				{
					int src_x = x + x2;

					if (clampToEdge)
					{
						src_x = CLAMP(src_x, 0, width - 1);
					}
					else
					{
						src_x = (src_x + width) % width;
					}

					s[i++] = *(out + (src_y * width + src_x) * 4 + 3);
				}
			}

			normal[0] =        s[0]            -     s[2]
						 + 2 * s[3]            - 2 * s[5]
						 +     s[6]            -     s[8];

			normal[1] =        s[0] + 2 * s[1] +     s[2]

						 -     s[6] - 2 * s[7] -     s[8];

			normal[2] = s[4] * 4;

			if (!VectorNormalize2(normal, normal))
			{
				VectorSet(normal, 0, 0, 1);
			}

			*outbyte++ = FloatToOffsetByte(normal[0]);
			*outbyte++ = FloatToOffsetByte(normal[1]);
			*outbyte++ = FloatToOffsetByte(normal[2]);
			outbyte++;
		}
	}
}

#define COPYSAMPLE(a,b) *(unsigned int *)(a) = *(unsigned int *)(b)

// based on Fast Curve Based Interpolation
// from Fast Artifacts-Free Image Interpolation (http://www.andreagiachetti.it/icbi/)
// assumes data has a 2 pixel thick border of clamped or wrapped data
// expects data to be a grid with even (0, 0), (2, 0), (0, 2), (2, 2) etc pixels filled
// only performs FCBI on specified component
static void DoFCBI(byte *in, byte *out, int width, int height, int component)
{
	int x, y;
	byte *outbyte, *inbyte;

	// copy in to out
	for (y = 2; y < height - 2; y += 2)
	{
		inbyte  = in  + (y * width + 2) * 4 + component;
		outbyte = out + (y * width + 2) * 4 + component;

		for (x = 2; x < width - 2; x += 2)
		{
			*outbyte = *inbyte;
			outbyte += 8;
			inbyte += 8;
		}
	}
	
	for (y = 3; y < height - 3; y += 2)
	{
		// diagonals
		//
		// NWp  - northwest interpolated pixel
		// NEp  - northeast interpolated pixel
		// NWd  - northwest first derivative
		// NEd  - northeast first derivative
		// NWdd - northwest second derivative
		// NEdd - northeast second derivative
		//
		// Uses these samples:
		//
		//         0
		//   - - a - b - -
		//   - - - - - - -
		//   c - d - e - f
		// 0 - - - - - - -
		//   g - h - i - j
		//   - - - - - - -
		//   - - k - l - -
		//
		// x+2 uses these samples:
		//
		//         0
		//   - - - - a - b - -
		//   - - - - - - - - -
		//   - - c - d - e - f
		// 0 - - - - - - - - -
		//   - - g - h - i - j
		//   - - - - - - - - -
		//   - - - - k - l - -
		//
		// so we can reuse 8 of them on next iteration
		//
		// a=b, c=d, d=e, e=f, g=h, h=i, i=j, k=l
		//
		// only b, f, j, and l need to be sampled on next iteration

		byte sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl;
		byte *line1, *line2, *line3, *line4;

		x = 3;

		// optimization one
		//                       SAMPLE2(sa, x-1, y-3);
		//SAMPLE2(sc, x-3, y-1); SAMPLE2(sd, x-1, y-1); SAMPLE2(se, x+1, y-1);
		//SAMPLE2(sg, x-3, y+1); SAMPLE2(sh, x-1, y+1); SAMPLE2(si, x+1, y+1);
		//                       SAMPLE2(sk, x-1, y+3);

		// optimization two
		line1 = in + ((y - 3) * width + (x - 1)) * 4 + component;
		line2 = in + ((y - 1) * width + (x - 3)) * 4 + component;
		line3 = in + ((y + 1) * width + (x - 3)) * 4 + component;
		line4 = in + ((y + 3) * width + (x - 1)) * 4 + component;

		//                                   COPYSAMPLE(sa, line1); line1 += 8;
		//COPYSAMPLE(sc, line2); line2 += 8; COPYSAMPLE(sd, line2); line2 += 8; COPYSAMPLE(se, line2); line2 += 8;
		//COPYSAMPLE(sg, line3); line3 += 8; COPYSAMPLE(sh, line3); line3 += 8; COPYSAMPLE(si, line3); line3 += 8;
		//                                   COPYSAMPLE(sk, line4); line4 += 8;

		                         sa = *line1; line1 += 8;
		sc = *line2; line2 += 8; sd = *line2; line2 += 8; se = *line2; line2 += 8;
		sg = *line3; line3 += 8; sh = *line3; line3 += 8; si = *line3; line3 += 8;
		                         sk = *line4; line4 += 8;

		outbyte = out + (y * width + x) * 4 + component;

		for ( ; x < width - 3; x += 2)
		{
			int NWd, NEd, NWp, NEp;

			// original
			//                       SAMPLE2(sa, x-1, y-3); SAMPLE2(sb, x+1, y-3);
			//SAMPLE2(sc, x-3, y-1); SAMPLE2(sd, x-1, y-1); SAMPLE2(se, x+1, y-1); SAMPLE2(sf, x+3, y-1);
			//SAMPLE2(sg, x-3, y+1); SAMPLE2(sh, x-1, y+1); SAMPLE2(si, x+1, y+1); SAMPLE2(sj, x+3, y+1);
			//                       SAMPLE2(sk, x-1, y+3); SAMPLE2(sl, x+1, y+3);

			// optimization one
			//SAMPLE2(sb, x+1, y-3);
			//SAMPLE2(sf, x+3, y-1);
			//SAMPLE2(sj, x+3, y+1);
			//SAMPLE2(sl, x+1, y+3);

			// optimization two
			//COPYSAMPLE(sb, line1); line1 += 8;
			//COPYSAMPLE(sf, line2); line2 += 8;
			//COPYSAMPLE(sj, line3); line3 += 8;
			//COPYSAMPLE(sl, line4); line4 += 8;

			sb = *line1; line1 += 8;
			sf = *line2; line2 += 8;
			sj = *line3; line3 += 8;
			sl = *line4; line4 += 8;

			NWp = sd + si;
			NEp = se + sh;
			NWd = abs(sd - si);
			NEd = abs(se - sh);

			if (NWd > 100 || NEd > 100 || abs(NWp-NEp) > 200)
			{
				if (NWd < NEd)
					*outbyte = NWp >> 1;
				else
					*outbyte = NEp >> 1;
			}
			else
			{
				int NWdd, NEdd;

				//NEdd = abs(sg + sd + sb - 3 * (se + sh) + sk + si + sf);
				//NWdd = abs(sa + se + sj - 3 * (sd + si) + sc + sh + sl);
				NEdd = abs(sg + sb - 3 * NEp + sk + sf + NWp);
				NWdd = abs(sa + sj - 3 * NWp + sc + sl + NEp);

				if (NWdd > NEdd)
					*outbyte = NWp >> 1;
				else
					*outbyte = NEp >> 1;
			}

			outbyte += 8;

			//                    COPYSAMPLE(sa, sb);
			//COPYSAMPLE(sc, sd); COPYSAMPLE(sd, se); COPYSAMPLE(se, sf);
			//COPYSAMPLE(sg, sh); COPYSAMPLE(sh, si); COPYSAMPLE(si, sj);
			//                    COPYSAMPLE(sk, sl);

			         sa = sb;
			sc = sd; sd = se; se = sf;
			sg = sh; sh = si; si = sj;
			         sk = sl;
		}
	}

	// hack: copy out to in again
	for (y = 3; y < height - 3; y += 2)
	{
		inbyte = out + (y * width + 3) * 4 + component;
		outbyte = in + (y * width + 3) * 4 + component;

		for (x = 3; x < width - 3; x += 2)
		{
			*outbyte = *inbyte;
			outbyte += 8;
			inbyte += 8;
		}
	}
	
	for (y = 2; y < height - 3; y++)
	{
		// horizontal & vertical
		//
		// hp  - horizontally interpolated pixel
		// vp  - vertically interpolated pixel
		// hd  - horizontal first derivative
		// vd  - vertical first derivative
		// hdd - horizontal second derivative
		// vdd - vertical second derivative
		// Uses these samples:
		//
		//       0
		//   - a - b -
		//   c - d - e
		// 0 - f - g -
		//   h - i - j
		//   - k - l -
		//
		// x+2 uses these samples:
		//
		//       0
		//   - - - a - b -
		//   - - c - d - e
		// 0 - - - f - g -
		//   - - h - i - j
		//   - - - k - l -
		//
		// so we can reuse 7 of them on next iteration
		//
		// a=b, c=d, d=e, f=g, h=i, i=j, k=l
		//
		// only b, e, g, j, and l need to be sampled on next iteration

		byte sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl;
		byte *line1, *line2, *line3, *line4, *line5;

		//x = (y + 1) % 2;
		x = (y + 1) % 2 + 2;
		
		// optimization one
		//            SAMPLE2(sa, x-1, y-2);
		//SAMPLE2(sc, x-2, y-1); SAMPLE2(sd, x,   y-1);
		//            SAMPLE2(sf, x-1, y  );
		//SAMPLE2(sh, x-2, y+1); SAMPLE2(si, x,   y+1);
		//            SAMPLE2(sk, x-1, y+2);

		line1 = in + ((y - 2) * width + (x - 1)) * 4 + component;
		line2 = in + ((y - 1) * width + (x - 2)) * 4 + component;
		line3 = in + ((y    ) * width + (x - 1)) * 4 + component;
		line4 = in + ((y + 1) * width + (x - 2)) * 4 + component;
		line5 = in + ((y + 2) * width + (x - 1)) * 4 + component;

		//                 COPYSAMPLE(sa, line1); line1 += 8;
		//COPYSAMPLE(sc, line2); line2 += 8; COPYSAMPLE(sd, line2); line2 += 8;
		//                 COPYSAMPLE(sf, line3); line3 += 8;
		//COPYSAMPLE(sh, line4); line4 += 8; COPYSAMPLE(si, line4); line4 += 8;
        //                 COPYSAMPLE(sk, line5); line5 += 8;

		             sa = *line1; line1 += 8;
		sc = *line2; line2 += 8; sd = *line2; line2 += 8;
		             sf = *line3; line3 += 8;
		sh = *line4; line4 += 8; si = *line4; line4 += 8;
		             sk = *line5; line5 += 8;

		outbyte = out + (y * width + x) * 4 + component;

		for ( ; x < width - 3; x+=2)
		{
			int hd, vd, hp, vp;

			//            SAMPLE2(sa, x-1, y-2); SAMPLE2(sb, x+1, y-2);
			//SAMPLE2(sc, x-2, y-1); SAMPLE2(sd, x,   y-1); SAMPLE2(se, x+2, y-1);
			//            SAMPLE2(sf, x-1, y  ); SAMPLE2(sg, x+1, y  );
			//SAMPLE2(sh, x-2, y+1); SAMPLE2(si, x,   y+1); SAMPLE2(sj, x+2, y+1);
			//            SAMPLE2(sk, x-1, y+2); SAMPLE2(sl, x+1, y+2);

			// optimization one
			//SAMPLE2(sb, x+1, y-2);
			//SAMPLE2(se, x+2, y-1);
			//SAMPLE2(sg, x+1, y  );
			//SAMPLE2(sj, x+2, y+1);
			//SAMPLE2(sl, x+1, y+2);

			//COPYSAMPLE(sb, line1); line1 += 8;
			//COPYSAMPLE(se, line2); line2 += 8;
			//COPYSAMPLE(sg, line3); line3 += 8;
			//COPYSAMPLE(sj, line4); line4 += 8;
			//COPYSAMPLE(sl, line5); line5 += 8;

			sb = *line1; line1 += 8;
			se = *line2; line2 += 8;
			sg = *line3; line3 += 8;
			sj = *line4; line4 += 8;
			sl = *line5; line5 += 8;

			hp = sf + sg; 
			vp = sd + si;
			hd = abs(sf - sg);
			vd = abs(sd - si);

			if (hd > 100 || vd > 100 || abs(hp-vp) > 200)
			{
				if (hd < vd)
					*outbyte = hp >> 1;
				else
					*outbyte = vp >> 1;
			}
			else
			{
				int hdd, vdd;

				//hdd = abs(sc[i] + sd[i] + se[i] - 3 * (sf[i] + sg[i]) + sh[i] + si[i] + sj[i]);
				//vdd = abs(sa[i] + sf[i] + sk[i] - 3 * (sd[i] + si[i]) + sb[i] + sg[i] + sl[i]);

				hdd = abs(sc + se - 3 * hp + sh + sj + vp);
				vdd = abs(sa + sk - 3 * vp + sb + sl + hp);

				if (hdd > vdd)
					*outbyte = hp >> 1;
				else 
					*outbyte = vp >> 1;
			}

			outbyte += 8;

			//          COPYSAMPLE(sa, sb);
			//COPYSAMPLE(sc, sd); COPYSAMPLE(sd, se);
			//          COPYSAMPLE(sf, sg);
			//COPYSAMPLE(sh, si); COPYSAMPLE(si, sj);
			//          COPYSAMPLE(sk, sl);
			    sa = sb;
			sc = sd; sd = se;
			    sf = sg;
			sh = si; si = sj;
			    sk = sl;
		}
	}
}

// Similar to FCBI, but throws out the second order derivatives for speed
static void DoFCBIQuick(byte *in, byte *out, int width, int height, int component)
{
	int x, y;
	byte *outbyte, *inbyte;

	// copy in to out
	for (y = 2; y < height - 2; y += 2)
	{
		inbyte  = in  + (y * width + 2) * 4 + component;
		outbyte = out + (y * width + 2) * 4 + component;

		for (x = 2; x < width - 2; x += 2)
		{
			*outbyte = *inbyte;
			outbyte += 8;
			inbyte += 8;
		}
	}

	for (y = 3; y < height - 4; y += 2)
	{
		byte sd, se, sh, si;
		byte *line2, *line3;

		x = 3;

		line2 = in + ((y - 1) * width + (x - 1)) * 4 + component;
		line3 = in + ((y + 1) * width + (x - 1)) * 4 + component;

		sd = *line2; line2 += 8;
		sh = *line3; line3 += 8;

		outbyte = out + (y * width + x) * 4 + component;

		for ( ; x < width - 4; x += 2)
		{
			int NWd, NEd, NWp, NEp;

			se = *line2; line2 += 8;
			si = *line3; line3 += 8;

			NWp = sd + si;
			NEp = se + sh;
			NWd = abs(sd - si);
			NEd = abs(se - sh);

			if (NWd < NEd)
				*outbyte = NWp >> 1;
			else
				*outbyte = NEp >> 1;

			outbyte += 8;

			sd = se;
			sh = si;
		}
	}

	// hack: copy out to in again
	for (y = 3; y < height - 3; y += 2)
	{
		inbyte  = out + (y * width + 3) * 4 + component;
		outbyte = in  + (y * width + 3) * 4 + component;

		for (x = 3; x < width - 3; x += 2)
		{
			*outbyte = *inbyte;
			outbyte += 8;
			inbyte += 8;
		}
	}
	
	for (y = 2; y < height - 3; y++)
	{
		byte sd, sf, sg, si;
		byte *line2, *line3, *line4;

		x = (y + 1) % 2 + 2;

		line2 = in + ((y - 1) * width + (x    )) * 4 + component;
		line3 = in + ((y    ) * width + (x - 1)) * 4 + component;
		line4 = in + ((y + 1) * width + (x    )) * 4 + component;

		outbyte = out + (y * width + x) * 4 + component;

		sf = *line3; line3 += 8;

		for ( ; x < width - 3; x+=2)
		{
			int hd, vd, hp, vp;

			sd = *line2; line2 += 8;
			sg = *line3; line3 += 8;
			si = *line4; line4 += 8;
			
			hp = sf + sg; 
			vp = sd + si;
			hd = abs(sf - sg);
			vd = abs(sd - si);

			if (hd < vd)
				*outbyte = hp >> 1;
			else
				*outbyte = vp >> 1;

			outbyte += 8;

			sf = sg;
		}
	}
}

// Similar to DoFCBIQuick, but just takes the average instead of checking derivatives
// as well, this operates on all four components
static void DoLinear(byte *in, byte *out, int width, int height)
{
	int x, y, i;
	byte *outbyte, *inbyte;

	// copy in to out
	for (y = 2; y < height - 2; y += 2)
	{
		x = 2;

		inbyte  = in  + (y * width + x) * 4;
		outbyte = out + (y * width + x) * 4;

		for ( ; x < width - 2; x += 2)
		{
			COPYSAMPLE(outbyte, inbyte);
			outbyte += 8;
			inbyte += 8;
		}
	}

	for (y = 1; y < height - 1; y += 2)
	{
		byte sd[4] = {0}, se[4] = {0}, sh[4] = {0}, si[4] = {0};
		byte *line2, *line3;

		x = 1;

		line2 = in + ((y - 1) * width + (x - 1)) * 4;
		line3 = in + ((y + 1) * width + (x - 1)) * 4;

		COPYSAMPLE(sd, line2); line2 += 8;
		COPYSAMPLE(sh, line3); line3 += 8;

		outbyte = out + (y * width + x) * 4;

		for ( ; x < width - 1; x += 2)
		{
			COPYSAMPLE(se, line2); line2 += 8;
			COPYSAMPLE(si, line3); line3 += 8;

			for (i = 0; i < 4; i++)
			{	
				*outbyte++ = (sd[i] + si[i] + se[i] + sh[i]) >> 2;
			}

			outbyte += 4;

			COPYSAMPLE(sd, se);
			COPYSAMPLE(sh, si);
		}
	}

	// hack: copy out to in again
	for (y = 1; y < height - 1; y += 2)
	{
		x = 1;

		inbyte  = out + (y * width + x) * 4;
		outbyte = in  + (y * width + x) * 4;

		for ( ; x < width - 1; x += 2)
		{
			COPYSAMPLE(outbyte, inbyte);
			outbyte += 8;
			inbyte += 8;
		}
	}
	
	for (y = 1; y < height - 1; y++)
	{
		byte sd[4], sf[4], sg[4], si[4];
		byte *line2, *line3, *line4;

		x = y % 2 + 1;

		line2 = in + ((y - 1) * width + (x    )) * 4;
		line3 = in + ((y    ) * width + (x - 1)) * 4;
		line4 = in + ((y + 1) * width + (x    )) * 4;

		COPYSAMPLE(sf, line3); line3 += 8;

		outbyte = out + (y * width + x) * 4;

		for ( ; x < width - 1; x += 2)
		{
			COPYSAMPLE(sd, line2); line2 += 8;
			COPYSAMPLE(sg, line3); line3 += 8;
			COPYSAMPLE(si, line4); line4 += 8;

			for (i = 0; i < 4; i++)
			{
				*outbyte++ = (sf[i] + sg[i] + sd[i] + si[i]) >> 2;
			}

			outbyte += 4;

			COPYSAMPLE(sf, sg);
		}
	}
}


static void ExpandHalfTextureToGrid( byte *data, int width, int height)
{
	int x, y;

	for (y = height / 2; y > 0; y--)
	{
		byte *outbyte = data + ((y * 2 - 1) * (width)     - 2) * 4;
		byte *inbyte  = data + (y           * (width / 2) - 1) * 4;

		for (x = width / 2; x > 0; x--)
		{
			COPYSAMPLE(outbyte, inbyte);

			outbyte -= 8;
			inbyte -= 4;
		}
	}
}

static void FillInNormalizedZ(const byte *in, byte *out, int width, int height)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		const byte *inbyte  = in  + y * width * 4;
		byte       *outbyte = out + y * width * 4;

		for (x = 0; x < width; x++)
		{
			byte nx, ny, nz, h;
			float fnx, fny, fll, fnz;

			nx = *inbyte++;
			ny = *inbyte++;
			inbyte++;
			h  = *inbyte++;

			fnx = OffsetByteToFloat(nx);
			fny = OffsetByteToFloat(ny);
			fll = 1.0f - fnx * fnx - fny * fny;
			if (fll >= 0.0f)
				fnz = (float)sqrt(fll);
			else
				fnz = 0.0f;

			nz = FloatToOffsetByte(fnz);

			*outbyte++ = nx;
			*outbyte++ = ny;
			*outbyte++ = nz;
			*outbyte++ = h;
		}
	}
}


// size must be even
#define WORKBLOCK_SIZE     128
#define WORKBLOCK_BORDER   4
#define WORKBLOCK_REALSIZE (WORKBLOCK_SIZE + WORKBLOCK_BORDER * 2)

// assumes that data has already been expanded into a 2x2 grid
static void FCBIByBlock(byte *data, int width, int height, qboolean clampToEdge, qboolean normalized)
{
	byte workdata[WORKBLOCK_REALSIZE * WORKBLOCK_REALSIZE * 4];
	byte outdata[WORKBLOCK_REALSIZE * WORKBLOCK_REALSIZE * 4];
	byte *inbyte, *outbyte;
	int x, y;
	int srcx, srcy;

	ExpandHalfTextureToGrid(data, width, height);

	for (y = 0; y < height; y += WORKBLOCK_SIZE)
	{
		for (x = 0; x < width; x += WORKBLOCK_SIZE)
		{
			int x2, y2;
			int workwidth, workheight, fullworkwidth, fullworkheight;

			workwidth =  MIN(WORKBLOCK_SIZE, width  - x);
			workheight = MIN(WORKBLOCK_SIZE, height - y);

			fullworkwidth =  workwidth  + WORKBLOCK_BORDER * 2;
			fullworkheight = workheight + WORKBLOCK_BORDER * 2;

			//memset(workdata, 0, WORKBLOCK_REALSIZE * WORKBLOCK_REALSIZE * 4);

			// fill in work block
			for (y2 = 0; y2 < fullworkheight; y2 += 2)
			{
				srcy = y + y2 - WORKBLOCK_BORDER;

				if (clampToEdge)
				{
					srcy = CLAMP(srcy, 0, height - 2);
				}
				else
				{
					srcy = (srcy + height) % height;
				}

				outbyte = workdata + y2   * fullworkwidth * 4;
				inbyte  = data     + srcy * width         * 4;		

				for (x2 = 0; x2 < fullworkwidth; x2 += 2)
				{
					srcx = x + x2 - WORKBLOCK_BORDER;

					if (clampToEdge)
					{
						srcx = CLAMP(srcx, 0, width - 2);
					}
					else
					{
						srcx = (srcx + width) % width;
					}

					COPYSAMPLE(outbyte, inbyte + srcx * 4);
					outbyte += 8;
				}
			}

			// submit work block
			DoLinear(workdata, outdata, fullworkwidth, fullworkheight);

			if (!normalized)
			{
				switch (r_imageUpsampleType->integer)
				{
					case 0:
						break;
					case 1:
						DoFCBIQuick(workdata, outdata, fullworkwidth, fullworkheight, 0);
						break;
					case 2:
					default:
						DoFCBI(workdata, outdata, fullworkwidth, fullworkheight, 0);
						break;
				}
			}
			else
			{
				switch (r_imageUpsampleType->integer)
				{
					case 0:
						break;
					case 1:
						DoFCBIQuick(workdata, outdata, fullworkwidth, fullworkheight, 0);
						DoFCBIQuick(workdata, outdata, fullworkwidth, fullworkheight, 1);
						break;
					case 2:
					default:
						DoFCBI(workdata, outdata, fullworkwidth, fullworkheight, 0);
						DoFCBI(workdata, outdata, fullworkwidth, fullworkheight, 1);
						break;
				}
			}

			// copy back work block
			for (y2 = 0; y2 < workheight; y2++)
			{
				inbyte = outdata + ((y2 + WORKBLOCK_BORDER) * fullworkwidth + WORKBLOCK_BORDER) * 4;
				outbyte = data +   ((y + y2)                * width         + x)                * 4;
				for (x2 = 0; x2 < workwidth; x2++)
				{
					COPYSAMPLE(outbyte, inbyte);
					outbyte += 4;
					inbyte += 4;
				}
			}
		}
	}
}
#undef COPYSAMPLE

/*
================
R_LightScaleTexture

Scale up the pixel values in a texture to increase the
lighting range
================
*/
void R_LightScaleTexture (byte *in, int inwidth, int inheight, qboolean only_gamma )
{
	if ( only_gamma )
	{
		if ( !glConfig.deviceSupportsGamma )
		{
			int		i, c;
			byte	*p;

			p = in;

			c = inwidth*inheight;
			for (i=0 ; i<c ; i++, p+=4)
			{
				p[0] = s_gammatable[p[0]];
				p[1] = s_gammatable[p[1]];
				p[2] = s_gammatable[p[2]];
			}
		}
	}
	else
	{
		int		i, c;
		byte	*p;

		p = in;

		c = inwidth*inheight;

		if ( glConfig.deviceSupportsGamma )
		{
			for (i=0 ; i<c ; i++, p+=4)
			{
				p[0] = s_intensitytable[p[0]];
				p[1] = s_intensitytable[p[1]];
				p[2] = s_intensitytable[p[2]];
			}
		}
		else
		{
			for (i=0 ; i<c ; i++, p+=4)
			{
				p[0] = s_gammatable[s_intensitytable[p[0]]];
				p[1] = s_gammatable[s_intensitytable[p[1]]];
				p[2] = s_gammatable[s_intensitytable[p[2]]];
			}
		}
	}
}


/*
================
R_MipMapsRGB

Operates in place, quartering the size of the texture
Colors are gamma correct 
================
*/
static void R_MipMapsRGB( byte *in, int inWidth, int inHeight)
{
	int x, y, c, stride;
	const byte *in2;
	float total;
	static float downmipSrgbLookup[256];
	static int downmipSrgbLookupSet = 0;
	byte *out = in;

	if (!downmipSrgbLookupSet) {
		for (x = 0; x < 256; x++)
			downmipSrgbLookup[x] = powf(x / 255.0f, 2.2f) * 0.25f;
		downmipSrgbLookupSet = 1;
	}

	if (inWidth == 1 && inHeight == 1)
		return;

	if (inWidth == 1 || inHeight == 1) {
		for (x = (inWidth * inHeight) >> 1; x; x--) {
			for (c = 3; c; c--, in++) {
				total  = (downmipSrgbLookup[*(in)] + downmipSrgbLookup[*(in + 4)]) * 2.0f;

				*out++ = (byte)(powf(total, 1.0f / 2.2f) * 255.0f);
			}
			*out++ = (*(in) + *(in + 4)) >> 1; in += 5;
		}
		
		return;
	}

	stride = inWidth * 4;
	inWidth >>= 1; inHeight >>= 1;

	in2 = in + stride;
	for (y = inHeight; y; y--, in += stride, in2 += stride) {
		for (x = inWidth; x; x--) {
			for (c = 3; c; c--, in++, in2++) {
				total = downmipSrgbLookup[*(in)]  + downmipSrgbLookup[*(in + 4)]
				      + downmipSrgbLookup[*(in2)] + downmipSrgbLookup[*(in2 + 4)];

				*out++ = (byte)(powf(total, 1.0f / 2.2f) * 255.0f);
			}

			*out++ = (*(in) + *(in + 4) + *(in2) + *(in2 + 4)) >> 2; in += 5, in2 += 5;
		}
	}
}


static void R_MipMapNormalHeight (const byte *in, byte *out, int width, int height, qboolean swizzle)
{
	int		i, j;
	int		row;
	int sx = swizzle ? 3 : 0;
	int sa = swizzle ? 0 : 3;

	if ( width == 1 && height == 1 ) {
		return;
	}

	row = width * 4;
	width >>= 1;
	height >>= 1;
	
	for (i=0 ; i<height ; i++, in+=row) {
		for (j=0 ; j<width ; j++, out+=4, in+=8) {
			vec3_t v;

			v[0] =  OffsetByteToFloat(in[sx      ]);
			v[1] =  OffsetByteToFloat(in[       1]);
			v[2] =  OffsetByteToFloat(in[       2]);

			v[0] += OffsetByteToFloat(in[sx    +4]);
			v[1] += OffsetByteToFloat(in[       5]);
			v[2] += OffsetByteToFloat(in[       6]);

			v[0] += OffsetByteToFloat(in[sx+row  ]);
			v[1] += OffsetByteToFloat(in[   row+1]);
			v[2] += OffsetByteToFloat(in[   row+2]);

			v[0] += OffsetByteToFloat(in[sx+row+4]);
			v[1] += OffsetByteToFloat(in[   row+5]);
			v[2] += OffsetByteToFloat(in[   row+6]);

			VectorNormalizeFast(v);

			//v[0] *= 0.25f;
			//v[1] *= 0.25f;
			//v[2] = 1.0f - v[0] * v[0] - v[1] * v[1];
			//v[2] = sqrt(MAX(v[2], 0.0f));

			out[sx] = FloatToOffsetByte(v[0]);
			out[1 ] = FloatToOffsetByte(v[1]);
			out[2 ] = FloatToOffsetByte(v[2]);
			out[sa] = MAX(MAX(in[sa], in[sa+4]), MAX(in[sa+row], in[sa+row+4]));
		}
	}
}


/*
==================
R_BlendOverTexture

Apply a color blend over a set of pixels
==================
*/
static void R_BlendOverTexture( byte *data, int pixelCount, byte blend[4] ) {
	int		i;
	int		inverseAlpha;
	int		premult[3];

	inverseAlpha = 255 - blend[3];
	premult[0] = blend[0] * blend[3];
	premult[1] = blend[1] * blend[3];
	premult[2] = blend[2] * blend[3];

	for ( i = 0 ; i < pixelCount ; i++, data+=4 ) {
		data[0] = ( data[0] * inverseAlpha + premult[0] ) >> 9;
		data[1] = ( data[1] * inverseAlpha + premult[1] ) >> 9;
		data[2] = ( data[2] * inverseAlpha + premult[2] ) >> 9;
	}
}

byte	mipBlendColors[16][4] = {
	{0,0,0,0},
	{255,0,0,128},
	{0,255,0,128},
	{0,0,255,128},
	{255,0,0,128},
	{0,255,0,128},
	{0,0,255,128},
	{255,0,0,128},
	{0,255,0,128},
	{0,0,255,128},
	{255,0,0,128},
	{0,255,0,128},
	{0,0,255,128},
	{255,0,0,128},
	{0,255,0,128},
	{0,0,255,128},
};

static void RawImage_SwizzleRA( byte *data, int width, int height )
{
	int i;
	byte *ptr = data, swap;

	for (i=0; i<width*height; i++, ptr+=4)
	{
		// swap red and alpha
		swap = ptr[0];
		ptr[0] = ptr[3];
		ptr[3] = swap;
	}
}


/*
===============
RawImage_ScaleToPower2

===============
*/
static qboolean RawImage_ScaleToPower2( const textureLevel_t *pic, int baseLevel, int *inout_width, int *inout_height, imgType_t type, imgFlags_t flags, int picmip, byte **resampledBuffer)
{
	int width =         *inout_width;
	int height =        *inout_height;
	int scaled_width;
	int scaled_height;
	qboolean mipmap = flags & IMGFLAG_MIPMAP;
	qboolean clampToEdge = flags & IMGFLAG_CLAMPTOEDGE;
	qboolean scaled;

	//
	// convert to exact power of 2 sizes
	//
	if (!mipmap)
	{
		scaled_width = width;
		scaled_height = height;
	}
	else
	{
		scaled_width = NextPowerOfTwo(width);
		scaled_height = NextPowerOfTwo(height);
	}

	if ( r_roundImagesDown->integer && scaled_width > width )
		scaled_width >>= 1;
	if ( r_roundImagesDown->integer && scaled_height > height )
		scaled_height >>= 1;

	if ( picmip && pic && pic->data && resampledBuffer && r_imageUpsample->integer && 
	     scaled_width < r_imageUpsampleMaxSize->integer && scaled_height < r_imageUpsampleMaxSize->integer)
	{
		int finalwidth, finalheight;
		//int startTime, endTime;

		//startTime = ri.Milliseconds();

		finalwidth = scaled_width << r_imageUpsample->integer;
		finalheight = scaled_height << r_imageUpsample->integer;

		while ( finalwidth > r_imageUpsampleMaxSize->integer
			|| finalheight > r_imageUpsampleMaxSize->integer ) {
			finalwidth >>= 1;
			finalheight >>= 1;
		}

		while ( finalwidth > glConfig.maxTextureSize
			|| finalheight > glConfig.maxTextureSize ) {
			finalwidth >>= 1;
			finalheight >>= 1;
		}

		*resampledBuffer = ri.Hunk_AllocateTempMemory( finalwidth * finalheight * 4 );

		if (scaled_width != width || scaled_height != height)
			ResampleTexture (pic->data, width, height, *resampledBuffer, scaled_width, scaled_height);
		else
			Com_Memcpy(*resampledBuffer, pic->data, width * height * 4);

		if (type == IMGTYPE_COLORALPHA)
			RGBAtoYCoCgA(*resampledBuffer, *resampledBuffer, scaled_width, scaled_height);

		while (scaled_width < finalwidth || scaled_height < finalheight)
		{
			scaled_width <<= 1;
			scaled_height <<= 1;

			FCBIByBlock(*resampledBuffer, scaled_width, scaled_height, clampToEdge, (type == IMGTYPE_NORMAL || type == IMGTYPE_NORMALHEIGHT));
		}

		if (type == IMGTYPE_COLORALPHA)
			YCoCgAtoRGBA(*resampledBuffer, *resampledBuffer, scaled_width, scaled_height);
		else if (type == IMGTYPE_NORMAL || type == IMGTYPE_NORMALHEIGHT)
			FillInNormalizedZ(*resampledBuffer, *resampledBuffer, scaled_width, scaled_height);

		//endTime = ri.Milliseconds();

		//ri.Printf(PRINT_ALL, "upsampled %dx%d to %dx%d in %dms\n", width, height, scaled_width, scaled_height, endTime - startTime);
	}
	else if ( scaled_width != width || scaled_height != height )
	{
		if (pic && pic->data && resampledBuffer)
		{
			*resampledBuffer = ri.Hunk_AllocateTempMemory( scaled_width * scaled_height * 4 );
			ResampleTexture (pic->data, width, height, *resampledBuffer, scaled_width, scaled_height);
		}
	}

	width  = scaled_width;
	height = scaled_height;

	//
	// perform optional picmip operation
	//
	if ( picmip - baseLevel > 0 ) {
		scaled_width >>= picmip - baseLevel;
		scaled_height >>= picmip - baseLevel;

		if ( pic && pic->data && resampledBuffer && !*resampledBuffer ) {
			*resampledBuffer = ri.Hunk_AllocateTempMemory( width * height * 4 );
			Com_Memcpy(*resampledBuffer, pic->data, width * height * 4);
		}
	}

	//
	// clamp to the current upper OpenGL limit
	// scale both axis down equally so we don't have to
	// deal with a half mip resampling
	//
	while ( scaled_width > glConfig.maxTextureSize
		|| scaled_height > glConfig.maxTextureSize ) {
		scaled_width >>= 1;
		scaled_height >>= 1;
	}

	//
	// clamp to minimum size
	//
	scaled_width  = MAX(1, scaled_width);
	scaled_height = MAX(1, scaled_height);

	scaled = (width != scaled_width) || (height != scaled_height);

	//
	// rescale texture to new size using existing mipmap functions
	//
	if (resampledBuffer && *resampledBuffer)
	{
		while (width > scaled_width || height > scaled_height)
		{
			if (type == IMGTYPE_NORMAL || type == IMGTYPE_NORMALHEIGHT)
				R_MipMapNormalHeight(*resampledBuffer, *resampledBuffer, width, height, qfalse);
			else
				R_MipMapsRGB(*resampledBuffer, width, height);

			width  = MAX(1, width >> 1);
			height = MAX(1, height >> 1);
		}
	}

	*inout_width  = width;
	*inout_height = height;

	return scaled;
}


static qboolean RawImage_HasAlpha(const byte *scan, int numPixels)
{
	int i;

	if (!scan)
		return qtrue;

	for ( i = 0; i < numPixels; i++ )
	{
		if ( scan[i*4 + 3] != 255 ) 
		{
			return qtrue;
		}
	}

	return qfalse;
}

static GLenum RawImage_GetFormat(const textureLevel_t *pic, int numPixels, qboolean lightMap, imgType_t type, imgFlags_t flags)
{
	int samples = 3;
	GLenum internalFormat = GL_RGB;
	qboolean forceNoCompression = (flags & IMGFLAG_NO_COMPRESSION);
	qboolean normalmap = (type == IMGTYPE_NORMAL || type == IMGTYPE_NORMALHEIGHT);
	const byte *data;

	// check if pic is a compressed format
	if ( pic->format != GL_RGBA8 ) {
		return pic->format;
	}

	data = pic[0].data;

	if(normalmap)
	{
		if ((type == IMGTYPE_NORMALHEIGHT) && RawImage_HasAlpha(data, numPixels) && r_parallaxMapping->integer)
		{
			if (!forceNoCompression && glRefConfig.textureCompression & TCR_BPTC)
			{
				internalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
			}
			else if (!forceNoCompression && glConfig.textureCompression == TC_S3TC_ARB)
			{
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			}
			else if ( r_texturebits->integer == 16 )
			{
				internalFormat = GL_RGBA4;
			}
			else if ( r_texturebits->integer == 32 )
			{
				internalFormat = GL_RGBA8;
			}
			else
			{
				internalFormat = GL_RGBA;
			}
		}
		else
		{
			if (!forceNoCompression && glRefConfig.textureCompression & TCR_RGTC)
			{
				internalFormat = GL_COMPRESSED_RG_RGTC2;
			}
			else if (!forceNoCompression && glRefConfig.textureCompression & TCR_BPTC)
			{
				internalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
			}
			else if (!forceNoCompression && glConfig.textureCompression == TC_S3TC_ARB)
			{
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			}
			else if (r_texturebits->integer == 16)
			{
				internalFormat = GL_RGB5;
			}
			else if (r_texturebits->integer == 32)
			{
				internalFormat = GL_RGB8;
			}
			else
			{
				internalFormat = GL_RGB;
			}
		}
	}
	else if(lightMap)
	{
		// GL_LUMINANCE is not valid for OpenGL 3.2 Core context and
		// everything becomes solid black
		if(0 && r_greyscale->integer)
			internalFormat = GL_LUMINANCE;
		else
			internalFormat = GL_RGBA;
	}
	else
	{
		if (RawImage_HasAlpha(data, numPixels))
		{
			samples = 4;
		}

		// select proper internal format
		if ( samples == 3 )
		{
			if(0 && r_greyscale->integer)
			{
				if(r_texturebits->integer == 16 || r_texturebits->integer == 32)
					internalFormat = GL_LUMINANCE8;
				else
					internalFormat = GL_LUMINANCE;
			}
			else
			{
				if ( !forceNoCompression && (glRefConfig.textureCompression & TCR_BPTC) )
				{
					internalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
				}
				else if ( !forceNoCompression && glConfig.textureCompression == TC_S3TC_ARB )
				{
					internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				}
				else if ( !forceNoCompression && glConfig.textureCompression == TC_S3TC )
				{
					internalFormat = GL_RGB4_S3TC;
				}
				else if ( r_texturebits->integer == 16 )
				{
					internalFormat = GL_RGB5;
				}
				else if ( r_texturebits->integer == 32 )
				{
					internalFormat = GL_RGB8;
				}
				else
				{
					internalFormat = GL_RGB;
				}
			}
		}
		else if ( samples == 4 )
		{
			if(0 && r_greyscale->integer)
			{
				if(r_texturebits->integer == 16 || r_texturebits->integer == 32)
					internalFormat = GL_LUMINANCE8_ALPHA8;
				else
					internalFormat = GL_LUMINANCE_ALPHA;
			}
			else
			{
				if ( !forceNoCompression && (glRefConfig.textureCompression & TCR_BPTC) )
				{
					internalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
				}
				else if ( !forceNoCompression && glConfig.textureCompression == TC_S3TC_ARB )
				{
					internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				}
				else if ( r_texturebits->integer == 16 )
				{
					internalFormat = GL_RGBA4;
				}
				else if ( r_texturebits->integer == 32 )
				{
					internalFormat = GL_RGBA8;
				}
				else
				{
					internalFormat = GL_RGBA;
				}
			}
		}
	}

	return internalFormat;
}

static void CompressMonoBlock(byte outdata[8], const byte indata[16])
{
	int hi, lo, diff, bias, outbyte, shift, i;
	byte *p = outdata;

	hi = lo = indata[0];
	for (i = 1; i < 16; i++)
	{
		hi = MAX(indata[i], hi);
		lo = MIN(indata[i], lo);
	}

	*p++ = hi;
	*p++ = lo;

	diff = hi - lo;

	if (diff == 0)
	{
		outbyte = (hi == 255) ? 255 : 0;

		for (i = 0; i < 6; i++)
			*p++ = outbyte;

		return;
	}

	bias = diff / 2 - lo * 7;
	outbyte = shift = 0;
	for (i = 0; i < 16; i++)
	{
		const byte fixIndex[8] = { 1, 7, 6, 5, 4, 3, 2, 0 };
		byte index = fixIndex[(indata[i] * 7 + bias) / diff];

		outbyte |= index << shift;
		shift += 3;
		if (shift >= 8)
		{
			*p++ = outbyte & 0xff;
			shift -= 8;
			outbyte >>= 8;
		}
	}
}

static void RawImage_UploadToRgtc2Texture(GLuint texture, int miplevel, int x, int y, int width, int height, byte *data)
{
	int wBlocks, hBlocks, iy, ix, size;
	byte *compressedData, *p;

	wBlocks = (width + 3) / 4;
	hBlocks = (height + 3) / 4;
	size = wBlocks * hBlocks * 16;

	p = compressedData = ri.Hunk_AllocateTempMemory(size);
	for (iy = 0; iy < height; iy += 4)
	{
		int oh = MIN(4, height - iy);

		for (ix = 0; ix < width; ix += 4)
		{
			byte workingData[16];
			int component;

			int ow = MIN(4, width - ix);

			for (component = 0; component < 2; component++)
			{
				int ox, oy;

				for (oy = 0; oy < oh; oy++)
					for (ox = 0; ox < ow; ox++)
						workingData[oy * 4 + ox] = data[((iy + oy) * width + ix + ox) * 4 + component];

				// dupe data to fill
				for (oy = 0; oy < 4; oy++)
					for (ox = (oy < oh) ? ow : 0; ox < 4; ox++)
						workingData[oy * 4 + ox] = workingData[(oy % oh) * 4 + ox % ow];

				CompressMonoBlock(p, workingData);
				p += 8;
			}
		}
	}

	// FIXME: Won't work for x/y that aren't multiples of 4.
	qglCompressedTextureSubImage2DEXT(texture, GL_TEXTURE_2D, miplevel, x, y, width, height, GL_COMPRESSED_RG_RGTC2, size, compressedData);

	ri.Hunk_FreeTempMemory(compressedData);
}

static int CalculateMipSize(int width, int height, GLenum picFormat)
{
	int numBlocks = ((width + 3) / 4) * ((height + 3) / 4);
	int numPixels = width * height;

	switch (picFormat)
	{
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
		case GL_COMPRESSED_SRGB_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RED_RGTC1:
		case GL_COMPRESSED_SIGNED_RED_RGTC1:
			return numBlocks * 8;

		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT:
		case GL_COMPRESSED_RG_RGTC2:
		case GL_COMPRESSED_SIGNED_RG_RGTC2:
		case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
		case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
		case GL_COMPRESSED_RGBA_BPTC_UNORM_ARB:
		case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
			return numBlocks * 16;

		case GL_RGBA8:
		case GL_SRGB8_ALPHA8_EXT:
			return numPixels * 4;

		case GL_RGBA16:
			return numPixels * 8;

		default:
			ri.Printf(PRINT_ALL, "Unsupported texture format %08x\n", picFormat);
			return 0;
	}

	return 0;
}


static GLenum PixelDataFormatFromInternalFormat(GLenum internalFormat)
{
	switch (internalFormat)
	{
		case GL_DEPTH_COMPONENT:
		case GL_DEPTH_COMPONENT16_ARB:
		case GL_DEPTH_COMPONENT24_ARB:
		case GL_DEPTH_COMPONENT32_ARB:
			return GL_DEPTH_COMPONENT;
		default:
			return GL_RGBA;
			break;
	}
}

static void RawImage_UploadTexture(GLuint texture, byte *data, int x, int y, int width, int height, GLenum target, GLenum picFormat, int numMips, GLenum internalFormat, imgType_t type, imgFlags_t flags, qboolean subtexture )
{
	GLenum dataFormat, dataType;
	qboolean rgtc = internalFormat == GL_COMPRESSED_RG_RGTC2;
	qboolean rgba8 = picFormat == GL_RGBA8 || picFormat == GL_SRGB8_ALPHA8_EXT;
	qboolean rgba = rgba8 || picFormat == GL_RGBA16;
	qboolean mipmap = !!(flags & IMGFLAG_MIPMAP);
	int size, miplevel;
	qboolean lastMip = qfalse;

	dataFormat = PixelDataFormatFromInternalFormat(internalFormat);
	dataType = picFormat == GL_RGBA16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;

	miplevel = 0;
	do
	{
		lastMip = (width == 1 && height == 1) || !mipmap;
		size = CalculateMipSize(width, height, picFormat);

		if (!rgba)
		{
			qglCompressedTextureSubImage2DEXT(texture, target, miplevel, x, y, width, height, picFormat, size, data);
		}
		else
		{
			if (rgba8 && miplevel != 0 && r_colorMipLevels->integer)
				R_BlendOverTexture((byte *)data, width * height, mipBlendColors[miplevel]);

			if (rgba8 && rgtc)
				RawImage_UploadToRgtc2Texture(texture, miplevel, x, y, width, height, data);
			else
				qglTextureSubImage2DEXT(texture, target, miplevel, x, y, width, height, dataFormat, dataType, data);
		}

		if (!lastMip && numMips < 2)
		{
			if (glRefConfig.framebufferObject)
			{
				qglGenerateTextureMipmapEXT(texture, target);
				break;
			}
			else if (rgba8)
			{
				if (type == IMGTYPE_NORMAL || type == IMGTYPE_NORMALHEIGHT)
					R_MipMapNormalHeight(data, data, width, height, glRefConfig.swizzleNormalmap);
				else
					R_MipMapsRGB(data, width, height);
			}
		}

		x >>= 1;
		y >>= 1;
		width = MAX(1, width >> 1);
		height = MAX(1, height >> 1);
		miplevel++;

		if (numMips > 1)
		{
			data += size;
			numMips--;
		}
	}
	while (!lastMip);
}


static qboolean UploadOneTexLevel( GLuint texture, int level, const textureLevel_t *pic )
{
	if ( pic->format != GL_RGBA8 ) {
		qglCompressedTextureSubImage2DEXT( texture, GL_TEXTURE_2D, level,
						0, 0, pic->width, pic->height,
						pic->format, pic->size, pic->data );
	} else {
		qglTextureSubImage2DEXT( texture, GL_TEXTURE_2D, level,
						0, 0, pic->width, pic->height,
						GL_RGBA, GL_UNSIGNED_BYTE, pic->data );
	}

	return qtrue;
}

/*
===============
Upload32

===============
*/
static void Upload32(int numTexLevels, const textureLevel_t *pics, int x, int y, int width, int height, GLenum picFormat, image_t *image, qboolean scaled, int picmip)
{
	int			i, c;
	byte		*scan;
	int			baseLevel;
	byte		*data;

	int numMips = numTexLevels;
	imgType_t type = image->type;
	imgFlags_t flags = image->flags;
	GLenum internalFormat = image->internalFormat;
	qboolean subtexture = (x != 0) || (y != 0) || (width != image->uploadWidth) || (height != image->uploadHeight);
	qboolean rgba8 = picFormat == GL_RGBA8 || picFormat == GL_SRGB8_ALPHA8_EXT;
	qboolean mipmap = !!(flags & IMGFLAG_MIPMAP) && (rgba8 || numMips > 1);
	qboolean cubemap = !!(flags & IMGFLAG_CUBEMAP);

	if ( subtexture || !pics ) {
		baseLevel = 0;
		data = pics ? pics[baseLevel].data : NULL;
	} else {
		// we may skip some textureLevels, if r_picmip is set
		if ( picmip ) {
			baseLevel = Com_Clamp( 0, numTexLevels - 1, picmip );
		} else {
			baseLevel = 0;
		}

		width = pics[baseLevel].width;
		height = pics[baseLevel].height;

		// ZTM: FIXME: Check for compressed format, not non-rgba8
		if ( !rgba8 && !cubemap ) {
			// compressed texture
			for( i = baseLevel; i < numTexLevels; i++ ) {
				if( !UploadOneTexLevel( image->texnum, i - baseLevel, &pics[i] ) )
					break;
			}
			if( i >= numTexLevels )
				return;

			// failed to upload all levels
			ri.Error(ERR_DROP, "Unsupported Texture format: %x for %s", pics[0].format, image->imgName );
		} else {
			data = pics[baseLevel].data;
		}
	}

	// These operations cannot be performed on non-rgba8 images.
	if (rgba8 && !cubemap)
	{
		c = width*height;
		scan = data;

		if (type == IMGTYPE_COLORALPHA)
		{
			if( r_greyscale->integer )
			{
				for ( i = 0; i < c; i++ )
				{
					byte luma = LUMA(scan[i*4], scan[i*4 + 1], scan[i*4 + 2]);
					scan[i*4] = luma;
					scan[i*4 + 1] = luma;
					scan[i*4 + 2] = luma;
				}
			}
			else if( r_greyscale->value )
			{
				for ( i = 0; i < c; i++ )
				{
					float luma = LUMA(scan[i*4], scan[i*4 + 1], scan[i*4 + 2]);
					scan[i*4] = LERP(scan[i*4], luma, r_greyscale->value);
					scan[i*4 + 1] = LERP(scan[i*4 + 1], luma, r_greyscale->value);
					scan[i*4 + 2] = LERP(scan[i*4 + 2], luma, r_greyscale->value);
				}
			}

			// This corresponds to what the OpenGL1 renderer does.
			if (!(flags & IMGFLAG_NOLIGHTSCALE) && (scaled || mipmap))
				R_LightScaleTexture(data, width, height, !mipmap);
		}

		if (glRefConfig.swizzleNormalmap && (type == IMGTYPE_NORMAL || type == IMGTYPE_NORMALHEIGHT))
			RawImage_SwizzleRA(data, width, height);
	}

	if (cubemap)
	{
		for (i = 0; i < 6; i++)
		{
			int w2 = width, h2 = height;
			RawImage_UploadTexture(image->texnum, data, x, y, width, height, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, picFormat, numMips, internalFormat, type, flags, qfalse);
			for (c = numMips; c; c--)
			{
				data += CalculateMipSize(w2, h2, picFormat);
				w2 = MAX(1, w2 >> 1);
				h2 = MAX(1, h2 >> 1);
			}
		}
	}
	else
	{
		RawImage_UploadTexture(image->texnum, data, x, y, width, height, GL_TEXTURE_2D, picFormat, numMips, internalFormat, type, flags, qfalse);
	}

	GL_CheckErrors();
}


/*
================
R_CreateImage

This is the only way any image_t are created
================
*/
image_t *R_CreateImage( const char *name, byte *pic, int width, int height,
		imgType_t type, imgFlags_t flags, int internalFormat ) {
	textureLevel_t texLevel;

	texLevel.format = GL_RGBA8;
	texLevel.width = width;
	texLevel.height = height;
	texLevel.size = width * height * 4;
	texLevel.data = pic;

	return R_CreateImage2( name, 1, &texLevel, type, flags, internalFormat );
}

/*
================
R_CreateImage2

This is the only way any image_t are created
================
*/
image_t *R_CreateImage2( const char *name, int numTexLevels, const textureLevel_t *pics,
		imgType_t type, imgFlags_t flags, int internalFormat ) {
	byte       *resampledBuffer = NULL;
	image_t    *image;
	qboolean    isLightmap = qfalse, scaled = qfalse;
	long        hash;
	int         glWrapClampMode, mipWidth, mipHeight, miplevel;
	GLenum      picFormat = pics[0].format;
	int         width = pics[0].width;
	int         height = pics[0].height;
	qboolean    rgba8 = picFormat == GL_RGBA8 || picFormat == GL_SRGB8_ALPHA8_EXT;
	qboolean    mipmap = !!(flags & IMGFLAG_MIPMAP);
	qboolean    cubemap = !!(flags & IMGFLAG_CUBEMAP);
	int         picmip;
	qboolean    lastMip;
	GLenum textureTarget = cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	GLenum dataFormat;

	if (strlen(name) >= MAX_QPATH ) {
		ri.Error (ERR_DROP, "R_CreateImage: \"%s\" is too long", name);
	}
	if ( !strncmp( name, "*lightmap", 9 ) ) {
		isLightmap = qtrue;
	}

	if ( tr.numImages == MAX_DRAWIMAGES ) {
		ri.Error( ERR_DROP, "R_CreateImage: MAX_DRAWIMAGES hit");
	}

	image = tr.images[tr.numImages] = ri.Hunk_Alloc( sizeof( image_t ), h_low );
	qglGenTextures(1, &image->texnum);
	tr.numImages++;

	image->type = type;
	image->flags = flags;

	strcpy (image->imgName, name);

	image->width = width;
	image->height = height;
	if (flags & IMGFLAG_CLAMPTOEDGE)
		glWrapClampMode = GL_CLAMP_TO_EDGE;
	else
		glWrapClampMode = GL_REPEAT;

	if ( image->flags & IMGFLAG_PICMIP2 )
		picmip = r_picmip2->integer;
	else if ( image->flags & IMGFLAG_PICMIP )
		picmip = r_picmip->integer;
	else
		picmip = 0;

	if (!internalFormat)
		internalFormat = RawImage_GetFormat(&pics[0], width * height, isLightmap, image->type, image->flags);

	image->internalFormat = internalFormat;

	// Possibly scale image before uploading.
	// if not rgba8 and uploading an image, skip picmips.
	if (!cubemap)
	{
		if (rgba8 && numTexLevels == 1)
			scaled = RawImage_ScaleToPower2(&pics[0], 0, &width, &height, type, flags, picmip, &resampledBuffer);
		else if (pics[0].data && picmip)
		{
			int baseLevel = Com_Clamp( 0, numTexLevels - 1, picmip );
			width = pics[baseLevel].width;
			height = pics[baseLevel].height;
		}
	}

	image->uploadWidth = width;
	image->uploadHeight = height;

	// Allocate texture storage so we don't have to worry about it later.
	dataFormat = PixelDataFormatFromInternalFormat(internalFormat);
	mipWidth = width;
	mipHeight = height;
	miplevel = 0;
	do
	{
		lastMip = !mipmap || (mipWidth == 1 && mipHeight == 1);
		if (cubemap)
		{
			int i;

			for (i = 0; i < 6; i++)
				qglTextureImage2DEXT(image->texnum, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, miplevel, internalFormat, mipWidth, mipHeight, 0, dataFormat, GL_UNSIGNED_BYTE, NULL);
		}
		else
		{
			qglTextureImage2DEXT(image->texnum, GL_TEXTURE_2D, miplevel, internalFormat, mipWidth, mipHeight, 0, dataFormat, GL_UNSIGNED_BYTE, NULL);
		}

		mipWidth  = MAX(1, mipWidth >> 1);
		mipHeight = MAX(1, mipHeight >> 1);
		miplevel++;
	}
	while (!lastMip);

	// Upload data.
	if (resampledBuffer != NULL) {
		textureLevel_t texLevel;

		texLevel.format = picFormat;
		texLevel.width = width;
		texLevel.height = height;
		texLevel.size = width * height * 4;
		texLevel.data = resampledBuffer;
		Upload32(1, &texLevel, 0, 0, width, height, picFormat, image, scaled, picmip);

		ri.Hunk_FreeTempMemory(resampledBuffer);
	}
	else if (pics[0].data) {
		Upload32(numTexLevels, pics, 0, 0, width, height, picFormat, image, scaled, picmip);
	}

	// Set all necessary texture parameters.
	qglTextureParameterfEXT(image->texnum, textureTarget, GL_TEXTURE_WRAP_S, glWrapClampMode);
	qglTextureParameterfEXT(image->texnum, textureTarget, GL_TEXTURE_WRAP_T, glWrapClampMode);

	if (cubemap)
		qglTextureParameteriEXT(image->texnum, textureTarget, GL_TEXTURE_WRAP_R, glWrapClampMode);

	if (glConfig.textureFilterAnisotropic && !cubemap)
		qglTextureParameteriEXT(image->texnum, textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT,
			mipmap ? (GLint)Com_Clamp(1, glConfig.maxAnisotropy, r_ext_max_anisotropy->integer) : 1);

	switch(internalFormat)
	{
		case GL_DEPTH_COMPONENT:
		case GL_DEPTH_COMPONENT16_ARB:
		case GL_DEPTH_COMPONENT24_ARB:
		case GL_DEPTH_COMPONENT32_ARB:
			// Fix for sampling depth buffer on old nVidia cards.
			// from http://www.idevgames.com/forums/thread-4141-post-34844.html#pid34844
			if ( !QGL_VERSION_ATLEAST( 3, 0 ) ) {
				qglTextureParameterfEXT(image->texnum, textureTarget, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
			}
			qglTextureParameterfEXT(image->texnum, textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			qglTextureParameterfEXT(image->texnum, textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		default:
			qglTextureParameterfEXT(image->texnum, textureTarget, GL_TEXTURE_MIN_FILTER, mipmap ? gl_filter_min : GL_LINEAR);
			qglTextureParameterfEXT(image->texnum, textureTarget, GL_TEXTURE_MAG_FILTER, mipmap ? gl_filter_max : GL_LINEAR);
			break;
	}

	GL_CheckErrors();

	hash = generateHashValue(name);
	image->next = hashTable[hash];
	hashTable[hash] = image;

	return image;
}

void R_UpdateSubImage( image_t *image, byte *pic, int x, int y, int width, int height, GLenum picFormat )
{
	textureLevel_t texLevel;

	texLevel.format = GL_RGBA8;
	texLevel.width = width;
	texLevel.height = height;
	texLevel.size = width * height * 4;
	texLevel.data = pic;

	Upload32(1, &texLevel, x, y, width, height, picFormat, image, qfalse, 0);
}

//===================================================================

typedef struct
{
	char *ext;
	void (*ImageLoader)( const char *, int *, textureLevel_t ** );
} imageExtToLoaderMap_t;

// Note that the ordering indicates the order of preference used
// when there are multiple images of different formats available
static imageExtToLoaderMap_t imageLoaders[ ] =
{
	{ "tga",  R_LoadTGA },
	{ "jpg",  R_LoadJPG },
	{ "jpeg", R_LoadJPG },
	{ "png",  R_LoadPNG },
	{ "ftx",  R_LoadFTX },
	{ "dds",  R_LoadDDS },
	{ "pcx",  R_LoadPCX },
	{ "bmp",  R_LoadBMP }
};

static int numImageLoaders = ARRAY_LEN( imageLoaders );

/*
=================
R_LoadImage

Loads any of the supported image types into a canonical
32 bit format.
=================
*/
void R_LoadImage( const char *name, int *numLevels, textureLevel_t **pic )
{
	qboolean orgNameFailed = qfalse;
	int orgLoader = -1;
	int i;
	char localName[ MAX_QPATH ];
	const char *ext;
	char *altName;

	*pic = NULL;
	*numLevels = 0;

	Q_strncpyz( localName, name, MAX_QPATH );

	ext = COM_GetExtension( localName );

	if( *ext )
	{
		// Look for the correct loader and use it
		for( i = 0; i < numImageLoaders; i++ )
		{
			if( !Q_stricmp( ext, imageLoaders[ i ].ext ) )
			{
				// Load
				imageLoaders[ i ].ImageLoader( localName, numLevels, pic );
				break;
			}
		}

		// A loader was found
		if( i < numImageLoaders )
		{
			if( *pic == NULL )
			{
				// Loader failed, most likely because the file isn't there;
				// try again without the extension
				orgNameFailed = qtrue;
				orgLoader = i;
				COM_StripExtension( name, localName, MAX_QPATH );
			}
			else
			{
				// Something loaded
				return;
			}
		}
	}

	// Try and find a suitable match using all
	// the image formats supported
	for( i = 0; i < numImageLoaders; i++ )
	{
		if (i == orgLoader)
			continue;

		altName = va( "%s.%s", localName, imageLoaders[ i ].ext );

		// Load
		imageLoaders[ i ].ImageLoader( altName, numLevels, pic );

		if( *pic )
		{
			if( orgNameFailed )
			{
				ri.Printf( PRINT_DEVELOPER, "WARNING: %s not present, using %s instead\n",
						name, altName );
			}

			break;
		}
	}
}


/*
===============
R_FindImageFile

Finds or loads the given image.
Returns NULL if it fails, not a default image.
==============
*/
image_t	*R_FindImageFile( const char *name, imgType_t type, imgFlags_t flags )
{
	image_t	*image;
	int	numLevels;
	textureLevel_t	*pic;
	long	hash;
	int		textureInternalFormat = 0;

	if (!name) {
		return NULL;
	}

	hash = generateHashValue(name);

	//
	// see if the image is already loaded
	//
	for (image=hashTable[hash]; image; image=image->next) {
		if ( !strcmp( name, image->imgName ) ) {
			// the white image can be used with any set of parms, but other mismatches are errors
			if ( strcmp( name, "*white" ) ) {
				if ( image->flags != flags ) {
					ri.Printf( PRINT_DEVELOPER, "WARNING: reused image %s with mixed flags (%i vs %i)\n", name, image->flags, flags );
				}
			}
			return image;
		}
	}

	//
	// load the pic from disk
	//
	R_LoadImage( name, &numLevels, &pic );
	if ( pic == NULL ) {
		return NULL;
	}

	// apply lightmap coloring
	if ( ( flags & IMGFLAG_LIGHTMAP ) && pic[0].format == GL_RGBA8 ) {
		R_ProcessLightmap( (byte**)&pic[0].data, 4, pic[0].width, pic[0].height, (byte**)&pic[0].data, qfalse, pic[0].format );
	}
	else if (r_normalMapping->integer && (pic[0].format == GL_RGBA8) && (type == IMGTYPE_COLORALPHA) &&
		(flags & (IMGFLAG_PICMIP|IMGFLAG_PICMIP2)) && (flags & IMGFLAG_MIPMAP) && (flags & IMGFLAG_GENNORMALMAP) && !(flags & IMGFLAG_CUBEMAP))
	{
		char normalName[MAX_QPATH];
		image_t *normalImage;
		int normalWidth, normalHeight;
		imgFlags_t normalFlags;

		normalFlags = (flags & ~IMGFLAG_GENNORMALMAP) | IMGFLAG_NOLIGHTSCALE;

		COM_StripExtension(name, normalName, MAX_QPATH);
		Q_strcat(normalName, MAX_QPATH, "_n");

		// find normalmap in case it's there
		normalImage = R_FindImageFile(normalName, IMGTYPE_NORMAL, normalFlags);

		// if not, generate it
		if (normalImage == NULL && pic->format == GL_RGBA8)
		{
			byte *normalPic;
			byte *basePic;
			int x, y, width, height;

			basePic = pic[0].data;
			width = pic[0].width;
			height = pic[0].height;

			normalWidth = width;
			normalHeight = height;
			normalPic = ri.Malloc(width * height * 4);
			RGBAtoNormal(basePic, normalPic, width, height, flags & IMGFLAG_CLAMPTOEDGE);

#if 1
			// Brighten up the original image to work with the normal map
			RGBAtoYCoCgA(basePic, basePic, width, height);
			for (y = 0; y < height; y++)
			{
				byte *picbyte  = basePic   + y * width * 4;
				byte *normbyte = normalPic + y * width * 4;
				for (x = 0; x < width; x++)
				{
					int div = MAX(normbyte[2] - 127, 16);
					picbyte[0] = CLAMP(picbyte[0] * 128 / div, 0, 255);
					picbyte  += 4;
					normbyte += 4;
				}
			}
			YCoCgAtoRGBA(basePic, basePic, width, height);
#else
			// Blur original image's luma to work with the normal map
			{
				byte *blurPic;

				RGBAtoYCoCgA(basePic, basePic, width, height);
				blurPic = ri.Malloc(width * height);

				for (y = 1; y < height - 1; y++)
				{
					byte *picbyte  = basePic + y * width * 4;
					byte *blurbyte = blurPic + y * width;

					picbyte += 4;
					blurbyte += 1;

					for (x = 1; x < width - 1; x++)
					{
						int result;

						result = *(picbyte - (width + 1) * 4) + *(picbyte - width * 4) + *(picbyte - (width - 1) * 4) +
						         *(picbyte -          1  * 4) + *(picbyte            ) + *(picbyte +          1  * 4) +
						         *(picbyte + (width - 1) * 4) + *(picbyte + width * 4) + *(picbyte + (width + 1) * 4);

						result /= 9;

						*blurbyte = result;
						picbyte += 4;
						blurbyte += 1;
					}
				}

				// FIXME: do borders

				for (y = 1; y < height - 1; y++)
				{
					byte *picbyte  = basePic + y * width * 4;
					byte *blurbyte = blurPic + y * width;

					picbyte += 4;
					blurbyte += 1;

					for (x = 1; x < width - 1; x++)
					{
						picbyte[0] = *blurbyte;
						picbyte += 4;
						blurbyte += 1;
					}
				}

				ri.Free(blurPic);

				YCoCgAtoRGBA(basePic, basePic, width, height);
			}
#endif

			R_CreateImage( normalName, normalPic, normalWidth, normalHeight, IMGTYPE_NORMAL, normalFlags, 0 );
			ri.Free( normalPic );	
		}
	}

	// force mipmaps off if image is compressed but doesn't have enough mips
	if ((flags & IMGFLAG_MIPMAP) && pic[0].format != GL_RGBA8 && pic[0].format != GL_SRGB8_ALPHA8_EXT)
	{
		int wh = MAX(pic[0].width, pic[0].height);
		int neededMips = 0;
		while (wh)
		{
			neededMips++;
			wh >>= 1;
		}
		if (neededMips > numLevels)
			flags &= ~IMGFLAG_MIPMAP;
	}

	image = R_CreateImage2( ( char * ) name, numLevels, pic, type, flags, textureInternalFormat );
	ri.Free( pic );
	return image;
}


/*
================
R_CreateDlightImage
================
*/
#define	DLIGHT_SIZE	128
static void R_CreateDlightImage( void ) {
	int		x,y;
	byte	data[DLIGHT_SIZE*DLIGHT_SIZE][4];
	int		b;
	int		dlightSize;

	dlightSize = r_dlightImageSize->integer;

	// check if not a power of two
	if ( ( dlightSize & ( dlightSize - 1 ) ) != 0 ) {
		if (dlightSize < 16)
			dlightSize = 16;
		else if (dlightSize < 32)
			dlightSize = 32;
		else if (dlightSize < 64)
			dlightSize = 64;
		else
			dlightSize = 128;

		ri.Printf( PRINT_WARNING, "WARNING: r_dlightImageSize (%d) is not a power of two, using next power of two (%d).\n", r_dlightImageSize->integer, dlightSize );
	}

	// make a centered inverse-square falloff blob for dynamic lighting
	for (x = 0; x < dlightSize; x++) {
		for (y = 0; y < dlightSize; y++) {
			float	d;

			d = ( dlightSize/2 - 0.5f - x ) * ( dlightSize/2 - 0.5f - x ) +
				( dlightSize/2 - 0.5f - y ) * ( dlightSize/2 - 0.5f - y );
			b = dlightSize * dlightSize * 15.625f / d;

			if (dlightSize >= 64) {
				// ZTM: Fade outside edge to look similar to the original 16x16 strected image.
				// Fade to black using 16 steps, unrelated to original size.
				// 75 - (FADE_STEPS / 2) = 67. 67 + FADE_STEPS = 83.
				if (b > 255) {
					b = 255;
				} else if (b < 83 && b > 67) {
					b = ( b - 67 ) / 16.0f * 83;
				} else if (b <= 67) {
					b = 0;
				}
			} else {
				if (b > 255) {
					b = 255;
				} else if (b < 75) {
					b = 0;
				}
			}

			data[y*dlightSize + x][0] =
			data[y*dlightSize + x][1] =
			data[y*dlightSize + x][2] = b;
			data[y*dlightSize + x][3] = 255;
		}
	}
	tr.dlightImage = R_CreateImage("*dlight", (byte *)data, dlightSize, dlightSize, IMGTYPE_COLORALPHA, IMGFLAG_CLAMPTOEDGE, 0 );
}


/*
=================
R_InitFogTable
=================
*/
void R_InitFogTable( void ) {
	int		i;
	float	d;

	for ( i = 0 ; i < FOG_TABLE_SIZE ; i++ ) {
		d = pow ( (float)i/(FOG_TABLE_SIZE-1), DEFAULT_FOG_EXP_DENSITY );

		tr.fogTable[i] = d;
	}
}

/*
================
R_FogFactor

Returns a 0.0 to 1.0 fog density value
This is called for each texel of the fog texture on startup
and for each vertex of transparent shaders in fog dynamically
================
*/
float	R_FogFactor( float s, float t ) {
	float	d;

	s -= 1.0/512;
	if ( s < 0 ) {
		return 0;
	}
	if ( t < 1.0/32 ) {
		return 0;
	}
	if ( t < 31.0/32 ) {
		s *= (t - 1.0f/32.0f) / (30.0f/32.0f);
	}

	// we need to leave a lot of clamp range
	s *= 8;

	if ( s > 1.0 ) {
		s = 1.0;
	}

	d = tr.fogTable[ (int)(s * (FOG_TABLE_SIZE-1)) ];

	return d;
}

/*
================
R_FogTcScale
================
*/
float R_FogTcScale( fogType_t fogType, float depthForOpaque, float density ) {
	float scale;

	if ( fogType == FT_LINEAR ) {
		scale = DEFAULT_FOG_LINEAR_DENSITY / density;
		return ( 1.0f / ( depthForOpaque * scale ) );
	}

	// exponential fog
	scale = DEFAULT_FOG_EXP_DENSITY / density;
	return ( 1.0f / ( depthForOpaque * 8 * scale ) );
}

/*
================
R_CreateFogImages

Create fog images for exponential and linear fog.
================
*/
static void R_CreateFogImages( void ) {
	int		x, y, alpha;
	byte	*data;
	float	d;
	int		fog_s, fog_t;

	// Create exponential fog image
	fog_s = 256;
	fog_t = 32;
	data = ri.Hunk_AllocateTempMemory( fog_s * fog_t * 4 );

	// S is distance, T is depth
	for (x=0 ; x<fog_s ; x++) {
		for (y=0 ; y<fog_t ; y++) {
			d = R_FogFactor( ( x + 0.5f ) / fog_s, ( y + 0.5f ) / fog_t );

			data[(y*fog_s+x)*4+0] = 
			data[(y*fog_s+x)*4+1] = 
			data[(y*fog_s+x)*4+2] = 255;
			data[(y*fog_s+x)*4+3] = 255*d;
		}
	}

	tr.fogImage = R_CreateImage("*fog", (byte *)data, fog_s, fog_t, IMGTYPE_COLORALPHA, IMGFLAG_CLAMPTOEDGE, 0 );
	ri.Hunk_FreeTempMemory( data );


	// Create linear fog image
	fog_s = 16;
	fog_t = 16;
	data = ri.Hunk_AllocateTempMemory( fog_s * fog_t * 4 );

	// ydnar: new, linear fog texture generating algo for GL_CLAMP_TO_EDGE (OpenGL 1.2+)

	// S is distance, T is depth
	for ( x = 0 ; x < fog_s ; x++ ) {
		for ( y = 0 ; y < fog_t ; y++ ) {
			alpha = 270 * ( (float) x / fog_s ) * ( (float) y / fog_t );    // need slop room for fp round to 0
			if ( alpha < 0 ) {
				alpha = 0;
			} else if ( alpha > 255 ) {
				alpha = 255;
			}

			// ensure edge/corner cases are fully transparent (at 0,0) or fully opaque (at 1,N where N is 0-1.0)
			if ( x == 0 ) {
				alpha = 0;
			} else if ( x == ( fog_s - 1 ) ) {
				alpha = 255;
			}

			data[( y * fog_s + x ) * 4 + 0] =
			data[( y * fog_s + x ) * 4 + 1] =
			data[( y * fog_s + x ) * 4 + 2] = 255;
			data[( y * fog_s + x ) * 4 + 3] = alpha;
		}
	}

	tr.linearFogImage = R_CreateImage("*linearfog", (byte *)data, fog_s, fog_t, IMGTYPE_COLORALPHA, IMGFLAG_CLAMPTOEDGE, 0 );
	ri.Hunk_FreeTempMemory( data );
}

/*
==================
R_CreateDefaultImage
==================
*/
#define	DEFAULT_SIZE	16
static void R_CreateDefaultImage( void ) {
	int		x;
	byte	data[DEFAULT_SIZE][DEFAULT_SIZE][4];

	// the default image will be a box, to allow you to see the mapping coordinates
	Com_Memset( data, 32, sizeof( data ) );
	for ( x = 0 ; x < DEFAULT_SIZE ; x++ ) {
		data[0][x][0] =
		data[0][x][1] =
		data[0][x][2] =
		data[0][x][3] = 255;

		data[x][0][0] =
		data[x][0][1] =
		data[x][0][2] =
		data[x][0][3] = 255;

		data[DEFAULT_SIZE-1][x][0] =
		data[DEFAULT_SIZE-1][x][1] =
		data[DEFAULT_SIZE-1][x][2] =
		data[DEFAULT_SIZE-1][x][3] = 255;

		data[x][DEFAULT_SIZE-1][0] =
		data[x][DEFAULT_SIZE-1][1] =
		data[x][DEFAULT_SIZE-1][2] =
		data[x][DEFAULT_SIZE-1][3] = 255;
	}
	tr.defaultImage = R_CreateImage("*default", (byte *)data, DEFAULT_SIZE, DEFAULT_SIZE, IMGTYPE_COLORALPHA, IMGFLAG_MIPMAP, 0);
}

/*
==================
R_CreateBuiltinImages
==================
*/
void R_CreateBuiltinImages( void ) {
	int		x,y;
	byte	data[DEFAULT_SIZE][DEFAULT_SIZE][4];

	R_CreateDefaultImage();

	// we use a solid white image instead of disabling texturing
	Com_Memset( data, 255, sizeof( data ) );
	tr.whiteImage = R_CreateImage("*white", (byte *)data, 8, 8, IMGTYPE_COLORALPHA, IMGFLAG_NONE, 0);

	if (r_dlightMode->integer >= 2)
	{
		for( x = 0; x < MAX_DLIGHTS; x++)
		{
			tr.shadowCubemaps[x] = R_CreateImage(va("*shadowcubemap%i", x), NULL, PSHADOW_MAP_SIZE, PSHADOW_MAP_SIZE, IMGTYPE_COLORALPHA, IMGFLAG_CLAMPTOEDGE | IMGFLAG_CUBEMAP, 0);
		}
	}

	// with overbright bits active, we need an image which is some fraction of full color,
	// for default lightmaps, etc
	for (x=0 ; x<DEFAULT_SIZE ; x++) {
		for (y=0 ; y<DEFAULT_SIZE ; y++) {
			data[y][x][0] = 
			data[y][x][1] = 
			data[y][x][2] = tr.identityLightByte;
			data[y][x][3] = 255;			
		}
	}

	tr.identityLightImage = R_CreateImage("*identityLight", (byte *)data, 8, 8, IMGTYPE_COLORALPHA, IMGFLAG_NONE, 0);


	for(x=0;x<32;x++) {
		// scratchimage is usually used for cinematic drawing
		tr.scratchImage[x] = R_CreateImage("*scratch", (byte *)data, DEFAULT_SIZE, DEFAULT_SIZE, IMGTYPE_COLORALPHA, IMGFLAG_PICMIP | IMGFLAG_CLAMPTOEDGE, 0);
	}

	R_CreateDlightImage();
	R_CreateFogImages();

	if (glRefConfig.framebufferObject)
	{
		int width, height, hdrFormat, rgbFormat;

		width = glConfig.vidWidth;
		height = glConfig.vidHeight;

		hdrFormat = GL_RGBA8;
		if (r_hdr->integer && glRefConfig.textureFloat)
			hdrFormat = GL_RGBA16F_ARB;

		rgbFormat = GL_RGBA8;

		tr.renderImage = R_CreateImage("_render", NULL, width, height, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, hdrFormat);

		if (r_shadowBlur->integer)
			tr.screenScratchImage = R_CreateImage("screenScratch", NULL, width, height, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, rgbFormat);

		if (r_shadowBlur->integer || r_ssao->integer)
			tr.hdrDepthImage = R_CreateImage("*hdrDepth", NULL, width, height, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_R32F);

		if (r_drawSunRays->integer)
			tr.sunRaysImage = R_CreateImage("*sunRays", NULL, width, height, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, rgbFormat);

		tr.renderDepthImage  = R_CreateImage("*renderdepth",  NULL, width, height, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_DEPTH_COMPONENT24);
		tr.textureDepthImage = R_CreateImage("*texturedepth", NULL, PSHADOW_MAP_SIZE, PSHADOW_MAP_SIZE, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_DEPTH_COMPONENT24);

		{
			void *p;

			data[0][0][0] = 0;
			data[0][0][1] = 0.45f * 255;
			data[0][0][2] = 255;
			data[0][0][3] = 255;
			p = data;

			tr.calcLevelsImage =   R_CreateImage("*calcLevels",    p, 1, 1, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, hdrFormat);
			tr.targetLevelsImage = R_CreateImage("*targetLevels",  p, 1, 1, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, hdrFormat);
			tr.fixedLevelsImage =  R_CreateImage("*fixedLevels",   p, 1, 1, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, hdrFormat);
		}

		for (x = 0; x < 2; x++)
		{
			tr.textureScratchImage[x] = R_CreateImage(va("*textureScratch%d", x), NULL, 256, 256, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_RGBA8);
		}
		for (x = 0; x < 2; x++)
		{
			tr.quarterImage[x] = R_CreateImage(va("*quarter%d", x), NULL, width / 2, height / 2, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_RGBA8);
		}

		if (r_ssao->integer)
		{
			tr.screenSsaoImage = R_CreateImage("*screenSsao", NULL, width / 2, height / 2, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_RGBA8);
		}

		for( x = 0; x < MAX_DRAWN_PSHADOWS; x++)
		{
			tr.pshadowMaps[x] = R_CreateImage(va("*shadowmap%i", x), NULL, PSHADOW_MAP_SIZE, PSHADOW_MAP_SIZE, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_DEPTH_COMPONENT24);
			//qglTextureParameterfEXT(tr.pshadowMaps[x]->texnum, GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			//qglTextureParameterfEXT(tr.pshadowMaps[x]->texnum, GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}

		if (r_sunlightMode->integer)
		{
			for ( x = 0; x < 4; x++)
			{
				tr.sunShadowDepthImage[x] = R_CreateImage(va("*sunshadowdepth%i", x), NULL, r_shadowMapSize->integer, r_shadowMapSize->integer, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_DEPTH_COMPONENT24);
				qglTextureParameterfEXT(tr.sunShadowDepthImage[x]->texnum, GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
				qglTextureParameterfEXT(tr.sunShadowDepthImage[x]->texnum, GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			}

			tr.screenShadowImage = R_CreateImage("*screenShadow", NULL, width, height, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, GL_RGBA8);
		}

		if (r_cubeMapping->integer)
		{
			tr.renderCubeImage = R_CreateImage("*renderCube", NULL, r_cubemapSize->integer, r_cubemapSize->integer, IMGTYPE_COLORALPHA, IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE | IMGFLAG_MIPMAP | IMGFLAG_CUBEMAP, rgbFormat);
		}
	}
}


/*
===============
R_SetColorMappings
===============
*/
void R_SetColorMappings( void ) {
	int		i, j;
	float	g;
	int		inf;

	// setup the overbright lighting
	tr.overbrightBits = r_overBrightBits->integer;

	// allow 2 overbright bits
	if ( tr.overbrightBits > 2 ) {
		tr.overbrightBits = 2;
	} else if ( tr.overbrightBits < 0 ) {
		tr.overbrightBits = 0;
	}

	// don't allow more overbright bits than map overbright bits
	if ( tr.overbrightBits > r_mapOverBrightBits->integer ) {
		tr.overbrightBits = r_mapOverBrightBits->integer;
	}

	tr.identityLight = 1.0f / ( 1 << tr.overbrightBits );
	tr.identityLightByte = 255 * tr.identityLight;


	if ( r_intensity->value <= 1 ) {
		ri.Cvar_Set( "r_intensity", "1" );
	}

	if ( r_gamma->value < 0.5f ) {
		ri.Cvar_Set( "r_gamma", "0.5" );
	} else if ( r_gamma->value > 3.0f ) {
		ri.Cvar_Set( "r_gamma", "3.0" );
	}

	g = r_gamma->value;

	for ( i = 0; i < 256; i++ ) {
		if ( g == 1 ) {
			inf = i;
		} else {
			inf = 255 * pow ( i/255.0f, 1.0f / g ) + 0.5f;
		}

		if (inf < 0) {
			inf = 0;
		}
		if (inf > 255) {
			inf = 255;
		}
		s_gammatable[i] = inf;
	}

	for (i=0 ; i<256 ; i++) {
		j = i * r_intensity->value;
		if (j > 255) {
			j = 255;
		}
		s_intensitytable[i] = j;
	}

	if ( glConfig.deviceSupportsGamma )
	{
		GLimp_SetGamma( s_gammatable, s_gammatable, s_gammatable );
	}
}

/*
===============
R_InitImages
===============
*/
void	R_InitImages( void ) {
	Com_Memset(hashTable, 0, sizeof(hashTable));
	// build brightness translation tables
	R_SetColorMappings();

	// create default texture and white texture
	R_CreateBuiltinImages();
}

/*
===============
R_DeleteTextures
===============
*/
void R_DeleteTextures( void ) {
	int		i;

	for ( i=0; i<tr.numImages ; i++ ) {
		qglDeleteTextures( 1, &tr.images[i]->texnum );
	}
	Com_Memset( tr.images, 0, sizeof( tr.images ) );

	tr.numImages = 0;

	GL_BindNullTextures();
}

/*
============================================================================

SKINS

============================================================================
*/

/*
===============
RE_AllocSkinSurface
===============
*/
qhandle_t	RE_AllocSkinSurface( const char *name, qhandle_t hShader ) {
	qhandle_t		hSurf;
	skinSurface_t	*surf;
	shader_t		*shader;
	char			*reuseName;
	int				nameLen;
	char			localName[MAX_QPATH];

	if ( !name || !name[0] ) {
		ri.Printf( PRINT_DEVELOPER, "Empty name passed to RE_AllocSkinSurface\n" );
		return 0;
	}

	if ( strlen( name ) >= MAX_QPATH ) {
		ri.Printf( PRINT_DEVELOPER, "Skin surface name exceeds MAX_QPATH\n" );
		return 0;
	}

	Q_strncpyz( localName, name, sizeof(localName) );

	// lowercase the surface name so skin compares are faster
	Q_strlwr( localName );

	reuseName = NULL;
	shader = R_GetShaderByHandle( hShader );

	// see if the surface is already loaded
	for ( hSurf = 1; hSurf < tr.numSkinSurfaces ; hSurf++ ) {
		surf = &tr.skinSurfaces[hSurf];
		if ( !strcmp( surf->name, localName ) ) {
			if ( surf->shader == shader ) {
				return hSurf;
			}
			reuseName = surf->name;
		}
	}

	if ( tr.numSkinSurfaces >= MAX_SKINSURFACES ) {
		ri.Printf( PRINT_WARNING, "WARNING: RE_AllocSkinSurface( '%s' ) MAX_SKINSURFACES hit\n", name );
		return 0;
	}

	// add a new skin surface
	tr.numSkinSurfaces++;

	surf = &tr.skinSurfaces[hSurf];
	surf->shader = shader;

	if ( reuseName ) {
		surf->name = reuseName;
	} else {
		nameLen = strlen( localName ) + 1;

		surf->name = ri.Hunk_Alloc( nameLen, h_low );
		tr.skinSurfaceNameMemory += nameLen;

		Q_strncpyz( surf->name, localName, nameLen );
	}

	return hSurf;
}


/*
===============
R_InitSkins
===============
*/
void	R_InitSkins( void ) {
	tr.numSkinSurfaces = 1;
}

/*
===============
R_SkinList_f
===============
*/
void	R_SkinList_f( void ) {
	int			i;
	skinSurface_t *surf;

	ri.Printf (PRINT_ALL, "Skin surface name memory: %d bytes\n", tr.skinSurfaceNameMemory);
	ri.Printf (PRINT_ALL, "------------------\n");

	for ( i = 1 ; i < tr.numSkinSurfaces ; i++ ) {
		surf = &tr.skinSurfaces[i];

		ri.Printf( PRINT_ALL, "%3i: %s = %s\n",
				i, surf->name, surf->shader->name );
	}
	ri.Printf (PRINT_ALL, "------------------\n");
}


