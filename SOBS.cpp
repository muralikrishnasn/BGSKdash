/* Uses only V channel of HSV for BG-Subtraction*/

#include "BgSubtract.h"
using namespace cv;
using namespace std;
#define MAPSIZE 3
#define pi 3.141593
#define THRESHOLD 60
#define gamma 0.01;
#define K 100 /* initial frames for training*/
#define VAR 0.5
#define WINDOW_SIZE 3
/*struct colorhsv
{
    uchar h,s,v;
};
typedef struct colorhsv colorHSV;*/

int DtSOBS(double val)
{
    if (val > THRESHOLD)
      return 1;
    return 0;
}
double gaussian2(int x, int y, double mean, double sigma)
{
    double A=1.0/((2.0*pi)*sigma*sigma);

    return(A*exp(-(((x-mean)*(x-mean)+(y-mean)*(y-mean))/(2*sigma*sigma))));
}

double distance(int i,int j)
{
    return (sqrt((i-j)*(i-j)));
}
/*
double distance(colorHSV p,colorHSV q)
{

}*/
int updatemodel(uchar *map,int &x,int &y,int s1, int v, uchar *maskim,int BM)
{

        /*update the neighbourhood of k x k*/
    int k=3;
    int shft=floor(k/2.0);
    float alpha;

    double gaussian2dweights[k][k];

    for (int i=-shft;i<=shft;i++)
      for (int j=-shft;j<=shft;j++)
        gaussian2dweights[i+shft][j+shft]=gaussian2(i,j,0.0,VAR);


    int i1=0;
    int j1=0;
    for(int i=x-shft; i <=x + shft ; i++)
    {
       j1=0;
       for(int j=y-shft; j <= y + shft; j++)
       {
           alpha=gaussian2dweights[i1][j1]*(1-DtSOBS(BM))*gamma;
           map[i*s1+j]= (1-alpha)*map[i*s1+j]+ alpha*v;
          *maskim=DtSOBS(BM)*255; //DtSOBS is a thresholding Function
          j1++;
       }
       i1++;
    }
   //myfile.close();
   return 0;

}

int findbestmatch(uchar *map,int &x,int &y,int s1, int v)
{
    int BM=9999;
    int mval;

    for(int k=0;k<MAPSIZE;k++)
    {
       for(int l=0;l<MAPSIZE;l++)
        {
            mval=distance(map[k*s1+l],v);
            if(BM > mval)
             {
                 BM = mval;
                 x=k; y=l;
             }
        }
    }


    return BM;
}

//double distanceHSV(double h,double s,)

int SOBS()
{
     //String Filename="b%5.bmp";
     //CvCapture *cap= cvCaptureFromFile( "dataset/MovedObject/b%5d.bmp" );
     CvCapture *cap= cvCaptureFromFile( "Dataset_EvaluationCode/Wallflower/Bootstrap/b%5d.bmp" );
     //CvCapture *cap= cvCaptureFromFile( "Ori_WaterObject.avi" );
     if (cap==NULL)
     {
        fprintf( stderr, "Cannot open AVI!\n" );
        return 1;
     }
     cvNamedWindow( "HSV", CV_WINDOW_AUTOSIZE );


     IplImage *curr_frame=cvQueryFrame(cap);

     if(!curr_frame) exit(0);
     cout<<"Frame size: "<<curr_frame->width<<"x"<<curr_frame->height<<endl;
     cout<<"Depth: "<<curr_frame->depth<<endl;
     cout<<"Channel: "<<curr_frame->nChannels<<endl;


     IplImage *hsvImage=cvCreateImage(cvSize( curr_frame->width, curr_frame->height ), curr_frame->depth, 3);
     cvCvtColor(curr_frame,hsvImage,CV_BGR2HSV);

     IplImage* ch = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
     IplImage* cs = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
     IplImage* cv = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
     IplImage* maskIm = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
// Split image onto the color planes
    cvSplit( hsvImage, ch, cs, cv, NULL );

    IplImage *hmap=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 1);
    IplImage *smap=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 1);
    IplImage *vmap=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 1);
/*
     // loop over rows of image
    for (int i = 5; i < cv.rows - 5; i++) {
        // loop over cols of image
        for (int j = 5; j < cv.cols - 5; j++) {
            cv::Mat sigma(3, 3, CV_32FC1);
            // loop over covariance matrix
            for (int a = 0; a < WINDOW_SIZE * 2 + 1; a++) {
                for (int b = 0; b < WINDOW_SIZE * 2 + 1; b++) {
                    // loop over window
                    for (int k = i - WINDOW_SIZE; k <= i + WINDOW_SIZE; k++) {
                        for (int l = j - WINDOW_SIZE; l <= j + WINDOW_SIZE; l++) {
                            sigma.at<float>(a, b) += (cv.at<Vec3f>(k, l)[a] - mu.at<Vec3f>(i, j)[a]) *
                                                     (cv.at<Vec3f>(k, l)[b] - mu.at<Vec3f>(i, j)[b]);
                        }
                    }
                    sigma.at<float>(a, b) *= (1.0 / ((WINDOW_SIZE * 2 + 1) * (WINDOW_SIZE * 2 + 1) - 1));
                }
            }
        }
    }
*/




    int inheight     = ch->height;
    int inwidth      = ch->width;
    int instep       = ch->widthStep/sizeof(uchar);

    int outheight     = hmap->height;
    int outwidth      = hmap->width;
    int outstep       = hmap->widthStep/sizeof(uchar);

        /* Map Initialization*/
    uchar* indata=(uchar *)cv->imageData;
    int shft=floor(MAPSIZE/2.0);
    uchar* outdata=(uchar *)vmap->imageData;

    uchar* maskdata=(uchar *)maskIm->imageData;
    int x,y;
    int BM; // dist of value of Best Matching pixel
    for(int i=0;i< ch->height; i++)
    {
        for(int j=0;j<ch->width; j++)
        {
            int val = indata[i*instep+j];
            int t1= (i*MAPSIZE)+ shft;
            int t2= (j*MAPSIZE)+ shft;
            for (int k=t1-shft; k <= t1+shft; k++)
                for (int l=t2-shft; l <= t2+shft; l++)
                       outdata[k*outstep + l]=val;
        }
    }



     while (1)
     {

          curr_frame=cvQueryFrame(cap);
          if(curr_frame==NULL) break;
          cvCvtColor(curr_frame,hsvImage,CV_BGR2HSV);
          cvSplit( hsvImage, ch, cs, cv, NULL );

          cvShowImage("HSV",curr_frame);
          /* Update the Bt(p) using Map*/
          for(int i=3;i< ch->height-3; i++)  //
            {
                for(int j=3;j<ch->width-3; j++)//
                {
                    int v=indata[i*instep+j];
                    int t1= (i*MAPSIZE)+ shft;
                    int t2= (j*MAPSIZE)+ shft;
                    uchar *m=&(outdata[(t1-1)*outstep+(t2-1)]);

                    BM=findbestmatch(m,x,y,outstep,v );
                    updatemodel(m,x,y,outstep,v, &maskdata[i*instep+j],BM);
                   // cout<<x<<" "<<y<<endl;
                }
            }
          cvShowImage("mask",maskIm);
          cvShowImage("vmap",vmap);
          char key=cvWaitKey(30);
          if(key==27) break;
     }
     cvReleaseImage(&ch);
     cvReleaseImage(&cs);
     cvReleaseImage(&cv);
     cvReleaseImage(&hmap);
     cvReleaseImage(&smap);
     cvReleaseImage(&vmap);
     cvReleaseImage(&hsvImage);
     cvDestroyWindow( "dest" );
     cvReleaseCapture( &cap );
      return 0;
}
