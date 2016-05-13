/**********************************************************************
**HOG
**����ÿһ��ͼ���HOG
**����HOG Data��Label���
**********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp> //����HOG

using namespace std;
using namespace cv;

const int FILE_NUMBER = 60; //ÿ��ȡ60����Ϊ������15����Ϊ����
const int GROUP_NUMBER = 5; //һ��5��
vector<string> imgPath;

//HOG��ز������ܹ�Ϊ28800ά
const Size winSize = Size(96, 48);
const Size winStride = Size(48, 24);
const Size blockSize = Size(32, 16);
const Size blockStride = Size(16, 8);
const Size cellSize = Size(8, 8);
const int nbins = 9;
const int dims = 28800;


//��ȡͼ���ַ
void GetImgPath(string filePath);

//int hog()
int main()
{
	//������������
	int lable = 0;
	GetImgPath("D:\\test\\sign\\samples\\a\\");
	GetImgPath("D:\\test\\sign\\samples\\b\\");
	GetImgPath("D:\\test\\sign\\samples\\c\\");
	GetImgPath("D:\\test\\sign\\samples\\d\\");
	GetImgPath("D:\\test\\sign\\samples\\e\\");

	//�Զ����ÿһ���������ݼ���HOG
	HOGDescriptor *hog = new HOGDescriptor(winSize, blockSize, blockStride, cellSize, nbins);
	vector <float> descriptors;

	CvMat *trainData, *trainLable;
	trainData = cvCreateMat(FILE_NUMBER * GROUP_NUMBER, dims, CV_32FC1);
	cvSetZero(trainData);
	//ע��trainLable���������ͺ�trainData��һ��
	//����ʱ����CV_32FC1���棬��ȡʱ����CV_32SC1��ȡ
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
		//����HOG����������trainData��
		//trainData(��, ��) = (��������, ��������)
		j = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			cvmSet(trainData, i, j, *iter);
			j++;
		}
		//����trainLable
		cvmSet(trainLable, i, 0, lable);

		i++;
	}

	cout << endl << "saving data......" << endl;

	cvSave("D:\\test\\sign\\HOGData\\trainData.xml", trainData);
	cvSave("D:\\test\\sign\\HOGData\\trainLable.xml", trainLable);
	cout << "save hog data successfully!" << endl;

	system("pause");
	
	//�ͷ��ڴ�
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
