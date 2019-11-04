#include "BgSubtract.h"

using namespace cv;
using namespace std;


/*
void RGB2HSI(Mat bgr, Mat hsi)  // OpenCV uses BGR format
 {
    for (int i = 0; i < bgr.rows; ++i)
    {
        const Vec3b* bgr_row = bgr.ptr<Vec3b>();
        Vec3b* hsi_row = hsi.ptr<Vec3b>();
        for (int j = 0; j < bgr.cols; ++j)
        {
            double B = bgr_row[j][0];
            double G = bgr_row[j][1];
            double R = bgr_row[j][2];
            double H ;
            double S ;
            double I ;
            double a;

            if (R < G && R < B)
                a = R;
            if (G < R && G < B)
                a = G;
            if (B < G && B < R)
                a = B;
            I = (R+G+B)/3.0;
            S = 1 - 3.0/(R+G+B)*a;
            if (S == 0.0)
            {
                H = 0.0;
            }
            else
            {
                if (B <= G)
                H = acos((((R-G)+(R-B))/2.0)/(sqrt((R-G)*(R-G) + (R-B)*(G-B))));
                else
                {
                    if (B > G)
                    H = 2*PI - acos((((R-G)+(R-B))/2.0)/(sqrt((R-G)*(R-G) + (R-B)*(G-B))));
                }


            }

            hsi_row[j] = Vec3b(100.0,100.0,255.0);
        }
    }
 }
*/

int FrameDifferenceHSI()
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
     IplImage *diff=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), prev_frame->depth, 1);
     IplImage *prevHSI=cvCloneImage(prev_frame);
     IplImage *currHSI=cvCreateImage(cvSize( prev_frame->width, prev_frame->height ), prev_frame->depth, 3);

     cvCvtColor(prev_frame,prevHSI,CV_RGB2HSV);

     IplImage* ph = cvCreateImage( cvGetSize(prevHSI), IPL_DEPTH_8U, 1 );
     IplImage* ps = cvCreateImage( cvGetSize(prevHSI), IPL_DEPTH_8U, 1 );
     IplImage* pv = cvCreateImage( cvGetSize(prevHSI), IPL_DEPTH_8U, 1 );


// Split image onto the color planes
    cvSplit( prevHSI, ph, ps, pv, NULL );

     IplImage* ch = cvCreateImage( cvGetSize(prevHSI), IPL_DEPTH_8U, 1 );
     IplImage* cs = cvCreateImage( cvGetSize(prevHSI), IPL_DEPTH_8U, 1 );
     IplImage* cv = cvCreateImage( cvGetSize(prevHSI), IPL_DEPTH_8U, 1 );

     while (1)
     {

          curr_frame=cvQueryFrame(cap);
          if(curr_frame==NULL) break;
          cvCvtColor(curr_frame,currHSI,CV_BGR2HSV);
          //Mat bgr(curr_frame);
          //Mat hsi=cvCreateMat(curr_frame->height,curr_frame->width,CV_8UC3);

          //RGB2HSI(bgr,hsi);
          //IplImage* image2=cvCloneImage(&(IplImage)image1);
          //currHSI =cvCloneImage(&(IplImage)hsi);
          cvSplit( currHSI, ch, cs, cv, NULL );
          cvAbsDiff(cv,pv,diff);

          cvThreshold(diff,diff,20.0,255.0,THRESH_BINARY | CV_THRESH_OTSU); //CV_THRESH_BINARY| CV_THRESH_OTSU
          cvShowImage("Current Frame",curr_frame);
          cvShowImage("Difference",diff);
          //imshow("Hello",hsi);

          char key=cvWaitKey(30);
          if(key==27) break;
           cvCopyImage(cv,pv);
          //cvReleaseImage(&pv);
     }
     cvReleaseImage(&ph);
     cvReleaseImage(&ps);
     cvReleaseImage(&pv);
     cvReleaseImage(&ch);
     cvReleaseImage(&cs);
     cvReleaseImage(&cv);

     cvReleaseImage(&currHSI);
     cvReleaseImage(&prevHSI);
      cvReleaseImage(&diff);
      cvDestroyWindow( "dest" );
      cvReleaseCapture( &cap );
      return 0;
}

