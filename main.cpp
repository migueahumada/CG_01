#include <iostream>
#include "Image.h"
#include "Texture.h"
void CopyImage(const Image& img){
	
}

int main() {
	

	Image imgScreen;
	//Aquí la imagen debería ser más grande 
	// la imagen de la pantalla, si no está eso, hay erro por eso le
	//le puse que sea 2000
	imgScreen.create(1920, 1080, 24);
	imgScreen.clearColor({ 0,0,255,0 });

	Image img;
	img.decode("Untitled.bmp");
	//img.setPixel(0, 0, { 255,255,0,255 });

	//bit bleet cuando juntas dos imágenes
	Color colorKey = { 255,0,255,255 };

	for (int y = 0; y < (imgScreen.getHeight() / img.getHeight()) + 1; y++)
	{
		for (int x = 0; x < (imgScreen.getWidth() / img.getWidth()) + 1; x++)
		{
			imgScreen.bitBlt(img, img.getWidth() * x, img.getHeight()*y);
		}
	}

	Image imgPlayer;
	imgPlayer.decode("Player.bmp");

	Texture playerTexture;

	playerTexture.createImage(imgPlayer);

	playerTexture.draw(imgScreen,0,0,0,0,0,0,TEXTURE_ADDRESS::CLAMP,BLEND_MODE::ADDITIVE);

	


	//imgScreen.bitBlt(img,0,0,100,100,250,250,&colorKey);

	/*for (int y = 0; y < imgScreen.getHeight(); ++y)
	{
		for (int x = 0; x < imgScreen.getWidth(); x++)
		{
			float u = static_cast<float>(x) / (imgScreen.getWidth() - 1);
			float v = static_cast<float>(y) / (imgScreen.getHeight() - 1);
		
			imgScreen.setPixel(x,y, playerTexture.sample(u * 8.0f,v * 8.0f,TEXTURE_ADDRESS::CLAMP, SAMPLER_FILTER::POINT).toColor());
		}
	}*/

	//imgScreen.line(0, 0, imgScreen.getWidth()-1, imgScreen.getHeight()-1, {255,0,23,255});
	//imgScreen.line(0, 0, imgScreen.getWidth() >> 1, imgScreen.getHeight() - 1, { 255,255,23,255 });
	//imgScreen.line(0, 0, imgScreen.getWidth() >> 2, imgScreen.getHeight() - 1, { 255,255,23,255 });

	imgScreen.encode("TestImageScreen.bmp");
	

	return 0;
}