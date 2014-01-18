#include "iostream"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\opencv.hpp"

using namespace cv;
using namespace std;

class ColorHistogram 
{
private:
	float hranges[2];
	int channels[1];
	int histSize[1];
	const float* ranges[1];
public:
	ColorHistogram()
	{
		// Prepare arguments for a 1d hue histogram
		histSize[0]= 180;
		hranges[0]= 0.0; // BRG range
		hranges[1]= 180.0;
		ranges[0]= hranges;
		channels[0]= 0; // the three channels
	}
	// Computes the 1D Hue histogram with a mask.
	// BGR source image is converted to HSV
	// Pixels with low saturation are ignored
	MatND getHueHistogram(const Mat &image,int minSaturation=0)
	{
		MatND hist;
		// Convert to HSV color space
		Mat hsv;
		cvtColor(image, hsv, CV_BGR2HSV);
		// Mask to be used (or not)
		Mat mask;
		if (minSaturation>0)
		{
			// Spliting the 3 channels into 3 images
			vector<Mat> v;
			split(hsv,v);
			// Mask out the low saturated pixels
			threshold(v[1],mask,minSaturation,255,THRESH_BINARY);
		}
		// Compute histogram
		calcHist(&hsv,1,channels,mask,hist,1,histSize,ranges);
		return hist;
	}
};

class ContentFinder
{
private:
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float threshold;
	cv::MatND histogram;
public:
	ContentFinder(): threshold(-1.0f)
	{
		ranges[0]= hranges; // all channels have same range
		ranges[1]= hranges;
		ranges[2]= hranges;
	}
	// Sets the threshold on histogram values [0,1]
	void setThreshold(float t)
	{
		threshold= t;
	}
	// Gets the threshold
	float getThreshold()
	{
		return threshold;
	}
	// Sets the reference histogram
	void setHistogram(const cv::MatND& h)
	{
		histogram= h;
		normalize(histogram,histogram,1.0);
	}

	Mat find(const Mat& image,float minValue, float maxValue,int *channels, int dim)
	{
		Mat result;
		hranges[0]= minValue;
		hranges[1]= maxValue;
		for (int i=0; i<dim; i++)
			this->channels[i]= channels[i];
		calcBackProject(&image, 1,channels,histogram,result,ranges,255.0);
		// Threshold back projection to obtain a binary image
		if (threshold>0.0)
			cv::threshold(result, result,255*threshold, 255,THRESH_BINARY);
		return result;
	}
};

int main()
{
	VideoCapture capture("Capture_20140117.mp4");
	if(!capture.isOpened())
	{
		cout<<"Camera could not be opened";
		exit(0);
	}
	Mat frame;
	capture.read(frame);
	cv::Rect rect(227,292,130,113);
	Mat imageROI= frame(rect);
	Mat hsv;
	ColorHistogram hc;
	ContentFinder finder;
	int minSat=65;int channels[]={0};
	namedWindow("image 2");
	while(1)
	{
		MatND colorhist=hc.getHueHistogram(imageROI,minSat);
		finder.setHistogram(colorhist);
		int a=capture.read(frame);	
		if(!a)
		{
			exit(0);
		}
		cv::cvtColor(frame, hsv, CV_BGR2HSV);
		std::vector<cv::Mat> v;
		cv::split(hsv,v);
		threshold(v[1],v[1],minSat,255,cv::THRESH_BINARY);
		// Get back-projection of hue histogram
		Mat result= finder.find(hsv,0.0f,180.0f,channels,1);
		// Eliminate low stauration pixels
		cv::bitwise_and(result,v[1],result);
		cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER,10,0.01);
		cv::meanShift(result,rect,criteria);
		cv::rectangle(frame, rect, cv::Scalar(0,0,255));
		imshow("image 2",frame);
		int c= cvWaitKey(10);
		if(char(c)==27)
		{
			exit(0);
		}
		//cv::imshow("Image",result);
		Mat imageROI= frame(rect);
	}
	cv::waitKey(0);
	return 0;
}