#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "stdio.h"

using namespace std;
using namespace cv;

class Histogram1D
{
private:
	int histSize[1]; // number of bins
	float hranges[2]; // min and max pixel value
	const float* ranges[1];
	int channels[1]; // only 1 channel used here
public:
	Histogram1D()
	{
		// Prepare arguments for 1D histogram
		histSize[0]= 256;
		hranges[0]= 0.0;
		hranges[1]= 255.0;
		ranges[0]= hranges;
		channels[0]= 0; // by default, we look at channel 0
	}

	// Computes the 1D histogram.
	cv::MatND getHistogram(const cv::Mat &image)
	{
		cv::MatND hist;
		// Compute histogram
		cv::calcHist(&image,1,channels,cv::Mat(),hist,1,histSize,ranges);
		return hist;
	}
	// Computes the 1D histogram and returns an image of it.
	cv::Mat getHistogramImage(const cv::Mat &image,cv::MatND hist)
	{
		// Compute histogram first
		// Get min and max bin values
		double maxVal=0;
		double minVal=0;
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);
		// Image on which to display histogram
		cv::Mat histImg(histSize[0], histSize[0],CV_8U,cv::Scalar(255));
		// set highest point at 90% of nbins
		int hpt = static_cast<int>(0.9*histSize[0]);
		// Draw a vertical line for each bin
		for( int h = 0; h < histSize[0]; h++ )
		{
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal*hpt/maxVal);
			// This function draws a line between 2 points
			cv::line(histImg,cv::Point(h,histSize[0]),
				cv::Point(h,histSize[0]-intensity),
				cv::Scalar::all(0));
		}
		return histImg;
	}
};

int main()
{	
	Mat image=imread("test.jpg",0);
	// The histogram object
	Histogram1D h;
	// Compute the histogram
	MatND hist=h.getHistogram(image);
	Mat histo= h.getHistogramImage(image,hist);
	// Loop over each bin
	for (int i=0; i<256; i++)
		cout << "Value " << i << " = " <<hist.at<float>(i) << endl;
	imshow("histogram",histo);
	for(;;)
	{
		int c=waitKey(0);
	}
}