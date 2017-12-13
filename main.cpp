/*************************************************************************************************
@FileName:main.cpp,image.cpp,image.h
@Problem:Car License Plate Localization
@Date:2017/12/13
@Opencv Version:3.3.0
@Dependency file:opencv_world330.lib
@Author:zhanghan
*************************************************************************************************/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"

using namespace std;
using namespace cv;

int main()
{	
	char imageName[10];
	char imageBwName[10];
	char imageDstName[10];
	int maxDif = 50;
	//找到蓝色区域
	int i = 0, j = 0;
	unsigned char * pPixel = NULL;
	unsigned char   pixelR = 0, pixelG = 0, pixelB = 0;
	unsigned char R = 28, G = 63, B = 138;

	Mat srcImage,image,bwImage,dstImage;

	double area, rectArea;
	double long2Short = 1.0; //体态比
							 //计算边界序列的参数 长度 面积 矩形 最小矩形 
							 //并输出每个边界的参数
	double axisLong = 0.0, axisShort = 0.0;
	double temp;
	const int num = 9;
	for (int i = 1; i <= num; i++)
	{
		sprintf_s(imageName,"%d.jpg",i);
		sprintf_s(imageBwName, "%d_bw.jpg", i);
		sprintf_s(imageDstName, "%d_dst.jpg", i);
		cout << imageName << ": " << endl;

		srcImage = imread(imageName);
		if (!srcImage.data) { printf("读取srcImage错误~！ \n"); return false; }
		image = srcImage.clone();
		//imshow(imageName,image);
		
		GaussianBlur(image, image, Size(3,3), 0, 0, BORDER_DEFAULT);
	/*	imshow(imageName,image);*/

		bwImage = Mat::zeros(image.size(),0);
		//imshow(imageBwName, bwImage);
		for (int i = 0; i< image.rows; i++)
		{
			for (int j = 0; j<image.cols; j++)
			{
				pPixel = (unsigned char*)image.data + i*image.step + j * 3;
				pixelB = pPixel[0];
				pixelG = pPixel[1];
				pixelR = pPixel[2];
				if (abs(pixelB - B) < maxDif && abs(pixelG - G)< maxDif && abs(pixelR - R)< maxDif)
				{
					*((unsigned char*)bwImage.data + i*bwImage.step + j) = 255;
				}
				else {
					*((unsigned char*)bwImage.data + i*bwImage.step + j) = 0;
				}
			}
		}
		//imshow(imageBwName, bwImage);
		//cvWaitKey();
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(bwImage, bwImage, element);
		erode(bwImage, bwImage, element);
		//imshow(imageBwName, bwImage);
		//cvWaitKey();

		vector<vector<Point>> contours;
		findContours(bwImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		Mat tempdstImage(bwImage.size(), CV_8U, Scalar(0));
		//drawContours(tempdstImage, contours, -1, Scalar(255), 2);
		//imshow(imageBwName, tempdstImage);
		//cvWaitKey();

		vector<vector<Point>>::const_iterator itc = contours.begin();
		while (itc != contours.end())
		{
			RotatedRect mr = minAreaRect(Mat(*itc));
			area = contourArea(*itc);
			if (!Image::verifySize(mr))
				itc = contours.erase(itc);
			else if(area < 1000 || area > 50000)
				itc = contours.erase(itc);
			else
				++itc;
		}
		//drawContours(tempdstImage, contours, -1, Scalar(255), 2);
		//imshow(imageBwName, tempdstImage);
		//cvWaitKey();
		
		dstImage = srcImage.clone();
		vector<RotatedRect> minRect(contours.size());
	    for (int i = 0; i< contours.size(); i++)
		{
			//drawContours(dstImage, contours, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
			//Rect rect = boundingRect(contours[i]);//检测外轮廓  
			//rectangle(dstImage, rect, CV_RGB(0, 255, 0), 2);	
			
			minRect[i] = minAreaRect(Mat(contours[i]));
			Point2f rect_points[4]; minRect[i].points(rect_points);
			
			axisLong = sqrt(pow(rect_points[1].x - rect_points[0].x, 2) + pow(rect_points[1].y - rect_points[0].y, 2));
			axisShort = sqrt(pow(rect_points[2].x - rect_points[1].x, 2) + pow(rect_points[2].y - rect_points[1].y, 2));
			if (axisShort > axisLong)
			{
				temp = axisLong;
				axisLong = axisShort;
				axisShort = temp;
			}
			rectArea = axisLong*axisShort;
			long2Short = axisLong / axisShort;
			cout << "long axis ：" << axisLong << endl;
			cout << "short axis: " << axisShort << endl;
			cout << "long2Short: " << long2Short << endl;
			cout << "rectArea:  " << rectArea << endl;
			if (long2Short > 2.8 && long2Short < 3.8)
			{
				for (int j = 0; j < 4; j++)
					line(dstImage, rect_points[j], rect_points[(j + 1) % 4], CV_RGB(255, 0, 0), 2);
			}			
		}		
		imshow(imageDstName,dstImage);
		imwrite(imageDstName, dstImage);
	}
	waitKey(0);
    return 0;
}
