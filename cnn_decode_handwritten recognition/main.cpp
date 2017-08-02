#include "net.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

extern double runtime[14];

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "main.exe [model] [img]\n";
		return 1;
	}

	/* some definitions	*/
	FILE* pFile = NULL;
	long fileSize = 0;
	char* buffer = NULL;
	size_t retVal = 0;

	pFile = fopen(argv[1], "rb");
	if (pFile == NULL)
	{
		perror("File opening error");
		exit(EXIT_FAILURE);
	}

	/* obtain file size. */
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	rewind(pFile);

	/* allocate memory to contain the whole file.
	   buffer cannot be free() unitl the end of the 
	   main() func. */
	buffer = (char*)malloc(sizeof(char)* fileSize);
	if (buffer == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	/* copy the file into the buffer. */
	retVal = fread(buffer, 1, fileSize, pFile);
	if (retVal != fileSize)
	{
		perror("Reading error");
		exit(EXIT_FAILURE);
	}

	/* close file. */
	fclose(pFile);

	// load net paramter from buffer.
	Net net;
	int modelSize = LoadNetParam(&net, buffer);
	if (modelSize != fileSize)
	{
		perror("Loading model error");
		exit(EXIT_FAILURE);
	}

	InitNet(&net);
	
	cv::Mat cv_img = cv::imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
		if (cv_img.empty())
		{
			return 1;
		}

		int label = -1;
		ForwardNet(&net, cv_img, &label);
		std::cout<<label<<std::endl;
	
	ReleaseNet(&net);

	return 0;
}
