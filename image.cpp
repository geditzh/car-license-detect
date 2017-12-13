#include "image.h"
Image::Image(void)
{
}

Image::~Image(void)
{
}

bool Image::verifySize(RotatedRect mr)
{
	float error = 0.5;
	//Spain car plate size: 52x11 aspect 4,7272
	//China car plate size: 440mm*140mm£¬aspect 3.142857
	float aspect = 3.142857;
	//Set a min and max area. All other patchs are discarded
	//int min= 1*aspect*1; // minimum area
	//int max= 2000*aspect*2000; // maximum area
	float min = 1 * aspect * 1; // minimum area
	float max = 2000 * aspect * 2000; // maximum area
									  //Get only patchs that match to a respect ratio.
	float rmin = aspect - aspect*error;
	float rmax = aspect + aspect*error;

	int area = mr.size.height * mr.size.width;
	float r = (float)mr.size.width / (float)mr.size.height;
	if (r < 1)
	{
		r = (float)mr.size.height / (float)mr.size.width;
	}

	if ((area < min || area > max) || (r < rmin || r > rmax))
	{
		return false;
	}
	else
	{
		return true;
	}
}