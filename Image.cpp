#include "Image.h"
#include <iostream>
#include <fstream>

using std::fstream;
using std::ios;

#pragma pack(push,2)
struct MY_BITMAPFILEHEADER {
	unsigned short bfType;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
};
#pragma pack(pop)

//El bitmp header puede ser variable pero
// también podemoc ponerlo todo en un core header así
// Esta versión es compatible literal.
struct MY_BITMAPCOREHEADER {
	unsigned long	bcSize;
	unsigned long	bcWidth;
	unsigned long	bcHeight;
	unsigned short	bcPlanes;
	unsigned short  bcBitCount;
};

struct MY_BITMAPSAVEHEADER {

	MY_BITMAPCOREHEADER coreHeader;
	unsigned long	biCompression;
	unsigned long	biSizeImage;
	long			biXPelsPerMeter;
	long			biYPelsPerMeter;
	unsigned long	biClrUsed;
	unsigned long	biClrImportant;
};


Image::Image()
{
}


void Image::decode(const char* filename)
{
	fstream imgFile(filename, ios::in | ios::binary | ios::ate);
	if (!imgFile.is_open())
	{
		return;
	}

	//Te dice donde llega el puntero final qu ete dice 
	//exactamente cuánto mide el archivo
	auto fileSize = imgFile.tellg();
	std::cout << fileSize;
	imgFile.seekp(ios::beg);

	MY_BITMAPFILEHEADER fileHeader;
	imgFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(MY_BITMAPFILEHEADER));

	if (fileHeader.bfType != 0x4D42) //'BM'
	{
		return;
	}

	MY_BITMAPCOREHEADER infoHeader;
	imgFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(MY_BITMAPCOREHEADER));
	
	/*m_width = infoHeader.bcWidth;
	m_height = infoHeader.bcHeight;
	m_bpp = infoHeader.bcBitCount;

	std::cout << "Info Header ->" << m_height <<std::endl;*/
	imgFile.seekg(fileHeader.bfOffBits);

	create(infoHeader.bcWidth, infoHeader.bcHeight, infoHeader.bcBitCount);

	int padding = getPitch() % 4;
	int lineMemoryWidth = getPitch();
	if (padding)
	{
		padding = 4 - padding;
		lineMemoryWidth += padding;
	}

	for (int line = m_height - 1; line >= 0; --line)
	{
		imgFile.seekp(lineMemoryWidth * line + fileHeader.bfOffBits);
		imgFile.read(reinterpret_cast<char*>(&m_pixels[getPitch() * (m_height - 1 - line)]), getPitch());
	}

	int ready = 1;
}

Color Image::getPixel(unsigned int x, unsigned int y)
{
	Color color;
	int pixelPos = (y * getPitch())+ (x*getBpp());

	color.r = m_pixels[pixelPos + 2];
	color.g = m_pixels[pixelPos + 1];
	color.b = m_pixels[pixelPos + 0];

	return color;


}

void Image::setPixel(unsigned int x, unsigned int y, const Color& color)
{

	int pixelPos = (y * getPitch()) + (x * getBpp());

	m_pixels[pixelPos + 2] = color.r;
	m_pixels[pixelPos + 1] = color.g;
	m_pixels[pixelPos + 0] = color.b;

	if (getBpp() == 4)
	{
		m_pixels[pixelPos + 3] = color.a;
	}
}

void Image::clearColor(const Color& color)
{
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			setPixel(x, y, color);
		}
	}
}



//Auqí tenemos un apantalla y solamente copiamos información para el espacio de la imagen

//void Image::bitBlt(Image& dest, int x, int y, Color* pColorKey)
//{
//	for (int i = 0; i < m_height; i++)
//	{
//		for (int j = 0; j < m_width; j++)
//		{
//			Color pxColor = getPixel(j,i);
//			if (pColorKey)
//			{
//				if (pxColor.r == pColorKey->r &&
//					pxColor.g == pColorKey->g &&
//					pxColor.b == pColorKey->b)
//				{
//					continue;
//				}
//			}
//			dest.setPixel(j+x, i +y, getPixel(j,i));
//		}
//	}
//}

void Image::bitBlt(Image& source, int x, int y, int srcInitX, int srcInitY, int srcEndX, int srcEndY, Color* pColorKey)
{
	//Caso en el cual la imagen está fuera de la pantalla
	if (x >= getWidth() || y >= getHeight())
	{
		return;
	}

	//Si no me dijeron los tamaños, dibujo la imagen completa
	if (0 == srcEndX) srcEndX = source.getWidth();
	if (0 == srcEndY) srcEndY = source.getHeight();

	
	if (x < 0)
	{
		srcInitX -= x; 
		x = 0;
	}

	if (y < 0)
	{
		srcInitY -= y;
		y = 0;
	}

	int realWidth = srcEndX - srcInitX;
	int realHeight = srcEndY - srcInitY;

	if (realWidth < 0 || realHeight < 0)
	{
		return;
	}

	if (x + realWidth > getWidth())
	{
		realWidth -= (x + realWidth) - getWidth();
	}

	if (y + realHeight > getHeight())
	{
		realHeight -= (y + realHeight) - getHeight();
	}

	for (int i = 0; i < realHeight; ++i)
	{
		for (int j = 0; j < realWidth; j++)
		{
			Color color = source.getPixel(srcInitX+j,srcInitY+i);
			if (pColorKey)
			{
				if (color.r == pColorKey->r &&
					color.g == pColorKey->g &&
					color.b == pColorKey->b)
				{
					continue;
				}
			}
			setPixel(x + j,  y + i, color);
		}
	}


	

}

void Image::line(int x0, int y0, int x1, int y1, const Color& color)
{
	float dx = x1 - x0;
	float dy = y1 - y0;

	float steps = std::max(std::abs(dx),std::abs(dy));

	float xInc = dx / steps;
	float yInc = dy / steps;

	float x = x0;
	float y = y0;

	for (int i = 0; i < int(steps); i++)
	{
		// el int(x) es un constructor
		setPixel(int(x), int(y), color);
		x += xInc;
		y += yInc;
	}
}

void Image::bresehamLine(int x0, int y0, int x1, int y1, const Color& color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	int err = dx - dy;
	int e2;

	while (x0 != x1 || y0 != y1)
	{
		setPixel(x0, y0, color);
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void Image::encode(const char* filename)
{
	std::fstream imgFile(filename, std::ios::out | std::ios::binary);
	
	MY_BITMAPFILEHEADER fileHeader;
	MY_BITMAPSAVEHEADER bmpInfo;
	memset(&fileHeader, 0, sizeof(MY_BITMAPFILEHEADER));
	memset(&bmpInfo, 0, sizeof(MY_BITMAPSAVEHEADER));

	int padding = getPitch() % 4;
	int lineMemoryWidth = getPitch();
	if (padding)
	{
		padding = 4 - padding;
		lineMemoryWidth += padding;
	}

	int headerSize = sizeof(MY_BITMAPFILEHEADER) + sizeof(MY_BITMAPSAVEHEADER);
	
	fileHeader.bfType = 0x4D42; //'BM'
	fileHeader.bfSize = headerSize + lineMemoryWidth * m_height;
	fileHeader.bfOffBits = headerSize;

	bmpInfo.coreHeader.bcSize = sizeof(MY_BITMAPSAVEHEADER);
	bmpInfo.coreHeader.bcWidth = m_width;
	bmpInfo.coreHeader.bcHeight = m_height;
	bmpInfo.coreHeader.bcPlanes = 1;
	bmpInfo.coreHeader.bcBitCount = m_bpp;
	bmpInfo.biXPelsPerMeter = 3780;
	bmpInfo.biYPelsPerMeter = 3780;

	imgFile.write(reinterpret_cast<char*>(&fileHeader),sizeof(MY_BITMAPFILEHEADER));
	imgFile.write(reinterpret_cast<char*>(&bmpInfo), sizeof(MY_BITMAPSAVEHEADER));

	char paddBuffer[3] = {0,0,0};

	for (int line = m_height - 1; line >= 0; --line)
	{
		imgFile.write(reinterpret_cast<char*>(&m_pixels[getPitch() * line]), getPitch());
		if (padding)
		{
			imgFile.write(paddBuffer, padding);
		}
	}
	
}
