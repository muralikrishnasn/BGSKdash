#include "BgSubtract.h"

using namespace cv;
using namespace std;
int FrameDifferenceGray()
{

     //CvCapture *cap= cvCaptureFromFile( "1-11200.avi" );
     CvCapture *cap= cvCaptureFromCAM(0);
     if (cap==NULL)
     {
        fprintf( stderr, "Cannot open AVI!\n" );
        return 1;
     }
     cvNamedWindow( "dest", CV_WINDOW_AUTOSIZE );


     IplImage *prev_frame=cvQueryFrame(cap);

     if(!prev_frame) exit(0);
     cout<<"Frame size: "<<prev_frame->width<<"x"<<prev_frame->height<<endl;
     cout<<"Depth: "<<prev_frame->depth<<endl;
     cout<<"Channel: "<<prev_frame->nChannels<<endl;

     IplImage *curr_frame;
     IplImage *grayprev=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), prev_frame->depth, 1);
     IplImage *graycurr=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), prev_frame->depth, 1);
     IplImage *diff=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), prev_frame->depth, 1);

     cvCvtColor(prev_frame,grayprev,CV_RGB2GRAY );
     while (1)
     {
          curr_frame=cvQueryFrame(cap);
          if(curr_frame==NULL) break;

          cvCvtColor(curr_frame,graycurr,CV_RGB2GRAY );
         // Mat m1(grayprev);
         // Mat m2(graycurr);
         // imshow("destination",abs(m2-m1));

          cvAbsDiff(graycurr,grayprev,diff);
          cvThreshold(diff,diff,180,255.0,CV_THRESH_BINARY | CV_THRESH_OTSU);

          cvShowImage("dest",diff);
          char key=cvWaitKey(30);
          if(key==27) break;
          cvCopyImage(graycurr,grayprev);
     }
      cvReleaseImage(&graycurr);
      cvReleaseImage(&grayprev);
      cvReleaseImage(&diff);

      cvDestroyWindow( "dest" );
      cvReleaseCapture( &cap );
      return 0;
}

