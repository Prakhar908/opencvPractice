#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{	
	Mat img = imread("test.jpg");
	if (!img.data) 
	{ 
	 cout << "No data";
	}
	Mat grayscale(img.size(),CV_8U);
	cvtColor( img, grayscale, CV_BGR2GRAY );
	imshow("Original",img);
	imshow("Grayscale",grayscale);
	int nr=grayscale.rows;
	int nc=grayscale.cols;
	for(int i=0;i<nr;i++)
	{
		uchar* data= grayscale.ptr<uchar>(i);
		for(int j=0;j<nc;j++)
		{
			if(data[j]<=127)
				data[j]=0;
			else
				data[j]=255;
		}
	}
	imshow("Binary",grayscale);
	waitKey(0);
	return 0;
}