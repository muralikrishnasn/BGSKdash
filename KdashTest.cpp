#include"BgSubtract.h"
#include "Features.h"
#include<string>
#include<stdlib.h>
#include<string>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<math.h>

using namespace std;

#define STD 2.5 //3.5
#define ALPHA 1 
#define GAMMA 1.25 //0.8
#define INF 99999
#define THRESH1 25
//#define WINDOW_SIZE 8

int KdashTest(char *testSet, char *dataModelFile, char *outputPath)//, char *clusterInfoFile)
{
	
	VideoCapture cap(testSet);

	// just to get the image dimension
	VideoCapture capTemp(testSet);

	int rows,cols;
	Mat im;
	int origRowsize,origColsize;
	capTemp>>im;
	if(!im.data)
	{
		cout<<"No Image data: Unable to get the dimension of the image..."<<endl;
		exit(0);
	}

        origRowsize=im.rows;
        origColsize=im.cols;

	pyrDown( im, im, Size( im.cols/2, im.rows/2 ));
	rows=im.rows;
	cols=im.cols;
	capTemp.release();

	int nFeatures=3;
	ifstream infile;
	infile.open(dataModelFile, ios::in);
	if(infile.fail()==true)
	{
		printf("****unable to open model file****\n");

	}

	int nComponents[rows*cols];
	Mat *var=new Mat[rows*cols];
	Mat *centers_nvar=new Mat[rows*cols];
	int pos=0;
	cout<<"Opening model file succeded:"<<endl;
	// Load the model file

	clock_t begin=clock();
	clock_t end;
	while(infile>>nComponents[pos])
	{
		centers_nvar[pos].create(nComponents[pos],nFeatures,CV_32F);
		var[pos].create(nComponents[pos],1,CV_32F);
		for(int i=0; i<nComponents[pos];i++)
		{

			for(int j=0; j< nFeatures ; j++)
			{
				double a;
				infile>>a;
				centers_nvar[pos].at<float>(i,j)=(float)a;
			}
			float v;
			infile>>v;
			var[pos].at<float>(i,0)=v;

		}

		pos++;
	}

	cout<<"Read the centroids:"<<endl;
	int frameno=1;

	cout<<"Wrinting result:"<<endl;
	//int totalFrames=capTemp.get(CV_CAP_PROP_FRAME_COUNT);
	//int complPerc=0;
	//int i=1;
	Mat prevgray;
	Mat flow;
	Mat flowUmat=Mat::zeros(rows,cols,CV_32FC2);
	Mat motionImage=Mat::zeros(rows,cols,CV_8UC1);
	vector<double> threshList;
	
	while (1)
	{
                
		Mat outMask=Mat::zeros(rows,cols,CV_8UC1);
		Mat dist = Mat::zeros(rows,cols,CV_32F);
		Mat img;

		cap>>img;
		if(!img.data)
		{
                       
			cout<<"No Image data found";
                        break;
			//return 0;
		}
                if(img.channels()<3)
                {
                   std::vector<cv::Mat> imgSeq;
                   for(int p=0;p<3;p++)
                       imgSeq.push_back(img.clone());
                   cv::merge(imgSeq, img);
                
                }
		//imshow("image",img);
                //if(waitKey(5) >= 0) break;
		Mat im;
		//GaussianBlur( img, img, Size(5 , 5), 0, 0 );
		
                //Changed for KTH dataset 16-April-2018
		pyrDown( img, img, Size( img.cols/2, img.rows/2 ));


		//colorNormalize(img,img);
		//combineColor_Texture(img);

		int pos=0;

		for(int y=0; y < cols ; y++)
		{
			for(int x = 0; x < rows ; x++)
			{

				int t=nComponents[pos];
				Vec3b intensity = img.at<Vec3b>(x, y);
				Mat c(1,nFeatures,CV_32F);
				c.at<float>(0,0)=(float)intensity.val[0];
				c.at<float>(0,1)=(float)intensity.val[1];
				c.at<float>(0,2)=(float)intensity.val[2];
				double d;

				//int foreground=1;
                                double mindist=INF;
                                double maxvar=0;

				for(int i=0;i < t; i++)
				{


					d=cv::norm(c,centers_nvar[pos].row(i),NORM_L2);

					float v=var[pos].at<float>(i,0);
                                          if(v > maxvar)
                                                maxvar=v;

					//if( d  < v*STD)
					//{
						//foreground = 0;
                                                if(mindist > d)
                                                     mindist = d;
						//break;
					//}
                                        //if(maxdist < d)
                                          //  maxdist = d;
				}
                                d=mindist;
                                //cout<<d<<"\t:";
                                //if(foreground)
                                  //  d=maxdist;
                                //else
                                  //  d=mindist;
                                
				/*		if(!foreground)
						{
				//write code for updating the centroid
				double mindist= cv::norm(c,centers_nvar[pos].row(0),NORM_L2);
				int minindx=0;
				for(int i=1;i < t; i++)
				{
				double dist=cv::norm(c,centers_nvar[pos].row(i),NORM_L2);
				if(mindist > dist)
				{
				minindx=i;
				mindist=dist;
				}
				}
				Mat tempMat(c);
				subtract(c,centers_nvar[pos].row(minindx),tempMat);

				// this is set as per the MIT material on online k means clustering
				double alpha=1.0;

				addWeighted(centers_nvar[pos].row(minindx), alpha,tempMat,0.0001 , 0.0, centers_nvar[pos].row(minindx));



				}*/
				//outMask.at<uchar>(x,y)=(uchar)foreground*255;

				//outMask.at<uchar>(x,y)=(uchar)foreground*255;

	                        if(d > 2.5 * maxvar)
                                   dist.at<float>(x,y) = d;//ALPHA*pow(d,GAMMA);

				pos++;
			}

		}
                //blur(dist, dist, Size(5,5));



		Mat img_bw;
                 
                normalize(dist, dist, 1, 0,NORM_MINMAX); //255
                pow(dist,GAMMA,dist);
		dist=ALPHA*dist * 255;
		//blur(dist, dist, Size(5,5));

                dist.convertTo(img_bw,CV_8UC1);
                
		//imshow("dist image",img_bw);
                //if(waitKey(1) >= 0) break;


                
		double thresh=cv::threshold(img_bw, img_bw, 0,255, CV_THRESH_BINARY | CV_THRESH_OTSU);             
//                cout<<thresh<<"\t";
	        //threshList.push_back(thresh);
                //bitwise_and(img_bw,outMask,img_bw);
                
                outMask=Mat::zeros(outMask.rows,outMask.cols,outMask.type());
                
		img_bw.copyTo(outMask);
		
                 
		//Mat temp;	
		//medianBlur (outMask , temp, 3);

                //temp.copyTo(outMask);                

		//imshow("Dist",outMask);
                //if(waitKey(1) >= 0) break;

                /*if(outMask.data)
		{

			vector< vector<Point> >  contours;
			vector<Vec4i> hierarchy;
			vector<int> small_blobs;
			double contour_area;
			Mat temp_image;

			// find all contours in the binary image

			outMask.copyTo(temp_image);
			findContours(temp_image, contours, hierarchy, CV_RETR_CCOMP,
					CV_CHAIN_APPROX_SIMPLE);
			outMask=Mat::zeros(outMask.rows,outMask.cols,outMask.type());

			// Find indices of contours whose area is less than `threshold` 
			if ( !contours.empty()) {
				for (size_t i=0; i < contours.size(); ++i) {
					contour_area = contourArea(contours[i]) ;
					int totalarea=rows*cols;
					if ( contour_area > 20  ) // at 10% of area
					{
						small_blobs.push_back(i);

					}

				}
			}


			// fill-in all small contours with zeros
			for (size_t i=0; i < small_blobs.size(); ++i) {
				drawContours(outMask, contours, small_blobs[i], cv::Scalar(255), 
						CV_FILLED, 8);
			}
		}
			 
			 if (!prevgray.empty()) 
			 {
			 motionImage=Mat::zeros(rows,cols,CV_8UC1);
			 if(img.channels()>=3)
			 cvtColor(img, im, COLOR_BGR2GRAY);
			 calcOpticalFlowFarneback(prevgray, im, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);
		// copy Umat container to standard Mat
		flowUmat.copyTo(flow);
		im.copyTo(prevgray);

		for (int y = 0; y < img.rows; y++) 
		{
		for (int x = 0; x < img.cols; x++)

		{
		const Point2f flowatxy = flow.at<Point2f>(y, x) * 10;
		if(flowatxy.x > 5 || flowatxy.y > 5)
		motionImage.at<uchar>(y,x)=255;


		}

		}
		}
		else
		{
		if(img.channels()>=3)
		cvtColor(img, im, COLOR_BGR2GRAY);
		im.copyTo(prevgray);

		}

		Mat res1,res2,res3;
		// either motion detected or object dissimlar to background
		// is detected
		bitwise_xor(motionHistory,outMask,res1); 
		//A moving Object is detected
		bitwise_and(motionHistory,outMask,res2);
		bitwise_or(res1,res2,res3); */
		//computeBackgroundNaiveBayes(motionImage,outMask,result);
		//namedWindow("prew", WINDOW_AUTOSIZE);
	        //imshow("prew",res3);
		//if(waitKey(5) >= 0) break; 
		
		//Mat outimg;
		//img.copyTo(outimg, outMask);                

		//pyrUp( outMask, outMask, Size( outMask.cols*2, outMask.rows*2 ));
		

                // Changed for KTH dataset on 16-April-2018
		pyrUp( outMask, outMask, Size(origColsize, origRowsize));

		end=clock();
		//imshow("outMask",outMask);
		//if(waitKey(1) >= 0) break;
		char filename[200];
		char a[10];
		sprintf(a,"bin%06d",frameno);
               
		strcpy(filename,outputPath);
		strcat(filename,a);
		strcat(filename,".png");

		//cvSaveImage(filename, &(IplImage(dist)));
		//cvSaveImage(filename, &(IplImage(outMask)));
		imwrite(filename,outMask);
		frameno++;

	}
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
	ofstream outfile;
        outfile.open("time.txt", std::ios_base::app);
        outfile<<endl<<"-------------------------------"<<endl;
        outfile<<testSet<<endl;
        outfile<<origRowsize<<"X"<<origColsize<<"\t"<<time_spent<<"\t"<<frameno<<endl;
        outfile<<"-------------------------------"<<endl;
	//cout<<"Threshold list"<<endl;
        //for(size_t i=0;i<threshList.size();i++)
          //   cout<<threshList[i]<<" ";
          
	return 0;
}






