// authot: Ying Jia
// time: 20170727
// contact: 1439352516@qq.com
// function: predict by svm (handwritten character recognition)

#include<opencv2/opencv.hpp>
#include"Windows.h"
#include<opencv2/ml/ml.hpp>
#include<opencv2/core/core.hpp>
#include"fstream"

using namespace std;
using namespace cv;

int main()
{
	DWORD start, end;  
    start = GetTickCount(); 
	CvSVM svm;
	svm.load("D:\\HOG_SVM_DATA1.xml");  // load svm model
	Mat test;
	char result[300];
	test=imread("D:\\2.bmp",1);  // load test image
	if(test.empty())
	{
		MessageBox(NULL,TEXT("test image do not exist!"), TEXT("hint"), MB_ICONWARNING);
		return -1;
	}
	Mat trainTempImg;
	resize(test, trainTempImg, Size(28,28));
	HOGDescriptor *hog=new HOGDescriptor(Size(28,28), Size(14,14), Size(7,7), Size(7,7),9);
	vector<float> descriptors;
	hog->compute(trainTempImg, descriptors, Size(1,1), Size(0,0)); // compute hog feature
	cout<<"HOG dims:"<<descriptors.size()<<endl;
	Mat SVMtrainMat(1, descriptors.size(), CV_32FC1);
	int n=0;
	for(vector<float>::iterator iter=descriptors.begin(); iter!=descriptors.end(); iter++)
	{
		SVMtrainMat.at<float>(0,n)=(*iter);
		n++;
	}
	int ret=svm.predict(SVMtrainMat); //predict result
	printf("%d\n",ret);
	sprintf(result, "%^d\r\n", ret);
	cvNamedWindow("dst",1);
	imshow("dst", test);
	end = GetTickCount();  
    printf("time: %d ms\n", end - start);   // record the running time
	return 0;
}
