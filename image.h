#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
class Image
{
public:
	Image(void);
	~Image(void);
	static bool verifySize(RotatedRect mr);
};