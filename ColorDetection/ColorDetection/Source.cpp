#include "opencv2/highgui/highgui.hpp"
#include "opencv2\opencv.hpp"

using namespace cv;
using namespace std;

Mat GetThresholdedImage(Mat imgHSV) 
{	
	Mat imgThresh(imgHSV.size(),CV_8U);	
	inRange(imgHSV,Scalar(40,69,149),Scalar(110,256,256),imgThresh);
	return imgThresh;
}

int main(int argc,char** argv)
{
	CvCapture* capture=0;
	capture=cvCaptureFromCAM(0);
	if(!capture)
	{
		cout<<"Camera could not be opened";
		int c=cvWaitKey(500);
		return 0;
	}
	cvNamedWindow("Camera",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Object",CV_WINDOW_AUTOSIZE);
	
	Mat frame;
	while(true)
	{
		frame=cvQueryFrame(capture);
		imshow("Camera",frame);
		int c= cvWaitKey(100);
		Mat imgHSV(frame.size(),frame.type());
		cvtColor(frame,imgHSV,CV_BGR2HSV);
		Mat imgThresh=GetThresholdedImage(imgHSV);
		imshow("HSV",imgHSV);
		imshow("Object",imgThresh);
		if(char(c)==27)
		{
			exit(0);
		}
	}
	return 0;
}

