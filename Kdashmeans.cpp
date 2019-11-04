#include "BgSubtract.h"
//#include "Features.h"
#include<string>
#include<stdlib.h>
#include <fstream>
#include"./kmeans/test.h"

using namespace cv;
using namespace std;
RNG rng(12323);
#define NFEATURES 3 

//void gpuFunction(int imageWidth,int imageHeight, int colorStep,int grayStep,unsigned char *in,unsigned char *out);

int phase2_kdashmeans(Mat datapoints, int initial_k, float E,Mat &centers, Mat &labels, int iterations, float *clustervar)
{
	int NClusters= initial_k ;

	int nFeatures=datapoints.cols;
	int nInstances=datapoints.rows;

	int steps=0;
	E=10;

	while(steps< iterations)
	{
		steps++;
		Mat freq=Mat::zeros(NClusters,1,CV_32F);

		//Get the number of points in each cluster
		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			freq.at<float>(cluster_idx)=freq.at<float>(cluster_idx) + 1 ;
		}


		//Caculate the information conveyed by each cluster
		Mat info=Mat::zeros(NClusters,1,CV_32F);

		double sum=0;
		for(int i=0;i<NClusters;i++)
		{

			//Probability of Ci - taken as a non-zero
			freq.at<float>(i)=(freq.at<float>(i)+1)/(nInstances);
			sum=sum+(double)freq.at<float>(i); //sum should equals 1
			info.at<float>(i) = E*log10(freq.at<float>(i)) / log10 (2);

		}


		// Reassign cluster labels to points
		for(int i=0;i<nInstances;i++)
		{
			double mindist;
			int index;

			// Assume current labeling is correct 
			// Gives the index of the cluster to which the 
			// points belongs
			index=labels.at<int>(i,0); 

			mindist = cv::norm(cv::Mat(datapoints.row(i)),cv::Mat(centers.row(index)),NORM_L2)-info.at<float>(index);
			for(int k=0;k<NClusters;k++)
			{
				double res = cv::norm(cv::Mat(datapoints.row(i)),cv::Mat(centers.row(k)),NORM_L2)-info.at<float>(k);
				if(mindist >res)
				{
					mindist = res;
					index = k;
				}
			}
			// Assigning the new label by reducing the uncertainity
			// taking into account the information conveyed by each cluster 
			labels.at<int>(i,0)=index;
		}
		//update the centroid

		freq=Mat::zeros(NClusters,1,CV_32F);
		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			freq.at<float>(cluster_idx)=freq.at<float>(cluster_idx) + 1 ;
		}

		//Update the centroid
		Mat sumRow=Mat::zeros(centers.rows,centers.cols,centers.type());
		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			for(int k=0;k<nFeatures;k++)
				sumRow.at<float>(cluster_idx,k)+=datapoints.at<float>(i,k);

		}
		for(int i=0;i<NClusters;i++)
		{
			for(int k=0;k<nFeatures;k++)
				centers.at<float>(i,k)=sumRow.at<float>(i,k)/(freq.at<float>(i)+1);
		}

	}

	// Find the varince of each cluster
	Mat freq=Mat::zeros(NClusters,1,CV_32F);
	for(int i=0;i<nInstances;i++)
	{
		int cluster_idx = labels.at<int>(i,0);
		freq.at<float>(cluster_idx)=freq.at<float>(cluster_idx) + 1 ;
	}

	Mat sepCluster[NClusters];
	for(int i=0;i<nInstances;i++)
	{
		int cluster_idx = labels.at<int>(i,0);
		sepCluster[cluster_idx].push_back(datapoints.row(i));
	}

	for(int i=0;i<NClusters;i++)
	{
		Mat covar;
		if(sepCluster[i].data)
		{
			Mat mean;
			double min,max;
			//To find the spread in each dimension
			// we are choosing the maxximum spread as the deviation
			calcCovarMatrix(sepCluster[i],covar, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS, CV_32F); 

			covar=covar/nInstances;
			/*float maxvar = covar.at<float>(0,0);

			  for(int j=1;j< nFeatures;j++)
			  {
			  float v=covar.at<float>(j,j);
			  if(v > maxvar)
			  maxvar= v;
			  }*/
			cv::minMaxLoc(covar, &min, &max);

			clustervar[i] = sqrt(max);		
		}        
	}

	for(int i=0;i<NClusters;i++)
	{

		if(freq.at<float>(i)==0.0)
			for(int k=0;k<nFeatures;k++)
				centers.at<float>(i,k) = -9999;
	}

	return 0;
}

/* Input Array Inp contains the data to be clustered which has a dimension
   of number_of_data_points X number of features*/
int Kdashmeans_algo(Mat datapoints, int initial_k, float E,Mat &centers, Mat &labels, int iterations, float *clustervar)
{
	int n=datapoints.rows;
	int d=datapoints.cols;
	centers=Mat::zeros(initial_k,d,CV_32F);
	labels=Mat::zeros(n,1,CV_32S);


//	kmeans(datapoints,initial_k, labels,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),3, KMEANS_PP_CENTERS,centers);

	//cout<<labels;
	cudaKmeans(iterations, n,  d, initial_k,(float *)datapoints.ptr(),(int *)labels.ptr(),(float *)centers.ptr());

	//cout<<"After clustering"<<labels;
	phase2_kdashmeans(datapoints,initial_k,E,centers,labels,iterations,clustervar);


	return 0;
}


int Kdashmeans(char *dataset, char *bgModelFile)
{
	//IplImage *curr_frame;
	int nTrainingFrames=10;
	Vector<Mat> frameList;
	int steps=0;
	VideoCapture cap(dataset);
	int rows,cols;

	Mat img;

	cout<<"The dataset is :"<<dataset<<endl;
	cout<<endl<<"Training Started: building BG model"<<endl;
	while (steps < nTrainingFrames)
	{
		steps++;
		cap>>img;
              //  imshow("inputimage",img);
		//if(waitKey(1) >= 0) break;
              //  cout<<"The number of channels is:"<< img.channels()<<endl;
                if(!img.data)
			break;
                if(img.channels()<3)
                {
                   std::vector<cv::Mat> imgSeq;
                   for(int p=0;p<3;p++) 
  		       imgSeq.push_back(img.clone());
		   cv::merge(imgSeq, img);
                      
		}

		//GaussianBlur( img, img, Size(5, 5), 0, 0 );
		 
		//Changed for KTH dataset 16-April-2018    
         	pyrDown( img, img, Size( img.cols/2, img.rows/2 ));
         
	       //colorNormalize(img,img);
		//combineColor_Texture(img);

                
		

		rows=img.rows;
		cols=img.cols;

		frameList.push_back(img.clone());
	}

	int nFeatures=NFEATURES;


	ofstream outfile;
	outfile.open(bgModelFile, ios::out);

	int complStatus=0;
	int complPerc=0;
	for(int y=0; y < cols ; y++)
	{
		for(int x = 0; x < rows ; x++)
		{
			//outfile<<"x = "<<x<<" y ="<<y<<" ";
			Mat dpoints(nTrainingFrames,nFeatures,CV_32F);
			int loc=0;
			for (Vector<Mat>::iterator iter = frameList.begin(); iter != frameList.end(); ++iter)
			{
				Mat temp=*iter;
				for(int z = 0;z < nFeatures; z++)
				{
                                        
					dpoints.at<float>(loc, z) = temp.at<Vec3b>(x,y)[z];
				}



				loc++;

			}
			Mat centers,labels;

			int NClusters=10;
			float var[NClusters];

			// Call the K'-Means algorithm
			Kdashmeans_algo(dpoints, NClusters, 10.0,centers,labels, 100,var);


			// Updated centers after second phase is used to find the
			// number of components K' < K
			int ncomponents=0;

			for(int cluster_idx=0;cluster_idx<NClusters;cluster_idx++)
			{
				int flag=0;
				for(int i=0;i< nFeatures;i++)
				{
					if(centers.at<float>(cluster_idx,i) ==-9999) //eliminated cluster
					{
						flag=1; // if it a removed cluster in phase-2 
						break;
					}

				}
				if(!flag)
					ncomponents++;
			}
			outfile<<ncomponents<<" ";
			for(int cluster_idx=0;cluster_idx<NClusters;cluster_idx++)
			{

				int flag=0;
				for(int i=0;i< nFeatures;i++)
				{

					if(centers.at<float>(cluster_idx,i)==-9999)
					{
						flag=1;
						break;
					}
				}
				if(!flag)
				{
					for(int i=0;i< nFeatures;i++)
						outfile<<centers.at<float>(cluster_idx,i)<<" ";
					outfile<<var[cluster_idx]<<" ";
				}
			}
			outfile<<endl;

			complStatus++;
			cout<<(char)13;
			if(((100 * complStatus)%(rows*cols))==0)
			{
				complPerc++;
				cout<<complPerc<<"%";
			}


		}

	}
	cout<<endl<<"bgModel File written in Path "<<bgModelFile<<endl;
	outfile.close();
	return 0;
}

