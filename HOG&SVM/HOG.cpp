/**********************************************************************
**HOG
**计算每一幅图像的HOG
**保存HOG Data和Label结果
**********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp> //包含HOG

using namespace std;
using namespace cv;

const int FILE_NUMBER = 60; //每组取60张作为样本，15张作为测试
const int GROUP_NUMBER = 5; //一共5组
vector<string> imgPath;

//HOG相关参数，总共为28800维
const Size winSize = Size(96, 48);
const Size winStride = Size(48, 24);
const Size blockSize = Size(32, 16);
const Size blockStride = Size(16, 8);
const Size cellSize = Size(8, 8);
const int nbins = 9;
const int dims = 28800;


//获取图像地址
void GetImgPath(string filePath);

//int hog()
int main()
{
	//读入样本数据
	int lable = 0;
	GetImgPath("D:\\test\\sign\\samples\\a\\");
	GetImgPath("D:\\test\\sign\\samples\\b\\");
	GetImgPath("D:\\test\\sign\\samples\\c\\");
	GetImgPath("D:\\test\\sign\\samples\\d\\");
	GetImgPath("D:\\test\\sign\\samples\\e\\");

	//对读入的每一幅样本数据计算HOG
	HOGDescriptor *hog = new HOGDescriptor(winSize, blockSize, blockStride, cellSize, nbins);
	vector <float> descriptors;

	CvMat *trainData, *trainLable;
	trainData = cvCreateMat(FILE_NUMBER * GROUP_NUMBER, dims, CV_32FC1);
	cvSetZero(trainData);
	//注意trainLable的数据类型和trainData不一样
	//保存时先用CV_32FC1保存，读取时再用CV_32SC1读取
	trainLable = cvCreateMat(FILE_NUMBER * GROUP_NUMBER, 1, CV_32FC1); 
	cvSetZero(trainLable);


	Mat trainImg;
	int i = 0, j = 0;
	for (vector<string>::iterator imgIter = imgPath.begin(); imgIter != imgPath.end(); imgIter++)
	{
		if (i < FILE_NUMBER)
			lable = 1;
		else if (i >= FILE_NUMBER && i < FILE_NUMBER * 2)
			lable = 2;
		else if (i >= FILE_NUMBER * 2 && i < FILE_NUMBER * 3)
			lable = 3;
		else if (i >= FILE_NUMBER * 3 && i < FILE_NUMBER * 4)
			lable = 4;
		else if (i >= FILE_NUMBER * 4 && i < FILE_NUMBER * 5)
			lable = 5;

		Mat trainImg = imread(*imgIter);
		hog->compute(trainImg, descriptors, winStride, Size(0, 0));

		cout << *imgIter << endl;
		cout << "HOG dims: " << descriptors.size() << endl;
		//保存HOG特征向量到trainData中
		//trainData(行, 列) = (数据样本, 特征变量)
		j = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			cvmSet(trainData, i, j, *iter);
			j++;
		}
		//保存trainLable
		cvmSet(trainLable, i, 0, lable);

		i++;
	}

	cout << endl << "saving data......" << endl;

	cvSave("D:\\test\\sign\\HOGData\\trainData.xml", trainData);
	cvSave("D:\\test\\sign\\HOGData\\trainLable.xml", trainLable);
	cout << "save hog data successfully!" << endl;

	system("pause");
	
	//释放内存
	cvReleaseMat(&trainData);
	cvReleaseMat(&trainLable);
	return 0;
}


void GetImgPath(string filePath)
{
	string imgList = "dataList.txt";
	ifstream fin;
	fin.open(filePath + imgList);
	char name[10];
	for (int i = 1; i <= FILE_NUMBER; i++)
	{
		fin.getline(name, 10);
		string imgName = name;
		string load = filePath + imgName;
		imgPath.push_back(load);
	}
}
