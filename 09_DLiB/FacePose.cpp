// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;

/*void d(int i)
{
	std::cout << i << std::endl;
}*/

int main()
{
		cv::VideoCapture cap(0);
		if (!cap.isOpened())
		{
			cerr << "Unable to connect to camera" << endl;
			return 1;
		}

		// Load face detection and pose estimation models.
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor pose_model;
		deserialize("sh.dat") >> pose_model;


		// 3D model points.
		std::vector<cv::Point3d> model_points;
		model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
		model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
		model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
		model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
		model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
		model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

		cv::Mat temp;
		// Grab and process frames until the main window is closed by the user.
		while (true)
		{
			// Grab a frame
			cap.read(temp);

			// Turn OpenCV's Mat into something dlib can deal with.  Note that this just
			// wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
			// long as temp is valid.  Also don't do anything to temp that would cause it
			// to reallocate the memory which stores the image as that will make cimg
			// contain dangling pointers.  This basically means you shouldn't modify temp
			// while using cimg.
			cv_image<bgr_pixel> cimg(temp);

			// Detect faces 
			std::vector<rectangle> faces = detector(cimg);
			// Find the pose of each face.
			std::vector<full_object_detection> shapes;
			for (unsigned long i = 0; i < faces.size(); ++i)
				shapes.push_back(pose_model(cimg, faces[i]));


			if (shapes.size() > 0)
			{
				// 2D image points. If you change the image, you need to change vector
				std::vector<cv::Point2d> image_points;

				image_points.push_back(cv::Point(shapes[0].part(30).x(), shapes[0].part(30).y()));    // Nose tip
				image_points.push_back(cv::Point(shapes[0].part(8).x(), shapes[0].part(8).y()));    // Chin
				image_points.push_back(cv::Point(shapes[0].part(45).x(), shapes[0].part(45).y()));     // Left eye left corner
				image_points.push_back(cv::Point(shapes[0].part(36).x(), shapes[0].part(36).y()));    // Right eye right corner
				image_points.push_back(cv::Point(shapes[0].part(54).x(), shapes[0].part(54).y()));    // Left Mouth corner
				image_points.push_back(cv::Point(shapes[0].part(48).x(), shapes[0].part(48).y()));    // Right mouth corner

				// Camera internals
				double focal_length = temp.cols; // Approximate focal length.
				cv::Point2d center = cv::Point2d(temp.cols / 2, temp.rows / 2);
				cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
				cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion

				// Output rotation and translation
				cv::Mat rotation_vector; // Rotation in axis-angle form
				cv::Mat translation_vector;

				// Solve for pose
				cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);


				// Project a 3D point (0, 0, 1000.0) onto the image plane.
				// We use this to draw a line sticking out of the nose

				std::vector<cv::Point3d> nose_end_point3D;
				std::vector<cv::Point2d> nose_end_point2D;
				nose_end_point3D.push_back(cv::Point3d(0, 0, 1000.0));

				projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);

				//Convert dlib::rectangle into cv::Rect
				cv::Point topLeft(faces[0].tl_corner().x(), faces[0].tl_corner().y());
				cv::Point bottomRight(faces[0].br_corner().x(), faces[0].br_corner().y());
				cv::Rect faceCV(topLeft, bottomRight);

				//Draw the position of the head
				cv::rectangle(temp, faceCV, cv::Scalar(255, 0, 255), 2);
				if (rotation_vector.at<double>(1, 0) * 100 > 45.0)
				{
					cv::putText(temp, "Right", topLeft, 0, 2, cv::Scalar(255, 0, 255), 4);
				}
				else if (rotation_vector.at<double>(1, 0) * 100 < -45.0)
				{
					cv::putText(temp, "Left", topLeft, 0, 2, cv::Scalar(255, 0, 255), 4);
				}
				else
				{
					cv::putText(temp, "Forward", topLeft, 0, 2, cv::Scalar(255, 0, 255), 4);
				}

				
				for (int i = 0; i < image_points.size(); i++)
				{
					circle(temp, image_points[i], 3, cv::Scalar(0, 0, 255), -1);
				}

				cv::line(temp, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);

				// Display image.
				cv::imshow("Output", temp);
				cv::waitKey(5);
			}
		}
	
}

