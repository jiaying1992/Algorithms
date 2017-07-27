// author: Ying Jia
// time: 20170727
// function: train svm
// contact: 1439352516@qq.com
// platform: VS2010+opencv2.4.10

#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include "windows.h"
#include "fstream"

using namespace std;
using namespace cv;

int main()
{
	vector<string> imgpath;  // path of train image
	vector<int> imglabel;    // label of train image
	int nLine = 0;
	string buf;
	ifstream svm_data;
	svm_data.open("D:\\hb.txt", ios::in);   //open file
	if(!svm_data.is_open())
	{
		cout<<"read error"<<endl;
		exit(EXIT_FAILURE);
	}
	unsigned long n;
	while(svm_data)
	{
		if(getline(svm_data, buf))
		{
			nLine++;
			if(nLine%2 == 0)
			{
				imglabel.push_back(atoi(buf.c_str()));
			}
			else
			{
				imgpath.push_back(buf);
			}
		}
	}
	svm_data.close();    //close file
	/*------------------------------train----------------------------------------------*/
	Mat data_mat, res_mat; 
	int nImgNum = nLine/2;
	data_mat = Mat::zeros(nImgNum, 324, CV_32FC1); // store hog feature 324=9*4*9 single channel float
	res_mat = Mat::zeros(nImgNum, 1, CV_32FC1);    // store label
	Mat src;
	for(string::size_type i = 0; i != imgpath.size(); i++)
	{
		src = imread(imgpath[i],1);  // read train image
		if(src.empty())
		{
			cout<<"can not read the image: "<<imgpath[i]<<endl;
			continue;
		}
		cout<<"processing:"<<imgpath[i]<<endl;
		Mat trainImg;
	    resize(src, trainImg, Size(28,28)); // resize to 28*28
		HOGDescriptor *hog = new HOGDescriptor(Size(28,28), Size(14,14), Size(7,7),Size(7,7),9); // hog descriptor
		vector<float> descriptors; // store result
		hog->compute(trainImg, descriptors, Size(1,1), Size(0,0)); // compute hog descriptor
		cout<<"HOG dims:"<<descriptors.size()<<endl;
		n = 0;
		for(vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			data_mat.at<float>(i,n) = (*iter); // put hog descriptor into data_mat
			n++;
		}
		res_mat.at<float>(i,0) = imglabel[i]; // put label into res_mat
		cout<<"processing done:"<<imgpath[i]<<" "<<imglabel[i]<<endl;
	}
	CvSVM svm;
	CvSVMParams param;
	CvTermCriteria criteria;
	criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON); // termination criterion of iterative algorithm
	param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL,criteria); // svm parameter
	svm.train(data_mat, res_mat, Mat(), Mat(), param); //train
	svm.save("D:\\HOG_SVM_DATA1.xml"); // preserve result
	/*----------------------predict----------------------------------------*/
	Mat test;
	char result[512];
	vector<string> img_tst_path;
	ifstream img_tst("D:\\num.txt");
	while(img_tst)
	{
		if(getline(img_tst, buf))
		{
			img_tst_path.push_back(buf);
		}
	}
	img_tst.close();
	ofstream predict_txt("D:\\SVM_PREDICT1.txt");
	for(string::size_type j  =0; j != img_tst_path.size(); j++)
	{
		test=imread(img_tst_path[j], 1);
		if(test.empty())
		{
			cout<<"can not load the image:"<<img_tst_path[j]<<endl;
			continue;
		}
		Mat trainTempImg;
		resize(test, trainTempImg, Size(28,28));
		HOGDescriptor *hog = new HOGDescriptor(Size(28,28),Size(14,14), Size(7,7), Size(7,7),9);
		vector<float> descriptors;
		hog->compute(trainTempImg, descriptors, Size(1,1), Size(0,0));
		cout<<"HOG dims:"<<descriptors.size()<<endl;
		Mat SVMtrainMat(1, descriptors.size(), CV_32FC1);
		int n=0;
		for(vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			SVMtrainMat.at<float>(0,n) = (*iter);
			n++;
		}
		int ret = svm.predict(SVMtrainMat); // predict by svm
		sprintf(result, "%s %d\r\n", img_tst_path[j], ret);
		predict_txt<<result;  //predict result
	}
	predict_txt.close();  
	return 0;
	}




