#include "BgSubtract.h"
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include "./kmeans/test.h"
using namespace cv;
using namespace std;


int phase2_kdashmeans(Mat datapoints, int initial_k, float E,Mat &centers, Mat &labels, int iterations, float *clustervar)
{
	int NClusters= initial_k ;

	int nFeatures=datapoints.cols;
	int nInstances=datapoints.rows;

	int steps=0;
	E=10;
	//cout<<endl<<"KMeans Successful:...."<<endl;
	//cout<<endl<<"Second phase started please wait..."<<endl;
	while(steps< iterations)
	{
		steps++;
		Mat freq=Mat::zeros(NClusters,1,CV_32F);
		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			freq.at<float>(cluster_idx)=freq.at<float>(cluster_idx) + 1 ;
		}

		Mat info=Mat::zeros(NClusters,1,CV_32F);

		double sum=0;
		for(int i=0;i<NClusters;i++)
		{

			freq.at<float>(i)=freq.at<float>(i)/(nInstances);
			if(freq.at<float>(i)!=0)
			{
				sum=sum+(double)freq.at<float>(i);
				info.at<float>(i) = -E*log10(freq.at<float>(i)) / log10 (2);
			}

		}


		for(int i=0;i<nInstances;i++)
		{
			double mindist;
			int index;

			/*Assume current labeling is correct */
			index=labels.at<int>(i,0);

			mindist = cv::norm(cv::Mat(datapoints.row(i)),cv::Mat(centers.row(index)),NORM_L2)-info.at<float>(index);
			for(int k=0;k<NClusters;k++)
			{
				//get the euclidean distance from each cluster centroid
				if(freq.at<float>(k)==0)
					continue;
				double res = cv::norm(cv::Mat(datapoints.row(i)),cv::Mat(centers.row(k)),NORM_L2)-info.at<float>(k);
				if(mindist >res)
				{
					mindist = res;
					index = k;
				}
			}
			labels.at<int>(i,0)=index;
		}
		/*update the centroid*/

		freq=Mat::zeros(NClusters,1,CV_32F);
		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			freq.at<float>(cluster_idx)=freq.at<float>(cluster_idx) + 1 ;
		}

		Mat tempCenter=Mat::zeros(centers.rows,centers.cols,centers.type());

		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			if(freq.at<float>(cluster_idx)!=0)
			{
				for(int k=0;k<nFeatures;k++)
					tempCenter.at<float>(cluster_idx,k)=tempCenter.at<float>(cluster_idx,k)+centers.at<float>(cluster_idx,k)/freq.at<float>(cluster_idx);

			}

		}

		for(int i=0;i<NClusters;i++)
			clustervar[i]=0.0f;
		for(int i=0;i<nInstances;i++)
		{
			int cluster_idx = labels.at<int>(i,0);
			double dist=cv::norm(cv::Mat(datapoints.row(i)),cv::Mat(centers.row(cluster_idx)),NORM_L2);
			if(freq.at<float>(cluster_idx)!=0)
				clustervar[cluster_idx]=clustervar[cluster_idx]+dist/freq.at<float>(cluster_idx);
		}
		int total=0;
		for(int i=0;i<NClusters;i++)
			total=total +(int)freq.at<float>(i);

		//cout<<norm(tempCenter,centers);
		centers=tempCenter;
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
        

//	kmeans(datapoints,NClusters, labels,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),3, KMEANS_PP_CENTERS,centers);

        
        //cout<<labels;
        cudaKmeans(iterations, n,  d, initial_k,(float *)datapoints.ptr(),(int *)labels.ptr(),(float *)centers.ptr());
        //cout<<"After clustering"<<labels;
        phase2_kdashmeans(datapoints,initial_k,E,centers,labels,iterations,clustervar);
 

	return 0;
}




