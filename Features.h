#ifndef FEATURE_H
#define FEATURE_H
int colorNormalize(Mat bgr_image, Mat &out);
void computeLBPFeatures(Mat inImage, Mat &outImage);
void combineColor_Texture(Mat &image);
//void gpuFunction(int imageWidth,int imageHeight, int colorStep,int grayStep,unsigned char *in,unsigned char *out);
void RGB2IlluminationInvariant(Mat image, float alpha,Mat &outImage);
#endif
