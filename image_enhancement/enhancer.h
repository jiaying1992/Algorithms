// author:yingjia
// time:20170816
// contact:1439352516@qq.com
#ifndef _ENHANCER_H_
#define _ENHANCER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using cv::Mat;
using cv::Rect;

namespace yingjia {

class TextEnhancer
{
public:
	TextEnhancer(){}
	~TextEnhancer(){}
	bool Run(cv::Mat &src_img, cv::Mat &dst_img);
private:
	bool BackgndEstimation(Mat &srcImg, Mat &dstImg);
	bool BackgndEstimationInternal(Mat &grayImg, Mat &backImg);
	bool ContrastCompensation(Mat &srcImg, Mat &bakgnImg, Mat &contrastImg);
	bool UnsharpMask(Mat& srcImg, Mat& dstImg, float amount=80, float radius=5, uchar threshold=0, int contrast=100);
	bool AdjustContrast(Mat& srcImg, Mat& dstImg, int contrast);
	bool ImgThreshold(Mat& srcImg, Mat& dstImg, int windowSize);
	bool ContrastEnhanceInternal(Mat& srcImg, Mat& dstImg);
};

}
#endif
