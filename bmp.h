#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#pragma once

class AUX_RGBImageRec {
	void convertBGRtoRGB();
public:
	unsigned char* data;
	GLint sizeX;
	GLint sizeY;
	bool NoErrors;
	AUX_RGBImageRec() : NoErrors(false), data(NULL) {};
	AUX_RGBImageRec(const char *FileName);
	~AUX_RGBImageRec();
	bool loadFile(const char *FileName);
	friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);
};