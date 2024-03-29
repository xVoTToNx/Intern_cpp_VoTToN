#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
#include <ctime>


int main()
{
	cv::VideoCapture cap(0);
	cv::Mat frame;
	while (true) {
		std::string str("Hello There!");

		cap.read(frame);
		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		cv::putText(frame,
			str,
			cv::Point(frame.cols - str.length() * 20, 25),
			cv::FONT_HERSHEY_COMPLEX,
			1.0,
			cv::Scalar(255, 100, 100),
			2);

		cv::Mat img;
		img = cv::imread("ME.jpg", cv::IMREAD_COLOR);
		cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);


		cv::resize(img, img, cv::Size(100, 100));
		cv::Rect rect(0, 0, img.cols, img.rows);
		cv::Mat croppedImg = frame(rect);
		cv::addWeighted(img, 0.5, croppedImg, 0.5, 0.0, img);
		img.copyTo(frame(rect));

		cv::imshow("MyFace", frame);

		int x = cv::waitKey(5);

		if (x == 27) {
			break;
		}
		else if (x == 32) {
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);

			char buffer[80];
			strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", now);
			
			cv::imwrite(std::string(buffer) + ".png", frame);
		}

	}
	return 0;
}