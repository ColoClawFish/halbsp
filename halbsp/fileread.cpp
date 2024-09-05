#include "halbsp.h"

int file::readInt() {
	int result = 0;
	for (int i = 3; i >= 0; i--) {
		result = result << 8 | static_cast<unsigned char>(memblock[pointer + i]);
	}
	pointer += 4;
	return result;
}

unsigned int file::readUnsignedInt() {
	unsigned int result = 0;
	for (int i = 3; i >= 0; i--) {
		result = result << 8 | static_cast<unsigned char>(memblock[pointer + i]);
	}
	pointer += 4;
	return result;
}

short file::readShort() {
	short result = 0;
	for (int i = 1; i >= 0; i--) {
		result = result << 8 | static_cast<unsigned char>(memblock[pointer + i]);
	}
	pointer += 2;
	return result;
}

unsigned short file::readUnsignedShort() {
	unsigned short result = 0;
	for (int i = 1; i >= 0; i--) {
		result = result << 8 | static_cast<unsigned char>(memblock[pointer + i]);
	}
	pointer += 2;
	return result;
}

char file::readChar() {
	pointer++;
	return memblock[pointer - 1];
}

float file::readFloat() {
	uint32_t result = 0;
	for (int i = 3; i >= 0; i--) {
		result = result << 8 | static_cast<unsigned char>(memblock[pointer + i]);
	}
	pointer += 4;
	return bit_cast<float>(result);
}

Vector file::readVector() {
	Vector vector;
	vector.x = readFloat();
	vector.y = readFloat();
	vector.z = readFloat();
	return vector;
}

file::file() {
	memblock = {};
	pointer = 0;
}