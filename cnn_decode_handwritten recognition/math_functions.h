#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_

#include "common.h"
#include <cmath>

// A simple way to define the vsl unary functions. The operation should
// be in the form e.g. y[i] = sqrt(a[i])
#define DEFINE_VSL_UNARY_FUNC(name, operation) \
	template<typename Dtype> \
	void v##name(const int n, const Dtype* a, Dtype* y) { \
for (int i = 0; i < n; ++i) { operation; } \
	} \
	inline void vs##name(\
	const int n, const float* a, float* y) { \
	v##name<float>(n, a, y); \
	} \
	inline void vd##name(\
	const int n, const double* a, double* y) { \
	v##name<double>(n, a, y); \
	}

DEFINE_VSL_UNARY_FUNC(Sqr, y[i] = a[i] * a[i]);
DEFINE_VSL_UNARY_FUNC(Exp, y[i] = std::exp(a[i]));
DEFINE_VSL_UNARY_FUNC(Ln, y[i] = std::log(a[i]));
DEFINE_VSL_UNARY_FUNC(Abs, y[i] = std::fabs(a[i]));

enum CBLAS_ORDER {
	CblasRowMajor = 101, 
	CblasColMajor=102
};

enum CBLAS_TRANSPOSE {
	CblasNoTrans = 111, 
	CblasTrans = 112, 
	CblasConjTrans = 113
};

enum CBLAS_DATATYPE {
	CblasTypeA16 = 114,
	CblasTypeB16 = 115, 
	CblasTypeS32 = 116,
    CblasTypeF32 = 117
};

void gemm(
	const enum CBLAS_TRANSPOSE TransA, 
	const enum CBLAS_TRANSPOSE TransB,
	const enum CBLAS_DATATYPE data_type,
	const S32 M, const S32 N, const S32 K,
	const F32 alpha, 
	const F32* A, 
	const F32* B, 
	const F32 beta, 
	F32* C);

void gemv(
	const CBLAS_TRANSPOSE TransA, 
	const int M, const int N,
	const float alpha, 
	const float* A, 
	const float* x, 
	const float beta, 
	float* y);

void scal(const int n, const float alpha, float* x);

#endif
