#ifndef NET_H_
#define NET_H_

#include "common.h"
#include "blob.h"
#include "layer.h"

#include <fstream>
#include <iostream>

#include <time.h>

typedef struct tagNet {
	HandwrittenImageDataLayer data_layer_;
	ConvolutionLayer   conv1_layer_;
	PoolingLayer       pool1_layer_;

	ConvolutionLayer   conv2_layer_;
	PoolingLayer       pool2_layer_;


	InnerProductLayer  fc5_layer_;
	InnerProductLayer  fc6_layer_;

	SoftmaxLayer       soft_max_layer_;

	Blob data_top_;
	Blob conv1_top_;
	Blob pool1_top_;

	Blob conv2_top_;
	Blob pool2_top_;

	Blob fc5_top_;
	Blob fc6_top_;

	Blob prob_;
} Net;

// Load model from file
U32 LoadNetParam(Net* pNet, S8* pBuffer);

void InitNet(Net* net);

void ResetNet(Net* net);

void ForwardNet(Net* net, const cv::Mat& img, int* label);

void ReleaseNet(Net* net);

#endif
