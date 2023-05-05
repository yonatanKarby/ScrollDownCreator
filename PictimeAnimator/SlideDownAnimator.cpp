#include "SlideDownAnimator.h"

#define IN_RANGE_CHECK(RangeStart, RangeEnd, Point) RangeStart <= Point && RangeEnd >= Point

SlideDownAnimator::SlideDownAnimator(_Inout_ SlideDownAnimatorConfiguraiton configuration)
{
	//Validate configuration
	if (configuration.TotalDistance - configuration.AccelerateTime < configuration.DecelerateTime)
		throw std::invalid_argument("Error in configuration time calculations");
	
	_configuration = configuration;
	prev_offset = 0;
	frameCount = 0;
	Acceleration_M = 0;
	Deceleration_B = 0;
	Deceleration_M = 0;

	double d = _configuration.TotalDistance;

	if (_configuration.IsAccelerate)
		d -= Init_Acceleration();
	if (_configuration.IsDecelerate)
		d -= Init_Deceleration();

	LiniarSpeedFrameCount = d / _configuration.SlideDownSpeed;

}

SlideDownAnimator::~SlideDownAnimator()
{
}

ProcessPhase __cdecl SlideDownAnimator::GetProcessPhase(_In_ ProcessContext* context)
{
	//Check in acceleration
	if (_configuration.IsAccelerate &&
		IN_RANGE_CHECK(0, _configuration.AccelerateTime, context->Time)
		)
	{
		return Phase_Acceleration;
	}
	//Check in deceleration
	if (_configuration.IsDecelerate && context->Time > (LiniarSpeedFrameCount + _configuration.AccelerateTime))
	{
		return Phase_Deceleratoin;
	}
	return Phase_Liniar;
}

double SlideDownAnimator::Init_Acceleration()
{
	Acceleration_M = _configuration.SlideDownSpeed / _configuration.AccelerateTime;
	return _configuration.SlideDownSpeed * _configuration.AccelerateTime / 2;
}

double SlideDownAnimator::Init_Deceleration()
{
	Deceleration_M = _configuration.SlideDownSpeed / -_configuration.DecelerateTime;
	Deceleration_B = _configuration.SlideDownSpeed;
	return _configuration.SlideDownSpeed * _configuration.DecelerateTime / 2;
}

int SlideDownAnimator::GetOffsetAcceleration(_In_ ProcessContext* context)
{
	double speed = Acceleration_M * context->Time;
	prev_offset += speed;
	return (int)prev_offset;

}

int SlideDownAnimator::GetOffsetDeceleration(_In_ ProcessContext* context)
{
	double relativeT = context->Time - (LiniarSpeedFrameCount + _configuration.AccelerateTime);
	double speed = (Deceleration_M * relativeT) + Deceleration_B;
	prev_offset += speed;
	return (int)prev_offset;
}

int SlideDownAnimator::GetOffsetLiniar(_In_ ProcessContext* context)
{
	prev_offset += _configuration.SlideDownSpeed;
	return (int)prev_offset;
}

_Out_ std::shared_ptr<cv::Mat> SlideDownAnimator::ProcessImage(_In_ ProcessContext context)
{
	ProcessPhase phase = GetProcessPhase(&context);
	int offset = 0;

	switch (phase)
	{
		case Phase_Acceleration:
			offset = GetOffsetAcceleration(&context);
			break;
		case Phase_Deceleratoin:
			offset = GetOffsetDeceleration(&context);
			break;
		case Phase_Liniar: 
			offset = GetOffsetLiniar(&context);
			break;
		default:
			return NULL;
	}
	cv::Rect Crop(_configuration.StartX, _configuration.StartY + offset, _configuration.Width, _configuration.Height);
	cv::Mat cropted = (*context.WorkingImage)(Crop);
	return std::make_shared<cv::Mat>(cropted);
}

uint SlideDownAnimator::GetNumerOfFrames()
{
	return LiniarSpeedFrameCount + _configuration.AccelerateTime + _configuration.DecelerateTime;
}
