#include "BgSubtract.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
	                    double scale, const Scalar& color)
	{
	    for(int y = 0; y < cflowmap.rows; y += step)
	        for(int x = 0; x < cflowmap.cols; x += step)
	        {
	            const Point2f& fxy = flow.at<Point2f>(y, x);
	            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
	                 color);
	            circle(cflowmap, Point(x,y), 2, color, -1);
	        }
	}


int MyFlowAlgo()
{

      CvCapture *cap= cvCaptureFromFile("Ori_WaterObject.avi");// "1-11200.avi" );

     //CvCapture *cap= cvCaptureFromCAM(0);
//     if (cap==NULL)
//     {
//        fprintf( stderr, "Cannot open AVI!\n" );
//        return 1;
//     }
//
//     IplImage *prev_frame=cvQueryFrame(cap);
//
//     if(!prev_frame) exit(0);
//     cout<<"Frame size: "<<prev_frame->width<<"x"<<prev_frame->height<<endl;
//     cout<<"Depth: "<<prev_frame->depth<<endl;
//     cout<<"Channel: "<<prev_frame->nChannels<<endl;
//
//     cvWaitKey(0);

//     Mat  grayprev = Mat::zeros(cvSize( prev_frame->width, prev_frame->height ), CV_8UC1);
//     Mat  graycurr = Mat::zeros(cvSize( prev_frame->width, prev_frame->height ), CV_8UC1);
//     Mat displayMat;// = Mat::zeros(cvSize(prev_frame->width, prev_frame->height), CV_32FC2);

//     cvCvtColor(prev_frame,grayprev,CV_RGB2GRAY );
     IplImage *curr_frame;
     Mat prevgray, gray, flow, cflow, frame;
     namedWindow("flow", 1);
     while (1)
     {
            curr_frame=cvQueryFrame(cap);
            frame=curr_frame;
            cvtColor(frame, gray, CV_BGR2GRAY);
	        if( prevgray.data && gray.data)
	        {
	            //calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 1, 5, 10, 5, 1.1, OPTFLOW_FARNEBACK_GAUSSIAN);
	            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
	            cvtColor(prevgray, cflow, CV_GRAY2BGR);
	            drawOptFlowMap(flow, cflow, 16, 1.5, CV_RGB(0, 255, 0));
	            imshow("flow", cflow);
	        }
	        else
                cout<<"No data"<<endl;
	       // if(waitKey(30)>=0)
	         //   break;
           std::swap(prevgray, gray);
           cvWaitKey(0);
     }
}

