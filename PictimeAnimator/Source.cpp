#include <opencv2/opencv.hpp>
#include "SlideDownAnimator.h"
#include "ArgumentParser.h"

#define _FLAG_INPUT_FILE_ "i"
#define _FLAG_OUTPUT_FILE_ "o"
#define _FLAG_CROPTED_HEIGHT_ "height"
#define _FLAG_CROPTED_WIDTH_ "width"
#define _FLAG_FPS_ "fps"
#define _FLAG_ACCELERATION_TIME_ "acTime"
#define _FLAG_DECELERATION_TIME_ "dcTime"
#define _FLAG_TOP_SPEED_ "s"

#define CONVERT_STR_INT(string) std::atoi(string.c_str())

int main(int argc, char* argv[]) {
	
	ArgumentParser parser = ArgumentParser();
	parser.AddOptions(_FLAG_INPUT_FILE_, "input file name");
	parser.AddOptions(_FLAG_OUTPUT_FILE_, "output file name", "./output.mp4");
	parser.AddOptions(_FLAG_CROPTED_HEIGHT_, "height of the cropted video");
	parser.AddOptions(_FLAG_CROPTED_WIDTH_, "width of the cropted video");
	parser.AddOptions(_FLAG_FPS_, "image fps", "60");
	parser.AddOptions(_FLAG_ACCELERATION_TIME_, "time of acceleration to top speed in frames", "0");
	parser.AddOptions(_FLAG_DECELERATION_TIME_, "time of deceleration from top speed in frames", "0");
	parser.AddOptions(_FLAG_TOP_SPEED_, "top speed of slide down animation, units in pixels per frame");
	
	if (parser.ParseArgument(argc, argv))
	{
		return EXIT_FAILURE;
	}
	

	std::shared_ptr<cv::Mat> image = std::make_shared<cv::Mat>(cv::imread(parser.GetValue(_FLAG_INPUT_FILE_)));

	int height = CONVERT_STR_INT(parser.GetValue(_FLAG_CROPTED_HEIGHT_));
	int width = CONVERT_STR_INT(parser.GetValue(_FLAG_CROPTED_WIDTH_));
	int speed = CONVERT_STR_INT(parser.GetValue(_FLAG_TOP_SPEED_));
	int fps = CONVERT_STR_INT(parser.GetValue(_FLAG_FPS_));

	int accTime = CONVERT_STR_INT(parser.GetValue(_FLAG_ACCELERATION_TIME_));
	int dccTime = CONVERT_STR_INT(parser.GetValue(_FLAG_DECELERATION_TIME_));

	SlideDownAnimatorConfiguraiton conf;

	conf.IsAccelerate = accTime != 0;
	conf.AccelerateTime = accTime;

	conf.IsDecelerate = dccTime != 0;
	conf.DecelerateTime = dccTime;

	conf.StartX = 0;
	conf.StartY = 0;
	conf.Width = image->cols;
	conf.Height = height;

	conf.TotalDistance = image->rows - height;
	conf.fps = fps;
	conf.SlideDownSpeed = speed;

	AnimationConfig aniConfig;
	aniConfig.Fps = fps;
	aniConfig.Height = height;
	aniConfig.Width = image->cols;

	auto animation = std::make_shared<SlideDownAnimator>(conf);
	auto animator = Animator();
	auto vec = std::vector<std::shared_ptr<AnimationBase>>();
	vec.push_back(animation);
	animator.Animate(aniConfig, vec, image, parser.GetValue(_FLAG_OUTPUT_FILE_));

	return 0;
}