#pragma once
#include <sal.h>
#include <opencv2/opencv.hpp>
#include <iostream>

struct ProcessContext
{
	int Frame;
	double Time;
	std::shared_ptr<cv::Mat> WorkingImage;
};

struct AnimationConfig
{
	int Fps;
	int Width;
	int Height;
};

class AnimationBase
{
public:
	virtual _Out_ std::shared_ptr<cv::Mat> ProcessImage(
		_In_ ProcessContext context
	) = 0;

	virtual _Out_ uint GetNumerOfFrames() = 0;
};

class Animator
{
public:
	Animator();
	~Animator();

	/// <summary>
	/// This function animates a mp4 video based on animators that run logic on an original image.
	/// </summary>
	/// <param name="Animations:">All animations that will go into the create the video\n</param>
	/// <param name="inputStream:">A stream containing the original image (Stream will not be closed by this function)\n</param>
	/// <param name="filename:">The path to the file that the mp4 will be writen to (All file handles are released at the end of the function)\n</param>
	int Animate(
		_In_ AnimationConfig& config,
		_In_ std::vector<std::shared_ptr<AnimationBase>> animations, 
		_In_ std::istream& inputStream,
		_Out_ const std::string& outputPath
	);

	int Animate(
		_In_ AnimationConfig& config,
		_In_ std::vector<std::shared_ptr<AnimationBase>> animations,
		_In_ std::shared_ptr<cv::Mat> image,
		_Out_ const std::string& filename
	);
};

