#include<stdio.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "BgSubtract.h"
#include<iostream>
#include<string.h>
#include<time.h>
#include "./kmeans/test.h"

using namespace std;
static int help()
{
    cout<<"ExecutableName [help] [training] [Inputs]"<<endl;
    cout<<"Training mode: char *dataset, char *bgModelFile"<<endl;
    cout<<"Test Mode: char*testSet, char *dataModelFile, char *outputPath "<<endl; //char *clusterInfoFile
    cout<<"Inputs: image sequence: ex: ./Dataset_EvaluationCode/Wallflower/wavingtrees/b%05d.bmp"<<endl;
return 0;
}
/*
void test_mat(cv::Mat mat)
{
    std::cout << "Channels: " << mat.channels() << std::endl;
}

int main(int argc, char* argv[])
{
    cv::Mat mat20(1024, 768, CV_8UC(20));
    test_mat(mat20);

    return 0;
}*/


int main(int argc, char *argv[])
{
    //FrameDifferenceGray();
    //FrameDifferenceHSI();
    //RunningAverage();
    //SC_SOBS_HSV();
    //SOBS();
    

//CUDA KMeans
    //tiny_test();
    //huge_test_double(); 
    if(argc < 3)
    {
        cout<<"Use help ...."<<endl;
        help();
        return -1;
    }
    else
    {
        if (strcmp(argv[1],"help")==0)
            help();
        else if(strcmp(argv[1],"training")==0)
        {
            clock_t begin=clock();
            Kdashmeans(argv[2],argv[3]);
            clock_t end=clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            cout<<"Time taken for training in secs"<<time_spent<<endl;
        }

        else
        {

           // clock_t begin=clock();
            KdashTest(argv[1],argv[2],argv[3] );//,argv[4]);
             //MyFlowAlgo(); // Optical flow algorithm
            //clock_t end=clock();

            //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            //cout<<"Time taken for testing in secs"<<time_spent<<endl;
        }

    }
return 0;
}

/* Example of LBP Computation using ComputerFeature function
int main()
{
    Mat im=imread("waterfall.bmp");
    if(!im.data)
    {
        cout<<"error reading file:"<<endl;
        return -1;
    }
    Mat temp(im.rows,im.cols,CV_8UC1);
    ComputeFeatures(im,temp);
    imshow("Output of LBP",temp);
    cvWaitKey(0);
}
*/
