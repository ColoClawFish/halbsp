#include "halbsp.h"

studiohdr_t mdl::readHeader() {
	studiohdr_t header;
	
	header.id = readInt();
	header.version = readInt();
	header.checksum = readInt();
	for (int i = 0; i < 64; i++) {
		header.name[i] = readChar();
	}

	header.dataLength = readInt();

	header.eyeposition = readVector();
	header.ilumposition = readVector();
	header.hull_min = readVector();
	header.hull_max = readVector();
	header.view_bbmin = readVector();
	header.view_bbmax = readVector();

	header.flags = readInt();

	header.bone_count = readInt();
	header.bone_offset = readInt();

	header.bonecontroller_count = readInt();
	header.bonecontroller_offset = readInt();

	header.hitbox_count = readInt();
	header.hitbox_offset = readInt();

	header.localanim_count = readInt();
	header.localanim_offset = readInt();

	header.localseq_count = readInt();
	header.localseq_offset = readInt();

	header.activitylistversion = readInt();
	header.eventsindexed = readInt();

	header.texture_count = readInt();
	header.texture_offset = readInt();

	header.texturedir_count = readInt();
	header.texturedir_offset = readInt();

	header.skinreference_count = readInt();
	header.skinrfamily_count = readInt();
	header.skinreference_index = readInt();

	header.bodypart_count = readInt();
	header.bodypart_offset = readInt();

	header.attachment_count = readInt();
	header.attachment_offset = readInt();

	header.localnode_count = readInt();
	header.localnode_index = readInt();
	header.localnode_name_index = readInt();

	header.flexdesc_count = readInt();
	header.flexdesc_index = readInt();

	header.flexcontroller_count = readInt();
	header.flexcontroller_index = readInt();

	header.flexrules_count = readInt();
	header.flexrules_index = readInt();

	header.ikchain_count = readInt();
	header.ikchain_index = readInt();

	header.mouths_count = readInt();
	header.mouths_index = readInt();

	header.localposeparam_count = readInt();
	header.localposeparam_index = readInt();

	header.surfaceprop_index = readInt();

	header.keyvalue_index = readInt();
	header.keyvalue_count = readInt();

	header.iklock_count = readInt();
	header.iklock_index = readInt();

	header.mass = readFloat();

	header.contents = readInt();

	header.includemodel_count = readInt();
	header.includemodel_index = readInt();

	header.virtualModel = readInt();

	header.animblocks_name_index = readInt();
	header.animblocks_count = readInt();
	header.animblocks_index = readInt();

	header.animblockModel = readInt();

	header.bonetablename_index = readInt();

	header.vertex_base = readInt();
	header.offset_base = readInt();

	header.directionaldotproduct = (byte)readChar();

	header.rootLod = (byte)readChar();

	header.numAllowedRootLods = (byte)readChar();

	header.unused0 = (byte)readChar();
	header.unused1 = readInt();

	header.flexcontrollerui_count = readInt();
	header.flexcontrollerui_index = readInt();

	header.vertAnimFixedPointScale = readFloat();
	header.unused2 = readInt();

	header.studiohdr2index = readInt();
	header.unused3 = readInt();

	return header;
}

mstudiotexture_t* mdl::readTextureData() {
	int texDataLength = mdlStruct.header.texture_count;
	mstudiotexture_t* textureTable = new mstudiotexture_t[texDataLength];
	for (int i = 0; i < texDataLength; i++) {
		textureTable[i].name_offset = readInt();
		textureTable[i].flags = readInt();

		textureTable[i].used = readInt();
		textureTable[i].unused = readInt();

		textureTable[i].material = readInt();
		textureTable[i].client_material = readInt();

		for (int j = 0; j < 10; j++) {
			textureTable[i].unused2[j] = readInt();
		}
	}
	return textureTable;
}

mdl::mdl(char* _memblock) {
	memblock = _memblock;
	pointer = 0;
	mdlStruct.header = readHeader();

	pointer = mdlStruct.header.texture_offset;
	mdlStruct.textureData = readTextureData();

	mdlStruct.textureNameData = new string [mdlStruct.header.texture_count];
	for (int i = 0; i < mdlStruct.header.texture_count; i++) {
		pointer = mdlStruct.textureData[i].name_offset + mdlStruct.header.texture_offset + i * 64;
		//cout << pointer << "\n";
		string name;
		char inChar = readChar();
		while (inChar != 0) {
			name += inChar;
			inChar = readChar();
		}
		mdlStruct.textureNameData[i] = name;
	}

	string texDir;
	char inChar = readChar();
	while (inChar != 0) {
		texDir += inChar;
		inChar = readChar();
	}
	mdlStruct.textureDir = texDir;
}