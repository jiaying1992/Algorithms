#include "net.h"

using namespace cv;

double runtime[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

U32 LoadNetParam(Net* pNet, S8* pBuffer)
{
	S8* ptr = pBuffer;

	LoadHandWrittenImageDataLayerParam(&pNet->data_layer_, &ptr);

	LoadConvolutionDataLayerParam(&pNet->conv1_layer_, &ptr);
	LoadPoolingLayerParam(&pNet->pool1_layer_,         &ptr);

	LoadConvolutionDataLayerParam(&pNet->conv2_layer_, &ptr);
	LoadPoolingLayerParam(&pNet->pool2_layer_,         &ptr);

	LoadInnerProductLayerParam(&pNet->fc5_layer_,      &ptr);
	LoadInnerProductLayerParam(&pNet->fc6_layer_,      &ptr);

	return (U32)(ptr - pBuffer);
}

void InitNet(Net* net)
{
	InitHandWrittenImageDataLayer(&(net->data_layer_), NULL, &(net->data_top_));

	InitConvolutionLayer(&(net->conv1_layer_), &(net->data_top_), &(net->conv1_top_));
	InitPoolingLayer(&(net->pool1_layer_), &(net->conv1_top_), &(net->pool1_top_));

	InitConvolutionLayer(&(net->conv2_layer_), &(net->pool1_top_), &(net->conv2_top_));
	InitPoolingLayer(&(net->pool2_layer_), &(net->conv2_top_), &(net->pool2_top_));

	InitInnerProductLayer(&(net->fc5_layer_), &(net->pool2_top_), &(net->fc5_top_));
	InitInnerProductLayer(&(net->fc6_layer_), &(net->fc5_top_), &(net->fc6_top_));

	InitSoftmaxLayer(&(net->soft_max_layer_), &(net->fc6_top_), &(net->prob_));
}

static int Argmax(Blob* prob)
{
	float max_val = -1;
	int max_idx = -1;

	for (int i = 0; i < prob->count_; ++i)
	{
		if (prob->data.data_f32_[i] > max_val)
		{
			max_val = prob->data.data_f32_[i];
			max_idx = i;
		}
	}

	return max_idx;
}

void ForwardNet(Net* net, const cv::Mat& img, int* label)
{
	time_t begin = 0;
	time_t end = 0;

	ResetNet(net);
	
	begin = clock();
	ForwardHandwrittenImageDataLayer(&(net->data_layer_), img, &(net->data_top_));
	end = clock();

	//std::cout << "dataLayer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";

	runtime[0] += double(end-begin);

	begin = clock();
	ForwardConvolutionLayer(&(net->conv1_layer_), &(net->data_top_), &(net->conv1_top_));
	//Relu(&(net->conv1_top_));
	end = clock();

	//std::cout << "conv1_layer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";
	runtime[1] += double(end-begin);

	begin = clock();
	ForwardPoolingLayer(&(net->pool1_layer_), &(net->conv1_top_), &(net->pool1_top_));
	end = clock();

	//std::cout << "pool1_layer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";

	runtime[2] += double(end-begin);

	begin = clock();
	ForwardConvolutionLayer(&(net->conv2_layer_), &(net->pool1_top_), &(net->conv2_top_));
	//Relu(&(net->conv2_top_));
	end = clock();

	//std::cout << "conv2_layer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";
	runtime[3] += double(end-begin);

	begin = clock();
	ForwardPoolingLayer(&(net->pool2_layer_), &(net->conv2_top_), &(net->pool2_top_));
	end = clock();

	//std::cout << "pool2_layer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";
	runtime[4] += double(end-begin);

	begin = clock();
	ForwardInnerProductLayer(&(net->fc5_layer_), &(net->pool2_top_), &(net->fc5_top_));
	Relu(&(net->fc5_top_));
	end = clock();

	//std::cout << "fc5_layer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";
	runtime[5] += double(end-begin);

	Relu(&(net->fc5_top_));

    begin = clock();
    ForwardInnerProductLayer(&(net->fc6_layer_), &(net->fc5_top_), &(net->fc6_top_));
    //Relu(&(net->fc6_top_));
    end = clock();

    //std::cout << "fc5_layer runtime: " << double(end-begin) / CLOCKS_PER_SEC * 1000 << " ms\n";
    runtime[6] += double(end - begin);

	ForwardSoftmaxLayer(&(net->soft_max_layer_), &(net->fc6_top_), &(net->prob_));

	*label = Argmax(&(net->prob_));
}

static void ResetToZero(F32* data, F32 count)
{
	for(int i = 0; i < count; ++i)
		data[i] = 0;
}

void ResetNet(Net* net)
{
	ResetToZero(net->data_top_.data.data_f32_, net->data_top_.count_);
	ResetToZero(net->conv1_top_.data.data_f32_, net->conv1_top_.count_);
	ResetToZero(net->conv2_top_.data.data_f32_, net->conv2_top_.count_);
	ResetToZero(net->fc5_top_.data.data_f32_, net->fc5_top_.count_);
	ResetToZero(net->fc6_top_.data.data_f32_, net->fc6_top_.count_);
	ResetToZero(net->prob_.data.data_f32_, net->prob_.count_);
}

void ReleaseNet(Net* net)
{
	if(net->data_top_.data.data_f32_ != NULL) free(net->data_top_.data.data_f32_);
	if(net->conv1_top_.data.data_f32_ != NULL) free(net->conv1_top_.data.data_f32_);
	if(net->pool1_top_.data.data_f32_ != NULL) free(net->pool1_top_.data.data_f32_);
	if(net->conv2_top_.data.data_f32_ != NULL) free(net->conv2_top_.data.data_f32_);
	if(net->pool2_top_.data.data_f32_ != NULL) free(net->pool2_top_.data.data_f32_);
	if(net->fc5_top_.data.data_f32_ != NULL) free(net->fc5_top_.data.data_f32_);
	if(net->fc6_top_.data.data_f32_ != NULL) free(net->fc6_top_.data.data_f32_);
	if(net->prob_.data.data_f32_ != NULL) free(net->prob_.data.data_f32_);

	if(net->conv1_layer_.col_buffer_.data.data_f32_ != NULL) free(net->conv1_layer_.col_buffer_.data.data_f32_);
	if(net->conv1_layer_.bias_multiplier_.data.data_f32_ != NULL) free(net->conv1_layer_.bias_multiplier_.data.data_f32_);
	if(net->conv2_layer_.col_buffer_.data.data_f32_ != NULL) free(net->conv2_layer_.col_buffer_.data.data_f32_);
	if(net->conv2_layer_.bias_multiplier_.data.data_f32_ != NULL) free(net->conv2_layer_.bias_multiplier_.data.data_f32_);
	if(net->fc5_layer_.bias_multiplier_.data.data_f32_ != NULL) free(net->fc5_layer_.bias_multiplier_.data.data_f32_);
	if(net->fc6_layer_.bias_multiplier_.data.data_f32_ != NULL) free(net->fc6_layer_.bias_multiplier_.data.data_f32_);
	if(net->soft_max_layer_.sum_multiplier_.data.data_f32_ != NULL) free(net->soft_max_layer_.sum_multiplier_.data.data_f32_);
	if(net->soft_max_layer_.scale_.data.data_f32_ != NULL) free(net->soft_max_layer_.scale_.data.data_f32_);
}

/*
void PrintNetParam(Net* net)
{
    std::ofstream fout("conv1_weight.txt", ios::trunc | ios::out);

    for (int i = 0; i < (*net).conv1_layer_.weight_.count_; ++i)
    {
        fout << (*net).conv1_layer_.weight_.data.data_f32_[i] << "\n";
    }
    fout.close();

    fout = ofstream("conv1_bias.txt", ios::trunc | ios::out);

    for (int i = 0; i < (*net).conv1_layer_.bias_.count_; ++i)
    {
        fout << (*net).conv1_layer_.bias_.data.data_f32_[i] << "\n";
    }

    fout = ofstream("conv2_weight.txt", ios::trunc | ios::out);

    for (int i = 0; i < (*net).conv2_layer_.weight_.count_; ++i)
    {
        fout << (*net).conv2_layer_.weight_.data.data_f32_[i] << "\n";
    }

    fout = ofstream("conv2_bias.txt", ios::trunc | ios::out);

    for (int i = 0; i < (*net).conv2_layer_.bias_.count_; ++i)
    {
        fout << (*net).conv2_layer_.bias_.data.data_f32_[i] << "\n";
    }

    fout.close();
}
*/
