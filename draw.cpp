#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

using namespace cv;
using namespace std;

Mat src;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/** @function main */
int main( int argc, char** argv )
{
	int has_objects = 0;
	if(argc != 3) {
		printf("Argument error! %s <input> <output>\n", argv[0]);
		return -1;
	}
	/// Load source image and convert it to gray
	src = imread( argv[1], 1 );

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny( src, canny_output, thresh, thresh*2, 3 );
	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	/// Draw contours
	for( int i = 0; i< contours.size(); i++ ) {
	//	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	//	drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );

		approxPolyDP( Mat(contours[i]), contours_poly[i], 1, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	}
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ ) {
		//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		Scalar color = Scalar(0, 0, 255);
		Scalar color_contour = Scalar(255, 255,255);
		drawContours( drawing, contours_poly, i, color_contour, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, CV_FILLED, 8, 0 );
		has_objects = 1;
	}
	if(has_objects) {

	}

	std::string dirname = argv[2];
	std::string filename = basename(argv[1]);
	std::string outFileName = dirname+"/"+filename;
	cv::imwrite(outFileName, drawing);

	return 0;
}
