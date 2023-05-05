#include "CameraAgent.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#define __CASCADE_FILE_PATH__ "D:\\Surveilance\\Surveilance\\x64\\Debug\\face_cascade.xml"

CameraAgent::CameraAgent(cv::VideoCapture* capture, uchar debug)
{
	_capture = capture;
	_debug = debug;
	facedetect.load(__CASCADE_FILE_PATH__);

}

struct Detection
{
	bool IsDetected;
	cv::Rect rectangle;
};


void CameraAgent::DetectFace(cv::Mat* frame)
{
	std::vector<cv::Rect> faces;
	facedetect.detectMultiScale((*frame), faces, 1.3, 5);
	for (auto face : faces) {
		cv::rectangle((*frame), face.tl(), face.br(), cv::Scalar(50, 50, 255), 3);
	}
}

void CameraAgent::StartCameraAgent(_CAMERA_TRIGGER_CALLBACK_(trigger))
{
	if (_debug) {
		InitDebugWindow();
	}
	cv::Mat frame;
	while (1) {
		(*_capture) >> frame;
		DetectFace(&frame);
		if (_debug) {
			cv::imshow(__DEBUG_WINDOW_NAME__, frame);
		}
		if (cv::waitKey(10) >= 0)
		{
			return;
		}
	}
}

void CameraAgent::InitDebugWindow()
{
	cv::namedWindow(__DEBUG_WINDOW_NAME__);
}

cv::Mat toGrayScale(cv::Mat image)
{
	cv::Mat returnImage;
	cv::cvtColor(image, returnImage, cv::COLOR_BGR2GRAY);
	return returnImage;
}