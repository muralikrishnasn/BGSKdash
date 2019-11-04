#include "BgSubtract.h"
#include<time.h>
using namespace cv;
using namespace std;
#define MAPSIZE 3
#define pi 3.141593
#define THRESHOLD 40
#define gamma 0.6
#define K 100 /* initial frames for training*/
#define VAR 0.5
#define h 2 //neighbourhood coherence factor
int k=3;  //window size of BM Pixel's neighburhood
int shft=floor(k/2.0);

int DtSC_SOBS(double ncf)
{
    if (ncf <= 0.5)
      return 1;
    return 0;
}
double sc_gaussian2D(int x, int y, double mean, double sigma)
{
    double A=1.0/((2.0*pi)*sigma*sigma);

    return(A*exp(-(((x-mean)*(x-mean)+(y-mean)*(y-mean))/(2*sigma*sigma))));
}

double sc_distance(int h1,int s1,int v1, int h2,int s2,int v2)
{

/* delta C perception distance from google-books*/
/*    double x1=h1*2.0;
    double y1=s1/255.0;
    double z1=v1/255.0;

    double x2=h2*2.0;
    double y2=s2/255.0;
    double z2=v2/255.0;

    return ((255.0/sqrt(5.0))*sqrt(pow((z1-z2),2) + pow((y1*cos(x1*pi/180.0)-y2*cos(x2*pi/180.0)),2)+ pow((y1*sin(x1*pi/180.0)-y2*sin(x2*pi/180.0)),2)));
*/

/* Ref: Change Detection in Color Image  (vscos2ph,vssin2ph,v) */
    double x1=(v1/255.0)*(s1/255.0)*cos(h1*2*pi/180.0);
    double y1=(v1/255.0)*(s1/255.0)*sin(h1*2*pi/180.0);
    double z1=(v1/255.0);

    double x2=(v2/255.0)*(s2/255.0)*cos(h2*2*pi/180.0);
    double y2=(v2/255.0)*(s2/255.0)*sin(h2*2*pi/180.0);
    double z2=(v2/255.0);
    return 255*sqrt(((x1-x2)*(x1-x2))+ ((y1-y2)*(y1-y2))+((z1-z2)*(z1-z2)));

  //  int x1=h1,x2=h2,y1=s1,y2=s2,z1=v1,z2=v2;

  //  return sqrt(((x1-x2)*(x1-x2))+ ((y1-y2)*(y1-y2))+((z1-z2)*(z1-z2)));

}
int sc_updatemodel(uchar *map1,uchar *map2,uchar *map3,int &x,int &y,int s1, int v[], uchar *maskim,int ncf)
{
    /*update the neighbourhood of k x k*/
    float alpha;
    double gaussian2dweights[k][k];

    for (int i=-shft;i<=shft;i++)
      for (int j=-shft;j<=shft;j++)
        gaussian2dweights[i+shft][j+shft]=sc_gaussian2D(i,j,0.0,VAR);

    int i1=0;
    int j1=0;
    for(int i=x-shft; i <=x + shft ; i++)
    {
      j1=0;
      for(int j=y-shft; j <= y + shft; j++)
       {

          alpha=gaussian2dweights[i1][j1]*(1-DtSC_SOBS(ncf))*gamma;
          map1[i*s1+j]= (1-alpha)*map1[i*s1+j]+ alpha*v[0];
          map2[i*s1+j]= (1-alpha)*map2[i*s1+j]+ alpha*v[1];
          map3[i*s1+j]= (1-alpha)*map3[i*s1+j]+ alpha*v[2];


          *maskim=DtSC_SOBS(ncf)*255;
          j1++;
       }
       i1++;
    }
 return 0;
}

int sc_findbestmatch(uchar *map1,uchar *map2,uchar *map3,int &x,int &y,int s1, int v[])
{
    int BM=9999;
    double mval;
    for(int k=0;k<MAPSIZE;k++)
    {
       for(int l=0;l<MAPSIZE;l++)
        {
            //cout<<map1[k*s1+l]<< " "<<map2[k*s1+l]<<" "<<map3[k*s1+l]<<endl;
            mval=sc_distance(map1[k*s1+l],map2[k*s1+l],map3[k*s1+l],v[0],v[1],v[2]);
            //mval=distance(map[k*s1+l],v);
            if(BM > mval)
             {
                 BM = mval;
                 x=k; y=l;

             }
        }
    }


   return BM;
}


/*Neighbourhood Coherence Factor */
double NCF(uchar *indata1,uchar *indata2,uchar *indata3,int instep,uchar *outdata1,uchar *outdata2,uchar *outdata3,int outstep,int i,int j,int &x,int &y)
{
    uchar *m1,*m2,*m3;

    int v[3]={0};
    int count=0;

    for(int qx1=-h; qx1<=h; qx1++)
    {
       for(int qy1=-h; qy1<=h; qy1++)
       {
           v[0]=indata1[(i+qx1)*instep+(j+qy1)];
           v[1]=indata2[(i+qx1)*instep+(j+qy1)];
           v[2]=indata3[(i+qx1)*instep+(j+qy1)];

           int t1= ((i+qx1)*MAPSIZE)+ shft;
           int t2= ((j+qy1)*MAPSIZE)+ shft;

           m1=&(outdata1[(t1-1)*outstep+(t2-1)]);
           m2=&(outdata2[(t1-1)*outstep+(t2-1)]);
           m3=&(outdata3[(t1-1)*outstep+(t2-1)]);

           int BM=sc_findbestmatch(m1,m2,m3,x,y,outstep,v);

           if(BM <= THRESHOLD)
                      count++;
        }
    }
     return ((double)count/pow(2.0*h+1,2));
}

int SC_SOBS_HSV()
{
     //String Filename="b%5.bmp";
     //CvCapture *cap= cvCaptureFromFile( "dataset/Bootstrap/b%5d.bmp" );
     //CvCapture *cap= cvCaptureFromFile("H:\BGSLibrary\dataset\video.avi");

     //CvCapture *cap= cvCaptureFromFile( "dataset/MovedObject/b%5d.bmp" );
     CvCapture *cap= cvCaptureFromFile( "Ori_WaterObject.avi" );
     //CvCapture *cap = cvCaptureFromCAM(0);
     if (cap==NULL)
     {
        fprintf( stderr, "Cannot open AVI!\n" );
        return 1;
     }
     cvNamedWindow( "HSV", CV_WINDOW_AUTOSIZE );

     cvWaitKey(30);

     IplImage *curr_frame=cvQueryFrame(cap);

     if(!curr_frame) exit(0);
     cout<<"Frame size: "<<curr_frame->width<<"x"<<curr_frame->height<<endl;
     cout<<"Depth: "<<curr_frame->depth<<endl;
     cout<<"Channel: "<<curr_frame->nChannels<<endl;


     IplImage *hsvImage=cvCreateImage(cvSize( curr_frame->width, curr_frame->height ), curr_frame->depth, 3);
     cvCvtColor(curr_frame,hsvImage,CV_BGR2HSV);
  //   cvCopyImage(curr_frame,hsvImage);

     IplImage* ch = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
     IplImage* cs = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
     IplImage* cv = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );
     IplImage* maskIm = cvCreateImage( cvGetSize(hsvImage), IPL_DEPTH_8U, 1 );

// Split image onto the color planes
    cvSplit( hsvImage, ch, cs, cv, NULL );

    IplImage *hmap=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 1);
    IplImage *smap=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 1);
    IplImage *vmap=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 1);

    int inheight     = ch->height;
    int inwidth      = ch->width;
    int instep       = ch->widthStep/sizeof(uchar);

    int outheight     = hmap->height;
    int outwidth      = hmap->width;
    int outstep       = hmap->widthStep/sizeof(uchar);

        /* Map Initialization*/
    uchar* indata1=(uchar *)ch->imageData;
    uchar* indata2=(uchar *)cs->imageData;
    uchar* indata3=(uchar *)cv->imageData;

    int shft=floor(MAPSIZE/2.0);
    uchar* outdata1=(uchar *)hmap->imageData;
    uchar* outdata2=(uchar *)smap->imageData;
    uchar* outdata3=(uchar *)vmap->imageData;

    uchar* maskdata=(uchar *)maskIm->imageData;
    uchar *m1;
    uchar *m2;
    uchar *m3;

    int v[3]={0};
    int x,y;
    int BM;// dist of value of Best Matching pixel
    double ncf=0.0;

    for(int i=0;i< ch->height; i++)
    {
        for(int j=0;j<ch->width; j++)
        {
            int val1 = indata1[i*instep+j];
            int val2 = indata2[i*instep+j];
            int val3 = indata3[i*instep+j];
            int t1= (i*MAPSIZE)+ shft;
            int t2= (j*MAPSIZE)+ shft;
            for (int k=t1-shft; k <= t1+shft; k++)
            {
                for (int l=t2-shft; l <= t2+shft; l++)
                {
                    outdata1[k*outstep + l]=val1;
                    outdata2[k*outstep + l]=val2;
                    outdata3[k*outstep + l]=val3;
                }

            }

        }
    }

     IplImage *mapim=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 3);
     IplImage *outim=cvCreateImage(cvSize( curr_frame->width*MAPSIZE, curr_frame->height*MAPSIZE), curr_frame->depth, 3);
     while (1)
     {

          curr_frame=cvQueryFrame(cap);
          if(curr_frame==NULL) break;


          cvCvtColor(curr_frame,hsvImage,CV_BGR2HSV);
  //        cvCopyImage(curr_frame,hsvImage);
          cvSplit( hsvImage, ch, cs, cv, NULL );
          cvShowImage("HSV",hsvImage);
          /* Update the Bt(p) using Map*/

         time_t start=clock();//         #pragma omp parallel for

          for(int i=5;i< ch->height-5; i++)
            {
                for(int j=5;j<ch->width-5; j++)
                {

                     int t1= (i*MAPSIZE)+ shft;
                     int t2= (j*MAPSIZE)+ shft;

                     m1=&(outdata1[(t1-1)*outstep+(t2-1)]);
                     m2=&(outdata2[(t1-1)*outstep+(t2-1)]);
                     m3=&(outdata3[(t1-1)*outstep+(t2-1)]);



                     v[0]=indata1[i*instep+j];
                     v[1]=indata2[i*instep+j];
                     v[2]=indata3[i*instep+j];

                     ncf=NCF(indata1,indata2,indata3,instep,outdata1,outdata2,outdata3,outstep,i,j,x,y);

                     BM=sc_findbestmatch(m1,m2,m3,x,y,outstep,v);

                     sc_updatemodel(m1,m2,m3,x,y,outstep,v, &maskdata[i*instep+j], ncf);
                   // cout<<x<<" "<<y<<endl;
                }
            }
            time_t end=clock();
            cout<<(((double)end-start)/CLOCKS_PER_SEC)<<endl;
        //Color Mask
           cv::Mat a(curr_frame);
           cv::Mat b(maskIm);
           cv::Mat c;
           cvtColor( b, c, CV_GRAY2BGR );
           cv::Mat res=a & c;
           imshow("masked",res);



  //          cvShowImage("Mask",maskIm);
  //          cvMerge(hmap, smap, vmap, NULL, mapim);
  //          cvShowImage("B-Map",mapim);

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
     cvReleaseImage(&outim);
     cvReleaseImage(&mapim);
     cvDestroyWindow( "dest" );
     cvReleaseCapture( &cap );
      return 0;
}
