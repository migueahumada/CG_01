#pragma once
#include <cmath>

// 1 byte
struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};



class Image
{
public:
	Image();
	~Image() {
		if (m_pixels)
		{
			//delete[] m_pixels;
		}
	}

	void create(int width, int height, int bpp) {
		m_width = width;
		m_height= height;
		m_bpp = bpp;
		m_pixels = new unsigned char[getPitch() * m_height];
	}

	void decode(const char* filename);

	int getWidth() const {
		return m_width;
	}

	int getHeight() const {
		return m_height;
	}

	//BitWISE OPERATION para dividir algo entre 8
	int getBpp() const {
		return m_bpp >> 3;
	}
	
	//Pitch es la densidad de información por línea.
	//Se da en bytes o sea dividir los bits entre 8
	int getPitch() const {
		return m_width * getBpp();
	}

	Color getPixel(unsigned int x, unsigned int y);
	void setPixel(unsigned int x, unsigned int y, const Color& color);
	void clearColor(const Color& color);

	
	//Proceso de copiar imágenes de un lugar a otro.
	//bit bleet es como si fuera el proceso de draw
	void bitBlt(Image& source, 
				int x, 
				int y,
				int srcInitX = 0,
				int srcInitY = 0,
				int srcEndX = 0,
				int srcEndY = 0,
				Color* pColorKey = nullptr);
	void line(int x0, int y0, int x1, int y1, const Color& color);
	void bresehamLine(int x0, int y0, int x1, int y1, const Color& color);
	void encode(const char* filename);

protected:
	//Tamaño -> Resolución
	int m_width =0;
	int m_height = 0;

	//Bits por pixel, cuantos bits hay por pixel
	int m_bpp = 0;

	//El contenido será solemente un arreglo o básicamente un buffer.
	//Contenedor de pixeles -> Densidad de cada color
	unsigned char* m_pixels = nullptr;
};


/*
	Imagen de 1 but
	*-*-*-*-*-*-*-*-*
	*-*-*-*-*-*-*-*-*
	*-*-*-*-*-*-*-*-*
	*-*-*-*-*-*-*-*-*
	*-*-*-*-*-*-*-*-*
	*-*-*-*-*-*-*-*-*
*/

