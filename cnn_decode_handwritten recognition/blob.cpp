#include "blob.h"

void InitBlob(Blob* pBlob, DataType data_type, const S32 num, const S32 channels, 
	const S32 height, const S32 width, void* pData, const S32 init_val, InitBlobMethod method)
{
	if (pBlob->head_ == INITIALIZED)
	{
		perror("Error: Blob has been initialized.");
		exit(EXIT_FAILURE);
	}

	pBlob->num_      = num;
	pBlob->channels_ = channels;
	pBlob->height_   = height;
	pBlob->width_    = width;
	pBlob->count_    = num * channels * height * width;

	if (method == INIT_BLOB_WITH_DATA)
	{
		if (pData == NULL)
		{
			perror("Error: Blob don't accept null data pointer");
			exit(EXIT_FAILURE);
		}

		if (data_type == TYPE_S16)
		{
			if (pBlob->data.data_s16_ != (S16*)pData)
			{
				pBlob->data.data_s16_ = (S16*)pData;
			}

			pBlob->data_type_ = TYPE_S16;
		}
		else if (data_type == TYPE_S32)
		{
			if (pBlob->data.data_s32_ != (S32*)pData)
			{
				pBlob->data.data_s32_ = (S32*)pData;
			}

			pBlob->data_type_ = TYPE_S32;
		}
        else if (data_type == TYPE_F32)
        {
            if (pBlob->data.data_f32_ != (F32*)pData)
            {
                pBlob->data.data_f32_ = (F32*)pData;
            }

            pBlob->data_type_ = TYPE_F32;
        }
		else
		{
			perror("Error: Unknown data type pointer");
			exit(EXIT_FAILURE);
		}
		
		pBlob->head_ = INITIALIZED;
	}
	else if (method == INIT_BLOB_WITHOUT_DATA)
	{
		pBlob->head_      = UNINITIALIZED;
		pBlob->data_type_ = data_type;
	}
	else if (method == INIT_BLOB_WITH_MALLOC)
	{
		if (data_type == TYPE_S16)
		{
			S32 i;

			pBlob->data_type_     = TYPE_S16;
			pBlob->data.data_s16_ = (S16*)malloc(sizeof(S16) * pBlob->count_);
			
			for (i = 0; i < pBlob->count_; ++i)
			{
				pBlob->data.data_s16_[i] = (S16)init_val;
			}
		}
		else if (data_type == TYPE_S32)
		{
			S32 i;

			pBlob->data_type_     = TYPE_S32;
			pBlob->data.data_s32_ = (S32*)malloc(sizeof(S32) * pBlob->count_);
		
			for (i = 0; i < pBlob->count_; ++i)
			{
				pBlob->data.data_s32_[i] = (S32)init_val;
			}
		}
		else if (data_type == TYPE_F32)
		{
			S32 i;

			pBlob->data_type_     = TYPE_F32;
			pBlob->data.data_f32_ = (F32*)malloc(sizeof(F32) * pBlob->count_);
		
			for (i = 0; i < pBlob->count_; ++i)
			{
				pBlob->data.data_f32_[i] = (F32)init_val;
			}
		}
		else
		{
			perror("Unknown data type for blob");
		}
		
		pBlob->head_ = INITIALIZED;
	}
}

S32 Offset(Blob* pBlob, const S32 n, const S32 c, const S32 h, const S32 w)
{
	return ( (n * pBlob->channels_ + c) * pBlob->height_ + h ) * pBlob->width_ + w;
}