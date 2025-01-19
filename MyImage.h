#pragma once
struct Color2 {
	char value;
};

class MyImage
{
public:
	MyImage();
	~MyImage();

	void create();
	void decode(const char* filename);

	int GetWidth() const {
		return m_width;
	}

	int GetHeight() const {
		return m_height;
	}

protected:
	//Resolución
	int m_width;
	int m_height;

	Color2* m_pixels;


};

