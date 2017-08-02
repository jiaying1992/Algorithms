#include "math_functions.h"

#define INDEX int
#define OFFSET(N, incX) ((incX) > 0 ?  0 : ((N) - 1) * (-(incX)))

void cblas_sgemm(const enum CBLAS_ORDER Order, 
    const enum CBLAS_TRANSPOSE TransA,
    const enum CBLAS_TRANSPOSE TransB, 
    const int M, const int N, const int K, 
    const F32 alpha,
    const F32 *A, const int lda,
    const F32 *B, const int ldb,
    const F32 beta,
    F32 *C, const int ldc)
{
#define BASE float
        INDEX i, j, k;
        INDEX n1, n2;
        INDEX ldf, ldg;
        int TransF, TransG;
        const BASE *F, *G;

        if (alpha == 0.0 && beta == 1.0)
            return;

        if (Order == CblasRowMajor) {
            n1 = M;
            n2 = N;
            F = A;
            ldf = lda;
            TransF = (TransA == CblasConjTrans) ? CblasTrans : TransA;
            G = B;
            ldg = ldb;
            TransG = (TransB == CblasConjTrans) ? CblasTrans : TransB;
        }
        else {
            n1 = N;
            n2 = M;
            F = B;
            ldf = ldb;
            TransF = (TransB == CblasConjTrans) ? CblasTrans : TransB;
            G = A;
            ldg = lda;
            TransG = (TransA == CblasConjTrans) ? CblasTrans : TransA;
        }

        /* form  y := beta*y */
        if (beta == 0.0) {
            for (i = 0; i < n1; i++) {
                for (j = 0; j < n2; j++) {
                    C[ldc * i + j] = 0.0;
                }
            }
        }
        else if (beta != 1.0) {
            for (i = 0; i < n1; i++) {
                for (j = 0; j < n2; j++) {
                    C[ldc * i + j] *= beta;
                }
            }
        }

        if (alpha == 0.0)
            return;

        if (TransF == CblasNoTrans && TransG == CblasNoTrans) {

            /* form  C := alpha*A*B + C */

            for (k = 0; k < K; k++) {
                for (i = 0; i < n1; i++) {
                    const BASE temp = alpha * F[ldf * i + k];
                    if (temp != 0.0) {
                        for (j = 0; j < n2; j++) {
                            C[ldc * i + j] += temp * G[ldg * k + j];
                        }
                    }
                }
            }

        }
        else if (TransF == CblasNoTrans && TransG == CblasTrans) {

            /* form  C := alpha*A*B' + C */

            for (i = 0; i < n1; i++) {
                for (j = 0; j < n2; j++) {
                    BASE temp = 0.0;
                    for (k = 0; k < K; k++) {
                        temp += F[ldf * i + k] * G[ldg * j + k];
                    }
                    C[ldc * i + j] += alpha * temp;
                }
            }

        }
        else if (TransF == CblasTrans && TransG == CblasNoTrans) {

            for (k = 0; k < K; k++) {
                for (i = 0; i < n1; i++) {
                    const BASE temp = alpha * F[ldf * k + i];
                    if (temp != 0.0) {
                        for (j = 0; j < n2; j++) {
                            C[ldc * i + j] += temp * G[ldg * k + j];
                        }
                    }
                }
            }

        }
        else if (TransF == CblasTrans && TransG == CblasTrans) {

            for (i = 0; i < n1; i++) {
                for (j = 0; j < n2; j++) {
                    BASE temp = 0.0;
                    for (k = 0; k < K; k++) {
                        temp += F[ldf * k + i] * G[ldg * j + k];
                    }
                    C[ldc * i + j] += alpha * temp;
                }
            }

        }
        else {
            perror("unrecognized operation");
        }
#undef BASE
}

void gemm(
	const enum CBLAS_TRANSPOSE TransA, 
	const enum CBLAS_TRANSPOSE TransB,
	const enum CBLAS_DATATYPE data_type,
	const S32 M, const S32 N, const S32 K,
	const F32 alpha, 
	const F32* A, 
	const F32* B, 
	const F32 beta, 
	F32* C)
{
	S32 lda = (TransA == CblasNoTrans) ? K : M;
	S32 ldb = (TransB == CblasNoTrans) ? N : K;

    cblas_sgemm(CblasRowMajor, TransA, TransB,
		M, N, K, alpha, A, lda, B, ldb, beta, C, N);

	return;
}

// ============================================================================

static void cblas_sgemv (const enum CBLAS_ORDER order, const enum CBLAS_TRANSPOSE TransA,
	const int M, const int N, const float alpha, const float *A,
	const int lda, const float *X, const int incX, const float beta,
	float *Y, const int incY)
{
#define BASE float
	INDEX i, j;
	INDEX lenX, lenY;

	const int Trans = (TransA != CblasConjTrans) ? TransA : CblasTrans;

	if (M == 0 || N == 0)
		return;

	if (alpha == 0.0 && beta == 1.0)
		return;

	if (Trans == CblasNoTrans) {
		lenX = N;
		lenY = M;
	} else {
		lenX = M;
		lenY = N;
	}

	/* form  y := beta*y */
	if (beta == 0.0) {
		INDEX iy = OFFSET(lenY, incY);
		for (i = 0; i < lenY; i++) {
			Y[iy] = 0.0;
			iy += incY;
		}
	} else if (beta != 1.0) {
		INDEX iy = OFFSET(lenY, incY);
		for (i = 0; i < lenY; i++) {
			Y[iy] *= beta;
			iy += incY;
		}
	}

	if (alpha == 0.0)
		return;

	if ((order == CblasRowMajor && Trans == CblasNoTrans)
		|| (order == CblasColMajor && Trans == CblasTrans)) {
			/* form  y := alpha*A*x + y */
			INDEX iy = OFFSET(lenY, incY);
			for (i = 0; i < lenY; i++) {
				BASE temp = 0.0;
				INDEX ix = OFFSET(lenX, incX);
				for (j = 0; j < lenX; j++) {
					temp += X[ix] * A[lda * i + j];
					ix += incX;
				}
				Y[iy] += alpha * temp;
				iy += incY;
			}
	} else if ((order == CblasRowMajor && Trans == CblasTrans)
		|| (order == CblasColMajor && Trans == CblasNoTrans)) {
			/* form  y := alpha*A'*x + y */
			INDEX ix = OFFSET(lenX, incX);
			for (j = 0; j < lenX; j++) {
				const BASE temp = alpha * X[ix];
				if (temp != 0.0) {
					INDEX iy = OFFSET(lenY, incY);
					for (i = 0; i < lenY; i++) {
						Y[iy] += temp * A[lda * j + i];
						iy += incY;
					}
				}
				ix += incX;
			}
	} else {
		exit(EXIT_FAILURE);
	}
#undef BASE
}

void cblas_sscal (const int N, const float alpha, float *X, const int incX)
{
#define BASE float
	INDEX i;
	INDEX ix;

	if (incX <= 0) {
		return;
	}

	ix = OFFSET(N, incX);

	for (i = 0; i < N; i++) {
		X[ix] *= alpha;
		ix += incX;
	}
#undef BASE
}

#undef INDEX
#undef OFFSET

void gemv(
	const CBLAS_TRANSPOSE TransA, 
	const int M, const int N,
	const float alpha, 
	const float* A, 
	const float* x, 
	const float beta, 
	float* y)
{
	cblas_sgemv(CblasRowMajor, TransA, M, N, 
		alpha, A, N, x, 1, beta, y, 1);
}

// ===========================================================

void scal(const int n, const float alpha, float* x)
{
	cblas_sscal(n, alpha, x, 1);
}
