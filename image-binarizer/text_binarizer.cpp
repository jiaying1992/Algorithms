#include "text_binarizer.h"
#include <vector>

namespace yingjia {
	TextBinarizer::TextBinarizer(int winSize1, int winSize2)
	{
		this->contrastWinSize_=winSize1;
		this->localThreshWinSize_=winSize2;
	}
	TextBinarizer::~TextBinarizer()
	{

	}

	bool TextBinarizer::Run2(Mat& src, Mat& dst)
	{
		bool stat=true;
		Mat tmp;
		cv::medianBlur(src, tmp, 5);
		stat=ImgThreshold2(tmp,dst,this->localThreshWinSize_);
		return stat;
	}
	bool TextBinarizer::Run(Mat& src, Mat& dst)
	{
		bool stat=true;
		Mat tmp;
		stat=ImgThreshold1(src, tmp);
		if(stat==false)
		{
			return false;
		}
		Mat binaryImg=255-tmp.clone();
		Mat elem=cv::getStructuringElement(cv::MORPH_RECT, Size(3,3));
		cv::dilate(binaryImg, binaryImg, Mat());
		ImgThreshold2(src, dst, this->localThreshWinSize_);
		cv::bitwise_and(binaryImg, dst, dst);
		return stat;
	}
	bool TextBinarizer::ImgThreshold1(Mat& src, Mat& dst, int windowSize)
	{
		Mat imgClone=src.clone();
		dst=src.clone();
		Mat imgContrastFLT(imgClone.rows, imgClone.cols, CV_32FC1);
		Mat imgContrastUCHAR(imgClone.rows, imgClone.cols, CV_8UC1);
		for(int i=0; i<imgClone.rows; i++)
		{
			for(int j=0; j<imgClone.rows; j++)
			{
				int tmpMax=INT_MAX;
				int tmpMin=INT_MIN;
				for (int m=i-this->contrastWinSize_/2; m<i-this->contrastWinSize_/2+this->contrastWinSize_; m++)
				{
					for(int n=j-this->contrastWinSize_/2; n<j-this->contrastWinSize_/2+this->contrastWinSize_; n++)
					{
						int p=m>0?m:0;
						p=m<=imgClone.rows-1?p:imgClone.rows-1;
						int q=n>=0?n:0;
						q=n<=imgClone.cols-1?q:imgClone.cols-1;
						uchar *ptr=(uchar*)imgClone.ptr(p);
						if(tmpMax<ptr[q])
						{
							tmpMax=ptr[q];
						}
						if(tmpMin>ptr[q])
						{
							tmpMin=ptr[q];
						}
					}
				}
				imgContrastFLT.at<uchar>(i,j)=(float)(tmpMax-tmpMin)/(tmpMax+tmpMin+1);
			}

		}
		float tmpMax=FLT_MIN;
		for(int i=0; i<imgContrastFLT.rows; i++)
		{
			float* ptr=(float*)imgContrastFLT.ptr(i);
			for(int j=0; j<imgContrastFLT.cols; j++)
			{
				if(tmpMax<ptr[j])
				{
					tmpMax=ptr[j];
				}
			}
		}
		double scale=255/tmpMax;
		cv::convertScaleAbs(imgContrastFLT, imgContrastUCHAR, scale);
		imgContrastUCHAR=255-imgContrastUCHAR;
		Mat imgContrastBin;
		cv::threshold(imgContrastUCHAR,imgContrastBin,0,255,CV_THRESH_OTSU);
		Mat imgCanny;
		Canny(imgClone, imgCanny, 50, 150);
		imgCanny=255-imgCanny;
		Mat imgCombine=imgContrastBin.clone();
		for(int i=0; i<imgCombine.rows; i++)
		{
			uchar* ptrCombine=(uchar*)imgCombine.ptr(i);
			uchar* ptrCanny=(uchar*)imgCanny.ptr(i);
			for(int j=0; j<imgCombine.cols; j++)
			{
				if(0==ptrCombine[j])
				{
					ptrCombine[j]=ptrCanny[j];
				}
			}
		}
		int strokeWidth=EstimateStrokeWidth(imgCombine, imgClone);
		if(strokeWidth%2==0)
		{
			strokeWidth++;
		}
		int mdSize=0.75*strokeWidth;
		if(mdSize%2==0)
		{
			mdSize++;
		}
		medianBlur(imgContrastBin, imgContrastBin, mdSize);
		imgCombine=imgContrastBin.clone();
		for(int i=0; i<imgCombine.rows; i++)
		{
			uchar* ptrCombine=(uchar*)imgCombine.ptr(i);
			uchar* ptrCanny=(uchar*)imgCanny.ptr(i);
			for(int j=0; j<imgCombine.cols; j++)
			{
				if(0==ptrCombine[j])
				{
					ptrCombine[j]=ptrCanny[j];
				}
			}
		}
		Mat imgIntensityMean(imgClone.rows, imgClone.cols, CV_32FC1);
		Mat imgCount(imgClone.rows, imgClone.cols, CV_32FC1);
		for(int i=0; i<imgClone.rows; i++)
		{
			uchar* ptrClone=(uchar*)imgClone.ptr(i);
			uchar* ptrCombine=(uchar*)imgCombine.ptr(i);
			float* ptrIntensityMean=(float*)imgIntensityMean.ptr(i);
			float* ptrCount=(float*)imgCount.ptr(i);
			for(int j=0; j<imgClone.cols; j++)
			{
				if(0==ptrCombine[j])
				{
					ptrCount[j]=1;
					ptrIntensityMean[j]=ptrClone[j];
				}
		        else
				{
					ptrCount[j]=0;
					ptrIntensityMean[j]=0;
				}
			}
		}
		Mat imgIntegralMean;
		Mat imgIntegralCount;
		CalcIntegralImg(imgIntensityMean, imgIntegralMean);
		CalcIntegralImg(imgCount, imgIntegralCount);
		Mat imgMean(imgClone.rows, imgClone.cols, CV_32FC1);
		for(int i=0; i<imgClone.rows; i++)
		{
			float* ptrMean=(float*)imgMean.ptr(i);
			for(int j=0; j<imgClone.cols; j++)
			{
				int a1=i-strokeWidth/2-1>=0 ? i-strokeWidth/2-1 : 0;
				a1=a1<imgClone.rows ? a1 : imgClone.rows-1;
				int a2=j-strokeWidth/2-1>=0 ? j-strokeWidth/2-1 : 0;
				a2=a2<imgClone.cols ? a2 : imgClone.cols-1;
				int b1=i-strokeWidth/2-1>=0 ? i-strokeWidth/2-1 : 0;
				b1=b1<imgClone.rows ? b1 : imgClone.rows-1;
				int b2=j+strokeWidth/2>=0 ? j+strokeWidth/2 : 0;
				b2=b2<imgClone.cols ? b2 : imgClone.cols-1;
				int c1=i+strokeWidth/2>=0 ? i+strokeWidth/2 : 0;
				c1=c1<imgClone.rows ? c1 : imgClone.rows-1;
				int c2=j-strokeWidth/2-1>=0 ? j-strokeWidth/2-1 : 0;
				c2=c2<imgClone.cols ? c2 : imgClone.cols-1;
				int d1=i+strokeWidth/2>=0 ? i+strokeWidth/2 : 0;
				d1=d1<imgClone.rows ? d1 : imgClone.rows-1;
				int d2=j+strokeWidth/2>=0 ? j+strokeWidth/2 : 0;
				d2=d2<imgClone.cols ? d2 : imgClone.cols-1;

				float aValueMean=((float*)imgIntegralMean.ptr(a1))[a2];
				float bValueMean=((float*)imgIntegralMean.ptr(b1))[b2];
				float cValueMean=((float*)imgIntegralMean.ptr(c1))[c2];
				float dValueMean=((float*)imgIntegralMean.ptr(d1))[d2];
				
				float aValueCount=((float*)imgIntegralCount.ptr(a1))[a2];
				float bValueCount=((float*)imgIntegralCount.ptr(b1))[b2];
				float cValueCount=((float*)imgIntegralCount.ptr(c1))[c2];
				float dValueCount=((float*)imgIntegralCount.ptr(d1))[d2];

				int count=dValueCount-bValueCount-cValueCount+aValueCount;
				if(count>0)
				{
					ptrMean[j]=(dValueMean-bValueMean-cValueMean+aValueMean)/count;
				}
				else
				{
					ptrMean[j]=0;
				}
			}
		}
		Mat imgIntensityVar(imgClone.rows, imgClone.cols, CV_32FC1);
		for(int i=0; i<imgClone.rows; i++)
		{
			uchar* ptrClone=(uchar*)imgClone.ptr(i);
			uchar* ptrCombine=(uchar*)imgCombine.ptr(i);
			float* ptrIntensityVar=(float*)imgIntensityVar.ptr(i);
			float* ptrMean=(float*)imgMean.ptr(i);
			for(int j=0; j<imgClone.cols; j++)
			{
				if(0==ptrCombine[j])
				{
					ptrIntensityVar[j]=(ptrClone[j]-ptrMean[j])*(ptrClone[j]-ptrMean[j]);
				}
				else
				{
					ptrIntensityVar[j]=0;
				}
			}
		}
		Mat imgIntegralVar;
		CalcIntegralImg(imgIntensityVar, imgIntegralVar);
		Mat imgVar(imgClone.rows, imgClone.cols, CV_32FC1);
		for(int i=0; i<imgClone.rows; i++)
		{
			float* ptrVar=(float*)imgVar.ptr(i);
			for(int j=0; j<imgClone.cols; j++)
			{
				int a1=i-strokeWidth/2-1>=0 ? i-strokeWidth/2-1 : 0;
				a1=a1<imgClone.rows ? a1 : imgClone.rows-1;
				int a2=j-strokeWidth/2-1>=0 ? j-strokeWidth/2-1 : 0;
				a2=a2<imgClone.cols ? a2 : imgClone.cols-1;
				int b1=i-strokeWidth/2-1>=0 ? i-strokeWidth/2-1 : 0;
				b1=b1<imgClone.rows ? b1 : imgClone.rows-1;
				int b2=j+strokeWidth/2>=0 ? j+strokeWidth/2 : 0;
				b2=b2<imgClone.cols ? b2 : imgClone.cols-1;
				int c1=i+strokeWidth/2>=0 ? i+strokeWidth/2 : 0;
				c1=c1<imgClone.rows ? c1 : imgClone.rows-1;
				int c2=j-strokeWidth/2-1>=0 ? j-strokeWidth/2-1 : 0;
				c2=c2<imgClone.cols ? c2 : imgClone.cols-1;
				int d1=i+strokeWidth/2>=0 ? i+strokeWidth/2 : 0;
				d1=d1<imgClone.rows ? d1 : imgClone.rows-1;
				int d2=j+strokeWidth/2>=0 ? j+strokeWidth/2 : 0;
				d2=d2<imgClone.cols ? d2 : imgClone.cols-1; 

				float aValueVar=((float*)imgIntegralVar.ptr(a1))[a2];
				float bValueVar=((float*)imgIntegralVar.ptr(b1))[b2];
				float cValueVar=((float*)imgIntegralVar.ptr(c1))[c2];
				float dValueVar=((float*)imgIntegralVar.ptr(d1))[d2];

				float aValueCount=((float*)imgIntegralCount.ptr(a1))[a2];
				float bValueCount=((float*)imgIntegralCount.ptr(b1))[b2];
				float cValueCount=((float*)imgIntegralCount.ptr(c1))[c2];
				float dValueCount=((float*)imgIntegralCount.ptr(d1))[d2];
				int count=dValueCount-bValueCount-cValueCount+aValueCount;
				if(count>0)
				{
					ptrVar[j]=(dValueVar-bValueVar-cValueVar+aValueVar)/count;
				}
				else
				{
					ptrVar[j]=0;
				}
			}
		}
		for(int i=0; i<dst.rows; i++)
		{
			uchar* ptrDst=(uchar*)dst.ptr(i);
			uchar* ptrClone=(uchar*)imgClone.ptr(i);
			for(int j=0; j<dst.cols; j++)
			{
				float mean=((float*)imgMean.ptr(i))[j];
				float std=sqrt(((float*)imgVar.ptr(i))[j]);
				std=sqrt(std);
				if(ptrClone[j]<=-mean+0.25*std)
				{
					ptrDst[j]=0;
				}
				else
				{
					ptrDst[j]=255;
				}
			}
		}
		return true;
	}
	void TextBinarizer::CalcIntegralImg(Mat& src, Mat& dst)
	{
		dst=src.clone();
		for(int i=0; i<src.rows; i++)
		{
			float* ptrSrc=(float*)src.ptr(i);
			float* ptrDst=(float*)dst.ptr(i);
			for(int j=0; j<src.cols; j++)
			{
				float a=(i-1>=0 && j-1>=0) ? ((float*)dst.ptr(i-1))[j-1]:0;
				float b=(i-1>=0) ? ((float*)dst.ptr(i-1))[j]:0;
				float c=(j-1>=0) ? ptrDst[j-1]:0;
				ptrDst[j]=b+c-a+ptrSrc[j];
				
			}
		}
	}
	void TextBinarizer::PostProcessing(Mat& src, Mat& dst)
	{
		src=255-src;
		for(int i=1; i<src.rows-1; i++)
		{
			uchar* prevLine=src.ptr<uchar>(i-1);
			uchar* currLine=src.ptr<uchar>(i);
			uchar* nextLine=src.ptr<uchar>(i+1);
			for(int j=1; j<src.cols-1; j++)
			{
				if(currLine[j]==255)
				{
				}
				int topVal=prevLine[j];
				int leftVal=currLine[j-1];
			}
		}
	}
	int TextBinarizer::EstimateStrokeWidth(Mat& imgCombine, Mat& imgSrc)
	{
		vector<Point2i> points;
		for(int i=0; i<imgCombine.rows; i++)
		{
			uchar* ptrCombine=(uchar*)imgCombine.ptr(i);
			uchar* ptrSrc=(uchar*)imgSrc.ptr(i);
			for(int j=0; j<imgCombine.cols-1; j++)
			{
				if(255==ptrCombine[j] && 0==ptrCombine[j+1] && ptrSrc[j]>ptrSrc[j+1])
				{
					points.push_back(Point2i(j,i));
				}
			}
		}
		int histogram[256];
		for(int i=0; i<256; i++)
		{
			histogram[i]=0;
		}
		for(int i=0; i<points.size(); i++)
		{
			if(points[i].x==points[i+1].x)
			{
				int dist=points[i+1].y-points[i].y;
				if(dist>=0 && dist<256)
				{
					histogram[dist]++;
				}
				i++;
			}
		}
		int tmpMax=INT_MIN;
		int indexMax=0;
		for(int i=3; i<256; i++)
		{
			if(tmpMax<histogram[i])
			{
				tmpMax=histogram[i];
				indexMax=i;
			}
		}
		return indexMax;
	}
	bool TextBinarizer::ImgThreshold2(Mat& src, Mat& dst, int windowSize)
	{
		if(dst.empty())
		{
			dst.create(src.rows, src.cols, CV_8UC1);
		}
		int rows=src.rows;
		int cols=src.cols;
		Mat tempImg(rows, cols, CV_8UC1);
		Mat thImg(rows, cols, CV_8UC1);
		int rNum=rows/windowSize+1;
		int cNum=cols/windowSize+1;
		int** thArr=new int*[rNum];
		for(int i=0; i<rNum; i++)
		{
			thArr[i]=new int[cNum];
		}
		float k=0.5;
		int rIndex=0;
		int cIndex=0;
		for(int i=0; i<rowsl; i+=windowSize)
		{
			rIndex=i/windowSize;
			int rStart=i;
			if(i>rows-windowSize)
			{
				rIndex=rNum-1;
				rStart=rows-windowSize;
			}
			for(int j=0; j<cols; j+=windowSize)
			{
				cIndex=j/windowSize;
				int cStart=j;
				if(j>cols-windowSize)
				{
					cIndex=cNum-1;
					cStart=cols-windowSize;
				}
				float mean=0;
				float sigma=0;
				for(int m=rStart; m<rStart+windowSize; m++)
				{
					for(int n=cStart; c<cStart+windowSize; n++)
					{
						mean+=src.data[m*cols+n];
					}
				}
				mean/=windowSize*windowSize;
				for(int m=rStart; m<rStart+windowSize; m++)
				{
					for(int n=cStart; n<cStart+windowSize; n++)
					{
						int val=src.data[m*cols+n];
						sigma+=(val-mean)*(val-mean);
					}
				}
				sigma/=windowSize*windowSize;
				sigma=sqrt(sigma);
				float th=mean-20;
				if(sigma>20)
				{
					th=mean-k*sigma;
				}
				thArr[rIndex][cIndex]=th;
			}
		}
		for(int i=0; i<rows; i++)
		{
			int rPos=i/windowSize;
			if(rPos<rIndex)
			{
				int rOffset=i-rPos*windowSize;
				for(int j=0; j<cols; j++)
				{
					int cPos=j/windowSize;
					float b=rOffset/(float)windowSize;
					if(cPos<cIndex)
					{
						int cOffset=j-cPos*windowSize;
						float a=cOffset/(float)windowSize;
						float R1=(1-a)*thArr[rPos][cPos]+a*thArr[rPos][cPos+1];
						float R2=(1-a)*thArr[rPos+1][cPos]+a*thArr[rPos+1][cPos+1];
						thImg.data[i*cols+j]=(1-b)*R1+b*R2;
					}
					else
					{
						thImg.data[i*cols+j]=(1-b)*thArr[rPos][cPos]+b*thArr[rPos+1][cPos];
					}
				}
			}
			else
			{
				int rOffset=i-rPos*windowSize;
				for(int j=0; j<cols; j++)
				{
					int cPos=j/windowSize;
					float b=rOffset/(float)windowSize;
					if(cPos<cIndex)
					{
						int cOffset=j-cPos*windowSize;
						float a=cOffset/(float)windowSize;
						float R=(1-a)*thArr[rPos][cPos]+a*thArr[rPos][cPos+1];
						thImg.data[i*cols+j]=R;
					}
					else
					{
						thImg.data[i*cols+j]=thArr[rPos][cPos];
					}
				}
			}
		}
		for(int i=0; i<rows; i++)
		{
			for(int j=0; j<cols; j++)
			{
				int offset=i*cols+j;
				if(src.data[offset]<thImg.data[offset])
				{
					tempImg.data[offset]=255;
				}
				else
				{
					tempImg.data[offset]=0;
				}
			}
		}
		for(int i=0; i<rows; i++)
		{
			for(int j=0; j<cols; j++)
			{
				int offset=i*cols+j;
				dst.data[offset]=tempImg.data[offset];
			}
		}
		for(int i=0; i<rNum; i++)
		{
			delete[] thArr[i];
		}
		delete[] thArr;
		return true;
	}
}
