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
// tr_map.c

#include "tr_local.h"

/*

Loads and prepares a map file for scene rendering.

A single entry point:

void RE_LoadWorldMap( const bspFile_t *bsp );

*/

#if 1 // ZTM: FIXME: BSP is already swapped by BSP_Load, but removing these probably makes merging ioq3 changes harder...
#undef LittleShort
#define LittleShort
#undef LittleLong
#define LittleLong
#undef LittleFloat
#define LittleFloat
#endif

static	world_t		s_worldData;

int			c_subdivisions;
int			c_gridVerts;

//===============================================================================

static void HSVtoRGB( float h, float s, float v, float rgb[3] )
{
	int i;
	float f;
	float p, q, t;

	h *= 5;

	i = floor( h );
	f = h - i;

	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch ( i )
	{
	case 0:
		rgb[0] = v;
		rgb[1] = t;
		rgb[2] = p;
		break;
	case 1:
		rgb[0] = q;
		rgb[1] = v;
		rgb[2] = p;
		break;
	case 2:
		rgb[0] = p;
		rgb[1] = v;
		rgb[2] = t;
		break;
	case 3:
		rgb[0] = p;
		rgb[1] = q;
		rgb[2] = v;
		break;
	case 4:
		rgb[0] = t;
		rgb[1] = p;
		rgb[2] = v;
		break;
	case 5:
		rgb[0] = v;
		rgb[1] = p;
		rgb[2] = q;
		break;
	}
}

/*
===============
R_ColorShiftLightingBytes

===============
*/
static	void R_ColorShiftLightingBytes( int inSize, byte in[4], byte out[4] ) {
	int		shift, r, g, b;

	// shift the color data based on overbright range
	shift = r_mapOverBrightBits->integer - tr.overbrightBits;

	// shift the data based on overbright range
	r = in[0] << shift;
	g = in[1] << shift;
	b = in[2] << shift;
	
	// normalize by color instead of saturating to white
	if ( ( r | g | b ) > 255 ) {
		int		max;

		max = r > g ? r : g;
		max = max > b ? max : b;
		r = r * 255 / max;
		g = g * 255 / max;
		b = b * 255 / max;
	}

	out[0] = r;
	out[1] = g;
	out[2] = b;
	if ( inSize == 4 ) {
		out[3] = in[3];
	} else {
		out[3] = 255;
	}
}

/*
===============
R_ProcessLightmap

	returns maxIntensity
===============
*/
float R_ProcessLightmap( byte **pic, int in_padding, int width, int height, byte **pic_out ) {
	int j;
	float maxIntensity = 0;
	double sumIntensity = 0;

	if ( r_lightmap->integer > 1 ) { // color code by intensity as development tool	(FIXME: check range)
		for ( j = 0; j < width * height; j++ )
		{
			float r = ( *pic )[j * in_padding + 0];
			float g = ( *pic )[j * in_padding + 1];
			float b = ( *pic )[j * in_padding + 2];
			float intensity;
			float out[3] = {0.0, 0.0, 0.0};

			intensity = 0.33f * r + 0.685f * g + 0.063f * b;

			if ( intensity > 255 ) {
				intensity = 1.0f;
			} else {
				intensity /= 255.0f;
			}

			if ( intensity > maxIntensity ) {
				maxIntensity = intensity;
			}

			HSVtoRGB( intensity, 1.00, 0.50, out );

			if ( r_lightmap->integer == 3 ) {
				// Arnout: artists wanted the colours to be inversed
				( *pic_out )[j * 4 + 0] = out[2] * 255;
				( *pic_out )[j * 4 + 1] = out[1] * 255;
				( *pic_out )[j * 4 + 2] = out[0] * 255;
			} else {
				( *pic_out )[j * 4 + 0] = out[0] * 255;
				( *pic_out )[j * 4 + 1] = out[1] * 255;
				( *pic_out )[j * 4 + 2] = out[2] * 255;
			}
			( *pic_out )[j * 4 + 3] = 255;

			sumIntensity += intensity;
		}
	} else {
		for ( j = 0 ; j < width * height; j++ ) {
			R_ColorShiftLightingBytes( 3, &( *pic )[j * in_padding], &( *pic_out )[j * 4] );
		}
	}

	return maxIntensity;
}

/*
===============
R_LoadLightmaps

===============
*/
#define	LIGHTMAP_SIZE	128
static	void R_LoadLightmaps( const bspFile_t *bsp ) {
	byte		*buf, *buf_p, *image_p;
	byte		image[LIGHTMAP_SIZE*LIGHTMAP_SIZE*4];
	int			i;
	float		intensity, maxIntensity = 0;
	int			numExternalLightmaps = 0;

	// ydnar: clear lightmaps first
	tr.numLightmaps = tr.maxLightmaps = 0;
	tr.lightmaps = NULL;

	// if we are in r_vertexLight mode, we don't need the lightmaps at all
	if ( r_vertexLight->integer ) {
		return;
	}

	// get number of external lightmaps
	if (tr.worldDir) {
		ri.FS_ListFiles(tr.worldDir, ".tga", &numExternalLightmaps);
	}

	if ( !bsp->numLightmaps ) {
		// Allocate data for external lightmaps.
		tr.maxLightmaps = numExternalLightmaps;
		tr.lightmaps = ri.Hunk_Alloc( tr.maxLightmaps * sizeof(image_t *), h_low );
		return;
	}
	buf = bsp->lightmapData;

	// we are about to upload textures
	R_IssuePendingRenderCommands();

	// create all the lightmaps
	tr.numLightmaps = bsp->numLightmaps;
	if ( tr.numLightmaps == 1 ) {
		//FIXME: HACK: maps with only one lightmap turn up fullbright for some reason.
		//this avoids this, but isn't the correct solution.
		tr.numLightmaps++;
	}

	tr.maxLightmaps = tr.numLightmaps + numExternalLightmaps;
	tr.lightmaps = ri.Hunk_Alloc( tr.maxLightmaps * sizeof(image_t *), h_low );
	for ( i = 0 ; i < tr.numLightmaps ; i++ ) {
		// expand the 24 bit on-disk to 32 bit
		buf_p = buf + i * LIGHTMAP_SIZE*LIGHTMAP_SIZE * 3;
		image_p = image;

		intensity = R_ProcessLightmap( &buf_p, 3, LIGHTMAP_SIZE, LIGHTMAP_SIZE, &image_p );
		if ( intensity > maxIntensity ) {
			maxIntensity = intensity;
		}

		tr.lightmaps[i] = R_CreateImage( va("*lightmap%d",i), image, 
			LIGHTMAP_SIZE, LIGHTMAP_SIZE, IMGTYPE_COLORALPHA,
			IMGFLAG_NOLIGHTSCALE | IMGFLAG_NO_COMPRESSION | IMGFLAG_CLAMPTOEDGE, 0 );
	}

	if ( r_lightmap->integer > 1 )	{
		ri.Printf( PRINT_ALL, "Brightest lightmap value: %d\n", ( int ) ( maxIntensity * 255 ) );
	}
}


/*
=================
RE_SetWorldVisData

This is called by the clipmodel subsystem so we can share the 1.8 megs of
space in big maps...
=================
*/
void		RE_SetWorldVisData( const byte *vis ) {
	tr.externalVisData = vis;
}


/*
=================
R_LoadVisibility
=================
*/
static	void R_LoadVisibility( const bspFile_t *bsp ) {
	int		len;

	len = ( s_worldData.numClusters + 63 ) & ~63;
	s_worldData.novis = ri.Hunk_Alloc( len, h_low );
	Com_Memset( s_worldData.novis, 0xff, len );

	if ( !bsp->visibilityLength ) {
		return;
	}

	s_worldData.numClusters = bsp->numClusters;
	s_worldData.clusterBytes = bsp->clusterBytes;

	// CM_Load should have given us the vis data to share, so
	// we don't need to allocate another copy
	if ( tr.externalVisData ) {
		s_worldData.vis = tr.externalVisData;
	} else {
		s_worldData.vis = bsp->visibility;
	}
}

//===============================================================================


/*
===============
ShaderForShaderNum
===============
*/
static shader_t *ShaderForShaderNum( int shaderNum, int lightmapNum ) {
	shader_t	*shader;
	dshader_t	*dsh;

	int _shaderNum = LittleLong( shaderNum );
	if ( _shaderNum < 0 || _shaderNum >= s_worldData.numShaders ) {
		ri.Error( ERR_DROP, "ShaderForShaderNum: bad num %i", _shaderNum );
	}
	dsh = &s_worldData.shaders[ _shaderNum ];

	if ( r_vertexLight->integer ) {
		lightmapNum = LIGHTMAP_BY_VERTEX;
	}

	if ( r_fullbright->integer ) {
		lightmapNum = LIGHTMAP_WHITEIMAGE;
	}

	shader = R_FindShader( dsh->shader, lightmapNum, MIP_RAW_IMAGE );

	// if the shader had errors, just use default shader
	if ( shader->defaultShader ) {
		return tr.defaultShader;
	}

	return shader;
}

/*
===============
ConvertBSPFogNum

convert -1 to 1 (global fog that is enabled/disabled by cgame)
shift all other numbers up
===============
*/
static int ConvertBSPFogNum( int fogNum ) {
	return LittleLong( fogNum ) + 2;
}

// Ridah, optimizations here
// memory block for use by surfaces
static byte *surfHunkPtr;
static int surfHunkSize;
#define SURF_HUNK_MAXSIZE 0x40000

/*
==============
R_InitSurfMemory
==============
*/
void R_InitSurfMemory( void ) {
	// allocate a new chunk
	surfHunkPtr = ri.Hunk_Alloc( SURF_HUNK_MAXSIZE, h_low );
	surfHunkSize = 0;
}

/*
==============
R_GetSurfMemory
==============
*/
void *R_GetSurfMemory( int size ) {
	byte *retval;

	// round to cacheline
	size = ( size + 31 ) & ~31;

	surfHunkSize += size;
	if ( surfHunkSize >= SURF_HUNK_MAXSIZE ) {
		// allocate a new chunk
		R_InitSurfMemory();
		surfHunkSize += size;   // since it just got reset
	}
	retval = surfHunkPtr;
	surfHunkPtr += size;

	return (void *)retval;
}



/*
===============
SphereFromBounds
creates a bounding sphere from a bounding box
===============
*/

static void SphereFromBounds( vec3_t mins, vec3_t maxs, vec3_t origin, float *radius ) {
	vec3_t temp;

	VectorAdd( mins, maxs, origin );
	VectorScale( origin, 0.5, origin );
	VectorSubtract( maxs, origin, temp );
	*radius = VectorLength( temp );
}



/*
===============
FinishGenericSurface
handles final surface classification
===============
*/

static void FinishGenericSurface( dsurface_t *ds, srfGeneric_t *gen, vec3_t pt ) {
	// set bounding sphere
	SphereFromBounds( gen->bounds[ 0 ], gen->bounds[ 1 ], gen->origin, &gen->radius );

	// take the plane normal from the lightmap vector and classify it
	gen->plane.normal[ 0 ] = LittleFloat( ds->lightmapVecs[ 2 ][ 0 ] );
	gen->plane.normal[ 1 ] = LittleFloat( ds->lightmapVecs[ 2 ][ 1 ] );
	gen->plane.normal[ 2 ] = LittleFloat( ds->lightmapVecs[ 2 ][ 2 ] );
	gen->plane.dist = DotProduct( pt, gen->plane.normal );
	SetPlaneSignbits( &gen->plane );
	gen->plane.type = PlaneTypeForNormal( gen->plane.normal );
}



/*
===============
ParseMesh
===============
*/
static void ParseMesh( dsurface_t *ds, drawVert_t *verts, msurface_t *surf ) {
	srfGridMesh_t   *grid;
	int i, j;
	int width, height, numPoints;
	drawVert_t points[MAX_PATCH_SIZE * MAX_PATCH_SIZE];
	int lightmapNum;
	vec3_t bounds[2];
	vec3_t tmpVec;
	static surfaceType_t	skipData = SF_SKIP;

	lightmapNum = LittleLong( ds->lightmapNum );

	// get fog volume
	surf->fogIndex = ConvertBSPFogNum( ds->fogNum );

	// get shader value
	surf->shader = ShaderForShaderNum( ds->shaderNum, lightmapNum );
	if ( r_singleShader->integer && !surf->shader->isSky ) {
		surf->shader = tr.defaultShader;
	}
	surf->originalShader = surf->shader;

	// we may have a nodraw surface, because they might still need to
	// be around for movement clipping
	if ( s_worldData.shaders[ LittleLong( ds->shaderNum ) ].surfaceFlags & r_surfaceFlagNoDraw->integer ) {
		surf->data = &skipData;
		return;
	}

	width = LittleLong( ds->patchWidth );
	height = LittleLong( ds->patchHeight );

	if(width < 0 || width > MAX_PATCH_SIZE || height < 0 || height > MAX_PATCH_SIZE)
		ri.Error(ERR_DROP, "ParseMesh: bad size");

	verts += LittleLong( ds->firstVert );
	numPoints = width * height;
	for ( i = 0 ; i < numPoints ; i++ ) {
		for ( j = 0 ; j < 3 ; j++ ) {
			points[i].xyz[j] = LittleFloat( verts[i].xyz[j] );
			points[i].normal[j] = LittleFloat( verts[i].normal[j] );
		}
		for ( j = 0 ; j < 2 ; j++ ) {
			points[i].st[j] = LittleFloat( verts[i].st[j] );
			points[i].lightmap[j] = LittleFloat( verts[i].lightmap[j] );
		}
		R_ColorShiftLightingBytes( 4, verts[i].color, points[i].color );
	}

	// pre-tesseleate
	grid = R_SubdividePatchToGrid( width, height, points );
	surf->data = (surfaceType_t*) grid;

	// copy the level of detail origin, which is the center
	// of the group of all curves that must subdivide the same
	// to avoid cracking
	for ( i = 0 ; i < 3 ; i++ ) {
		bounds[0][i] = LittleFloat( ds->lightmapVecs[0][i] );
		bounds[1][i] = LittleFloat( ds->lightmapVecs[1][i] );
	}
	VectorAdd( bounds[0], bounds[1], bounds[1] );
	VectorScale( bounds[1], 0.5f, grid->lodOrigin );
	VectorSubtract( bounds[0], grid->lodOrigin, tmpVec );
	grid->lodRadius = VectorLength( tmpVec );

	// finish surface
	FinishGenericSurface( ds, (srfGeneric_t*) grid, grid->verts[ 0 ].xyz );
}


/*
===============
ParseTriSurf
===============
*/
static void ParseTriSurf( dsurface_t *ds, drawVert_t *verts, msurface_t *surf, int *indexes, qboolean planar ) {
	srfTriangles_t	*tri;
	int				i, j;
	int				numVerts, numIndexes;
	int lightmapNum;

	// get lightmap num
	lightmapNum = LittleLong( ds->lightmapNum );

	// q3map misc_model has LIGHTMAP_NONE. q3map2 misc_model has LIGHTMAP_BY_VERTEX.
	// Vanilla Q3 always used LIGHTMAP_BY_VERTEX, so correct old q3map misc_model value.
	// (LIGHTMAP_NONE isn't valid on world surfaces anyway.)
	if ( lightmapNum == LIGHTMAP_NONE ) {
		lightmapNum = LIGHTMAP_BY_VERTEX;
	}

	// get fog volume
	surf->fogIndex = ConvertBSPFogNum( ds->fogNum );

	// get shader
	surf->shader = ShaderForShaderNum( ds->shaderNum, lightmapNum );
	if ( r_singleShader->integer && !surf->shader->isSky ) {
		surf->shader = tr.defaultShader;
	}
	surf->originalShader = surf->shader;

	numVerts = LittleLong( ds->numVerts );
	numIndexes = LittleLong( ds->numIndexes );

	if ( numVerts >= SHADER_MAX_VERTEXES ) {
		ri.Error(ERR_DROP, "ParseTriSurf: verts > MAX (%d > %d)", numVerts, SHADER_MAX_VERTEXES );
	}
	if ( numIndexes >= SHADER_MAX_INDEXES ) {
		ri.Error(ERR_DROP, "ParseTriSurf: indices > MAX (%d > %d)", numIndexes, SHADER_MAX_INDEXES );
	}

	//tri = ri.Hunk_Alloc( sizeof( *tri ) + numVerts * sizeof( tri->verts[0] ) 
	//	+ numIndexes * sizeof( tri->indexes[0] ), h_low );
	tri = R_GetSurfMemory( sizeof( *tri ) + numVerts * sizeof( tri->verts[0] )
						   + numIndexes * sizeof( tri->indexes[0] ) );
	tri->surfaceType = SF_TRIANGLES;
	tri->planar = planar;
	tri->numVerts = numVerts;
	tri->numIndexes = numIndexes;
	tri->verts = (drawVert_t *)(tri + 1);
	tri->indexes = (int *)(tri->verts + tri->numVerts );

	surf->data = (surfaceType_t *)tri;

	// copy vertexes
	ClearBounds( tri->bounds[0], tri->bounds[1] );
	verts += LittleLong( ds->firstVert );
	for ( i = 0 ; i < numVerts ; i++ ) {
		for ( j = 0 ; j < 3 ; j++ ) {
			tri->verts[i].xyz[j] = LittleFloat( verts[i].xyz[j] );
			tri->verts[i].normal[j] = LittleFloat( verts[i].normal[j] );
		}
		AddPointToBounds( tri->verts[i].xyz, tri->bounds[0], tri->bounds[1] );
		for ( j = 0 ; j < 2 ; j++ ) {
			tri->verts[i].st[j] = LittleFloat( verts[i].st[j] );
			tri->verts[i].lightmap[j] = LittleFloat( verts[i].lightmap[j] );
		}

		R_ColorShiftLightingBytes( 4, verts[i].color, tri->verts[i].color );
	}

	// copy indexes
	indexes += LittleLong( ds->firstIndex );
	for ( i = 0 ; i < numIndexes ; i++ ) {
		tri->indexes[i] = LittleLong( indexes[i] );
		if ( tri->indexes[i] < 0 || tri->indexes[i] >= numVerts ) {
			ri.Error( ERR_DROP, "Bad index in triangle surface" );
		}
	}

	// finish surface
	FinishGenericSurface( ds, (srfGeneric_t*) tri, tri->verts[ 0 ].xyz );
}



/*
===============
ParseFoliage
parses a foliage drawsurface
===============
*/
static void ParseFoliage( dsurface_t *ds, drawVert_t *verts, msurface_t *surf, int *indexes ) {
	srfFoliage_t    *foliage;
	int i, j, numVerts, numIndexes, numInstances, size;
	//vec4_t          *origin;
//	fcolor4ub_t		*color;
	vec3_t bounds[ 2 ], boundsTranslated[ 2 ];
	float scale;


	// get fog volume
	surf->fogIndex = ConvertBSPFogNum( ds->fogNum );

	// get shader
	surf->shader = ShaderForShaderNum( ds->shaderNum, LIGHTMAP_BY_VERTEX );
	if ( r_singleShader->integer && !surf->shader->isSky ) {
		surf->shader = tr.defaultShader;
	}
	surf->originalShader = surf->shader;

	// foliage surfaces have their actual vert count in patchHeight
	// and the instance count in patchWidth
	// the instances are just additional drawverts

	// get counts
	numVerts = LittleLong( ds->patchHeight );
	numIndexes = LittleLong( ds->numIndexes );
	numInstances = LittleLong( ds->patchWidth );

	if ( numVerts >= SHADER_MAX_VERTEXES ) {
		ri.Error(ERR_DROP, "ParseFoliage: verts > MAX (%d > %d)", numVerts, SHADER_MAX_VERTEXES );
	}
	if ( numIndexes >= SHADER_MAX_INDEXES ) {
		ri.Error(ERR_DROP, "ParseFoliage: indices > MAX (%d > %d)", numIndexes, SHADER_MAX_INDEXES );
	}

	// calculate size
	size = sizeof( *foliage ) +
		   numVerts * ( sizeof( foliage->xyz[ 0 ] ) + sizeof( foliage->normal[ 0 ] ) + sizeof( foliage->texCoords[ 0 ] ) + sizeof( foliage->lmTexCoords[ 0 ] ) ) +
		   numIndexes * sizeof( foliage->indexes[ 0 ] ) +
		   numInstances * sizeof( foliage->instances[ 0 ] );

	// get memory
	foliage = R_GetSurfMemory( size );

	// set up surface
	foliage->surfaceType = SF_FOLIAGE;
	foliage->numVerts = numVerts;
	foliage->numIndexes = numIndexes;
	foliage->numInstances = numInstances;
	foliage->xyz = ( vec4_t* )( foliage + 1 );
	foliage->normal = ( vec4_t* )( foliage->xyz + foliage->numVerts );
	foliage->texCoords = ( vec2_t* )( foliage->normal + foliage->numVerts );
	foliage->lmTexCoords = ( vec2_t* )( foliage->texCoords + foliage->numVerts );
	foliage->indexes = ( int* )( foliage->lmTexCoords + foliage->numVerts );
	foliage->instances = ( foliageInstance_t* )( foliage->indexes + foliage->numIndexes );

	surf->data = (surfaceType_t*) foliage;

	// get foliage drawscale
	scale = r_drawfoliage->value;
	if ( scale < 0.0f ) {
		scale = 1.0f;
	} else if ( scale > 2.0f ) {
		scale = 2.0f;
	}

	// copy vertexes
	ClearBounds( bounds[ 0 ], bounds[ 1 ] );
	verts += LittleLong( ds->firstVert );
	for ( i = 0; i < numVerts; i++ )
	{
		// copy xyz and normal
		for ( j = 0; j < 3; j++ )
		{
			foliage->xyz[ i ][ j ] = LittleFloat( verts[ i ].xyz[ j ] );
			foliage->normal[ i ][ j ] = LittleFloat( verts[ i ].normal[ j ] );
		}

		// scale height
		foliage->xyz[ i ][ 2 ] *= scale;

		// finish
		foliage->xyz[ i ][ 3 ] = foliage->normal[ i ][ 3 ] = 0;
		AddPointToBounds( foliage->xyz[ i ], bounds[ 0 ], bounds[ 1 ] );

		// copy texture coordinates
		for ( j = 0; j < 2; j++ )
		{
			foliage->texCoords[ i ][ j ] = LittleFloat( verts[ i ].st[ j ] );
			foliage->lmTexCoords[ i ][ j ] = LittleFloat( verts[ i ].lightmap[ j ] );
		}
	}

	// copy indexes
	indexes += LittleLong( ds->firstIndex );
	for ( i = 0; i < numIndexes; i++ )
	{
		foliage->indexes[ i ] = LittleLong( indexes[ i ] );
		if ( foliage->indexes[ i ] < 0 || foliage->indexes[ i ] >= numVerts ) {
			ri.Error( ERR_DROP, "Bad index in triangle surface" );
		}
	}

	// copy origins and colors
	ClearBounds( foliage->bounds[ 0 ], foliage->bounds[ 1 ] );
	verts += numVerts;
	for ( i = 0; i < numInstances; i++ )
	{
		// copy xyz
		for ( j = 0; j < 3; j++ )
			foliage->instances[ i ].origin[ j ] = LittleFloat( verts[ i ].xyz[ j ] );
		VectorAdd( bounds[ 0 ], foliage->instances[ i ].origin, boundsTranslated[ 0 ] );
		VectorAdd( bounds[ 1 ], foliage->instances[ i ].origin, boundsTranslated[ 1 ] );
		AddPointToBounds( boundsTranslated[ 0 ], foliage->bounds[ 0 ], foliage->bounds[ 1 ] );
		AddPointToBounds( boundsTranslated[ 1 ], foliage->bounds[ 0 ], foliage->bounds[ 1 ] );

		// copy color
		R_ColorShiftLightingBytes( 4, verts[ i ].color, foliage->instances[ i ].color );
	}

	// finish surface
	FinishGenericSurface( ds, (srfGeneric_t*) foliage, foliage->xyz[ 0 ] );
}



/*
===============
ParseFlare
===============
*/
static void ParseFlare( dsurface_t *ds, drawVert_t *verts, msurface_t *surf, int *indexes ) {
	srfFlare_t		*flare;
	int				i;

	// get fog volume
	surf->fogIndex = ConvertBSPFogNum( ds->fogNum );

	// get shader
	surf->shader = ShaderForShaderNum( ds->shaderNum, LIGHTMAP_BY_VERTEX );
	if ( r_singleShader->integer && !surf->shader->isSky ) {
		surf->shader = tr.defaultShader;
	}
	surf->originalShader = surf->shader;

	flare = ri.Hunk_Alloc( sizeof( *flare ), h_low );
	flare->surfaceType = SF_FLARE;

	surf->data = (surfaceType_t *)flare;

	for ( i = 0 ; i < 3 ; i++ ) {
		flare->origin[i] = LittleFloat( ds->lightmapOrigin[i] );
		flare->color[i] = LittleFloat( ds->lightmapVecs[0][i] );
		flare->normal[i] = LittleFloat( ds->lightmapVecs[2][i] );
	}
}


/*
=================
R_MergedWidthPoints

returns true if there are grid points merged on a width edge
=================
*/
int R_MergedWidthPoints(srfGridMesh_t *grid, int offset) {
	int i, j;

	for (i = 1; i < grid->width-1; i++) {
		for (j = i + 1; j < grid->width-1; j++) {
			if ( fabs(grid->verts[i + offset].xyz[0] - grid->verts[j + offset].xyz[0]) > .1) continue;
			if ( fabs(grid->verts[i + offset].xyz[1] - grid->verts[j + offset].xyz[1]) > .1) continue;
			if ( fabs(grid->verts[i + offset].xyz[2] - grid->verts[j + offset].xyz[2]) > .1) continue;
			return qtrue;
		}
	}
	return qfalse;
}

/*
=================
R_MergedHeightPoints

returns true if there are grid points merged on a height edge
=================
*/
int R_MergedHeightPoints(srfGridMesh_t *grid, int offset) {
	int i, j;

	for (i = 1; i < grid->height-1; i++) {
		for (j = i + 1; j < grid->height-1; j++) {
			if ( fabs(grid->verts[grid->width * i + offset].xyz[0] - grid->verts[grid->width * j + offset].xyz[0]) > .1) continue;
			if ( fabs(grid->verts[grid->width * i + offset].xyz[1] - grid->verts[grid->width * j + offset].xyz[1]) > .1) continue;
			if ( fabs(grid->verts[grid->width * i + offset].xyz[2] - grid->verts[grid->width * j + offset].xyz[2]) > .1) continue;
			return qtrue;
		}
	}
	return qfalse;
}

/*
=================
R_FixSharedVertexLodError_r

NOTE: never sync LoD through grid edges with merged points!

FIXME: write generalized version that also avoids cracks between a patch and one that meets half way?
=================
*/
void R_FixSharedVertexLodError_r( int start, srfGridMesh_t *grid1 ) {
	int j, k, l, m, n, offset1, offset2, touch;
	srfGridMesh_t *grid2;

	for ( j = start; j < s_worldData.numsurfaces; j++ ) {
		//
		grid2 = (srfGridMesh_t *) s_worldData.surfaces[j].data;
		// if this surface is not a grid
		if ( grid2->surfaceType != SF_GRID ) continue;
		// if the LOD errors are already fixed for this patch
		if ( grid2->lodFixed == 2 ) continue;
		// grids in the same LOD group should have the exact same lod radius
		if ( grid1->lodRadius != grid2->lodRadius ) continue;
		// grids in the same LOD group should have the exact same lod origin
		if ( grid1->lodOrigin[0] != grid2->lodOrigin[0] ) continue;
		if ( grid1->lodOrigin[1] != grid2->lodOrigin[1] ) continue;
		if ( grid1->lodOrigin[2] != grid2->lodOrigin[2] ) continue;
		//
		touch = qfalse;
		for (n = 0; n < 2; n++) {
			//
			if (n) offset1 = (grid1->height-1) * grid1->width;
			else offset1 = 0;
			if (R_MergedWidthPoints(grid1, offset1)) continue;
			for (k = 1; k < grid1->width-1; k++) {
				for (m = 0; m < 2; m++) {

					if (m) offset2 = (grid2->height-1) * grid2->width;
					else offset2 = 0;
					if (R_MergedWidthPoints(grid2, offset2)) continue;
					for ( l = 1; l < grid2->width-1; l++) {
					//
						if ( fabs(grid1->verts[k + offset1].xyz[0] - grid2->verts[l + offset2].xyz[0]) > .1) continue;
						if ( fabs(grid1->verts[k + offset1].xyz[1] - grid2->verts[l + offset2].xyz[1]) > .1) continue;
						if ( fabs(grid1->verts[k + offset1].xyz[2] - grid2->verts[l + offset2].xyz[2]) > .1) continue;
						// ok the points are equal and should have the same lod error
						grid2->widthLodError[l] = grid1->widthLodError[k];
						touch = qtrue;
					}
				}
				for (m = 0; m < 2; m++) {

					if (m) offset2 = grid2->width-1;
					else offset2 = 0;
					if (R_MergedHeightPoints(grid2, offset2)) continue;
					for ( l = 1; l < grid2->height-1; l++) {
					//
						if ( fabs(grid1->verts[k + offset1].xyz[0] - grid2->verts[grid2->width * l + offset2].xyz[0]) > .1) continue;
						if ( fabs(grid1->verts[k + offset1].xyz[1] - grid2->verts[grid2->width * l + offset2].xyz[1]) > .1) continue;
						if ( fabs(grid1->verts[k + offset1].xyz[2] - grid2->verts[grid2->width * l + offset2].xyz[2]) > .1) continue;
						// ok the points are equal and should have the same lod error
						grid2->heightLodError[l] = grid1->widthLodError[k];
						touch = qtrue;
					}
				}
			}
		}
		for (n = 0; n < 2; n++) {
			//
			if (n) offset1 = grid1->width-1;
			else offset1 = 0;
			if (R_MergedHeightPoints(grid1, offset1)) continue;
			for (k = 1; k < grid1->height-1; k++) {
				for (m = 0; m < 2; m++) {

					if (m) offset2 = (grid2->height-1) * grid2->width;
					else offset2 = 0;
					if (R_MergedWidthPoints(grid2, offset2)) continue;
					for ( l = 1; l < grid2->width-1; l++) {
					//
						if ( fabs(grid1->verts[grid1->width * k + offset1].xyz[0] - grid2->verts[l + offset2].xyz[0]) > .1) continue;
						if ( fabs(grid1->verts[grid1->width * k + offset1].xyz[1] - grid2->verts[l + offset2].xyz[1]) > .1) continue;
						if ( fabs(grid1->verts[grid1->width * k + offset1].xyz[2] - grid2->verts[l + offset2].xyz[2]) > .1) continue;
						// ok the points are equal and should have the same lod error
						grid2->widthLodError[l] = grid1->heightLodError[k];
						touch = qtrue;
					}
				}
				for (m = 0; m < 2; m++) {

					if (m) offset2 = grid2->width-1;
					else offset2 = 0;
					if (R_MergedHeightPoints(grid2, offset2)) continue;
					for ( l = 1; l < grid2->height-1; l++) {
					//
						if ( fabs(grid1->verts[grid1->width * k + offset1].xyz[0] - grid2->verts[grid2->width * l + offset2].xyz[0]) > .1) continue;
						if ( fabs(grid1->verts[grid1->width * k + offset1].xyz[1] - grid2->verts[grid2->width * l + offset2].xyz[1]) > .1) continue;
						if ( fabs(grid1->verts[grid1->width * k + offset1].xyz[2] - grid2->verts[grid2->width * l + offset2].xyz[2]) > .1) continue;
						// ok the points are equal and should have the same lod error
						grid2->heightLodError[l] = grid1->heightLodError[k];
						touch = qtrue;
					}
				}
			}
		}
		if (touch) {
			grid2->lodFixed = 2;
			R_FixSharedVertexLodError_r ( start, grid2 );
			//NOTE: this would be correct but makes things really slow
			//grid2->lodFixed = 1;
		}
	}
}

/*
=================
R_FixSharedVertexLodError

This function assumes that all patches in one group are nicely stitched together for the highest LoD.
If this is not the case this function will still do its job but won't fix the highest LoD cracks.
=================
*/
void R_FixSharedVertexLodError( void ) {
	int i;
	srfGridMesh_t *grid1;

	for ( i = 0; i < s_worldData.numsurfaces; i++ ) {
		//
		grid1 = (srfGridMesh_t *) s_worldData.surfaces[i].data;
		// if this surface is not a grid
		if ( grid1->surfaceType != SF_GRID )
			continue;
		//
		if ( grid1->lodFixed )
			continue;
		//
		grid1->lodFixed = 2;
		// recursively fix other patches in the same LOD group
		R_FixSharedVertexLodError_r( i + 1, grid1);
	}
}


/*
===============
R_StitchPatches
===============
*/
int R_StitchPatches( int grid1num, int grid2num ) {
	float *v1, *v2;
	srfGridMesh_t *grid1, *grid2;
	int k, l, m, n, offset1, offset2, row, column;

	grid1 = (srfGridMesh_t *) s_worldData.surfaces[grid1num].data;
	grid2 = (srfGridMesh_t *) s_worldData.surfaces[grid2num].data;
	for (n = 0; n < 2; n++) {
		//
		if (n) offset1 = (grid1->height-1) * grid1->width;
		else offset1 = 0;
		if (R_MergedWidthPoints(grid1, offset1))
			continue;
		for (k = 0; k < grid1->width-2; k += 2) {

			for (m = 0; m < 2; m++) {

				if ( grid2->width >= MAX_GRID_SIZE )
					break;
				if (m) offset2 = (grid2->height-1) * grid2->width;
				else offset2 = 0;
				for ( l = 0; l < grid2->width-1; l++) {
				//
					v1 = grid1->verts[k + offset1].xyz;
					v2 = grid2->verts[l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[k + 2 + offset1].xyz;
					v2 = grid2->verts[l + 1 + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[l + offset2].xyz;
					v2 = grid2->verts[l + 1 + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert column into grid2 right after after column l
					if (m) row = grid2->height-1;
					else row = 0;
					grid2 = R_GridInsertColumn( grid2, l+1, row,
									grid1->verts[k + 1 + offset1].xyz, grid1->widthLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
			for (m = 0; m < 2; m++) {

				if (grid2->height >= MAX_GRID_SIZE)
					break;
				if (m) offset2 = grid2->width-1;
				else offset2 = 0;
				for ( l = 0; l < grid2->height-1; l++) {
					//
					v1 = grid1->verts[k + offset1].xyz;
					v2 = grid2->verts[grid2->width * l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[k + 2 + offset1].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[grid2->width * l + offset2].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert row into grid2 right after after row l
					if (m) column = grid2->width-1;
					else column = 0;
					grid2 = R_GridInsertRow( grid2, l+1, column,
										grid1->verts[k + 1 + offset1].xyz, grid1->widthLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
		}
	}
	for (n = 0; n < 2; n++) {
		//
		if (n) offset1 = grid1->width-1;
		else offset1 = 0;
		if (R_MergedHeightPoints(grid1, offset1))
			continue;
		for (k = 0; k < grid1->height-2; k += 2) {
			for (m = 0; m < 2; m++) {

				if ( grid2->width >= MAX_GRID_SIZE )
					break;
				if (m) offset2 = (grid2->height-1) * grid2->width;
				else offset2 = 0;
				for ( l = 0; l < grid2->width-1; l++) {
				//
					v1 = grid1->verts[grid1->width * k + offset1].xyz;
					v2 = grid2->verts[l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[grid1->width * (k + 2) + offset1].xyz;
					v2 = grid2->verts[l + 1 + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[l + offset2].xyz;
					v2 = grid2->verts[(l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert column into grid2 right after after column l
					if (m) row = grid2->height-1;
					else row = 0;
					grid2 = R_GridInsertColumn( grid2, l+1, row,
									grid1->verts[grid1->width * (k + 1) + offset1].xyz, grid1->heightLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
			for (m = 0; m < 2; m++) {

				if (grid2->height >= MAX_GRID_SIZE)
					break;
				if (m) offset2 = grid2->width-1;
				else offset2 = 0;
				for ( l = 0; l < grid2->height-1; l++) {
				//
					v1 = grid1->verts[grid1->width * k + offset1].xyz;
					v2 = grid2->verts[grid2->width * l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[grid1->width * (k + 2) + offset1].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[grid2->width * l + offset2].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert row into grid2 right after after row l
					if (m) column = grid2->width-1;
					else column = 0;
					grid2 = R_GridInsertRow( grid2, l+1, column,
									grid1->verts[grid1->width * (k + 1) + offset1].xyz, grid1->heightLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
		}
	}
	for (n = 0; n < 2; n++) {
		//
		if (n) offset1 = (grid1->height-1) * grid1->width;
		else offset1 = 0;
		if (R_MergedWidthPoints(grid1, offset1))
			continue;
		for (k = grid1->width-1; k > 1; k -= 2) {

			for (m = 0; m < 2; m++) {

				if ( !grid2 || grid2->width >= MAX_GRID_SIZE )
					break;
				if (m) offset2 = (grid2->height-1) * grid2->width;
				else offset2 = 0;
				for ( l = 0; l < grid2->width-1; l++) {
				//
					v1 = grid1->verts[k + offset1].xyz;
					v2 = grid2->verts[l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[k - 2 + offset1].xyz;
					v2 = grid2->verts[l + 1 + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[l + offset2].xyz;
					v2 = grid2->verts[(l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert column into grid2 right after after column l
					if (m) row = grid2->height-1;
					else row = 0;
					grid2 = R_GridInsertColumn( grid2, l+1, row,
										grid1->verts[k - 1 + offset1].xyz, grid1->widthLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
			for (m = 0; m < 2; m++) {

				if (!grid2 || grid2->height >= MAX_GRID_SIZE)
					break;
				if (m) offset2 = grid2->width-1;
				else offset2 = 0;
				for ( l = 0; l < grid2->height-1; l++) {
				//
					v1 = grid1->verts[k + offset1].xyz;
					v2 = grid2->verts[grid2->width * l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[k - 2 + offset1].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[grid2->width * l + offset2].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert row into grid2 right after after row l
					if (m) column = grid2->width-1;
					else column = 0;
					grid2 = R_GridInsertRow( grid2, l+1, column,
										grid1->verts[k - 1 + offset1].xyz, grid1->widthLodError[k+1]);
					if (!grid2)
						break;
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
		}
	}
	for (n = 0; n < 2; n++) {
		//
		if (n) offset1 = grid1->width-1;
		else offset1 = 0;
		if (R_MergedHeightPoints(grid1, offset1))
			continue;
		for (k = grid1->height-1; k > 1; k -= 2) {
			for (m = 0; m < 2; m++) {

				if ( !grid2 || grid2->width >= MAX_GRID_SIZE )
					break;
				if (m) offset2 = (grid2->height-1) * grid2->width;
				else offset2 = 0;
				for ( l = 0; l < grid2->width-1; l++) {
				//
					v1 = grid1->verts[grid1->width * k + offset1].xyz;
					v2 = grid2->verts[l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[grid1->width * (k - 2) + offset1].xyz;
					v2 = grid2->verts[l + 1 + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[l + offset2].xyz;
					v2 = grid2->verts[(l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert column into grid2 right after after column l
					if (m) row = grid2->height-1;
					else row = 0;
					grid2 = R_GridInsertColumn( grid2, l+1, row,
										grid1->verts[grid1->width * (k - 1) + offset1].xyz, grid1->heightLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
			for (m = 0; m < 2; m++) {

				if (!grid2 || grid2->height >= MAX_GRID_SIZE)
					break;
				if (m) offset2 = grid2->width-1;
				else offset2 = 0;
				for ( l = 0; l < grid2->height-1; l++) {
				//
					v1 = grid1->verts[grid1->width * k + offset1].xyz;
					v2 = grid2->verts[grid2->width * l + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;

					v1 = grid1->verts[grid1->width * (k - 2) + offset1].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) > .1)
						continue;
					if ( fabs(v1[1] - v2[1]) > .1)
						continue;
					if ( fabs(v1[2] - v2[2]) > .1)
						continue;
					//
					v1 = grid2->verts[grid2->width * l + offset2].xyz;
					v2 = grid2->verts[grid2->width * (l + 1) + offset2].xyz;
					if ( fabs(v1[0] - v2[0]) < .01 &&
							fabs(v1[1] - v2[1]) < .01 &&
							fabs(v1[2] - v2[2]) < .01)
						continue;
					//
					//ri.Printf( PRINT_ALL, "found highest LoD crack between two patches\n" );
					// insert row into grid2 right after after row l
					if (m) column = grid2->width-1;
					else column = 0;
					grid2 = R_GridInsertRow( grid2, l+1, column,
										grid1->verts[grid1->width * (k - 1) + offset1].xyz, grid1->heightLodError[k+1]);
					grid2->lodStitched = qfalse;
					s_worldData.surfaces[grid2num].data = (void *) grid2;
					return qtrue;
				}
			}
		}
	}
	return qfalse;
}

/*
===============
R_TryStitchPatch

This function will try to stitch patches in the same LoD group together for the highest LoD.

Only single missing vertice cracks will be fixed.

Vertices will be joined at the patch side a crack is first found, at the other side
of the patch (on the same row or column) the vertices will not be joined and cracks
might still appear at that side.
===============
*/
int R_TryStitchingPatch( int grid1num ) {
	int j, numstitches;
	srfGridMesh_t *grid1, *grid2;

	numstitches = 0;
	grid1 = (srfGridMesh_t *) s_worldData.surfaces[grid1num].data;
	for ( j = 0; j < s_worldData.numsurfaces; j++ ) {
		//
		grid2 = (srfGridMesh_t *) s_worldData.surfaces[j].data;
		// if this surface is not a grid
		if ( grid2->surfaceType != SF_GRID ) continue;
		// grids in the same LOD group should have the exact same lod radius
		if ( grid1->lodRadius != grid2->lodRadius ) continue;
		// grids in the same LOD group should have the exact same lod origin
		if ( grid1->lodOrigin[0] != grid2->lodOrigin[0] ) continue;
		if ( grid1->lodOrigin[1] != grid2->lodOrigin[1] ) continue;
		if ( grid1->lodOrigin[2] != grid2->lodOrigin[2] ) continue;
		//
		while (R_StitchPatches(grid1num, j))
		{
			numstitches++;
		}
	}
	return numstitches;
}

/*
===============
R_StitchAllPatches
===============
*/
void R_StitchAllPatches( void ) {
	int i, stitched, numstitches;
	srfGridMesh_t *grid1;

	numstitches = 0;
	do
	{
		stitched = qfalse;
		for ( i = 0; i < s_worldData.numsurfaces; i++ ) {
			//
			grid1 = (srfGridMesh_t *) s_worldData.surfaces[i].data;
			// if this surface is not a grid
			if ( grid1->surfaceType != SF_GRID )
				continue;
			//
			if ( grid1->lodStitched )
				continue;
			//
			grid1->lodStitched = qtrue;
			stitched = qtrue;
			//
			numstitches += R_TryStitchingPatch( i );
		}
	}
	while (stitched);

	ri.Printf(PRINT_DEVELOPER, "stitched %d LoD cracks\n", numstitches);
}

/*
===============
R_MovePatchSurfacesToHunk
===============
*/
void R_MovePatchSurfacesToHunk(void) {
	int i, size;
	srfGridMesh_t *grid, *hunkgrid;

	for ( i = 0; i < s_worldData.numsurfaces; i++ ) {
		//
		grid = (srfGridMesh_t *) s_worldData.surfaces[i].data;
		// if this surface is not a grid
		if ( grid->surfaceType != SF_GRID )
			continue;
		//
		size = (grid->width * grid->height - 1) * sizeof( drawVert_t ) + sizeof( *grid );
		hunkgrid = ri.Hunk_Alloc( size, h_low );
		Com_Memcpy(hunkgrid, grid, size);

		hunkgrid->widthLodError = ri.Hunk_Alloc( grid->width * 4, h_low );
		Com_Memcpy( hunkgrid->widthLodError, grid->widthLodError, grid->width * 4 );

		hunkgrid->heightLodError = ri.Hunk_Alloc( grid->height * 4, h_low );
		Com_Memcpy( hunkgrid->heightLodError, grid->heightLodError, grid->height * 4 );

		R_FreeSurfaceGridMesh( grid );

		s_worldData.surfaces[i].data = (void *) hunkgrid;
	}
}

/*
===============
R_LoadSurfaces
===============
*/
static	void R_LoadSurfaces( const bspFile_t *bsp ) {
	dsurface_t	*in;
	msurface_t	*out;
	drawVert_t	*dv;
	int			*indexes;
	int			count;
	int			numFaces, numMeshes, numTriSurfs, numFlares, numFoliage, numTerrain;
	int			i;

	numFaces = 0;
	numMeshes = 0;
	numTriSurfs = 0;
	numFlares = 0;
	numFoliage = 0;
	numTerrain = 0;

	in = bsp->surfaces;
	count = bsp->numSurfaces;

	dv = bsp->drawVerts;

	indexes = bsp->drawIndexes;

	out = ri.Hunk_Alloc ( count * sizeof(*out), h_low );	

	s_worldData.surfaces = out;
	s_worldData.numsurfaces = count;

	// Ridah, init the surface memory. This is optimization, so we don't have to
	// look for memory for each surface, we allocate a big block and just chew it up
	// as we go
	R_InitSurfMemory();

	for ( i = 0 ; i < count ; i++, in++, out++ ) {
		switch ( LittleLong( in->surfaceType ) ) {
		case MST_PATCH:
			ParseMesh ( in, dv, out );
			numMeshes++;
			break;
		case MST_TRIANGLE_SOUP:
			ParseTriSurf( in, dv, out, indexes, qfalse );
			numTriSurfs++;
			break;
		case MST_PLANAR:
			ParseTriSurf( in, dv, out, indexes, qtrue );
			numFaces++;
			break;
		case MST_FLARE:
			ParseFlare( in, dv, out, indexes );
			numFlares++;
			break;
		case MST_FOLIAGE:
			ParseFoliage( in, dv, out, indexes );
			numFoliage++;
			break;
		case MST_TERRAIN:
			ParseTriSurf( in, dv, out, indexes, qfalse );
			numTerrain++;
			break;
		default:
			ri.Error( ERR_DROP, "Bad surfaceType" );
		}
	}

#ifdef PATCH_STITCHING
	R_StitchAllPatches();
#endif

	R_FixSharedVertexLodError();

#ifdef PATCH_STITCHING
	R_MovePatchSurfacesToHunk();
#endif

	ri.Printf(PRINT_DEVELOPER, "...loaded %d faces, %i meshes, %i trisurfs, %i flares, %i foliage, %i terrain\n", 
		numFaces, numMeshes, numTriSurfs, numFlares, numFoliage, numTerrain );
}



/*
=================
R_LoadSubmodels
=================
*/
static	void R_LoadSubmodels( const bspFile_t *bsp ) {
	dmodel_t	*in;
	bmodel_t	*out;
	int			i, j, count;

	in = bsp->submodels;
	count = bsp->numSubmodels;

	s_worldData.numBModels = count;
	s_worldData.bmodels = out = ri.Hunk_Alloc( count * sizeof(*out), h_low );

	for ( i=0 ; i<count ; i++, in++, out++ ) {
		model_t *model;

		model = R_AllocModel();

		assert( model != NULL );			// this should never happen
		if ( model == NULL ) {
			ri.Error(ERR_DROP, "R_LoadSubmodels: R_AllocModel() failed");
		}

		model->type = MOD_BRUSH;
		model->bmodel = out;
		Com_sprintf( model->name, sizeof( model->name ), "*%d", i );

		for (j=0 ; j<3 ; j++) {
			out->bounds[0][j] = LittleFloat (in->mins[j]);
			out->bounds[1][j] = LittleFloat (in->maxs[j]);
		}

		out->firstSurface = s_worldData.surfaces + LittleLong( in->firstSurface );
		out->numSurfaces = LittleLong( in->numSurfaces );

		// for attaching fog brushes to models
		out->firstBrush = LittleLong( in->firstBrush );
		out->numBrushes = LittleLong( in->numBrushes );
	}
}



//==================================================================

/*
=================
R_SetParent
=================
*/
static	void R_SetParent (mnode_t *node, mnode_t *parent) {
	//  set parent
	node->parent = parent;

	// handle leaf nodes
	if ( node->isLeaf ) {
		// add node surfaces to bounds
		if ( node->nummarksurfaces > 0 ) {
			int c;
			msurface_t      *surf, **mark;
			surfaceType_t	surfaceType;

			mark = node->firstmarksurface;
			c = node->nummarksurfaces;
			while ( c-- ) {
				surf = *mark;
				surfaceType = *surf->data;
				if ( surfaceType == SF_GRID || surfaceType == SF_TRIANGLES || surfaceType == SF_FOLIAGE ) {
					srfGeneric_t *gen = ( srfGeneric_t* )surf->data;

					AddPointToBounds( gen->bounds[ 0 ], node->surfMins, node->surfMaxs );
					AddPointToBounds( gen->bounds[ 1 ], node->surfMins, node->surfMaxs );
				}
				mark++;
			}
		}

		// go back
		return;
	}

	// recurse to child nodes
	R_SetParent (node->children[0], node);
	R_SetParent (node->children[1], node);

	// surface bounds
	AddPointToBounds( node->children[ 0 ]->surfMins, node->surfMins, node->surfMaxs );
	AddPointToBounds( node->children[ 0 ]->surfMaxs, node->surfMins, node->surfMaxs );
	AddPointToBounds( node->children[ 1 ]->surfMins, node->surfMins, node->surfMaxs );
	AddPointToBounds( node->children[ 1 ]->surfMaxs, node->surfMins, node->surfMaxs );
}

/*
=================
R_LoadNodesAndLeafs
=================
*/
static	void R_LoadNodesAndLeafs( const bspFile_t *bsp ) {
	int			i, j, p;
	dnode_t		*in;
	dleaf_t		*inLeaf;
	mnode_t 	*out;
	int			numNodes, numLeafs;

	in = bsp->nodes;
	numNodes = bsp->numNodes;
	numLeafs = bsp->numLeafs;

	out = ri.Hunk_Alloc ( (numNodes + numLeafs) * sizeof(*out), h_low);	

	s_worldData.nodes = out;
	s_worldData.numnodes = numNodes + numLeafs;
	s_worldData.numDecisionNodes = numNodes;

	// load nodes
	for ( i=0 ; i<numNodes; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{
			out->mins[j] = LittleLong (in->mins[j]);
			out->maxs[j] = LittleLong (in->maxs[j]);
		}
	
		// surface bounds
		VectorCopy( out->mins, out->surfMins );
		VectorCopy( out->maxs, out->surfMaxs );

		p = LittleLong(in->planeNum);
		out->plane = s_worldData.planes + p;

		out->isLeaf = qfalse;	// differentiate from leafs

		for (j=0 ; j<2 ; j++)
		{
			p = LittleLong (in->children[j]);
			if (p >= 0)
				out->children[j] = s_worldData.nodes + p;
			else
				out->children[j] = s_worldData.nodes + numNodes + (-1 - p);
		}
	}
	
	// load leafs
	inLeaf = bsp->leafs;
	for ( i=0 ; i<numLeafs ; i++, inLeaf++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{
			out->mins[j] = LittleLong (inLeaf->mins[j]);
			out->maxs[j] = LittleLong (inLeaf->maxs[j]);
		}

		// surface bounds
		ClearBounds( out->surfMins, out->surfMaxs );

		out->isLeaf = qtrue;	// differentiate from nodes

		out->cluster = LittleLong(inLeaf->cluster);
		out->area = LittleLong(inLeaf->area);

		if ( out->cluster >= s_worldData.numClusters ) {
			s_worldData.numClusters = out->cluster + 1;
		}

		out->firstmarksurface = s_worldData.marksurfaces +
			LittleLong(inLeaf->firstLeafSurface);
		out->nummarksurfaces = LittleLong(inLeaf->numLeafSurfaces);
	}	

	// chain descendants
	R_SetParent (s_worldData.nodes, NULL);
}

//=============================================================================

/*
=================
R_LoadShaders
=================
*/
static	void R_LoadShaders( const bspFile_t *bsp ) {
	s_worldData.shaders = bsp->shaders;
	s_worldData.numShaders = bsp->numShaders;
}


/*
=================
R_LoadMarksurfaces
=================
*/
static	void R_LoadMarksurfaces( const bspFile_t *bsp )
{	
	int		i, j, count;
	int		*in;
	msurface_t **out;
	
	in = bsp->leafSurfaces;
	count = bsp->numLeafSurfaces;
	out = ri.Hunk_Alloc ( count*sizeof(*out), h_low);	

	s_worldData.marksurfaces = out;
	s_worldData.nummarksurfaces = count;

	for ( i=0 ; i<count ; i++)
	{
		j = LittleLong(in[i]);
		out[i] = s_worldData.surfaces + j;
	}
}


/*
=================
R_LoadPlanes
=================
*/
static	void R_LoadPlanes( const bspFile_t *bsp ) {
	int			i, j;
	cplane_t	*out;
	dplane_t 	*in;
	int			count;
	int			bits;
	
	in = bsp->planes;
	count = bsp->numPlanes;
	out = ri.Hunk_Alloc ( count*2*sizeof(*out), h_low);	
	
	s_worldData.planes = out;
	s_worldData.numplanes = count;

	for ( i=0 ; i<count ; i++, in++, out++) {
		bits = 0;
		for (j=0 ; j<3 ; j++) {
			out->normal[j] = LittleFloat (in->normal[j]);
			if (out->normal[j] < 0) {
				bits |= 1<<j;
			}
		}

		out->dist = LittleFloat (in->dist);
		out->type = PlaneTypeForNormal( out->normal );
		out->signbits = bits;
	}
}

/*
=================
R_LoadFogs

=================
*/
static	void R_LoadFogs( const bspFile_t *bsp ) {
	int			i, j;
	fog_t		*out;
	dfog_t		*fogs;
	dbrush_t 	*brushes, *brush;
	dbrushside_t	*sides;
	int			count, brushesCount, sidesCount;
	int			sideNum;
	int			planeNum;
	shader_t	*shader;
	float		d;
	int			firstSide;

	fogs = bsp->fogs;
	count = bsp->numFogs;

	// create fog structures for them
	s_worldData.numfogs = count + 2;
	s_worldData.fogs = ri.Hunk_Alloc ( s_worldData.numfogs*sizeof(*out), h_low);
	out = s_worldData.fogs + 1;

	// reset bsp global fog number
	s_worldData.globalFogNum = -1;

	// setup global fog
	out->originalBrushNumber = -1;
	VectorSet( out->bounds[ 0 ], MIN_WORLD_COORD, MIN_WORLD_COORD, MIN_WORLD_COORD );
	VectorSet( out->bounds[ 1 ], MAX_WORLD_COORD, MAX_WORLD_COORD, MAX_WORLD_COORD );
	out++;

	if ( !count ) {
		return;
	}

	brushes = bsp->brushes;
	brushesCount = bsp->numBrushes;

	sides = bsp->brushSides;
	sidesCount = bsp->numBrushSides;

	for ( i=2 ; i<count+2 ; i++, fogs++) {
		out->originalBrushNumber = LittleLong( fogs->brushNum );

		// global fog has a brush number of -1, and no visible side
		if ( out->originalBrushNumber == -1 ) {
			VectorSet( out->bounds[ 0 ], MIN_WORLD_COORD, MIN_WORLD_COORD, MIN_WORLD_COORD );
			VectorSet( out->bounds[ 1 ], MAX_WORLD_COORD, MAX_WORLD_COORD, MAX_WORLD_COORD );

			firstSide = 0;
		} else {
			if ( (unsigned)out->originalBrushNumber >= brushesCount ) {
				ri.Error( ERR_DROP, "fog brushNumber out of range" );
			}

			// find which bsp submodel the fog volume belongs to
			for ( j = 0; j < s_worldData.numBModels; j++ )
			{
				if ( out->originalBrushNumber >= s_worldData.bmodels[ j ].firstBrush &&
					 out->originalBrushNumber < ( s_worldData.bmodels[ j ].firstBrush + s_worldData.bmodels[ j ].numBrushes ) ) {
					out->modelNum = j;
					break;
				}
			}

			brush = brushes + out->originalBrushNumber;

			firstSide = LittleLong( brush->firstSide );

			if ( (unsigned)firstSide > sidesCount - 6 ) {
				ri.Error( ERR_DROP, "fog brush sideNumber out of range" );
			}

			// brushes are always sorted with the axial sides first
			sideNum = firstSide + 0;
			planeNum = LittleLong( sides[ sideNum ].planeNum );
			out->bounds[0][0] = -s_worldData.planes[ planeNum ].dist;

			sideNum = firstSide + 1;
			planeNum = LittleLong( sides[ sideNum ].planeNum );
			out->bounds[1][0] = s_worldData.planes[ planeNum ].dist;

			sideNum = firstSide + 2;
			planeNum = LittleLong( sides[ sideNum ].planeNum );
			out->bounds[0][1] = -s_worldData.planes[ planeNum ].dist;

			sideNum = firstSide + 3;
			planeNum = LittleLong( sides[ sideNum ].planeNum );
			out->bounds[1][1] = s_worldData.planes[ planeNum ].dist;

			sideNum = firstSide + 4;
			planeNum = LittleLong( sides[ sideNum ].planeNum );
			out->bounds[0][2] = -s_worldData.planes[ planeNum ].dist;

			sideNum = firstSide + 5;
			planeNum = LittleLong( sides[ sideNum ].planeNum );
			out->bounds[1][2] = s_worldData.planes[ planeNum ].dist;
		}

		// get information from the shader for fog parameters
		shader = R_FindShader( fogs->shader, LIGHTMAP_NONE, MIP_RAW_IMAGE );

		out->shader = shader;

		out->colorInt = ColorBytes4 ( shader->fogParms.color[0] * tr.identityLight, 
			                          shader->fogParms.color[1] * tr.identityLight, 
			                          shader->fogParms.color[2] * tr.identityLight, 1.0 );

		d = shader->fogParms.depthForOpaque < 1 ? 1 : shader->fogParms.depthForOpaque;
		out->tcScale = R_FogTcScale( shader->fogParms.fogType, d, shader->fogParms.density );

		if ( out->originalBrushNumber == -1 ) {
			s_worldData.globalFogNum = i;
		} else {
			// set the gradient vector
			sideNum = LittleLong( fogs->visibleSide );

			if ( sideNum < 0 || sideNum >= sidesCount ) {
				out->hasSurface = qfalse;
			} else {
				out->hasSurface = qtrue;
				planeNum = LittleLong( sides[ firstSide + sideNum ].planeNum );
				VectorSubtract( vec3_origin, s_worldData.planes[ planeNum ].normal, out->surface );
				out->surface[3] = -s_worldData.planes[ planeNum ].dist;
			}
		}

		out++;
	}
}


/*
================
R_LoadLightGrid

================
*/
void R_LoadLightGrid( const bspFile_t *bsp ) {
	int		i;
	vec3_t	maxs;
	int		numGridPoints;
	world_t	*w;
	float	*wMins, *wMaxs;

	w = &s_worldData;

	w->lightGridInverseSize[0] = 1.0f / w->lightGridSize[0];
	w->lightGridInverseSize[1] = 1.0f / w->lightGridSize[1];
	w->lightGridInverseSize[2] = 1.0f / w->lightGridSize[2];

	wMins = w->bmodels[0].bounds[0];
	wMaxs = w->bmodels[0].bounds[1];

	for ( i = 0 ; i < 3 ; i++ ) {
		w->lightGridOrigin[i] = w->lightGridSize[i] * ceil( wMins[i] / w->lightGridSize[i] );
		maxs[i] = w->lightGridSize[i] * floor( wMaxs[i] / w->lightGridSize[i] );
		w->lightGridBounds[i] = (maxs[i] - w->lightGridOrigin[i])/w->lightGridSize[i] + 1;
	}

	numGridPoints = w->lightGridBounds[0] * w->lightGridBounds[1] * w->lightGridBounds[2];

	if ( !bsp->lightGridData || !bsp->numGridPoints ) {
		ri.Printf( PRINT_WARNING, "WARNING: no light grid data\n" );
		w->lightGridData = NULL;
		return;
	}

	if ( bsp->numGridArrayPoints ) {
		if ( numGridPoints != bsp->numGridArrayPoints ) {
			ri.Printf( PRINT_WARNING, "WARNING: light grid array mismatch: %d points found, expected %d\n", numGridPoints, bsp->numGridArrayPoints );
			w->lightGridData = NULL;
			return;
		}
	}
	else if ( numGridPoints != bsp->numGridPoints ) {
		ri.Printf( PRINT_WARNING, "WARNING: light grid mismatch: %d points found, expected %d\n", numGridPoints, bsp->numGridPoints );
		w->lightGridData = NULL;
		return;
	}

	w->lightGridData = bsp->lightGridData;
	w->numGridPoints = bsp->numGridPoints;
	w->lightGridArray = bsp->lightGridArray;
	w->numGridArrayPoints = bsp->numGridArrayPoints;

	// deal with overbright bits
	for ( i = 0 ; i < bsp->numGridPoints ; i++ ) {
		R_ColorShiftLightingBytes( 4, &w->lightGridData[i*8], &w->lightGridData[i*8] );
		R_ColorShiftLightingBytes( 4, &w->lightGridData[i*8+3], &w->lightGridData[i*8+3] );
	}
}

/*
================
R_LoadEntities
================
*/
void R_LoadEntities( const bspFile_t *bsp ) {
	char *p, *token, *s;
	char keyname[MAX_TOKEN_CHARS];
	char value[MAX_TOKEN_CHARS];
	world_t	*w;

	w = &s_worldData;
	w->lightGridSize[0] = bsp->defaultLightGridSize[0];
	w->lightGridSize[1] = bsp->defaultLightGridSize[1];
	w->lightGridSize[2] = bsp->defaultLightGridSize[2];

	p = bsp->entityString;

	// store for reference by the cgame
	w->entityString = bsp->entityString;
	w->entityParsePoint = w->entityString;

	token = COM_ParseExt( &p, qtrue );
	if (!*token || *token != '{') {
		return;
	}

	// only parse the world spawn
	while ( 1 ) {	
		// parse key
		token = COM_ParseExt( &p, qtrue );

		if ( !*token || *token == '}' ) {
			break;
		}
		Q_strncpyz(keyname, token, sizeof(keyname));

		// parse value
		token = COM_ParseExt( &p, qtrue );

		if ( !*token || *token == '}' ) {
			break;
		}
		Q_strncpyz(value, token, sizeof(value));

		// check for remapping of shaders for vertex lighting
		s = "vertexremapshader";
		if (!Q_strncmp(keyname, s, strlen(s)) ) {
			s = strchr(value, ';');
			if (!s) {
				ri.Printf( PRINT_WARNING, "WARNING: no semi colon in vertexshaderremap '%s'\n", value );
				break;
			}
			*s++ = 0;
			if (r_vertexLight->integer) {
				R_RemapShader(value, s, "0");
			}
			continue;
		}
		// check for remapping of shaders
		s = "remapshader";
		if (!Q_strncmp(keyname, s, strlen(s)) ) {
			s = strchr(value, ';');
			if (!s) {
				ri.Printf( PRINT_WARNING, "WARNING: no semi colon in shaderremap '%s'\n", value );
				break;
			}
			*s++ = 0;
			R_RemapShader(value, s, "0");
			continue;
		}
		// check for a different grid size
		if (!Q_stricmp(keyname, "gridsize")) {
			sscanf(value, "%f %f %f", &w->lightGridSize[0], &w->lightGridSize[1], &w->lightGridSize[2] );
			continue;
		}
	}
}

/*
=================
R_GetEntityToken
=================
*/
qboolean R_GetEntityToken( char *buffer, int size ) {
	const char	*s;

	s = COM_Parse( &s_worldData.entityParsePoint );
	Q_strncpyz( buffer, s, size );
	if ( !s_worldData.entityParsePoint && !s[0] ) {
		s_worldData.entityParsePoint = s_worldData.entityString;
		return qfalse;
	} else {
		return qtrue;
	}
}

/*
=================
RE_LoadWorldMap

Called directly from cgame
=================
*/
void RE_LoadWorldMap( const bspFile_t *bsp ) {
	byte		*startMarker;
	const char	*name;

	if ( tr.worldMapLoaded ) {
		ri.Error( ERR_DROP, "ERROR: attempted to redundantly load world map" );
	}

	if ( !bsp ) {
		ri.Error( ERR_DROP, "RE_LoadWorldMap: bsp is NULL" );
	}

	name = bsp->name;

	// set default sun direction to be used if it isn't
	// overridden by a shader
	tr.sunDirection[0] = 0.45f;
	tr.sunDirection[1] = 0.3f;
	tr.sunDirection[2] = 0.9f;

	VectorNormalize( tr.sunDirection );

	tr.worldMapLoaded = qtrue;
	tr.worldDir = ri.Hunk_Alloc( strlen(name)+1, h_low );
	COM_StripExtension(name, tr.worldDir, strlen(name)+1);

	// clear tr.world so if the level fails to load, the next
	// try will not look at the partially loaded version
	tr.world = NULL;

	Com_Memset( &s_worldData, 0, sizeof( s_worldData ) );
	Q_strncpyz( s_worldData.name, name, sizeof( s_worldData.name ) );

	Q_strncpyz( s_worldData.baseName, COM_SkipPath( s_worldData.name ), sizeof( s_worldData.name ) );
	COM_StripExtension(s_worldData.baseName, s_worldData.baseName, sizeof(s_worldData.baseName));

	startMarker = ri.Hunk_Alloc(0, h_low);
	c_gridVerts = 0;

	// load into heap
	R_LoadShaders( bsp );
	R_LoadLightmaps( bsp );
	R_LoadPlanes( bsp );
	R_LoadSurfaces( bsp );
	R_LoadMarksurfaces( bsp );
	R_LoadNodesAndLeafs( bsp );
	R_LoadSubmodels( bsp );
	R_LoadFogs( bsp );
	R_LoadVisibility( bsp );
	R_LoadEntities( bsp );
	R_LoadLightGrid( bsp );

	s_worldData.dataSize = (byte *)ri.Hunk_Alloc(0, h_low) - startMarker;

	// only set tr.world now that we know the entire level has loaded properly
	tr.world = &s_worldData;

	// If global fog was found in BSP and hasn't been set by a shader, use it
	if ( tr.world->globalFogNum != -1 && tr.world->fogs[ tr.world->globalFogNum ].shader && !tr.globalFogType ) {
		shader_t *shader = tr.world->fogs[ tr.world->globalFogNum ].shader;

		// Set default global fog
		tr.globalFogType = shader->fogParms.fogType;
		VectorCopy( shader->fogParms.color, tr.globalFogColor );
		tr.globalFogDepthForOpaque = shader->fogParms.depthForOpaque;
		tr.globalFogDensity = shader->fogParms.density;
		tr.globalFogFarClip = shader->fogParms.farClip;
	}

	R_InitExternalShaders();
}

