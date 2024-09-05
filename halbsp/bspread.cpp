#include "halbsp.h"

lump_t bsp::readLump() {
	lump_t lump;

	lump.fileofs = readInt();
	lump.filelen = readInt();
	lump.version = readInt();

	for (int i = 0; i < 4; i++) {
		lump.fourCC[i] = readChar();
	}

	return lump;
}

dheader_t bsp::readHeader() {
	dheader_t header;
	header.ident = readInt();
	header.version = readInt();

	for (int i = 0; i < HEADER_LUMPS; i++) {
		header.lumps[i] = readLump();
	}

	header.mapRevision = readInt();

	return header;
}

dplane_t bsp::readPlane() {
	dplane_t plane;
	plane.normal = readVector();
	plane.dist = readFloat();
	plane.type = readInt();
	return plane;
}

dtexdata_t bsp::readTexData() {
	dtexdata_t texdata;
	texdata.reflectivity = readVector();
	texdata.nameStringTableID = readInt();
	texdata.width = readInt();
	texdata.height = readInt();
	texdata.view_width = readInt();
	texdata.view_height = readInt();
	return texdata;
}

dface_t bsp::readFace() {
	dface_t face;
	face.planenum = readUnsignedShort();
	face.side = (byte) readChar();
	face.onNode = (byte) readChar();
	face.firstedge = readInt();
	face.numedges = readShort();
	face.texinfo = readShort();
	face.dispinfo = readShort();
	face.surfaceFogVolumeID = readShort();
	for (int i = 0; i < 4; i++) {
		face.styles[i] = (byte) readChar();
	}
	face.lightofs = readInt();
	face.area = readFloat();
	face.LightmapTextureMinsInLuxels[0] = readInt();
	face.LightmapTextureMinsInLuxels[1] = readInt();
	face.LightmapTextureSizeInLuxels[0] = readInt();
	face.LightmapTextureSizeInLuxels[1] = readInt();
	face.origFace = readInt();
	face.numPrims = readUnsignedShort();
	face.firstPrimID = readUnsignedShort();
	face.smoothingGroups = readUnsignedInt();
	return face;
}

texinfo_t bsp::readTexInfo() {
	texinfo_t texinfo;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			texinfo.textureVecs[i][j] = readFloat();
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			texinfo.lightmapVecs[i][j] = readFloat();
		}
	}
	texinfo.flags = readInt();
	texinfo.texdata = readInt();
	return texinfo;
}

char* bsp::readEntityLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_ENTITIES].filelen;
	char* entityLump = new char [lumpLength];
	copy(memblock + pointer, memblock + pointer + lumpLength, entityLump);
	return entityLump;
}

dplane_t* bsp::readPlaneLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_PLANES].filelen/20;
	dplane_t* planeLump = new dplane_t[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		planeLump[i] = readPlane();
	}
	return planeLump;
}

dtexdata_t* bsp::readTexDataLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_TEXDATA].filelen / 32;
	dtexdata_t* texDataLump = new dtexdata_t[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		texDataLump[i] = readTexData();
	}
	return texDataLump;
}

Vector* bsp::readVertexLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_VERTEXES].filelen / 12;
	Vector* vertexLump = new Vector[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		vertexLump[i] = readVector();
	}
	return vertexLump;
}

texinfo_t* bsp::readTexInfoLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_TEXDATA].filelen / 72;
	texinfo_t* texInfoLump = new texinfo_t[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		texInfoLump[i] = readTexInfo();
	}
	return texInfoLump;
}

dface_t* bsp::readFaceLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_FACES].filelen / 56;
	dface_t* faceLump = new dface_t[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		faceLump[i] = readFace();
	}
	return faceLump;
}

dgamelumpheader_t bsp::readGameLump() {
	dgamelumpheader_t gameLump;
	gameLump.lumpCount = readInt();
	gameLump.gamelump = new dgamelump_t[gameLump.lumpCount];
	for (int i = 0; i < gameLump.lumpCount; i++) {
		gameLump.gamelump[i].id = readInt();
		gameLump.gamelump[i].flags = readUnsignedShort();
		gameLump.gamelump[i].version = readUnsignedShort();
		gameLump.gamelump[i].fileofs = readInt();
		gameLump.gamelump[i].filelen = readInt();
	}
	return gameLump;
}

StaticPropDictLump_t bsp::readStaticPropDictLump() {
	StaticPropDictLump_t staticPropDictLump;
	staticPropDictLump.dictEntries = readInt();
	staticPropDictLump.name = new char*[staticPropDictLump.dictEntries];
	for (int i = 0; i < staticPropDictLump.dictEntries; i++) {
		staticPropDictLump.name[i] = new char[128];
		for (int j = 0; j < 128; j++) {
			staticPropDictLump.name[i][j] = readChar();
		}
	}
	return staticPropDictLump;
}

StaticPropLeafLump_t bsp::readStaticPropLeafLump() {
	StaticPropLeafLump_t staticPropLeafLump;
	staticPropLeafLump.leafEntries = readInt();
	staticPropLeafLump.leaf = new unsigned short [staticPropLeafLump.leafEntries];
	for (int i = 0; i < staticPropLeafLump.leafEntries; i++) {
		staticPropLeafLump.leaf[i] = readUnsignedShort();
	}
	return staticPropLeafLump;
}

StaticPropLumps_t bsp::readStaticPropLumps() {
	int lumpLength = readInt();
	unsigned short version = bspStruct.gameLump.gamelump[bspStruct.staticPropLumpIndex].version;

	StaticPropLump_t* staticPropLumps = new StaticPropLump_t[lumpLength];
	for (int i = 0; i < lumpLength; i++) {
		staticPropLumps[i].Origin = readVector();
		staticPropLumps[i].Angles = readVector();

		staticPropLumps[i].PropType = readUnsignedShort();
		staticPropLumps[i].FirstLeaf = readUnsignedShort();
		staticPropLumps[i].LeafCount = readUnsignedShort();
		staticPropLumps[i].Solid = bit_cast<unsigned char>(readChar());

		staticPropLumps[i].Flags = bit_cast<unsigned char>(readChar());

		staticPropLumps[i].Skin = readInt();
		staticPropLumps[i].FadeMinDist = readFloat();
		staticPropLumps[i].FadeMaxDist = readFloat();
		staticPropLumps[i].LightingOrigin = readVector();

		if (version < 5) continue;
		staticPropLumps[i].ForcedFadeScale = readFloat();

		if (version >= 6 && version <= 7) {
			staticPropLumps[i].MinDXLevel = readUnsignedShort();
			staticPropLumps[i].MaxDXLevel = readUnsignedShort();
		}
		
	}

	StaticPropLumps_t staticPropLumpsStruct;
	staticPropLumpsStruct.lumpEntries = lumpLength;
	staticPropLumpsStruct.prop = staticPropLumps;

	return staticPropLumpsStruct;
}

int* bsp::readTexDataStringTableLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_TEXDATA_STRING_TABLE].filelen / 4;
	int* texDataStringTable = new int[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		texDataStringTable[i] = readInt();
	}
	return texDataStringTable;
}

char* bsp::readTexDataStringDataLump() {
	int lumpLength = bspStruct.header.lumps[LUMP_TEXDATA_STRING_DATA].filelen;
	char* texDataStringData = new char[lumpLength];
	//cout << "Plane Lump Count: " << lumpLength << "\n";
	for (int i = 0; i < lumpLength; i++) {
		texDataStringData[i] = readChar();
	}
	return texDataStringData;
}

bsp::bsp(char* _memblock) {
	memblock = _memblock;
	pointer = 0;
	bspStruct.header = readHeader();

	pointer = bspStruct.header.lumps[LUMP_ENTITIES].fileofs;
	bspStruct.entityLump = readEntityLump();

	pointer = bspStruct.header.lumps[LUMP_PLANES].fileofs;
	bspStruct.planeLump = readPlaneLump();

	pointer = bspStruct.header.lumps[LUMP_TEXDATA].fileofs;
	bspStruct.texDataLump = readTexDataLump();

	pointer = bspStruct.header.lumps[LUMP_VERTEXES].fileofs;
	bspStruct.vertexLump = readVertexLump();

	pointer = bspStruct.header.lumps[LUMP_TEXINFO].fileofs;
	bspStruct.texInfoLump = readTexInfoLump();

	pointer = bspStruct.header.lumps[LUMP_FACES].fileofs;
	bspStruct.faceLump = readFaceLump();

	pointer = bspStruct.header.lumps[LUMP_GAME_LUMP].fileofs;
	bspStruct.gameLump = readGameLump();

	for (int i = 0; i < bspStruct.gameLump.lumpCount; i++) {
		if (bspStruct.gameLump.gamelump[i].id == 1936749168) {
			bspStruct.staticPropLumpIndex = i;
			pointer = bspStruct.gameLump.gamelump[bspStruct.staticPropLumpIndex].fileofs;
			bspStruct.staticPropDictLump = readStaticPropDictLump();
			bspStruct.staticPropLeafLump = readStaticPropLeafLump();
			bspStruct.staticPropLumps = readStaticPropLumps();
		}
	}

	pointer = bspStruct.header.lumps[LUMP_TEXDATA_STRING_TABLE].fileofs;
	bspStruct.texDataStringTableLump = readTexDataStringTableLump();

	pointer = bspStruct.header.lumps[LUMP_TEXDATA_STRING_DATA].fileofs;
	bspStruct.texDataStringDataLump = readTexDataStringDataLump();
}