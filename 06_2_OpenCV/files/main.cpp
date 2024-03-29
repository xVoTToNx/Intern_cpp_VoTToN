#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

int main()
{
	cv::Mat src, blur, gray, threshold, corners, affineMat, result;
	int thresh = 230, max_thresh = 255;

	//Upload an image;
	src = cv::imread("deBook.jpg", 1);

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
		9, 7, 0.05,
		cv::BORDER_DEFAULT);

	//Values for findind 4 book corners;

	//The main idea is that cornerHarris returns a matrix, where corner dots have the highest value,
	//so I'm looking for these values and check, if the dot is far from another, cuz these corner dots
	//like to be in clusters;

	std::vector <cv::Point2f> coordCorners;

	double minVal;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::minMaxLoc(corners, &minVal, &maxVal, &minLoc, &maxLoc);

	coordCorners.push_back(maxLoc);
	//After additing to vector I do not need them anymore;
	corners.at<float>(maxLoc) = 0;

	bool isInRect;
	//If there is no angles in the picture I do not want the program to run forever;
	int failCounter = 0;

	while (coordCorners.size() != 4)
	{
		isInRect = false;
		cv::minMaxLoc(corners, &minVal, &maxVal, &minLoc, &maxLoc);
		for (cv::Point2f p : coordCorners)
		{
			//Making rect for checking, if the dot is near the added one.
			cv::Rect r(p, cv::Size(26, 26));
			r -= cv::Point(13, 13);
			if (r.contains(maxLoc))
			{
				isInRect = true;
				break;
			}
		}
		if (!isInRect)
		{
			coordCorners.push_back(maxLoc);
		}
		corners.at<float>(maxLoc) = 0;
		++failCounter;
		if (failCounter > 1500)
		{
			std::cout << "Corners are not found.";
			return 0;
		}
	}

	//DeBook is found!
	cv::RotatedRect book = cv::minAreaRect(coordCorners);

	//Make new Mat for rotation;
	affineMat = cv::getRotationMatrix2D(
		book.center,
		book.angle + 90,
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
