#ifndef CUDAKMEANS_H
#define CUDAKMEANS_H
void cudaKmeans(int iterations,int n, int d,int  k, float *dataset,int *labels,float *centers);
//template<typename T> void huge_test(void);
void huge_test_double();
void tiny_test();
#endif
