#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <bit>
#include <string>
#include <vector>
#include "KeyValue.h"

#include <boost/filesystem.hpp>
//#include <filesystem>
#include <iterator>

using namespace std;
using namespace boost::filesystem;
//using namespace filesystem;


#define HEADER_LUMPS 64

#define LUMP_ENTITIES 0
#define LUMP_PLANES 1
#define LUMP_TEXDATA 2
#define LUMP_VERTEXES 3
#define LUMP_VISIBILITY 4
#define LUMP_NODES 5
#define LUMP_TEXINFO 6
#define LUMP_FACES 7
#define LUMP_LIGHTING 8
#define LUMP_OCCLUSION 9
#define LUMP_LEAFS 10
#define LUMP_FACEIDS 11
#define LUMP_EDGES 12
#define LUMP_SURFEDGES 13
#define LUMP_MODELS 14
#define LUMP_WORLDLIGHTS 15
#define LUMP_LEAFFACES 16
#define LUMP_LEAFBRUSHES 17
#define LUMP_BRUSHES 18
#define LUMP_BRUSHSIDES 19
#define LUMP_AREAS 20
#define LUMP_AREAPORTALS 21
#define LUMP_UNUSED0 22
#define LUMP_UNUSED1 23
#define LUMP_UNUSED2 24
#define LUMP_UNUSED3 25
#define LUMP_DISPINFO 26
#define LUMP_ORIGINALFACES 27
#define LUMP_PHYSDISP 28
#define LUMP_PHYSCOLLIDE 29
#define LUMP_VERTNORMALS 30
#define LUMP_VERTNORMALINDICES 31
#define LUMP_DISP_LIGHTMAP_ALPHAS 32
#define LUMP_DISP_VERTS 33
#define LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS 34
#define LUMP_GAME_LUMP 35
#define LUMP_LEAFWATERDATA 36
#define LUMP_PRIMITIVES 37
#define LUMP_PRIMVERTS 38
#define LUMP_PRIMINDICES 39
#define LUMP_PAKFILE 40
#define LUMP_CLIPPORTALVERTS 41
#define LUMP_CUBEMAPS 42
#define LUMP_TEXDATA_STRING_DATA 43
#define LUMP_TEXDATA_STRING_TABLE 44
#define LUMP_OVERLAYS 45
#define LUMP_LEAFMINDISTTOWATER 46
#define LUMP_FACE_MACRO_TEXTURE_INFO 47
#define LUMP_DISP_TRIS 48
#define LUMP_PHYSCOLLIDESURFACE 49
#define LUMP_WATEROVERLAYS 50
#define LUMP_LEAF_AMBIENT_INDEX_HDR 51
#define LUMP_LEAF_AMBIENT_INDEX 52
#define LUMP_LIGHTING_HDR 53
#define LUMP_WORLDLIGHTS_HDR 54
#define LUMP_LEAF_AMBIENT_LIGHTING_HDR 55
#define LUMP_LEAF_AMBIENT_LIGHTING 56
#define LUMP_XZIPPAKFILE 57
#define LUMP_FACES_HDR 58
#define LUMP_MAP_FLAGS 59
#define LUMP_OVERLAY_FADES 60

struct color32 {
	char r, g, b, a;
};

struct Vector {
	float x, y, z;
};

struct dplane_t {
	Vector	normal;	// normal vector
	float	dist;	// distance from origin
	int		type;	// plane axis identifier
};

struct dtexdata_t {
	Vector	reflectivity;		// RGB reflectivity
	int		nameStringTableID;	// index into TexdataStringTable
	int		width, height;		// source image
	int		view_width, view_height;
};

struct texinfo_t {
	float	textureVecs[2][4];	// [s/t][xyz offset]
	float	lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
	int		flags;				// miptex flags overrides
	int		texdata;			// Pointer to texture name, size, etc.
};

struct dface_t {
	unsigned short	planenum;						// the plane number
	byte			side;							// faces opposite to the node's plane direction
	byte			onNode;							// 1 of on node, 0 if in leaf
	int				firstedge;						// index into surfedges
	short			numedges;						// number of surfedges
	short			texinfo;						// texture info
	short			dispinfo;						// displacement info
	short			surfaceFogVolumeID;				// ?
	byte			styles[4];						// switchable lighting info
	int				lightofs;						// offset into lightmap lump
	float			area;							// face area in units^2
	int             LightmapTextureMinsInLuxels[2];	// texture lighting info
	int             LightmapTextureSizeInLuxels[2];	// texture lighting info
	int				origFace;						// original face this was split from
	unsigned short	numPrims;						// primitives
	unsigned short	firstPrimID;
	unsigned int	smoothingGroups;				// lightmap smoothing group
};

struct dgamelump_t {
	int				id;
	unsigned short	flags;
	unsigned short	version;
	int				fileofs;
	int				filelen;
};

struct dgamelumpheader_t {
	int lumpCount;
	dgamelump_t* gamelump;
};

struct StaticPropDictLump_t {
	int dictEntries;
	char** name;
};

struct StaticPropLeafLump_t {
	int leafEntries;
	unsigned short* leaf;
};

struct StaticPropLump_t {
	Vector			Origin;
	Vector			Angles;

	unsigned short	PropType;
	unsigned short	FirstLeaf;
	unsigned short	LeafCount;
	unsigned char	Solid;
	unsigned char	Flags;
	int				Skin;
	float			FadeMinDist;
	float			FadeMaxDist;
	Vector			LightingOrigin;
	float			ForcedFadeScale;
	unsigned short	MinDXLevel;
	unsigned short	MaxDXLevel;
	unsigned int	FlagsV7;
	unsigned short	LightmapResX;
	unsigned short	LightmapResY;
	unsigned char	MinCPULevel;
	unsigned char	MaxCPULevel;
	unsigned char	MinGPULevel;
	unsigned char	MaxGPULevel;
	color32			DiffuseModulation;
	bool			DisableX360;
	unsigned int	FlagsEx;
	float			UniformScale;
};

struct StaticPropLumps_t {
	int lumpEntries;
	StaticPropLump_t* prop;
};

struct lump_t {
	int		fileofs;	// offset into file (bytes)
	int		filelen;	// length of lump (bytes)
	int		version;	// lump format version
	char	fourCC[4];	// lump ident code
};

struct dheader_t {
	int		ident;					// BSP file identifier
	int		version;				// BSP file version
	lump_t	lumps[HEADER_LUMPS];	// lump directory array
	int		mapRevision;			// the map's revision (iteration, version) number
};

struct bsp_t {
	dheader_t header;
	char* entityLump;
	dplane_t* planeLump;
	dtexdata_t* texDataLump;
	Vector* vertexLump;
	texinfo_t* texInfoLump;
	dface_t* faceLump;
	dgamelumpheader_t gameLump;
	StaticPropDictLump_t staticPropDictLump;
	StaticPropLeafLump_t staticPropLeafLump;
	StaticPropLumps_t staticPropLumps;
	int staticPropLumpIndex;
	int* texDataStringTableLump;
	char* texDataStringDataLump;
};

struct studiohdr_t {
	int			id;
	int			version;
	int			checksum;
	char		name[64];

	int			dataLength;

	Vector		eyeposition;
	Vector		ilumposition;
	Vector		hull_min;
	Vector		hull_max;
	Vector		view_bbmin;
	Vector		view_bbmax;

	int			flags;

	int			bone_count;
	int			bone_offset;

	int			bonecontroller_count;
	int			bonecontroller_offset;

	int			hitbox_count;
	int			hitbox_offset;

	int			localanim_count;
	int			localanim_offset;

	int			localseq_count;
	int			localseq_offset;

	int			activitylistversion;
	int			eventsindexed;

	int			texture_count;
	int			texture_offset;

	int			texturedir_count;
	int			texturedir_offset;

	int			skinreference_count;
	int			skinrfamily_count;
	int			skinreference_index;

	int			bodypart_count;
	int			bodypart_offset;

	int			attachment_count;
	int			attachment_offset;

	int			localnode_count;
	int			localnode_index;
	int			localnode_name_index;

	int			flexdesc_count;
	int			flexdesc_index;

	int			flexcontroller_count;
	int			flexcontroller_index;

	int			flexrules_count;
	int			flexrules_index;

	int			ikchain_count;
	int			ikchain_index;

	int			mouths_count;
	int			mouths_index;

	int			localposeparam_count;
	int			localposeparam_index;

	int			surfaceprop_index;

	int			keyvalue_index;
	int			keyvalue_count;

	int			iklock_count;
	int			iklock_index;

	float		mass;

	int			contents;

	int			includemodel_count;
	int			includemodel_index;

	int			virtualModel;

	int			animblocks_name_index;
	int			animblocks_count;
	int			animblocks_index;

	int			animblockModel;

	int			bonetablename_index;

	int			vertex_base;
	int			offset_base;

	byte		directionaldotproduct;

	byte		rootLod;

	byte		numAllowedRootLods;

	byte		unused0;
	int			unused1;

	int			flexcontrollerui_count;
	int			flexcontrollerui_index;

	float		vertAnimFixedPointScale;
	int			unused2;

	int			studiohdr2index;

	int			unused3;
};

struct mstudiotexture_t
{
	// Number of bytes past the beginning of this structure
	// where the first character of the texture name can be found.
	int    name_offset; // Offset for null-terminated string
	int    flags;

	int    used;        // Padding?
	int    unused;      // Padding.

	int    material;        // Placeholder for IMaterial
	int    client_material; // Placeholder for void*

	int    unused2[10]; // Final padding
	// Struct is 64 bytes long
};

struct mdl_t {
	studiohdr_t header;
	mstudiotexture_t* textureData;
	string* textureNameData;
	string textureDir;
};

class file {
public:
	file();
protected:
	unsigned int readUnsignedInt();
	short readShort();
	unsigned short readUnsignedShort();
	int readInt();
	char readChar();
	float readFloat();
	Vector readVector();

	char* memblock;
	unsigned int pointer;
};

class bsp: private file {
public:
	bsp(char* _memblock);
	bsp_t bspStruct;
private:
	dheader_t readHeader();
	lump_t readLump();
	dplane_t readPlane();
	dtexdata_t readTexData();
	texinfo_t readTexInfo();
	dface_t readFace();

	char* readEntityLump();
	dplane_t* readPlaneLump();
	dtexdata_t* readTexDataLump();
	Vector* readVertexLump();
	texinfo_t* readTexInfoLump();
	dface_t* readFaceLump();
	dgamelumpheader_t readGameLump();
	StaticPropDictLump_t readStaticPropDictLump();
	StaticPropLeafLump_t readStaticPropLeafLump();
	StaticPropLumps_t readStaticPropLumps();
	int* readTexDataStringTableLump();
	char* readTexDataStringDataLump();
};

class mdl : private file {
public:
	mdl(char* _memblock);
	mdl_t mdlStruct;
private:
	studiohdr_t readHeader();
	mstudiotexture_t* readTextureData();
};