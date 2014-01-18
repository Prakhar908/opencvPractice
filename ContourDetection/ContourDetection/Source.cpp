#include "opencv2\opencv.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "iostream"

using namespace cv;

int main()
{
	Mat img=imread("test.jpg");
	imshow("Original Image",img);
	// Apply Canny algorithm
	cv::Mat contours;
	cv::Canny(img, // gray-level image
		contours, // output contours
		125, // low threshold
		150); // high threshold
	//imshow("Edges",contours);
	cv::Mat contoursInv; // inverted image
	cv::threshold(contours,contoursInv,
		128, // values below this
		255, // becomes this
		cv::THRESH_BINARY_INV);
	imshow("Edges1",contoursInv);
	for(;;)
	{
		int c=waitKey(0);
	}
	return 0;
}