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
// tr_marks.c -- polygon projection on the world polygons

#include "tr_local.h"
//#include "assert.h"

#define MAX_VERTS_ON_POLY		64

#define MARKER_OFFSET			0	// 1

/*
=============
R_ChopPolyBehindPlane

Out must have space for two more vertexes than in
=============
*/
#define	SIDE_FRONT	0
#define	SIDE_BACK	1
#define	SIDE_ON		2
static void R_ChopPolyBehindPlane( int numInPoints, vec3_t inPoints[MAX_VERTS_ON_POLY],
								   int *numOutPoints, vec3_t outPoints[MAX_VERTS_ON_POLY], 
							vec3_t normal, vec_t dist, vec_t epsilon) {
	float		dists[MAX_VERTS_ON_POLY+4] = { 0 };
	int			sides[MAX_VERTS_ON_POLY+4] = { 0 };
	int			counts[3];
	float		dot;
	int			i, j;
	float		*p1, *p2, *clip;
	float		d;

	// don't clip if it might overflow
	if ( numInPoints >= MAX_VERTS_ON_POLY - 2 ) {
		*numOutPoints = 0;
		return;
	}

	counts[0] = counts[1] = counts[2] = 0;

	// determine sides for each point
	for ( i = 0 ; i < numInPoints ; i++ ) {
		dot = DotProduct( inPoints[i], normal );
		dot -= dist;
		dists[i] = dot;
		if ( dot > epsilon ) {
			sides[i] = SIDE_FRONT;
		} else if ( dot < -epsilon ) {
			sides[i] = SIDE_BACK;
		} else {
			sides[i] = SIDE_ON;
		}
		counts[sides[i]]++;
	}
	sides[i] = sides[0];
	dists[i] = dists[0];

	*numOutPoints = 0;

	if ( !counts[0] ) {
		return;
	}
	if ( !counts[1] ) {
		*numOutPoints = numInPoints;
		Com_Memcpy( outPoints, inPoints, numInPoints * sizeof(vec3_t) );
		return;
	}

	for ( i = 0 ; i < numInPoints ; i++ ) {
		p1 = inPoints[i];
		clip = outPoints[ *numOutPoints ];
		
		if ( sides[i] == SIDE_ON ) {
			VectorCopy( p1, clip );
			(*numOutPoints)++;
			continue;
		}
	
		if ( sides[i] == SIDE_FRONT ) {
			VectorCopy( p1, clip );
			(*numOutPoints)++;
			clip = outPoints[ *numOutPoints ];
		}

		if ( sides[i+1] == SIDE_ON || sides[i+1] == sides[i] ) {
			continue;
		}
			
		// generate a split point
		p2 = inPoints[ (i+1) % numInPoints ];

		d = dists[i] - dists[i+1];
		if ( d == 0 ) {
			dot = 0;
		} else {
			dot = dists[i] / d;
		}

		// clip xyz

		for (j=0 ; j<3 ; j++) {
			clip[j] = p1[j] + dot * ( p2[j] - p1[j] );
		}

		(*numOutPoints)++;
	}
}

/*
=================
R_AddSurfaceToList

mins and maxs needs to be in bmodel local space
=================
*/
void R_AddSurfaceToList( int bmodelNum, int surfNum, vec3_t mins, vec3_t maxs, surfaceType_t **list, int *listbmodel, int listsize, int *listlength, vec3_t dir ) {
	int *surfViewCount;
	msurface_t	*surf;
	int s;

	if (*listlength >= listsize)
		return;

	surfViewCount = &tr.world->surfacesViewCount[surfNum];
	surf = ( msurface_t * )( tr.world->surfaces + surfNum );

	// check if the surface has NOIMPACT or NOMARKS set
	if ( surf->shader->surfaceParms & ( SURF_NOIMPACT | SURF_NOMARKS | SURF_FOG ) ) {
		*surfViewCount = tr.viewCount;
	}
	// extra check for surfaces to avoid list overflows
	else if (*(surf->data) == SF_FACE) {
		// the face plane should go through the box
		s = BoxOnPlaneSide( mins, maxs, &surf->cullinfo.plane );
		if (s == 1 || s == 2) {
			*surfViewCount = tr.viewCount;
		} else if (DotProduct(surf->cullinfo.plane.normal, dir) > -0.5) {
		// don't add faces that make sharp angles with the projection direction
			*surfViewCount = tr.viewCount;
		}
	}
	else if (*(surf->data) != SF_GRID &&
		 *(surf->data) != SF_TRIANGLES)
		*surfViewCount = tr.viewCount;
	// check the viewCount because the surface may have
	// already been added if it spans multiple leafs
	if (*surfViewCount != tr.viewCount) {
		*surfViewCount = tr.viewCount;
		list[*listlength] = surf->data;
		listbmodel[*listlength] = bmodelNum;
		(*listlength)++;
	}
}

/*
=================
R_BmodelSurfaces

mins and maxs needs to be in local space for R_AddSurfaceToList
=================
*/
void R_BmodelSurfaces(int bmodelNum, vec3_t mins, vec3_t maxs, surfaceType_t **list, int *listbmodel, int listsize, int *listlength, vec3_t dir) {
	int i, j;
	bmodel_t *bmodel = &tr.world->bmodels[bmodelNum];
	const float	*bmins = bmodel->bounds[ 0 ];
	const float	*bmaxs = bmodel->bounds[ 1 ];

	for ( j = 0; j < 3; j++ )
	{
		if ( bmins[ j ] > maxs[ j ] ) {
			break;
		}
		if ( bmaxs[ j ] < mins[ j ] ) {
			break;
		}
	}

	if ( j != 3 ) {
		// not touching
		return;
	}

	// add model surfaces
	for ( i = 0; i < bmodel->numSurfaces; i++ ) {
		if (*listlength >= listsize) break;

		R_AddSurfaceToList( bmodelNum, bmodel->firstSurface + i, mins, maxs, list, listbmodel, listsize, listlength, dir );
	}
}

// XXX tr_scene.c
int R_RefEntityForBmodelNum( int bmodelNum );

/*
=================
R_GetBmodelInfo
=================
*/
void R_GetBmodelInfo( int bmodelNum, int *pEntityNum, vec3_t origin, vec3_t axis[3] ) {
	int entityNum;

	entityNum = R_RefEntityForBmodelNum( bmodelNum );

	if ( pEntityNum ) {
		*pEntityNum = entityNum;
	}

	if ( entityNum == -1 ) {
		bmodel_t *bmodel;

		bmodel = &tr.world->bmodels[bmodelNum];

		// not added this scene, use last valid data
		// this is bad, mark might not be on brush
		//ri.Printf( PRINT_ALL, "DEBUG: bmodel %d not added this scene\n", bmodelNum );

		VectorCopy( bmodel->orientation.origin, origin );
		VectorCopy( bmodel->orientation.axis[0], axis[0] );
		VectorCopy( bmodel->orientation.axis[1], axis[1] );
		VectorCopy( bmodel->orientation.axis[2], axis[2] );
	} else {
		// ZTM: TODO: remove this code or fix stuff so it's actually used? player shadow and bullet marks don't use it.
		trRefEntity_t *ent;

		ent = &backEndData->entities[entityNum];

		VectorCopy( ent->e.origin, origin );
		VectorCopy( ent->e.axis[0], axis[0] );
		VectorCopy( ent->e.axis[1], axis[1] );
		VectorCopy( ent->e.axis[2], axis[2] );
	}
}

/*
=================
R_TransformMarkProjection

transform world space data to bmodel local space
=================
*/
void R_TransformMarkProjection( int bmodelNum, const vec3_t inProjection, vec3_t outProjection, int numPlanes, vec3_t *inNormals, float *inDists, vec3_t *outNormals, float *outDists ) {
	int i;
	int entityNum;
	vec3_t origin, axis[3];

	R_GetBmodelInfo( bmodelNum, &entityNum, origin, axis );

	if ( entityNum == REFENTITYNUM_WORLD ) {
		for ( i = 0; i < numPlanes; i++ ) {
			VectorCopy( inNormals[i], outNormals[i] );
			outDists[i] = inDists[i];
		}

		VectorCopy( inProjection, outProjection );
		return;
	}

	for ( i = 0; i < numPlanes; i++ ) {
		outNormals[i][0] = DotProduct( inNormals[i], axis[0] );
		outNormals[i][1] = DotProduct( inNormals[i], axis[1] );
		outNormals[i][2] = DotProduct( inNormals[i], axis[2] );
		outDists[i] = inDists[i] - DotProduct( inNormals[i], origin );
	}

	outProjection[0] = DotProduct( inProjection, axis[0] );
	outProjection[1] = DotProduct( inProjection, axis[1] );
	outProjection[2] = DotProduct( inProjection, axis[2] );
}

/*
=================
R_BoxSurfaces_r

=================
*/
void R_BoxSurfaces_r(mnode_t *node, vec3_t mins, vec3_t maxs, surfaceType_t **list, int *listbmodel, int listsize, int *listlength, vec3_t dir) {

	int			s, c;
	int *mark;

	// do the tail recursion in a loop
	while ( !node->isLeaf ) {
		s = BoxOnPlaneSide( mins, maxs, node->plane );
		if (s == 1) {
			node = node->children[0];
		} else if (s == 2) {
			node = node->children[1];
		} else {
			R_BoxSurfaces_r(node->children[0], mins, maxs, list, listbmodel, listsize, listlength, dir);
			node = node->children[1];
		}
	}

	// add the individual surfaces
	mark = tr.world->marksurfaces + node->firstmarksurface;
	c = node->nummarksurfaces;
	while (c--) {
		//
		if (*listlength >= listsize) break;
		//
		R_AddSurfaceToList( 0, *mark, mins, maxs, list, listbmodel, listsize, listlength, dir );

		mark++;
	}
}

/*
=================
R_AddMarkFragments

=================
*/
void R_AddMarkFragments(int numClipPoints, vec3_t clipPoints[2][MAX_VERTS_ON_POLY],
				   int numPlanes, vec3_t *normals, float *dists,
				   int maxPoints, vec3_t pointBuffer,
				   int maxFragments, markFragment_t *fragmentBuffer,
				   int *returnedPoints, int *returnedFragments,
				   vec3_t mins, vec3_t maxs, int bmodelNum, vec3_t projectionDir) {
	int pingPong, i;
	markFragment_t	*mf;

	// chop the surface by all the bounding planes of the to be projected polygon
	pingPong = 0;

	for ( i = 0 ; i < numPlanes ; i++ ) {

		R_ChopPolyBehindPlane( numClipPoints, clipPoints[pingPong],
						   &numClipPoints, clipPoints[!pingPong],
							normals[i], dists[i], 0.5 );
		pingPong ^= 1;
		if ( numClipPoints == 0 ) {
			break;
		}
	}
	// completely clipped away?
	if ( numClipPoints == 0 ) {
		return;
	}

	// add this fragment to the returned list
	if ( numClipPoints + (*returnedPoints) > maxPoints ) {
		return;	// not enough space for this polygon
	}
	/*
	// all the clip points should be within the bounding box
	for ( i = 0 ; i < numClipPoints ; i++ ) {
		int j;
		for ( j = 0 ; j < 3 ; j++ ) {
			if (clipPoints[pingPong][i][j] < mins[j] - 0.5) break;
			if (clipPoints[pingPong][i][j] > maxs[j] + 0.5) break;
		}
		if (j < 3) break;
	}
	if (i < numClipPoints) return;
	*/

	mf = fragmentBuffer + (*returnedFragments);
	mf->firstPoint = (*returnedPoints);
	mf->numPoints = numClipPoints;
	mf->bmodelNum = bmodelNum;
	R_GetBmodelInfo( bmodelNum, NULL, mf->bmodelOrigin, mf->bmodelAxis );
	VectorCopy( projectionDir, mf->projectionDir );

	Com_Memcpy( pointBuffer + (*returnedPoints) * 3, clipPoints[pingPong], numClipPoints * sizeof(vec3_t) );

	(*returnedPoints) += numClipPoints;
	(*returnedFragments)++;
}

/*
=================
R_MarkFragments

=================
*/
int R_MarkFragments( int numPoints, const vec3_t *points, const vec3_t projection,
				   int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer ) {
	int				numsurfaces, numPlanes;
	int				i, j, k, m, n;
	surfaceType_t	*surfaces[64];
	int				surfacesBmodel[64];
	int				lastBmodel;
	vec3_t			mins, maxs;
	int				returnedFragments;
	int				returnedPoints;
	vec3_t			normals[MAX_VERTS_ON_POLY+2], localNormals[MAX_VERTS_ON_POLY+2];
	float			dists[MAX_VERTS_ON_POLY+2], localDists[MAX_VERTS_ON_POLY+2];
	vec3_t			clipPoints[2][MAX_VERTS_ON_POLY];
	int				numClipPoints;
	float			*v;
	srfBspSurface_t	*cv;
	glIndex_t		*tri;
	srfVert_t		*dv;
	vec3_t			normal;
	vec3_t			projectionDir, localProjectionDir;
	vec3_t			v1, v2;

	if (numPoints <= 0) {
		return 0;
	}

	//increment view count for double check prevention
	tr.viewCount++;

	//
	VectorNormalize2( projection, projectionDir );
	// find all the brushes that are to be considered
	ClearBounds( mins, maxs );
	for ( i = 0 ; i < numPoints ; i++ ) {
		vec3_t	temp;

		AddPointToBounds( points[i], mins, maxs );
		VectorAdd( points[i], projection, temp );
		AddPointToBounds( temp, mins, maxs );
		// make sure we get all the leafs (also the one(s) in front of the hit surface)
		VectorMA( points[i], -20, projectionDir, temp );
		AddPointToBounds( temp, mins, maxs );
	}

	if (numPoints > MAX_VERTS_ON_POLY) numPoints = MAX_VERTS_ON_POLY;
	// create the bounding planes for the to be projected polygon
	for ( i = 0 ; i < numPoints ; i++ ) {
		VectorSubtract(points[(i+1)%numPoints], points[i], v1);
		VectorAdd(points[i], projection, v2);
		VectorSubtract(points[i], v2, v2);
		CrossProduct(v1, v2, normals[i]);
		VectorNormalizeFast(normals[i]);
		dists[i] = DotProduct(normals[i], points[i]);
	}
	// add near and far clipping planes for projection
	VectorCopy(projectionDir, normals[numPoints]);
	dists[numPoints] = DotProduct(normals[numPoints], points[0]) - 32;
	VectorCopy(projectionDir, normals[numPoints+1]);
	VectorInverse(normals[numPoints+1]);
	dists[numPoints+1] = DotProduct(normals[numPoints+1], points[0]) - 20;
	numPlanes = numPoints + 2;

	numsurfaces = 0;
	R_BoxSurfaces_r(tr.world->nodes, mins, maxs, surfaces, surfacesBmodel, 64, &numsurfaces, projectionDir);
	//assert(numsurfaces <= 64);
	//assert(numsurfaces != 64);

	if (r_marksOnBrushModels->integer) {
		// add bmodel surfaces
		for ( j = 1; j < tr.world->numBModels; j++ ) {
			vec3_t localProjection, bmodelOrigin, bmodelAxis[3];

			R_TransformMarkProjection( j, projection, localProjection, 0, NULL, NULL, NULL, NULL );
			R_GetBmodelInfo( j, NULL, bmodelOrigin, bmodelAxis );

			VectorNormalize2( localProjection, localProjectionDir );
			// find all the brushes that are to be considered
			ClearBounds( mins, maxs );
			for ( i = 0 ; i < numPoints ; i++ ) {
				vec3_t	temp;
				vec3_t	delta;
				vec3_t	localPoint;

				// convert point to bmodel local space
				VectorSubtract( points[i], bmodelOrigin, delta );
				localPoint[0] = DotProduct( delta, bmodelAxis[0] );
				localPoint[1] = DotProduct( delta, bmodelAxis[1] );
				localPoint[2] = DotProduct( delta, bmodelAxis[2] );

				AddPointToBounds( localPoint, mins, maxs );
				VectorAdd( localPoint, localProjection, temp );
				AddPointToBounds( temp, mins, maxs );
				// make sure we get all the leafs (also the one(s) in front of the hit surface)
				VectorMA( localPoint, -20, localProjectionDir, temp );
				AddPointToBounds( temp, mins, maxs );
			}

			R_BmodelSurfaces( j, mins, maxs, surfaces, surfacesBmodel, 64, &numsurfaces, localProjectionDir);
		}
	}

	returnedPoints = 0;
	returnedFragments = 0;
	lastBmodel = -1;

	for ( i = 0 ; i < numsurfaces ; i++ ) {
		if (i == 0 || surfacesBmodel[i] != lastBmodel) {
			R_TransformMarkProjection( surfacesBmodel[i], projectionDir, localProjectionDir, numPlanes, normals, dists, localNormals, localDists );
			lastBmodel = surfacesBmodel[i];

			// don't use projectionDir, normals, or dists beyond this point !!!
			// mins and maxs are not setup, so they are not valid !!!
		}

		if (*surfaces[i] == SF_GRID) {

			cv = (srfBspSurface_t *) surfaces[i];
			for ( m = 0 ; m < cv->height - 1 ; m++ ) {
				for ( n = 0 ; n < cv->width - 1 ; n++ ) {
					// We triangulate the grid and chop all triangles within
					// the bounding planes of the to be projected polygon.
					// LOD is not taken into account, not such a big deal though.
					//
					// It's probably much nicer to chop the grid itself and deal
					// with this grid as a normal SF_GRID surface so LOD will
					// be applied. However the LOD of that chopped grid must
					// be synced with the LOD of the original curve.
					// One way to do this; the chopped grid shares vertices with
					// the original curve. When LOD is applied to the original
					// curve the unused vertices are flagged. Now the chopped curve
					// should skip the flagged vertices. This still leaves the
					// problems with the vertices at the chopped grid edges.
					//
					// To avoid issues when LOD applied to "hollow curves" (like
					// the ones around many jump pads) we now just add a 2 unit
					// offset to the triangle vertices.
					// The offset is added in the vertex normal vector direction
					// so all triangles will still fit together.
					// The 2 unit offset should avoid pretty much all LOD problems.
					vec3_t fNormal;

					numClipPoints = 3;

					dv = cv->verts + m * cv->width + n;

					VectorCopy(dv[0].xyz, clipPoints[0][0]);
					R_VaoUnpackNormal(fNormal, dv[0].normal);
					VectorMA(clipPoints[0][0], MARKER_OFFSET, fNormal, clipPoints[0][0]);
					VectorCopy(dv[cv->width].xyz, clipPoints[0][1]);
					R_VaoUnpackNormal(fNormal, dv[cv->width].normal);
					VectorMA(clipPoints[0][1], MARKER_OFFSET, fNormal, clipPoints[0][1]);
					VectorCopy(dv[1].xyz, clipPoints[0][2]);
					R_VaoUnpackNormal(fNormal, dv[1].normal);
					VectorMA(clipPoints[0][2], MARKER_OFFSET, fNormal, clipPoints[0][2]);
					// check the normal of this triangle
					VectorSubtract(clipPoints[0][0], clipPoints[0][1], v1);
					VectorSubtract(clipPoints[0][2], clipPoints[0][1], v2);
					CrossProduct(v1, v2, normal);
					VectorNormalizeFast(normal);
					if (DotProduct(normal, localProjectionDir) < -0.1) {
						// add the fragments of this triangle
						R_AddMarkFragments(numClipPoints, clipPoints,
										   numPlanes, localNormals, localDists,
										   maxPoints, pointBuffer,
										   maxFragments, fragmentBuffer,
										   &returnedPoints, &returnedFragments, mins, maxs, lastBmodel, localProjectionDir);

						if ( returnedFragments == maxFragments ) {
							return returnedFragments;	// not enough space for more fragments
						}
					}

					VectorCopy(dv[1].xyz, clipPoints[0][0]);
					R_VaoUnpackNormal(fNormal, dv[1].normal);
					VectorMA(clipPoints[0][0], MARKER_OFFSET, fNormal, clipPoints[0][0]);
					VectorCopy(dv[cv->width].xyz, clipPoints[0][1]);
					R_VaoUnpackNormal(fNormal, dv[cv->width].normal);
					VectorMA(clipPoints[0][1], MARKER_OFFSET, fNormal, clipPoints[0][1]);
					VectorCopy(dv[cv->width+1].xyz, clipPoints[0][2]);
					R_VaoUnpackNormal(fNormal, dv[cv->width + 1].normal);
					VectorMA(clipPoints[0][2], MARKER_OFFSET, fNormal, clipPoints[0][2]);
					// check the normal of this triangle
					VectorSubtract(clipPoints[0][0], clipPoints[0][1], v1);
					VectorSubtract(clipPoints[0][2], clipPoints[0][1], v2);
					CrossProduct(v1, v2, normal);
					VectorNormalizeFast(normal);
					if (DotProduct(normal, localProjectionDir) < -0.05) {
						// add the fragments of this triangle
						R_AddMarkFragments(numClipPoints, clipPoints,
										   numPlanes, localNormals, localDists,
										   maxPoints, pointBuffer,
										   maxFragments, fragmentBuffer,
										   &returnedPoints, &returnedFragments, mins, maxs, lastBmodel, localProjectionDir);

						if ( returnedFragments == maxFragments ) {
							return returnedFragments;	// not enough space for more fragments
						}
					}
				}
			}
		}
		else if (*surfaces[i] == SF_FACE) {

			srfBspSurface_t *surf = ( srfBspSurface_t * ) surfaces[i];

			// check the normal of this face
			if (DotProduct(surf->cullPlane.normal, localProjectionDir) > -0.5) {
				continue;
			}

			for(k = 0, tri = surf->indexes; k < surf->numIndexes; k += 3, tri += 3)
			{
				for(j = 0; j < 3; j++)
				{
					v = surf->verts[tri[j]].xyz;
					VectorMA(v, MARKER_OFFSET, surf->cullPlane.normal, clipPoints[0][j]);
				}

				// add the fragments of this face
				R_AddMarkFragments( 3 , clipPoints,
								   numPlanes, localNormals, localDists,
								   maxPoints, pointBuffer,
								   maxFragments, fragmentBuffer,
								   &returnedPoints, &returnedFragments, mins, maxs, lastBmodel, localProjectionDir);
				if ( returnedFragments == maxFragments ) {
					return returnedFragments;	// not enough space for more fragments
				}
			}
		}
		else if(*surfaces[i] == SF_TRIANGLES && r_marksOnTriangleMeshes->integer) {

			srfBspSurface_t *surf = (srfBspSurface_t *) surfaces[i];

			for(k = 0, tri = surf->indexes; k < surf->numIndexes; k += 3, tri += 3)
			{
				for(j = 0; j < 3; j++)
				{
					vec3_t fNormal;
					v = surf->verts[tri[j]].xyz;
					R_VaoUnpackNormal(fNormal, surf->verts[tri[j]].normal);
					VectorMA(v, MARKER_OFFSET, fNormal, clipPoints[0][j]);
				}

				// add the fragments of this face
				R_AddMarkFragments(3, clipPoints,
								   numPlanes, localNormals, localDists,
								   maxPoints, pointBuffer,
								   maxFragments, fragmentBuffer, &returnedPoints, &returnedFragments, mins, maxs, lastBmodel, localProjectionDir);
				if(returnedFragments == maxFragments)
				{
					return returnedFragments;	// not enough space for more fragments
				}
			}
		}
	}
	return returnedFragments;
}





