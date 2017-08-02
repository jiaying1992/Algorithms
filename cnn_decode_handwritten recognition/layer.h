#ifndef LAYER_H_
#define LAYER_H_

#include <limits.h>
#include "opencv2/opencv.hpp"
#include "common.h"
#include "blob.h"
//#include "feat.h"
//#include "feat_c.h"
#include "math_functions.h"

//#include "opencv2/opencv.hpp"

enum LayerType
{
	HANDWRITTEN_IMAGE_DATA = 5,
	PENSTROKE_DATA = 0,
	CONVOLUTION    = 1,
	CCCP           = 2,
	POOLING        = 3,
	INNER_PRODUCT  = 4,
	SOFTMAX        = 6  
};


typedef struct tagHandwrittenImageDataLayerParameter {
	LayerType type_;

	float mean_;
	float scale_;
	U16 data_channels_;
	U16 data_inner_size_;
	U16 data_outer_size_;
} HandwrittenImageDataLayerParameter;


typedef struct tagPenStrokeDataLayerParameter {
	LayerType type_;

	U16 data_channels_;
	U16 data_inner_size_;
	U16 data_outer_size_;
}  PenStrokeDataLayerParameter;

typedef struct tagConvolutionLayerParameter {
	LayerType type_;

	U16 weight_num_;
	U16 weight_channels_;
	U16 weight_height_;
	U16 weight_width_;

	U16 bias_num_;
	U16 bias_channels_;
	U16 bias_height_;
	U16 bias_width_;
} ConvolutionLayerParameter;

typedef struct tagCCCPLayerParameter
{
	LayerType type_;

	U16 weight_num_;
	U16 weight_channels_;
	U16 weight_height_;
	U16 weight_width_;

	U16 bias_num_;
	U16 bias_channels_;
	U16 bias_height_;
	U16 bias_width_;
} CCCPLayerParameter;

typedef struct tagPoolingLayerParamter {
	LayerType type_;
	U16 kernel_size_;
	U16 stride_;
} PoolingLayerParameter;

typedef struct tagInnerProductLayerParameter {
	LayerType type_;

	U16 weight_num_;
	U16 weight_channels_;
	U16 weight_height_;
	U16 weight_width_;

	U16 bias_num_;
	U16 bias_channels_;
	U16 bias_height_;
	U16 bias_width_;
} InnerProductLayerParameter;

// ======================================================

typedef struct tagHandwrittenImageDataLayer {
	HandwrittenImageDataLayerParameter param_;

	U16 batch_size_;
	U16 datum_channels_;
	U16 datum_height_;
	U16 datum_width_;
	U16 datum_size_;
} HandwrittenImageDataLayer;

typedef struct tagPenStrokeDataLayer {
		PenStrokeDataLayerParameter param_;

		U16 batch_size_;
		U16 datum_channels_;
		U16 datum_height_;
		U16 datum_width_;
		U16 datum_size_;
} PenStrokeDataLayer;

typedef struct tagConvolutionLayer {
	ConvolutionLayerParameter param_;

	Blob weight_;
	Blob bias_;
	Blob col_buffer_;
	Blob bias_multiplier_;

	S32 num_;
	S32 channels_;
	S32 height_;
	S32 width_;
	S32 num_output_;
	S32 kernel_size_;

	S32 M_;
	S32 K_;
	S32 N_;
} ConvolutionLayer;

typedef struct {
	PoolingLayerParameter param_;

	S32 kernel_size_;
	S32 stride_;
	S32 channels_;
	S32 height_;
	S32 width_;
	S32 pooled_height_;
	S32 pooled_width_;
} PoolingLayer;

typedef struct tagCCCPLayer {
	CCCPLayerParameter param_;

	Blob weight_;
	Blob bias_;
	Blob bias_multiplier_;

	S32 num_;
	S32 channels_;
	S32 height_;
	S32 width_;
	S32 num_output_;

	S32 M_;
	S32 K_;
	S32 N_;
} CCCPLayer;

typedef struct tagInnerProductLayer {
	InnerProductLayerParameter param_;

	Blob weight_;
	Blob bias_;
	Blob bias_multiplier_;

	S32 num_output_;

	S32 M_;
	S32 K_;
	S32 N_;
} InnerProductLayer;

typedef struct tagSoftmaxLayer {
    Blob sum_multiplier_;
    Blob scale_;
} SoftmaxLayer;

// load layer parameter
void LoadPenStrokeDataLayerParam(PenStrokeDataLayer* pLayer, S8** ptr);

void LoadHandWrittenImageDataLayerParam(HandwrittenImageDataLayer* pLayer, S8** ptr);

void LoadConvolutionDataLayerParam(ConvolutionLayer* pLayer, S8** ptr);

void LoadPoolingLayerParam(PoolingLayer* pLayer, S8** ptr);

void LoadCCCPLayerParam(CCCPLayer* pLayer, S8** ptr);

void LoadInnerProductLayerParam(InnerProductLayer* pLayer, S8** ptr);

// initialize each individual layer
void InitPenStrokeDataLayer(PenStrokeDataLayer* pLayer, Blob* pBottom, Blob* pTop);

void InitHandWrittenImageDataLayer(HandwrittenImageDataLayer* pLayer, Blob* pBottom, Blob* pTop);

void InitConvolutionLayer(ConvolutionLayer* pLayer, Blob* pBottom, Blob* pTop);

void InitPoolingLayer(PoolingLayer* pLayer, Blob* pBottom, Blob* pTop);

void InitCCCPLayer(CCCPLayer* pLayer, Blob* pBottom, Blob* pTop);

void InitInnerProductLayer(InnerProductLayer* pLayer, Blob* pBottom, Blob* pTop);

void InitSoftmaxLayer(SoftmaxLayer* pLayer, Blob* pBottom, Blob* pTop);

// forward propagation for each layer
void ForwardHandwrittenImageDataLayer(HandwrittenImageDataLayer* pLayer, const cv::Mat& img, Blob* pTop);

#if 0
void ForwardPenStrokeDataLayer(PenStrokeDataLayer* pLayer, vector<Penstroke>* pPenStrokeVec, Blob* pTop);
void ForwardPenStrokeDataLayer(PenStrokeDataLayer* pLayer, SPOINT* pInks, S32 nPts, Blob* pTop);
#endif

void ForwardConvolutionLayer(ConvolutionLayer* pLayer, Blob* pBottom, Blob* pTop);

void ForwardPoolingLayer(PoolingLayer* pLayer, Blob* pBottom, Blob* pTop);

void ForwardCCCPLayer(CCCPLayer* pLayer, Blob* pBottom, Blob* pTop);

void ForwardInnerProductLayer(InnerProductLayer* pLayer, Blob* pBottom, Blob* pTop);

void ForwardSoftmaxLayer(SoftmaxLayer* pLayer, Blob* pBottom, Blob* pTop);

// im2col
void im2col(const F32* data_im, const S32 channels, const S32 height,
	const S32 width, const S32 ksize, F32* data_col);

void Relu(Blob* pBlob);

#endif
