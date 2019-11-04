#include "BgSubtract.h"

using namespace cv;
using namespace std;
int RunningGaussianAverage()
{

     CvCapture *cap= cvCaptureFromFile( "1-11200.avi" );
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

     IplImage *Im_rgb32f=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 3);
     IplImage *Im_gray32f=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 1);

     IplImage *currGrayIm32f=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 1);

     IplImage *prevAvg=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 1);
     IplImage *diff=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 1);
     IplImage *disp=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 1);
     IplImage *oldAvg=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), IPL_DEPTH_32F, 1);

     cvConvert(prev_frame,Im_rgb32f);
     cvCvtColor(Im_rgb32f,Im_gray32f,CV_RGB2GRAY );
    //previous image average is Im_gray32
     int totalframes=1;
     while (1)
     {
          curr_frame=cvQueryFrame(cap);
          if(curr_frame==NULL) break;
          cvConvert(curr_frame,Im_rgb32f);
          //current gray image is currGrayIm32
          cvCvtColor(Im_rgb32f,currGrayIm32f,CV_RGB2GRAY); //xn+1


         //compute the new average. Old average is copied to prevAvg
          cvCopyImage(Im_gray32f,prevAvg); //
          cvCopyImage(Im_gray32f,oldAvg); //

          // For display
          cvConvertScale(prevAvg,disp,(1.0/255.0)); // 1/n+1
          cvShowImage("Average",disp);

          // compute (curr-prevAvg) / n+1
          cvSub(currGrayIm32f,prevAvg,Im_gray32f);
          cvScale(Im_gray32f,Im_gray32f,(1.0/(totalframes+1)));

          cvAdd(prevAvg,Im_gray32f,Im_gray32f,NULL);
          totalframes++;

          // Diffrence from average image
          cvAbsDiff(Im_gray32f,currGrayIm32f,diff);

          //cvAbsDiff(oldAvg,Im_gray32f,disp);
          //cvScale(disp,disp,(1.0/255.0));
          //cvShowImage("Difference in Avg",disp);


          cvConvertScale(diff,diff,(1.0/255.0));
          cvThreshold(diff,diff,0.06,1.0,CV_THRESH_BINARY); //CV_THRESH_BINARY| CV_THRESH_OTSU  THRESH_BINARY
          cvShowImage("Dest",diff);


          char key=cvWaitKey(30);
          if(key==27) break;
          // cvCopyImage(cv,pv);
          //cvReleaseImage(&pv);*/
     }
//     cvReleaseImage(&ph);
//     cvReleaseImage(&ps);
//     cvReleaseImage(&pv);
//     cvReleaseImage(&ch);
//     cvReleaseImage(&cs);
//     cvReleaseImage(&cv);

//     cvReleaseImage(&currHSI);
//     cvReleaseImage(&prevHSI);
//      cvReleaseImage(&diff);
      cvDestroyWindow( "dest" );
      cvReleaseCapture( &cap );
      return 0;
}


