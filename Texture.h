#pragma once
#include "Image.h"

//Textura un contenedor que ocntien euna imagen o varias
//Puede ser la misma pero con imagenes puros
/*
	Primero normlaizar lso valores en todo sentido.
	La textura la usaremos ocn valores normalizados
	EL oclor se usará de manera normalizada porque
	así nos interesa el formato

	Cuando leemos texturas no leemos PIXELES
	Vamos  aleer la informaicóna partir de un filtro
	Para ver cómo lo buscamos para leerlo
	No solo va  atener la imagen sino los bitmaps.
*/

namespace TEXTURE_ADDRESS {
	enum E {
		CLAMP = 0,
		WRAP,
		MIRROR,
		MIRROR_ONCE //Espejo en solament en un eje
	};
}


namespace BLEND_MODE {
	enum E {
		NONE = 0,
		ALPHA_BLEND, // Es la mezcla
		ADDITIVE // Es lo aditivo
	};
}

//soLO SE PUED ELE IFLTRO TRILINEAL SI TENGO LOS BITMAPS

namespace SAMPLER_FILTER {
	enum E {
		POINT = 0,
		LINEAR
	};
}


struct FloatColor {

	FloatColor() = default;
	FloatColor(float _r, float _g, float _b, float _a) :
		r(_r), g(_g), b(_b), a(_a){}
	FloatColor(const Color& color)
		:r(color.r / 255.0f),
		g(color.g / 255.0f),
		b(color.b / 255.0f),
		a(color.a / 255.0f) {}

	Color toColor() 
	{
		return Color{
			static_cast<unsigned char>(r * 255.0f),
			static_cast<unsigned char>(g * 255.0f),
			static_cast<unsigned char>(b * 255.0f),
			static_cast<unsigned char>(a * 255.0f)};
	}

	void saturate() {
		r = r > 1.0f ? 1.0f : r;
		g = g > 1.0f ? 1.0f : g;
		b = b > 1.0f ? 1.0f : b;
		a = a > 1.0f ? 1.0f : a;
	}

	FloatColor operator+(const FloatColor& color) 
	{
		return FloatColor(r + color.r , g + color.g,  b + color.b, a + color.a);
	}

	FloatColor operator*(const FloatColor& color) 
	{
		return FloatColor(r * color.r, g * color.g, b * color.b, a * color.a);
	}
	
	FloatColor operator*(float escalar) {
		return FloatColor(r * escalar, g * escalar, b * escalar, a * escalar);
	}

	float r;
	float g;
	float b;
	float a;
};

class Texture
{
public:

	Texture() =default;
	~Texture() = default;

	void createImage(const Image& img) {
		m_image = img;
	}

	//Va a tomar las UVs
	void adjustTextureAddress(TEXTURE_ADDRESS::E textAddressMode, float& u, float& v);
	FloatColor sample(	float u, 
						float v, 
						TEXTURE_ADDRESS::E textureAdress = TEXTURE_ADDRESS::CLAMP, 
						SAMPLER_FILTER::E filter = SAMPLER_FILTER::POINT);
	
	//FloatColor getColor(float u, float v, TEXTURE_ADDRESS::E textureAddress = TEXTURE_ADDRESS::CLAMP);
	void draw(	Image& img, 
				int x, 
				int y, 
				int srcInitX, 
				int srcInitY, 
				int srcEndX, 
				int srcEndY, 
				TEXTURE_ADDRESS::E textureAdress = TEXTURE_ADDRESS::CLAMP, 
				BLEND_MODE::E blendMode = BLEND_MODE::NONE);

	Image m_image;
};

