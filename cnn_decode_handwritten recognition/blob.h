#ifndef _BLOB_H_
#define _BLOB_H_

#include "common.h"

typedef enum {
    TYPE_S16 = 0,
    TYPE_S32 = 1,
    TYPE_F32 = 2
} DataType;

typedef enum {
	UNINITIALIZED = 0,
	INITIALIZED   = 1
} SynceHead;

typedef enum {
	INIT_BLOB_WITH_DATA    = 0,
	INIT_BLOB_WITHOUT_DATA = 1,
	INIT_BLOB_WITH_MALLOC  = 2
} InitBlobMethod;

typedef struct tagBlob {
	SynceHead head_;
	DataType data_type_;

	union {
		S16* data_s16_;
		S32*   data_s32_;
        F32*   data_f32_;
	} data;

	S32 num_;
	S32 channels_;
	S32 height_;
	S32 width_;
	S32 count_;
} Blob;

void InitBlob(Blob* pBlob, DataType data_type, const S32 num, const S32 channels, 
	const S32 height, const S32 width, void* pData, const S32 init_val, InitBlobMethod method);

S32 Offset(Blob* pBlob, const S32 n, const S32 c, const S32 h, const S32 w);

#endif
