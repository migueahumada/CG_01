#include "MyImage.h"
#include <fstream>

struct BMPFileHeader {
	uint16_t signature;
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t pixelArrayOffset;
};

MyImage::MyImage()
{
	
}

MyImage::~MyImage()
{
}

void MyImage::create()
{
}

void MyImage::decode(const char* filename)
{
	//Abrimos el archivo
	std::fstream file(filename,std::ios::binary | std::ios::ate | std::ios::in);

	//Revisamos si el archivo no está abierto
	if (!file.is_open())
	{
		return;
	}

	//Vamos a ver cual es el tamaño del file
	auto fileSize = file.tellg();
	file.seekp(std::ios::beg);
}
