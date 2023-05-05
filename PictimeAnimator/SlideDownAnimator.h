#pragma once
#include <opencv2/opencv.hpp>
#include "Animator.h"

struct SlideDownAnimatorConfiguraiton 
{
	double SlideDownSpeed;	// The max speed of the SlideDown animation
	double TotalDistance;		// SlideDown total time
	uint fps;				// Writer fps

	//Crop settings
	uint Height;			// The height of the croped image
	uint Width;				// The width of the croped image
	uint StartX;			// Were the first crop starts on x axis
	uint StartY;			// Were the second crop starts on y axis

	//Accelerate configuration
	bool IsAccelerate;		// Is starting with acceleration
	double AccelerateTime;	// Acceleration time

	//Decelerate configuration
	bool IsDecelerate;		// Is starting with deceleration
	double DecelerateTime;	// The from the end that the deceleration should take
};

enum ProcessPhase
{
	Phase_Acceleration,
	Phase_Deceleratoin,
	Phase_Liniar
};

class SlideDownAnimator : public AnimationBase
{
public:
	SlideDownAnimator(_Inout_ SlideDownAnimatorConfiguraiton configuration);
	~SlideDownAnimator();

	/// <summary>
	/// Process the current frame
	/// </summary>
	/// <param name="context">Holds information relavite to processing, including the working image</param>
	/// <returns>The processed image</returns>
	_Out_ std::shared_ptr<cv::Mat> ProcessImage(
		_In_ ProcessContext context
	);

	uint GetNumerOfFrames();
private:
	SlideDownAnimatorConfiguraiton _configuration;

	ProcessPhase GetProcessPhase(_In_ ProcessContext* context);

	double prev_offset;
	int frameCount;

	int LiniarSpeedFrameCount;

	double Acceleration_M;
	double Init_Acceleration();

	double Deceleration_M;
	double Deceleration_B;
	double Init_Deceleration();

	int GetOffsetAcceleration(_In_ ProcessContext* context);
	int GetOffsetDeceleration(_In_ ProcessContext* context);
	int GetOffsetLiniar(_In_ ProcessContext* context);
};

