#include "Animator.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

cv::VideoWriter CreateNewWriter(const std::string& filename, int fps, int width, int height)
{
	cv::VideoWriter writer;
	int codec = cv::VideoWriter::fourcc('H', '2', '6', '4');
	cv::Size output(width, height);

	writer.open(filename, codec, fps, output);
	return writer;
}


int Animator::Animate(
	_In_ AnimationConfig& config,
	_In_ std::vector<std::shared_ptr<AnimationBase>> animations,
	_In_ std::istream& inputStream,
	_Out_ const std::string& filename
)
{
	std::streampos startPos = inputStream.tellg();

	inputStream.seekg(0, std::ios_base::_Seekend);
	std::streamoff size = inputStream.tellg() - startPos;
	inputStream.seekg(startPos);

	char* buffer = new char[size];

	cv::Mat image = cv::imdecode(cv::Mat(1, size, CV_8UC1, buffer), cv::IMREAD_UNCHANGED);
	std::shared_ptr<cv::Mat> image_ptr = std::make_shared<cv::Mat>(image);
	return this->Animate(config, animations, image_ptr, filename);
}

int Animator::Animate(
	_In_ AnimationConfig& config,
	_In_ std::vector<std::shared_ptr<AnimationBase>> animations,
	_In_ std::shared_ptr<cv::Mat> image,
	_Out_ const std::string& filename
)
{
	int outCode = 0;
	cv::VideoWriter writer = CreateNewWriter(filename, config.Fps, config.Width, config.Height);

	try {
		for (auto animator : animations)
		{
			int frames = animator->GetNumerOfFrames();
			for (int i = 0; i < frames; i++)
			{
				ProcessContext context;
				context.Frame = i;
				context.Time = i;
				context.WorkingImage = image;
				std::shared_ptr<cv::Mat> outputImage = animator->ProcessImage(context);
				writer.write(*outputImage);
			}
		}
	}
	catch (int code) {
		outCode = code;
	}
	//Release file handles
	writer.release();
	return outCode;
}