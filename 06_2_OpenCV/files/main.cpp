#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

int main()
{
	cv::Mat src, blur, gray, threshold, corners, cornersNorm, cornersNormScaled, affineMat, result;

	//Upload an image;
	src = cv::imread("deBook.jpg", 1);

	int thresh = 230, max_thresh = 255;

	//Make the image 50 shades of gray;
	cv::cvtColor(src,
		gray,
		cv::COLOR_BGR2GRAY);

	//Make the image less sharp;
	cv::blur(gray,
		blur,
		cv::Size(3, 3));

	//See only the book. The book is black;
	cv::threshold(blur,
		threshold,
		thresh,
		max_thresh,
		cv::THRESH_BINARY);

	//See borders. Only borders. Borders are good.¯\_(ツ)_/¯;
	cv::cornerHarris(threshold,
		corners, 
		7, 5, 0.05,
		cv::BORDER_DEFAULT);

	//At the moment of writing this line I do not know what it does. Honestly. Will learn tomorrow (Sunday);
	cv::normalize(corners,
		cornersNorm, 
		0, 255, 
		cv::NORM_MINMAX,
		CV_32FC1, 
		cv::Mat());

	//Neither this line;
	cv::convertScaleAbs(cornersNorm, cornersNormScaled);

	std::vector <cv::Point2d> coordCorners;

	//Looking whole image for a condition. If the condition is true - we have a corner;
	for (int j = 0; j < cornersNorm.rows; j++)
	{
		for (int i = 0; i < cornersNorm.cols; i++)
		{
			if ((int)cornersNorm.at<float>(j, i) > thresh)
			{
				coordCorners.push_back(cv::Point2d(i, j));
			}
		}
	}

	//Find center of the book;
	int xMax, yMax;
	xMax = yMax = INT_MIN;
	int xMin, yMin;
	xMin = yMin = INT_MAX;

	for (cv::Point2d p : coordCorners)
	{
		if (xMax < p.x)
			xMax = p.x;
		if (xMin > p.x)
			xMin = p.x;
		if (yMax < p.y)
			yMax = p.y;
		if (yMin > p.y)
			yMin = p.y;
	}

	cv::Point2d center((xMax + xMin) / 2, (yMax + yMin) / 2);

	//Find left book edge;
	cv::Point2d vector(abs(coordCorners[0].x - coordCorners[2].x),
		abs(coordCorners[0].y - coordCorners[2].y));

	double vectorLength = sqrt(pow(vector.x, 2) +
									 pow(vector.y, 2));
	
	//Angle between oY and left book border;
	double degree = asin(vector.x / vectorLength) * 180.0 / 3.14;

	//Make new Mat for rotation;
	affineMat = cv::getRotationMatrix2D(
		center,
		degree,
		1);

	//HomeTask is done;
	cv::warpAffine(src,
		result,
		affineMat,
		src.size(),
		1,
		cv::BORDER_TRANSPARENT);

	cv::imshow("deBoOk", result);

	cv::waitKey(0);
	return(0);
}
