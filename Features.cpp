#include"BgSubtract.h"
#include "Features.h"
#include<iostream>
using namespace std;
int colorNormalize(Mat bgr_image, Mat &out)
{
    // READ RGB color image and convert it to Lab

    cv::Mat bgr_image_f;
    bgr_image.convertTo(bgr_image_f, CV_32FC3);

    // Extract the color planes and calculate I = (r + g + b) / 3
    std::vector<cv::Mat> planes(3);
    cv::split(bgr_image_f, planes);

    cv::Mat intensity_f((planes[0] + planes[1] + planes[2]) / 3.0f);
    cv::Mat intensity;
    intensity_f.convertTo(intensity, CV_8UC1);


    //void divide(InputArray src1, InputArray src2, OutputArray dst, double scale=1, int dtype=-1)
    cv::Mat b_normalized_f;
    cv::divide(planes[0], intensity_f, b_normalized_f);
    cv::Mat b_normalized;
    b_normalized_f.convertTo(b_normalized, CV_8UC1, 255.0);


    cv::Mat g_normalized_f;
    cv::divide(planes[1], intensity_f, g_normalized_f);
    cv::Mat g_normalized;
    g_normalized_f.convertTo(g_normalized, CV_8UC1, 255.0);

    cv::Mat r_normalized_f;
    cv::divide(planes[2], intensity_f, r_normalized_f);
    cv::Mat r_normalized;
    r_normalized_f.convertTo(r_normalized, CV_8UC1, 255.0);


    vector<Mat> ch;
    ch.push_back(r_normalized);
    ch.push_back(g_normalized);
    ch.push_back(b_normalized);
    merge(ch, out);
    return 0;
}
void computeLBPFeatures(Mat inImage, Mat &outImage)
{
        if(inImage.channels()>=3)
                cvtColor(inImage,inImage,CV_BGR2GRAY);
        copyMakeBorder(inImage,inImage,1,1,1,1,BORDER_REPLICATE);
        outImage = Mat::zeros(inImage.rows-2, inImage.cols-2, CV_8UC1);
        for(int x=1; x < inImage.rows-1;x++)
        {
                for(int y=1; y< inImage.cols-1;y++)
                {
                        unsigned char center = inImage.at<uchar>(x,y) ;
                        unsigned char code=0;
                        code |=(inImage.at<uchar>(x-1,y+1) > center)<<0;
                        code |=(inImage.at<uchar>(x,y+1) > center)<<1;
                        code |=(inImage.at<uchar>(x+1,y+1) > center)<<2;
                        code |=(inImage.at<uchar>(x+1,y) > center)<<3;
                        code |=(inImage.at<uchar>(x+1,y-1) > center)<<4;
                        code |=(inImage.at<uchar>(x,y-1) > center)<<5;
                        code |=(inImage.at<uchar>(x-1,y-1) > center)<<6;
                        code |= (inImage.at<uchar>(x-1,y) > center)<<7;
                        outImage.at<uchar>(x-1,y-1)=code;

                }

        }
}

void RGB2IlluminationInvariant(Mat image, float alpha,Mat &outImage)
{
  assert(image.channels()==3);
  vector<Mat> channels(3);
  if(image.channels()==3)
  {
      split(image,channels);
      Mat ch0,ch1,ch2; 
      
      channels[1].convertTo(ch1,CV_32F);
      ch1=ch1+0.5;
      log(ch1,ch1);
      

      channels[2].convertTo(ch2,CV_32F);
      ch2=ch2+0.5;
      log(ch2,ch2);

      channels[0].convertTo(ch0,CV_32F);
      ch0=ch0+0.5;
      log(ch0,ch0);
      outImage =  ch1 - alpha*ch2-(1-alpha)*ch0;
  }
 return ;
}

void combineColor_Texture(Mat &image)
{
   assert (image.channels()==3);
   vector<Mat> rgbChannels(3);
   split(image, rgbChannels);
   computeLBPFeatures(image,rgbChannels[2]);
   merge(rgbChannels,image);

}

