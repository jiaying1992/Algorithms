#include "layer.h"
using namespace cv;

void LoadPenStrokeDataLayerParam(PenStrokeDataLayer* pLayer, S8** ptr)
{
	pLayer->param_.type_ = *(LayerType*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.type_);

	pLayer->param_.data_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.data_channels_);

	pLayer->param_.data_inner_size_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.data_inner_size_);

	pLayer->param_.data_outer_size_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.data_outer_size_);
}

void LoadHandWrittenImageDataLayerParam(HandwrittenImageDataLayer* pLayer, S8** ptr)
{
	pLayer->param_.type_ = *(LayerType*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.type_);

	pLayer->param_.data_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.data_channels_);

	pLayer->param_.data_inner_size_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.data_inner_size_);

	pLayer->param_.data_outer_size_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.data_outer_size_);

	pLayer->param_.mean_ = *(float*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.mean_);

	pLayer->param_.scale_ = *(float*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.scale_);
}

void LoadConvolutionDataLayerParam(ConvolutionLayer* pLayer, S8** ptr)
{
	pLayer->param_.type_ = *(LayerType*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.type_);

	pLayer->param_.weight_num_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_num_);

	pLayer->param_.weight_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_channels_ );

	pLayer->param_.weight_height_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_height_);

	pLayer->param_.weight_width_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_width_);

	pLayer->param_.bias_num_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_num_);

	pLayer->param_.bias_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_channels_);

	pLayer->param_.bias_height_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_height_);

	pLayer->param_.bias_width_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_width_);

	pLayer->weight_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->weight_, TYPE_F32, 
		pLayer->param_.weight_num_, pLayer->param_.weight_channels_, 
		pLayer->param_.weight_height_, pLayer->param_.weight_width_, 
		*ptr, 0, INIT_BLOB_WITH_DATA);
	(*ptr) += (pLayer->weight_.count_ * sizeof(F32));

	pLayer->bias_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->bias_, TYPE_F32, 
		pLayer->param_.bias_num_, pLayer->param_.bias_channels_, 
		pLayer->param_.bias_height_, pLayer->param_.bias_width_, 
		*ptr, 0, INIT_BLOB_WITH_DATA);
	(*ptr) += (pLayer->bias_.count_ * sizeof(F32));
}

void LoadPoolingLayerParam(PoolingLayer* pLayer, S8** ptr)
{
	pLayer->param_.type_ = *(LayerType*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.type_);

	pLayer->param_.kernel_size_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.kernel_size_);

	pLayer->param_.stride_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.stride_);
}

void LoadCCCPLayerParam(CCCPLayer* pLayer, S8** ptr)
{
	pLayer->param_.type_ = *(LayerType*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.type_);

	pLayer->param_.weight_num_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_num_);

	pLayer->param_.weight_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_channels_ );

	pLayer->param_.weight_height_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_height_);

	pLayer->param_.weight_width_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_width_);

	pLayer->param_.bias_num_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_num_);

	pLayer->param_.bias_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_channels_);

	pLayer->param_.bias_height_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_height_);

	pLayer->param_.bias_width_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_width_);

	pLayer->weight_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->weight_, TYPE_F32, 
		pLayer->param_.weight_num_, pLayer->param_.weight_channels_, 
		pLayer->param_.weight_height_, pLayer->param_.weight_width_, 
		*ptr, 0, INIT_BLOB_WITH_DATA);
	(*ptr) += (pLayer->weight_.count_ * sizeof(F32));

	pLayer->bias_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->bias_, TYPE_F32, 
		pLayer->param_.bias_num_, pLayer->param_.bias_channels_, 
		pLayer->param_.bias_height_, pLayer->param_.bias_width_, 
		*ptr, 0, INIT_BLOB_WITH_DATA);
	(*ptr) += (pLayer->bias_.count_ * sizeof(F32));
}

void LoadInnerProductLayerParam(InnerProductLayer* pLayer, S8** ptr)
{
	pLayer->param_.type_ = *(LayerType*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.type_);

	pLayer->param_.weight_num_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_num_);

	pLayer->param_.weight_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_channels_ );

	pLayer->param_.weight_height_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_height_);

	pLayer->param_.weight_width_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.weight_width_);

	pLayer->param_.bias_num_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_num_);

	pLayer->param_.bias_channels_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_channels_);

	pLayer->param_.bias_height_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_height_);

	pLayer->param_.bias_width_ = *(U16*)(*ptr);
	(*ptr) += sizeof(pLayer->param_.bias_width_);

	pLayer->weight_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->weight_, TYPE_F32, 
		pLayer->param_.weight_num_, pLayer->param_.weight_channels_, 
		pLayer->param_.weight_height_, pLayer->param_.weight_width_, 
		*ptr, 0, INIT_BLOB_WITH_DATA);
	(*ptr) += (pLayer->weight_.count_ * sizeof(F32));

	pLayer->bias_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->bias_, TYPE_F32, 
		pLayer->param_.bias_num_, pLayer->param_.bias_channels_, 
		pLayer->param_.bias_height_, pLayer->param_.bias_width_, 
		*ptr, 0, INIT_BLOB_WITH_DATA);
	(*ptr) += (pLayer->bias_.count_ * sizeof(F32));
}

void InitPenStrokeDataLayer(PenStrokeDataLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	pLayer->batch_size_     = 1;
	pLayer->datum_channels_ = pLayer->param_.data_channels_;
	pLayer->datum_height_   = pLayer->param_.data_outer_size_;
	pLayer->datum_width_    = pLayer->param_.data_outer_size_;
	pLayer->datum_size_     = pLayer->datum_channels_ 
		* pLayer->datum_height_ * pLayer->datum_width_;

	// top data blob
	pTop->head_ = UNINITIALIZED;
	InitBlob(pTop, TYPE_S32, pLayer->batch_size_, pLayer->datum_channels_,
		pLayer->datum_height_, pLayer->datum_width_, NULL, 0, INIT_BLOB_WITH_MALLOC);
}

void InitHandWrittenImageDataLayer(HandwrittenImageDataLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	pLayer->batch_size_     = 1;
	pLayer->datum_channels_ = pLayer->param_.data_channels_;
	pLayer->datum_height_   = pLayer->param_.data_outer_size_;
	pLayer->datum_width_    = pLayer->param_.data_outer_size_;
	pLayer->datum_size_     = pLayer->datum_channels_ 
		* pLayer->datum_height_ * pLayer->datum_width_;

	// top data blob
	pTop->head_ = UNINITIALIZED;
	InitBlob(pTop, TYPE_F32, pLayer->batch_size_, pLayer->datum_channels_,
		pLayer->datum_height_, pLayer->datum_width_, NULL, 0, INIT_BLOB_WITH_MALLOC);
}

void InitConvolutionLayer(ConvolutionLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	S32 height_out = 0;
	S32 width_out  = 0;
	
	pLayer->num_         = pBottom->num_;
	pLayer->channels_    = pBottom->channels_;
	pLayer->height_      = pBottom->height_;
	pLayer->width_       = pBottom->width_;
	pLayer->kernel_size_ = pLayer->param_.weight_height_;
	pLayer->num_output_  = pLayer->param_.weight_num_;

	height_out = pLayer->height_ - pLayer->kernel_size_ + 1;
	width_out  = pLayer->width_ - pLayer->kernel_size_ + 1;

	// Figure out the dimension for individual gemms
	pLayer->M_ = pLayer->num_output_;
	pLayer->K_ = pLayer->channels_ * pLayer->kernel_size_ * pLayer->kernel_size_;
	pLayer->N_ = height_out * width_out;

	// The im2col result buffer would only hold one image at a time to avoid
	// overly large memory usage.
	pLayer->col_buffer_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->col_buffer_, TYPE_F32, 1, pLayer->K_, height_out, width_out,
		NULL, 0, INIT_BLOB_WITH_MALLOC);

	// Initialize top blob
	pTop->head_ = UNINITIALIZED;
	InitBlob(pTop, TYPE_F32, pLayer->num_, pLayer->num_output_, height_out, width_out,
		NULL, 0, INIT_BLOB_WITH_MALLOC);

	// Initialize bias_muliplier
	pLayer->bias_multiplier_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->bias_multiplier_, TYPE_F32, 1, 1, 1, pLayer->N_,
		NULL, 1, INIT_BLOB_WITH_MALLOC);
}

void InitPoolingLayer(PoolingLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	pLayer->kernel_size_ = pLayer->param_.kernel_size_;
	pLayer->stride_      = pLayer->param_.stride_;
	pLayer->channels_    = pBottom->channels_;
	pLayer->height_      = pBottom->height_;
	pLayer->width_       = pBottom->width_;

	pLayer->pooled_height_ = (S32)(ceil((float)(pLayer->height_ 
		- pLayer->kernel_size_) / pLayer->stride_)) + 1;

	pLayer->pooled_width_ = (S32)(ceil((float)(pLayer->width_ 
		- pLayer->kernel_size_) / pLayer->stride_)) + 1;
	
	// Initialize the top data.
	pTop->head_ = UNINITIALIZED;
	InitBlob(pTop, TYPE_F32, pBottom->num_, pLayer->channels_, 
		pLayer->pooled_height_, pLayer->pooled_width_,
		NULL, 0, INIT_BLOB_WITH_MALLOC);
}

void InitCCCPLayer(CCCPLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	pLayer->num_        = pBottom->num_;
	pLayer->channels_   = pBottom->channels_;
	pLayer->height_     = pBottom->height_;
	pLayer->width_      = pBottom->width_;
	pLayer->num_output_ = pLayer->weight_.num_;

	// Figure out the dimension for individual gemms
	pLayer->M_ = pLayer->num_output_;
	pLayer->K_ = pLayer->channels_;
	pLayer->N_ = pLayer->height_ * pLayer->width_;

	// Initialize top data
	pTop->head_ = UNINITIALIZED;
	InitBlob(pTop, TYPE_F32, pLayer->num_, pLayer->num_output_, 
		pLayer->height_, pLayer->width_, NULL, 0, INIT_BLOB_WITH_MALLOC);

	// Initialize bias_multiplier
	pLayer->bias_multiplier_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->bias_multiplier_, TYPE_F32, 1, 1, 1, pLayer->N_,
		NULL, 1, INIT_BLOB_WITH_MALLOC);
}

void InitInnerProductLayer(InnerProductLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	pLayer->num_output_ = pLayer->param_.weight_height_;

	// Figure out the dimension for individual gemms
	pLayer->M_ = pBottom->num_;
	pLayer->K_ = pBottom->count_ / pBottom->num_;
	pLayer->N_ = pLayer->num_output_;

	// Initialize top data
	pTop->head_ = UNINITIALIZED;
	InitBlob(pTop, TYPE_F32, pBottom->num_, pLayer->num_output_, 1, 1,
		NULL, 0, INIT_BLOB_WITH_MALLOC);

	// Initialize bias_multiplier
	pLayer->bias_multiplier_.head_ = UNINITIALIZED;
	InitBlob(&pLayer->bias_multiplier_, TYPE_F32, 1, 1, 1, pLayer->M_,
		NULL, 1, INIT_BLOB_WITH_MALLOC);
}

void InitSoftmaxLayer(SoftmaxLayer* pLayer, Blob* pBottom, Blob* pTop)
{
    // Initialize top data
    pTop->head_ = UNINITIALIZED;
    InitBlob(pTop, TYPE_F32, pBottom->num_, pBottom->channels_, pBottom->height_, pBottom->width_,
        NULL, 0, INIT_BLOB_WITH_MALLOC);

    // Initialzie sum_multiplier
    pLayer->sum_multiplier_.head_ = UNINITIALIZED;
    InitBlob(&pLayer->sum_multiplier_, TYPE_F32, 1, pBottom->channels_, pBottom->height_, 
        pBottom->width_, NULL, 1, INIT_BLOB_WITH_MALLOC);

    // Initialize scale
    pLayer->scale_.head_ = UNINITIALIZED;
    InitBlob(&pLayer->scale_, TYPE_F32, pBottom->num_, 1, 1, 1, NULL, 0, INIT_BLOB_WITH_MALLOC);
}

static void preproc(const cv::Mat& cv_img, cv::Mat& dst_img)
{
	Mat cv_img_resize;
	resize(cv_img, cv_img_resize, Size(28,28));
	int threshold_type=CV_THRESH_BINARY;
	int adaptive_method=CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	int blocksize=31;
	double offset=15;
	Mat cv_img_erzhi;
	adaptiveThreshold(cv_img_resize, cv_img_erzhi, 255, adaptive_method, threshold_type, blocksize, offset);
	cv_img_erzhi=255-cv_img_erzhi;
	vector<vector<Point> > storage;
	vector<Point> contmax;
	vector<Vec4i> hierarchy;
	Mat cv_img_tmp=cv_img_erzhi.clone();
	findContours(cv_img_erzhi, storage, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	double maxArea=0;
	Rect rect(0,0,0,0);
	for(int i=0; i<storage.size(); i++)
	{
		double area=fabs(contourArea(storage[i]));
		if(area>maxArea)
		{
			maxArea=area;
			contmax=storage[i];
			rect=boundingRect(contmax);
		}
	}

	Mat cv_img_roi;
	cv_img_roi=cv_img_tmp(Rect(rect.x, rect.y, rect.width, rect.height));
	Mat cv_img_dst = Mat::zeros(28,28, cv_img_roi.type());
	if(rect.width>rect.height)
	{
		int reheight=((float)20/rect.width)*rect.height;
		resize(cv_img_roi, cv_img_roi, Size(20, reheight));
		for(int j=0; j<reheight; j++)
	   {
			for(int i=0; i<20; i++)
			{
				cv_img_dst.at<uchar>((28-reheight)/2+j,4+i)=cv_img_roi.at<uchar>(j, i);
			}
		}
	}
	else
	{
		int rewidth=((float)20/rect.height)*rect.width;
		resize(cv_img_roi, cv_img_roi, Size(rewidth, 20));
		for(int j=0; j<20; j++)
		{
			for(int i=0; i<rewidth; i++)
			{
				cv_img_dst.at<uchar>(j+4,(28-rewidth)/2+i)=cv_img_roi.at<uchar>(j, i);
			}
		}
	}

	dst_img = cv_img_dst.clone();
}

void ForwardHandwrittenImageDataLayer(HandwrittenImageDataLayer* pLayer, const Mat& img, Blob* pTop)
{
	cv::Mat cv_img;
	preproc(img, cv_img);

    for (int i = 0; i < cv_img.rows; i++)
    {
        for (int j = 0; j < cv_img.cols; ++j)
        {
            int idx = Offset(pTop, 0, 0, i, j);
            pTop->data.data_f32_[idx] = (float(cv_img.at<uchar>(i, j)) - pLayer->param_.mean_) * pLayer->param_.scale_;
        }
    }
}

#if 0

void ForwardPenStrokeDataLayer(PenStrokeDataLayer* pLayer, vector<Penstroke>* pPenStrokeVec, Blob* pTop)
{	
	for(int i = 0; i < pTop->count_; ++i)
	{
		pTop->data.data_s32_[i] = 0;
	}

	calcEightDirectFeat(*pPenStrokeVec, pTop);

	S32 n = pTop->height_ * pTop->width_;
	for (int i = n; i < pTop->count_; ++i) // channel 1 ~ 8 multiplied by 2
	{
		pTop->data.data_s32_[i] <<= 1;
	}


#ifdef SHOW_FEATMAP

	for (int i = 0; i < NUM_INPUT_FEATMAP; ++i)
	{
		cv::Mat mat = cv::Mat::zeros(EXT_ONLINE_CHARACTER_SIZE, EXT_ONLINE_CHARACTER_SIZE, CV_8UC1);
		for (int y = 0; y < EXT_ONLINE_CHARACTER_SIZE; ++y)
		{
			for (int x = 0; x < EXT_ONLINE_CHARACTER_SIZE; ++x)
			{
				if(pTop->data.data_s32_[Offset(pTop, 0, i, y, x)] > 0)
					mat.at<uchar>(y,x) = 255;
			}
		}
		cv::imwrite("D:/" + num2str(i) + ".jpg", mat);
		cv::imshow(num2str(i), mat);
		cv::waitKey(0);
	}

#endif
}

void ForwardPenStrokeDataLayer(PenStrokeDataLayer* pLayer, SPOINT* pInks, S32 nPts, Blob* pTop)
{
	shift_origin(pInks, nPts);
	norm_XxY(pInks, nPts, 40, 40);
	shift_center(pInks, nPts);
	calc_feat_ex(pInks, nPts, pTop);

	S32 n = pTop->height_ * pTop->width_;
	for (int i = n; i < pTop->count_; ++i) // channel 1 ~ 8 multiplied by 2
	{
		pTop->data.data_s32_[i] <<= 1;
	}

#ifdef SHOW_FEATMAP

	for (int i = 0; i < NUM_INPUT_FEATMAP; ++i)
	{
		cv::Mat mat = cv::Mat::zeros(EXT_ONLINE_CHARACTER_SIZE, EXT_ONLINE_CHARACTER_SIZE, CV_8UC1);
		for (int y = 0; y < EXT_ONLINE_CHARACTER_SIZE; ++y)
		{
			for (int x = 0; x < EXT_ONLINE_CHARACTER_SIZE; ++x)
			{
				if(pTop->data.data_s32_[Offset(pTop, 0, i, y, x)] > 0)
					mat.at<uchar>(y,x) = 255;
			}
		}
		cv::imwrite("D:/" + num2str(i) + ".jpg", mat);
		cv::imshow(num2str(i), mat);
		cv::waitKey(0);
	}

#endif
}

#endif

void ForwardConvolutionLayer(ConvolutionLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	S32 n;

	const F32* bottom_data = pBottom->data.data_f32_;
	F32* top_data          = pTop->data.data_f32_;
	F32* col_data          = pLayer->col_buffer_.data.data_f32_;

	const F32* weight          = pLayer->weight_.data.data_f32_;
	const F32* bias            = pLayer->bias_.data.data_f32_;
	const F32* bias_multiplier = pLayer->bias_multiplier_.data.data_f32_;

	for (n = 0; n < pLayer->num_; ++n) 
	{
		// First, im2col
		im2col(bottom_data + Offset(pBottom, n, 0, 0, 0), 
			pLayer->channels_, pLayer->height_, 
			pLayer->width_, pLayer->kernel_size_, col_data);

		// Second, inner-product weight with col_buffer
		gemm(CblasNoTrans, CblasNoTrans, CblasTypeF32,
			pLayer->M_, pLayer->N_, pLayer->K_,
			1, weight, col_data,
			0, top_data + Offset(pTop, n, 0, 0, 0));

		// Third, add bias
		gemm(CblasNoTrans, CblasNoTrans, CblasTypeF32,
			pLayer->M_, pLayer->N_, 1, 
			1, bias, bias_multiplier,
			1, top_data + Offset(pTop, n, 0, 0, 0));
	}
}

void ForwardPoolingLayer(PoolingLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	S32 i, n, c, ph, pw, h, w;

	const F32* bottom_data = pBottom->data.data_f32_;
	F32* top_data = pTop->data.data_f32_;

	// Initialize
	for (i = 0; i < pTop->count_; ++i) 
	{
		top_data[i] = -INT_MAX;
	}

	// The main loop
	for (n = 0; n < pBottom->num_; ++n) 
	{
		for (c = 0; c < pLayer->channels_; ++c) 
		{
			for (ph = 0; ph < pLayer->pooled_height_; ++ph) 
			{
				for (pw = 0; pw < pLayer->pooled_width_; ++pw) 
				{
					S32 hstart = ph * pLayer->stride_;
					S32 wstart = pw * pLayer->stride_;
					S32 hend = MIN(hstart + pLayer->kernel_size_, pLayer-> height_);
					S32 wend = MIN(wstart + pLayer->kernel_size_, pLayer->width_);

					for (h = hstart; h < hend; ++h) 
					{
						for (w = wstart; w < wend; ++w) 
						{
							top_data[ph * pLayer->pooled_width_ + pw] 
								= MAX(top_data[ph * pLayer->pooled_width_ + pw],
									bottom_data[h * pLayer->width_ + w]);
						}
					}
				}
			}

			// compute offset
			bottom_data += Offset(pBottom, 0, 1, 0, 0);
			top_data += Offset(pTop, 0, 1, 0, 0);
		}
	}
}

void ForwardCCCPLayer(CCCPLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	S32 n;

	const F32* bottom_data = pBottom->data.data_f32_;
	F32* top_data = pTop->data.data_f32_;
	 
	const F32* weight          = pLayer->weight_.data.data_f32_;
	const F32* bias            = pLayer->bias_.data.data_f32_;
	const F32* bias_multiplier = pLayer->bias_multiplier_.data.data_f32_;


	for (n = 0; n < pLayer->num_; ++n)
	{
		 // First, inner-product weight with bottom_data
		gemm(CblasNoTrans, CblasNoTrans, CblasTypeF32,
			pLayer->M_, pLayer->N_, pLayer->K_,
			1, weight, bottom_data + Offset(pBottom, n, 0, 0, 0),
			0, top_data + Offset(pTop, n, 0, 0, 0));

		 // Second, add bias
		gemm(CblasNoTrans, CblasNoTrans, CblasTypeF32,
			pLayer->M_, pLayer->N_, 1,
			1, bias, bias_multiplier,
			1, top_data + Offset(pTop, n, 0, 0, 0));
	}
}

void ForwardInnerProductLayer(InnerProductLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	const F32* bottom_data = pBottom->data.data_f32_;
	F32* top_data          = pTop->data.data_f32_;

	const F32* weight          = pLayer->weight_.data.data_f32_;
	const F32* bias            = pLayer->bias_.data.data_f32_;
	const F32* bias_multiplier = pLayer->bias_multiplier_.data.data_f32_;

	// First, inner-product bottom data with weight
	gemm(CblasNoTrans, CblasTrans, CblasTypeF32,
		pLayer->M_, pLayer->N_, pLayer->K_,
		1, bottom_data, weight, 0, top_data);

	// Second, add bias
	gemm(CblasNoTrans, CblasNoTrans, CblasTypeF32, 
		pLayer->M_, pLayer->N_, 1, 
		1, bias_multiplier, bias, 1, top_data);
}

void ForwardSoftmaxLayer(SoftmaxLayer* pLayer, Blob* pBottom, Blob* pTop)
{
	const float* bottom_data    = pBottom->data.data_f32_;
	float* top_data             = pTop->data.data_f32_;	
	const float* sum_multiplier = pLayer->sum_multiplier_.data.data_f32_;
	float* scale_data           = pLayer->scale_.data.data_f32_;

	int num = pBottom->num_;
	int dim = pBottom->count_ / pBottom->num_;
	memcpy(top_data, bottom_data, sizeof(float) * pBottom->count_);

	// we need to subtract the max to avoid numerical issues,
	// compute the exp, and then normalize
	for (int i = 0; i < num; ++i)
	{
		scale_data[i] = bottom_data[i*dim];
		for (int j = 0; j < dim; ++j)
		{
			scale_data[i] = MAX(scale_data[i], bottom_data[i * dim + j]);
		}
	}

	// subtraction
	gemm(CblasNoTrans, CblasNoTrans, CblasTypeF32, 
		num, dim, 1, 
		-1.0, scale_data, sum_multiplier, 
		1.0, top_data);

	// perform exponentiation
	vsExp(num * dim, top_data, top_data);

	// sum after exp
	gemv(CblasNoTrans, num, dim,
		1.0, top_data, sum_multiplier, 
		0, scale_data);

	// do division
	for (int i = 0; i < num; ++i)
	{
		scal(dim, 1.0 / scale_data[i], top_data + i * dim);
	}
}

void im2col(const F32* data_im, const S32 channels, const S32 height, 
	const S32 width, const S32 ksize, F32* data_col) 
{
	S32 height_col   = height - ksize + 1;
	S32 width_col    = width - ksize + 1;
	S32 channels_col = channels * ksize * ksize;

	S32 c, h, w;
	for (c = 0; c < channels_col; ++c) 
	{
		S32 w_offset = c % ksize;
		S32 h_offset = (c / ksize) % ksize;
		S32 c_im = c / ksize / ksize;

		for (h = 0; h < height_col; ++h) 
		{
			for (w = 0; w < width_col; ++w) 
			{
				S32 h_pad = h + h_offset;
				S32 w_pad = w + w_offset;

				if (h_pad >= 0 && h_pad < height && w_pad >= 0 && w_pad < width)
				{
					data_col[(c * height_col + h) * width_col + w] 
						= data_im[(c_im * height + h_pad) * width + w_pad];
				}
				else
				{
					data_col[(c * height_col + h) * width_col + w] = 0;
				}
			}
		}
	}
}

void Relu(Blob* pBlob)
{
	S32 i;
	if (pBlob->data_type_ == TYPE_S16)
	{
		for (i = 0; i < pBlob->count_; ++i)
		{
			pBlob->data.data_s16_[i] = MAX(pBlob->data.data_s16_[i], 0);
		}
	}
	else if (pBlob->data_type_ == TYPE_S32)
	{
		for (i = 0; i < pBlob->count_; ++i)
		{
		   pBlob->data.data_s32_[i] = MAX(pBlob->data.data_s32_[i], 0);
		}
	}
    else if (pBlob->data_type_ == TYPE_F32)
    {
        for (i = 0; i < pBlob->count_; ++i)
        {
            pBlob->data.data_f32_[i] = MAX(pBlob->data.data_f32_[i], 0);
        }
    }
}
