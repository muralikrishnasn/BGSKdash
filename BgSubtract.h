#include<stdio.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include<iostream>


#ifndef BGSUBTRACT_H_INCLUDED
#define BGSUBTRACT_H_INCLUDED

#define PI 3.142
using namespace cv;

int FrameDifferenceGray();
int FrameDifferenceHSI();
int RunningAverage();
int RunningGaussianAverage();
int SC_SOBS_HSV();
int SOBS();
int MyFlowAlgo();
int KdashTest(char *testSet, char *dataModelFile, char *outputPath);//, char *clusterInfoFile);
int Kdashmeans(char *dataset, char *bgModelFile);
int colorNormalize(Mat bgr_image, Mat &out);
int Kdashmeans_algo(Mat datapoints, int initial_k, float E, Mat &cneters,Mat &labels,int iterations,float *clustervar);
void ComputeFeatures(Mat inImage, Mat &outImage);

#endif // BGSUBTRACT_H_INCLUDED
