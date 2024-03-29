#include <iostream>
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <fstream>
#include <string>
#include <chrono>

#define D_SURF
#define D_KAZE
#define D_ORB
#define D_AKAZE
#define D_BRISK

cv::Mat MatchingDrawingShowing(cv::Mat object, cv::Mat scene, std::vector<cv::KeyPoint> keypoints_object,
	std::vector<cv::KeyPoint> keypoints_scene, cv::Mat descriptors_object, cv::Mat descriptors_scene,
	cv::DescriptorMatcher::MatcherType Mtype, std::string& report)
{
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(Mtype);
	std::vector< std::vector<cv::DMatch> > knn_matches;
	matcher->knnMatch(descriptors_object, descriptors_scene, knn_matches, 2);



	const float ratio_thresh = 0.75f;
	std::vector<cv::DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}


	std::vector<cv::Point2f> obj_points;
	std::vector<cv::Point2f> scene_points;
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		obj_points.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene_points.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}


	cv::Mat H = findHomography(obj_points, scene_points, cv::RANSAC);
	std::vector<cv::Point2f> obj_corners(4);
	obj_corners[0] = cv::Point2f(0, 0);
	obj_corners[1] = cv::Point2f((float)object.cols, 0);
	obj_corners[2] = cv::Point2f((float)object.cols, (float)object.rows);
	obj_corners[3] = cv::Point2f(0, (float)object.rows);
	std::vector<cv::Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);

	cv::Mat img_matches;
	cv::drawMatches(object, keypoints_object, scene, keypoints_scene, good_matches, img_matches, cv::Scalar::all(-1),
		cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	cv::Point2f object_corners[4];;
	object_corners[0] = scene_corners[0];// +cv::Point2f((float)object.cols, 0);
	object_corners[1] = scene_corners[1];// +cv::Point2f((float)object.cols, 0);
	object_corners[2] = scene_corners[2];// +cv::Point2f((float)object.cols, 0);
	object_corners[3] = scene_corners[3];// +cv::Point2f((float)object.cols, 0);

	cv::Point2f* lastItemPointer = (object_corners + sizeof object_corners / sizeof object_corners[0]);
	std::vector<cv::Point2f> contour(object_corners, lastItemPointer);

	double indicator;
	unsigned int actualy_good_points = 0;
	for (auto i = good_matches.begin(); i != good_matches.end(); ++i)
	{
		indicator = cv::pointPolygonTest(contour, keypoints_scene[i->trainIdx].pt, false);
		if (indicator >= 0)
		{
			actualy_good_points++;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		object_corners[i] += cv::Point2f((float)object.cols, 0);
	}

	report += "Total object | scene keypoints: ";
	report += std::to_string(keypoints_object.size()) + " | " + std::to_string(keypoints_scene.size()) + "\n";
	report += "\"Good\" matches: ";
	report += std::to_string(good_matches.size()) + "\n";
	report += "Keypoints, which belongs to object in scene: ";
	std::string KOSTYL = std::to_string((float)actualy_good_points / (float)good_matches.size() * 100);
	report += std::to_string(actualy_good_points) +
		" ( " + KOSTYL[0] + KOSTYL[1] + "." + KOSTYL[3] + KOSTYL[4] + "% )\n";

	line(img_matches, object_corners[0],
		object_corners[1], cv::Scalar(0, 255, 0), 4);
	line(img_matches, object_corners[1],
		object_corners[2], cv::Scalar(0, 255, 0), 4);
	line(img_matches, object_corners[2],
		object_corners[3], cv::Scalar(0, 255, 0), 4);
	line(img_matches, object_corners[3],
		object_corners[0], cv::Scalar(0, 255, 0), 4);


	return img_matches;
}

int main(int argc, char* argv[])
{
	std::string src_object, src_scene;
	src_object = "src.jpg";
	src_scene = "dst.jpg";
	cv::Mat img_object = cv::imread(src_object, cv::IMREAD_GRAYSCALE);
	cv::Mat img_scene = cv::imread(src_scene, cv::IMREAD_GRAYSCALE);
	int minHessian = 1000;
	std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
	cv::Mat descriptors_object, descriptors_scene, result;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point stop;
	std::chrono::seconds duration;
	std::string report = "Report:\n";
	report += "Object file: " + src_object + "\n";
	report += "Scene file: " + src_scene + "\n\n";

#ifdef D_SURF
	report += "SURF:\n\n";
	start = std::chrono::high_resolution_clock::now();
	cv::Ptr<cv::xfeatures2d::SURF> detectorSURF = cv::xfeatures2d::SURF::create(minHessian);
	detectorSURF->detectAndCompute(img_object, cv::noArray(), keypoints_object, descriptors_object);
	detectorSURF->detectAndCompute(img_scene, cv::noArray(), keypoints_scene, descriptors_scene);
	result = MatchingDrawingShowing(img_object, img_scene,
		keypoints_object, keypoints_scene,
		descriptors_object, descriptors_scene,
		cv::DescriptorMatcher::FLANNBASED, report);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	report += "Execution time:\n";
	report += std::to_string(duration.count()) + " sec.\n\n\n";
	cv::imwrite("SURF_img.jpg", result);
	keypoints_object.clear();
	keypoints_scene.clear();
#endif

#ifdef D_KAZE
	report += "KAZE:\n\n";
	start = std::chrono::high_resolution_clock::now();
	cv::Ptr<cv::KAZE> detectorKAZE = cv::KAZE::create();
	detectorKAZE->detectAndCompute(img_object, cv::noArray(), keypoints_object, descriptors_object);
	detectorKAZE->detectAndCompute(img_scene, cv::noArray(), keypoints_scene, descriptors_scene);
	result = MatchingDrawingShowing(img_object, img_scene,
		keypoints_object, keypoints_scene,
		descriptors_object, descriptors_scene,
		cv::DescriptorMatcher::FLANNBASED, report);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	report += "Execution time:\n";
	report += std::to_string(duration.count()) + " sec.\n\n\n";
	cv::imwrite("KAZE_img.jpg", result);
	keypoints_object.clear();
	keypoints_scene.clear();
#endif

#ifdef D_ORB
	report += "ORB:\n\n";
	start = std::chrono::high_resolution_clock::now();
	cv::Ptr<cv::ORB> detectorORB = cv::ORB::create(minHessian);
	detectorORB->detectAndCompute(img_object, cv::noArray(), keypoints_object, descriptors_object);
	detectorORB->detectAndCompute(img_scene, cv::noArray(), keypoints_scene, descriptors_scene);
	result = MatchingDrawingShowing(img_object, img_scene,
		keypoints_object, keypoints_scene,
		descriptors_object, descriptors_scene,
		cv::DescriptorMatcher::BRUTEFORCE_HAMMING, report);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	report += "Execution time:\n";
	report += std::to_string(duration.count()) + " sec.\n\n\n";
	cv::imwrite("ORB_img.jpg", result);
	keypoints_object.clear();
	keypoints_scene.clear();
#endif

#ifdef D_AKAZE
	report += "AKAZE:\n\n";
	start = std::chrono::high_resolution_clock::now();
	cv::Ptr<cv::AKAZE> detectorAKAZE = cv::AKAZE::create();
	detectorAKAZE->detectAndCompute(img_object, cv::noArray(), keypoints_object, descriptors_object);
	detectorAKAZE->detectAndCompute(img_scene, cv::noArray(), keypoints_scene, descriptors_scene);
	result = MatchingDrawingShowing(img_object, img_scene,
		keypoints_object, keypoints_scene,
		descriptors_object, descriptors_scene,
		cv::DescriptorMatcher::BRUTEFORCE_HAMMING, report);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	report += "Execution time:\n";
	report += std::to_string(duration.count()) + " sec.\n\n\n";
	cv::imwrite("AKAZE_img.jpg", result);
	keypoints_object.clear();
	keypoints_scene.clear();
#endif

#ifdef D_BRISK
	report += "BRISK:\n\n";
	start = std::chrono::high_resolution_clock::now();
	cv::Ptr<cv::BRISK> detectorBRISK = cv::BRISK::create();
	detectorBRISK->detectAndCompute(img_object, cv::noArray(), keypoints_object, descriptors_object);
	detectorBRISK->detectAndCompute(img_scene, cv::noArray(), keypoints_scene, descriptors_scene);
	result = MatchingDrawingShowing(img_object, img_scene,
		keypoints_object, keypoints_scene,
		descriptors_object, descriptors_scene,
		cv::DescriptorMatcher::BRUTEFORCE_HAMMING, report);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	report += "Execution time:\n";
	report += std::to_string(duration.count()) + " sec.\n\n\n";
	cv::imwrite("BRISK_img.jpg", result);
	keypoints_object.clear();
	keypoints_scene.clear();
#endif

	std::ofstream report_file;
	report_file.open("Report.txt");
	report_file << report;
	report_file.close();
	return 0;
}
