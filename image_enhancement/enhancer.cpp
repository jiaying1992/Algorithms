// author: yingjia
// time: 20170818
// contact: 1439352516@qq.com

#include "enhancer.h"
#include <vector>

namespace yingjia {

bool TextEnhancer::Run(cv::Mat& src_img, cv::Mat& dst_img)
{
	bool stat=true;
	Mat enhanceImg;
	stat=ContrastEnhanceInternal(src_img, enhanceImg);
	if(stat==false)
	{
		return false;
	}
	dst_img=enhanceImg.clone();
	return stat;
}

static float SetK(uchar val)
{
	float k;
	if(val<20)
	{
		k=2.5;
	}
	else if(val>=20 && val<=100)
	{
		k=1+((2.5f-1)*(100-val))/80;
	}
	else if(val>100 && val<200)
	{
		k=1;
	}
	else
	{
		k=1+(val-220)/35.f;
	}
	return k;
}

bool TextEnhancer::ContrastCompensation(Mat& srcImg, Mat& bakgnImg, Mat& dstImg)
{
	bool stat=true;
	int imgHeight=srcImg.rows;
	int imgWidth=srcImg.cols;
	if(dstImg.empty())
	{
		dstImg.create(imgHeight, imgWidth, CV_8UC1);
	}
	int F=255;
	float thresh=F*0.75f;
	for(int i=0; i<imgHeight; i++)
	{
		uchar *pSrc = srcImg.ptr<uchar>(i);
		uchar *pBak = bakgnImg.ptr<uchar>(i);
		uchar *pDst = dstImg.ptr<uchar>(i);
		for(int j=0; j<imgWidth; j++)
		{
			 float k=SetK(pBak[j]);
			 if(pBak[j]>pSrc[j])
			 {
				 float outPixel = F-k*(pBak[j]-pSrc[j]);
				 if(outPixel<thresh)
				 {
					 pDst[j] = (uchar)thresh;
				 }
				 else
				 {
					 pDst[j] = (uchar)outPixel;
				 }
			 }
			 else
			 {
				 pDst[j]=F;
			 }
		}
	}
	return true;
}

bool TextEnhancer::UnsharpMask(Mat &srcImg, Mat &dstImg, float amount, float radius, uchar threshold, int contrast)
{
	bool stat=true;
	if(srcImg.empty())
	{
		return false;
	}
	int imgHeight=srcImg.rows;
	int imgWidth=srcImg.cols;
	int imgChannels=srcImg.channels();
	if(dstImg.empty())
	{
		dstImg.create(imgHeight, imgWidth, CV_8UC3);
	}
	// 原图的高对比度图像
	Mat highContrastImg(imgHeight, imgWidth, CV_8UC3);
	stat=AdjustContrast(srcImg, highContrastImg, contrast);
	if(stat==false)
	{
		return false;
	}
	// 原图的模糊图像
	Mat blurImg(imgHeight, imgWidth, CV_8UC3);
	cv::GaussianBlur(srcImg, blurImg, cv::Size((int)radius, (int)radius), 0.3);
	// 原图与模糊图像作差的图像
	Mat diffImg=srcImg-blurImg;
	// 锐化
	for(int y=0; y<imgHeight; y++)
	{
		for(int x=0; x<imgWidth; x++)
		{
			cv::Vec3b hc=highContrastImg.at<cv::Vec3b>(y,x);
			cv::Vec3b diff=diffImg.at<cv::Vec3b>(y,x);
			cv::Vec3b sBGR = srcImg.at<cv::Vec3b>(y,x);
			cv::Vec3b dBGR = dstImg.at<cv::Vec3b>(y,x);
			for(int k=0; k<imgChannels; k++)
			{
				if(diff[k]>threshold)
				{
					// 最终图像=原始图像*(1-r)+高对比图像*r
					dBGR[k]=(uchar)((sBGR[k]*(100-amount)+hc.val[k]*amount)/100);
				}
				else
				{
					dBGR[k]=sBGR[k];
				}
			}
		}
	}
	return true;
}

/*-------------调整图像对比度----------------------------------------------*/
bool TextEnhancer::AdjustContrast(Mat& srcImg, Mat& dstImg, int contrast)
{
	if(srcImg.empty())
	{
		return false;
	}
	int imgHeight=srcImg.rows;
	int imgWidth=srcImg.cols;
	int imgChannels = srcImg.channels();
	if(dstImg.empty())
	{
		dstImg.create(imgHeight, imgWidth, CV_8UC3);
	}
	// 求原图均值
	cv::Scalar mean(0,0,0);
	for(int y=0; y<imgHeight; y++)
	{
		for(int x=0; x<imgWidth; x++)
		{
			const cv::Vec3b bgr=srcImg.at<cv::Vec3b>(y,x);
			mean[0]+=bgr[0];
			mean[1]+=bgr[1];
			mean[2]+=bgr[2];
		}
	}
	for(int k=0; k<imgChannels; k++)
	{
		mean[k] /= (imgWidth*imgHeight);
	}
	// 调整对比度
	if(contrast <= -255)
	{
		// 当增量等于-255时，是图像对比度的下端极限，此时，图像RGB各个分量都等于阀值，图像呈全灰色，灰度图上只有1条线，即阀值灰度；
		for(int y=0; y<imgHeight; y++)
		{
			for(int x=0; x<imgWidth; x++)
			{
				cv::Vec3b& bgr=dstImg.at<cv::Vec3b>(y,x);
				bgr[0]=(uchar)mean[0];
				bgr[1]=(uchar)mean[1];
				bgr[2]=(uchar)mean[2];
			}
		}
	}
	else if(contrast>-255 && contrast<=0)
	{
		for(int y=0; y<imgHeight; y++)
		{
			for(int x=0; x<imgWidth; x++)
			{
				const cv::Vec3b sBGR=srcImg.at<cv::Vec3b>(y,x);
				cv::Vec3b& dBGR = dstImg.at<cv::Vec3b>(y,x);
				dBGR[0]=(uchar)(sBGR[0]+(sBGR[0]-mean[0])*contrast/255);
				dBGR[1]=(uchar)(sBGR[1]+(sBGR[1]-mean[1])*contrast/255);
				dBGR[2]=(uchar)(sBGR[1]+(sBGR[1]-mean[2])*contrast/255);
			}
		}
	}
	else if(contrast>0 && contrast<255)
	{
		int nContrast=255*255/(255-contrast)-255;
		for(int y=0; y<imgHeight; y++)
		{
			for(int x=0; x<imgWidth; x++)
			{
				cv::Vec3b& dBGR=dstImg.at<cv::Vec3b>(y,x);
				const cv::Vec3b sBGR=srcImg.at<cv::Vec3b>(y,x);
				dBGR[0]=(uchar)(sBGR[0]+(sBGR[0]-mean[0])*nContrast/255);
				dBGR[1]=(uchar)(sBGR[1]+(sBGR[1]-mean[1])*nContrast/255);
				dBGR[2]=(uchar)(sBGR[2]+(sBGR[2]-mean[2])*nContrast/255);
			}
		}
	}
	else
	{
		// 当增量等于255时，是图像对比度的上端极限，实际等于设置图像阀值，图像由最多八种颜色组成，灰度图上最多8条线，即红、黄、绿、青、蓝、紫及黑与白；
		for(int y=0; y<imgHeight; y++)
		{
			for(int x=0; x<imgWidth; x++)
			{
				cv::Vec3b sBGR = srcImg.at<cv::Vec3b>(y,x);
				cv::Vec3b& dBGR=dstImg.at<cv::Vec3b>(y,x);
				for(int k=0; k<imgChannels; k++)
				{
					if(sBGR[k]>mean[k])
					{
						dBGR[k]=255;
					}
					else
					{
						dBGR[k]=0;
					}
				}
			}
		}
	}
	return true;
}

bool TextEnhancer::ContrastEnhanceInternal(Mat& srcImg, Mat& dstImg)
{
	if(dstImg.empty())
	{
		dstImg.create(srcImg.rows, srcImg.cols, CV_8UC1);
	}
	if(srcImg.channels()==3)
	{
		cv::cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	}
	Mat binImg;
	ImgThreshold(srcImg, binImg, 25);
	int rows=srcImg.rows;
	int cols=srcImg.cols;
	// 统计二值化图像中黑色像素点的均值
	double forgndMean=0;
	int forgndCnt=0;
	double bakgndMean=0;
	int bakgndCnt=0;
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			if(binImg.data[i*cols+j]==255)
			{
				forgndMean+=srcImg.data[i*cols+j];
				forgndCnt++;
			}
			else
			{
				bakgndMean+=srcImg.data[i*cols+j];
				bakgndCnt++;
			}
		}
	}
	forgndMean /= forgndCnt;
	bakgndMean /= bakgndCnt;
	int blackVal=(int)forgndMean;
	int whiteVal=(int)(bakgndMean-(bakgndMean-forgndMean)/3);
	Mat tempImg(rows, cols, CV_8UC1);
	float a=255/float(whiteVal-blackVal);
	float c=(blackVal*255)/float(blackVal-whiteVal);
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j>cols; j++)
		{
			int srcVal=srcImg.data[i*cols+j];
			int dstVal=(int)(a*srcVal+c);
			if(dstVal<0)
			{
				dstVal=0;
			}
			if(dstVal >255)
			{
				dstVal=255;
			}
			tempImg.data[i*cols+j]=dstVal;
		}
	}
	tempImg.copyTo(dstImg);
	return true;
}

bool TextEnhancer::ImgThreshold(Mat& srcImg, Mat& dstImg, int windowSize)
{
	if(dstImg.empty())
	{
		dstImg.create(srcImg.rows, srcImg.cols,CV_8UC1);
	}
	int rows=srcImg.rows;
	int cols=srcImg.cols;
	Mat tempImg(rows,cols,CV_8UC1);
	Mat thImg(rows,cols,CV_8UC1);
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
	for(int i=0; i<rows; i+=windowSize)
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
				for(int n=cStart; n<cStart+windowSize; n++)
				{
					mean+=srcImg.data[m*cols+n];
				}
			}
			mean/=windowSize*windowSize;
			for(int m=rStart; m<rStart+windowSize; m++)
			{
				for(int n=cStart;n<cStart+windowSize; n++)
				{
					int val=srcImg.data[m*cols+n];
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
			thArr[rIndex][cIndex]=(int)th;
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
					thImg.data[i*cols+j]=(uchar)((1-b)*R1+b*R2);
				}
				else
				{
					thImg.data[i*cols+j]=(uchar)((1-b)*thArr[rPos][cPos]+b*thArr[rPos+1][cPos]);
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
					thImg.data[i*cols+j]=(uchar)R;
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
			if(srcImg.data[offset]<thImg.data[offset])
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
			dstImg.data[offset]=tempImg.data[offset];
		}
	}
	for(int i=0; i<rNum; i++)
	{
		delete[] thArr[i];
	}
	delete[] thArr;
	return true;
}

static void CalcImgHist(Mat& block, int* blockHist, int blockSize)
{
	for(int i=0; i<blockSize; i++)
	{
		const uchar* pdata=block.ptr<uchar>(i);
		for(int j=0; j<blockSize; j++)
		{
			++blockHist[pdata[j]];
		}
	}
}

static int CalcBakgndVal(int* blockHist)
{
	int pixelCnt=0;
	int startIdx=0;
	int endIdx=0;
	// 确定直方图统计的起始位置和结束位置
	for(int idx=255;idx>-1; idx--)
	{
		if(blockHist[idx]==0)
		{
			continue;
		}
		else
		{
			if(startIdx==0)
			{
				startIdx==idx;
			}
			pixelCnt+=blockHist[idx];
			if(pixelCnt>50)
			{
				endIdx=idx;
				break;
			}
		}
	}
	int pixelValSum=0;
	for(int idx=startIdx; idx>=endIdx; idx--)
	{
		pixelValSum+=idx*blockHist[idx];
	}
	return pixelValSum/pixelCnt;
}

bool TextEnhancer::BackgndEstimationInternal(Mat& grayImg, Mat& backImg)
{
	int imgSize=std::min(grayImg.rows, grayImg.cols);
	if(imgSize%2==0)
	{
		imgSize-=1;
	}
	int blockSize=2*std::max(15,15)+1;
	if(blockSize>imgSize)
	{
		blockSize=imgSize;
	}
	const int halfBlockSize=static_cast<int> (0.5*(blockSize-1));
	const int Rows=grayImg.rows;
	const int Cols=grayImg.cols;
	const int startRow=halfBlockSize;
	const int startCol=halfBlockSize;
	const int endRow=Rows-halfBlockSize;
	const int endCol=Cols-halfBlockSize;
	// 初始化
	int* initBlockHist=new int[256];
	int* currBlockHist=new int[256];
	memset(initBlockHist, 0, 256*sizeof(int));
	memset(currBlockHist, 0, 256*sizeof(int));
    if(backImg.empty())
	{
		backImg=Mat::zeros(Rows, Cols, CV_8UC1);
	}
	Mat initBlock=grayImg(Rect(0,0,blockSize, blockSize));
	CalcImgHist(initBlock, initBlockHist, blockSize);
	// 背景估计
	for(int i=startRow; i<endRow; i++)
	{
		uchar* pdata_bakgnd=backImg.ptr<uchar>(i);
		if(i>startRow)
		{
			Mat topVec=grayImg(Rect(0,i-halfBlockSize-1,blockSize,1));
			Mat botmVec=grayImg(Rect(0,i+halfBlockSize, blockSize, 1));
			const uchar* pdata_top=topVec.ptr<uchar>(0);
			const uchar* pdata_botm=botmVec.ptr<uchar>(0);
			for(int k=0; k<blockSize; k++)
			{
				--initBlockHist[pdata_top[k]];
				++initBlockHist[pdata_botm[k]];
			}
		}
		// 计算当前点的背景图的像素值
		pdata_bakgnd[startCol]=CalcBakgndVal(initBlockHint);
		// 拷贝initBlockHist的副本给blockHist, 作为每一行的初始Hist
		memcpy(currBlockHist, initBlockHist, 256*sizeof(int));
		for(int j=startCol+1; j<endCol; j++)
		{
			Mat leftVec=grayImg(Rect(j-halfBlockSize-1, i-halfBlockSize, 1, blockSize));
			Mat rightVec=grayImg(Rect(j+halfBlockSize, i-halfBlockSize, 1, blockSize));
			for(int k=0; k<blockSize; k++)
			{
				const uchar* pdata_left=leftVec.ptr<uchar>(k);
				const uchar* pdata_right=rightVec.ptr<uchar>(k);
				--currBlockHist[pdata_left[0]];
				++currBlockHist[pdata_right[0]];
			}
			// 计算当前点的背景图的像素值
			pdata_bakgnd[j]=CalcBakgndVal(currBlockHist);
		}
	}
	// 填充四个边缘的背景灰度
	for(int i=0; i<startRow; i++)
	{
		backImg.row(startRow).copyTo(backImg.row(i));
	}
	for(int i=endRow; i<Rows; i++)
	{
		backImg.row(endRow-1).copyTo(backImg.row(i));
	}
	for(int j=0; j<startCol; j++)
	{
		backImg.col(startCol).copyTo(backImg.col(j));
	}
	for(int j=endCol; j<Cols; j++)
	{
		backImg.col(endCol-1).copyTo(backImg.col(j));
	}
	delete [] initBlockHint;
	delete [] currBlockHint;
	return true;
}

bool TextEnhancer::BackgndEstimaion(Mat& srcImg, Mat& dstImg)
{

	bool stat=true;
	int imgHeight=srcImg.rows;
	int imgWidth=srcImg.cols;
	Mat tempImg;
	cv::resize(srcImg, tempImg, cv::Size(imgHeight/2, imgWidth/2));
	Mat tempImg2;
	stat=BackgndEstimationInternal(tempImg, tempImg2);
	if(stat==false)
	{
		return false;
	}
	cv::resize(tempImg2, dstImg, cv::Size(imgWidth, imgHeight));
	return stat;
}

}
