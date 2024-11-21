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

#include "tr_common.h"

/*
========================================================================

TGA files are used for 24/32 bit images

========================================================================
*/

typedef struct _TargaHeader {
	unsigned char 	id_length, colormap_type, image_type;
	unsigned short	colormap_index, colormap_length;
	unsigned char	colormap_size;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	pixel_size, attributes;
} TargaHeader;

void R_LoadTGA ( const char *name, int *numTexLevels, textureLevel_t **pic)
{
	unsigned	columns, rows, numPixels;
	byte	*pixbuf;
	int		row, column;
	byte	*buf_p;
	byte	*end;
	union {
		byte *b;
		void *v;
	} buffer;
	TargaHeader	targa_header;
	byte		*targa_rgba;
	int length;

	*pic = NULL;
	*numTexLevels = 0;

	//
	// load the file
	//
	length = ri.FS_ReadFile ( ( char * ) name, &buffer.v);
	if (!buffer.b || length < 0) {
		return;
	}

	if(length < 18)
	{
		ri.Error( ERR_DROP, "LoadTGA: header too short (%s)", name );
	}

	buf_p = buffer.b;
	end = buffer.b + length;

	targa_header.id_length = buf_p[0];
	targa_header.colormap_type = buf_p[1];
	targa_header.image_type = buf_p[2];
	
	memcpy(&targa_header.colormap_index, &buf_p[3], 2);
	memcpy(&targa_header.colormap_length, &buf_p[5], 2);
	targa_header.colormap_size = buf_p[7];
	memcpy(&targa_header.x_origin, &buf_p[8], 2);
	memcpy(&targa_header.y_origin, &buf_p[10], 2);
	memcpy(&targa_header.width, &buf_p[12], 2);
	memcpy(&targa_header.height, &buf_p[14], 2);
	targa_header.pixel_size = buf_p[16];
	targa_header.attributes = buf_p[17];

	targa_header.colormap_index = LittleShort(targa_header.colormap_index);
	targa_header.colormap_length = LittleShort(targa_header.colormap_length);
	targa_header.x_origin = LittleShort(targa_header.x_origin);
	targa_header.y_origin = LittleShort(targa_header.y_origin);
	targa_header.width = LittleShort(targa_header.width);
	targa_header.height = LittleShort(targa_header.height);

	buf_p += 18;

	if (targa_header.image_type!=2 
		&& targa_header.image_type!=10
		&& targa_header.image_type != 3 ) 
	{
		ri.Error (ERR_DROP, "LoadTGA: Only type 2 (RGB), 3 (gray), and 10 (RGB) TGA images supported");
	}

	if ( targa_header.colormap_type != 0 )
	{
		ri.Error( ERR_DROP, "LoadTGA: colormaps not supported" );
	}

	if ( ( targa_header.pixel_size != 32 && targa_header.pixel_size != 24 ) && targa_header.image_type != 3 )
	{
		ri.Error (ERR_DROP, "LoadTGA: Only 32 or 24 bit images supported (no colormaps)");
	}

	columns = targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows * 4;

	if(!columns || !rows || numPixels > 0x7FFFFFFF || numPixels / columns / 4 != rows)
	{
		ri.Error (ERR_DROP, "LoadTGA: %s has an invalid image size", name);
	}


	*pic = (textureLevel_t *)ri.Malloc( sizeof(textureLevel_t) + numPixels );
	(*pic)->format = GL_RGBA8;
	(*pic)->width = columns;
	(*pic)->height = rows;
	(*pic)->size = numPixels;
	(*pic)->data = targa_rgba = (byte *)(*pic + 1);
	*numTexLevels = 1;

	if (targa_header.id_length != 0)
	{
		if (buf_p + targa_header.id_length > end)
			ri.Error( ERR_DROP, "LoadTGA: header too short (%s)", name );

		buf_p += targa_header.id_length;  // skip TARGA image comment
	}
	
	if ( targa_header.image_type==2 || targa_header.image_type == 3 )
	{ 
		if(buf_p + columns*rows*targa_header.pixel_size/8 > end)
		{
			ri.Error (ERR_DROP, "LoadTGA: file truncated (%s)", name);
		}

		// Uncompressed RGB or gray scale image
		for(row=rows-1; row>=0; row--) 
		{
			pixbuf = targa_rgba + row*columns*4;
			for(column=0; column<columns; column++) 
			{
				unsigned char red,green,blue,alphabyte;
				switch (targa_header.pixel_size) 
				{
					
				case 8:
					blue = *buf_p++;
					green = blue;
					red = blue;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = 255;
					break;

				case 24:
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = 255;
					break;
				case 32:
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					alphabyte = *buf_p++;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = alphabyte;
					break;
				default:
					ri.Error( ERR_DROP, "LoadTGA: illegal pixel_size '%d' in file '%s'", targa_header.pixel_size, name );
					break;
				}
			}
		}
	}
	else if (targa_header.image_type==10) {   // Runlength encoded RGB images
		unsigned char red,green,blue,alphabyte,packetHeader,packetSize,j;

		for(row=rows-1; row>=0; row--) {
			pixbuf = targa_rgba + row*columns*4;
			for(column=0; column<columns; ) {
				if(buf_p + 1 > end)
					ri.Error (ERR_DROP, "LoadTGA: file truncated (%s)", name);
				packetHeader= *buf_p++;
				packetSize = 1 + (packetHeader & 0x7f);
				if (packetHeader & 0x80) {        // run-length packet
					if(buf_p + targa_header.pixel_size/8 > end)
						ri.Error (ERR_DROP, "LoadTGA: file truncated (%s)", name);
					switch (targa_header.pixel_size) {
						case 24:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								alphabyte = 255;
								break;
						case 32:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								alphabyte = *buf_p++;
								break;
						default:
							ri.Error( ERR_DROP, "LoadTGA: illegal pixel_size '%d' in file '%s'", targa_header.pixel_size, name );
							break;
					}
	
					for(j=0;j<packetSize;j++) {
						*pixbuf++=red;
						*pixbuf++=green;
						*pixbuf++=blue;
						*pixbuf++=alphabyte;
						column++;
						if (column==columns) { // run spans across rows
							column=0;
							if (row>0)
								row--;
							else
								goto breakOut;
							pixbuf = targa_rgba + row*columns*4;
						}
					}
				}
				else {                            // non run-length packet

					if(buf_p + targa_header.pixel_size/8*packetSize > end)
						ri.Error (ERR_DROP, "LoadTGA: file truncated (%s)", name);
					for(j=0;j<packetSize;j++) {
						switch (targa_header.pixel_size) {
							case 24:
									blue = *buf_p++;
									green = *buf_p++;
									red = *buf_p++;
									*pixbuf++ = red;
									*pixbuf++ = green;
									*pixbuf++ = blue;
									*pixbuf++ = 255;
									break;
							case 32:
									blue = *buf_p++;
									green = *buf_p++;
									red = *buf_p++;
									alphabyte = *buf_p++;
									*pixbuf++ = red;
									*pixbuf++ = green;
									*pixbuf++ = blue;
									*pixbuf++ = alphabyte;
									break;
							default:
								ri.Error( ERR_DROP, "LoadTGA: illegal pixel_size '%d' in file '%s'", targa_header.pixel_size, name );
								break;
						}
						column++;
						if (column==columns) { // pixel packet run spans across rows
							column=0;
							if (row>0)
								row--;
							else
								goto breakOut;
							pixbuf = targa_rgba + row*columns*4;
						}						
					}
				}
			}
			breakOut:;
		}
	}

#if 0 
  // TTimo: this is the chunk of code to ensure a behavior that meets TGA specs 
  // bit 5 set => top-down
  if (targa_header.attributes & 0x20) {
    unsigned char *flip = (unsigned char*)malloc (columns*4);
    unsigned char *src, *dst;

    for (row = 0; row < rows/2; row++) {
      src = targa_rgba + row * 4 * columns;
      dst = targa_rgba + (rows - row - 1) * 4 * columns;

      memcpy (flip, src, columns*4);
      memcpy (src, dst, columns*4);
      memcpy (dst, flip, columns*4);
    }
    free (flip);
  }
#endif
  // instead we just print a warning
  if (targa_header.attributes & 0x20) {
    ri.Printf( PRINT_WARNING, "WARNING: '%s' TGA file header declares top-down image, ignoring\n", name);
  }

  ri.FS_FreeFile (buffer.v);
}

void RE_SaveTGA(char * filename, int image_width, int image_height, byte *image_buffer, int padding) {
	byte *srcptr, *destptr;
	byte *endline, *endmem;
	byte *out;
	size_t bufSize;
	int linelen;

	bufSize = image_width * image_height * 3 + 18;
	out = ri.Hunk_AllocateTempMemory(bufSize);

	Com_Memset (out, 0, 18);
	out[2] = 2;		// uncompressed type
	out[12] = image_width & 255;
	out[13] = image_width >> 8;
	out[14] = image_height & 255;
	out[15] = image_height >> 8;
	out[16] = 24;	// pixel size

	// swap rgb to bgr and remove padding from line endings
	linelen = image_width * 3;

	srcptr = image_buffer;
	destptr = out + 18;
	endmem = srcptr + (linelen + padding) * image_height;

	while(srcptr < endmem)
	{
		endline = srcptr + linelen;

		while(srcptr < endline)
		{
			*destptr++ = srcptr[2];
			*destptr++ = srcptr[1];
			*destptr++ = srcptr[0];
			
			srcptr += 3;
		}
		
		// Skip the pad
		srcptr += padding;
	}

	ri.FS_WriteFile(filename, out, bufSize);

	ri.Hunk_FreeTempMemory(out);
}

void RE_SaveTGA_EXT(char * filename, int image_width, int image_height, int bytesPerPixel, byte *image_buffer, int padding) {
	byte *srcptr, *destptr;
	byte *endline, *endmem;
	byte *out;
	size_t bufSize;
	int linelen;

	if ( bytesPerPixel != 3 && bytesPerPixel != 4 ) {
		Com_Error( ERR_DROP, "RE_SaveTGA: Unsupported bytes per pixel (%d)\n", bytesPerPixel );
	}

	bufSize = image_width * image_height * bytesPerPixel + 18;
	out = ri.Hunk_AllocateTempMemory(bufSize);

	Com_Memset (out, 0, 18);
	out[2] = 2;		// uncompressed type
	out[12] = image_width & 255;
	out[13] = image_width >> 8;
	out[14] = image_height & 255;
	out[15] = image_height >> 8;
	out[16] = bytesPerPixel*8;	// pixel size

	// swap rgb to bgr and remove padding from line endings
	linelen = image_width * bytesPerPixel;

	srcptr = image_buffer;
	destptr = out + 18;
	endmem = srcptr + (linelen + padding) * image_height;

	while(srcptr < endmem)
	{
		endline = srcptr + linelen;

		while(srcptr < endline)
		{
			*destptr++ = srcptr[2];
			*destptr++ = srcptr[1];
			*destptr++ = srcptr[0];

			if ( bytesPerPixel == 4 )
				*destptr++ = srcptr[3];

			srcptr += bytesPerPixel;
		}
		
		// Skip the pad
		srcptr += padding;
	}

	ri.FS_WriteFile(filename, out, bufSize);

	ri.Hunk_FreeTempMemory(out);
}

