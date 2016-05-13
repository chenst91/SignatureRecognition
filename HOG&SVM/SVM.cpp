#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "core.hpp"
#include "imgproc.hpp"
#include "highgui.hpp"
#include "objdetect.hpp"  //包含HOG
#include "ml.hpp"		  //包含SVM

using namespace std;
using namespace cv;
using namespace ml;

//HOG相关参数，总共为28800维
const Size winSize = Size(96, 48);
const Size winStride = Size(48, 24);
const Size blockSize = Size(32, 16);
const Size blockStride = Size(16, 8);
const Size cellSize = Size(8, 8);
const int nbins = 9;
const int dims = 28800;

//获取测试图片
vector<string> imgPath;
void GetImgPath(string filePath);

int train()
//int main()
{
	Mat trainData, trainLable;
	cout << "loading data......" << endl;
	FileStorage finData("D:\\test\\sign\\HOGData\\trainData.xml", FileStorage::READ);
	FileStorage finLable("D:\\test\\sign\\HOGData\\trainLable.xml", FileStorage::READ);
	finData["trainData"] >> trainData;
	finLable["trainLable"] >> trainLable;

	trainData.convertTo(trainData, CV_32FC1);
	trainLable.convertTo(trainLable, CV_32SC1);

	finData.release();
	finLable.release();
	cout << "load hog data successfully!" << endl;

	cout << "svm training......" << endl;
	//设置迭代次数与终止条件
	CvTermCriteria criteria;
	criteria = cvTermCriteria(CV_TERMCRIT_ITER, 100, FLT_EPSILON); //迭代终止条件为最大迭代次数

	//SVM训练
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC); //分类N >= 2
	svm->setKernel(SVM::LINEAR);

	//svm->setKernel(SVM::RBF); //使用径向基，稳定性较好，一般是最优选择，linear速度最快
	//svm->setGamma(8);		  //设定初始Gamma值
	svm->setTermCriteria(criteria);
	svm->train(trainData, ml::ROW_SAMPLE, trainLable);
	//Ptr<TrainData> trainData = TrainData::create(train_data, ROW_SAMPLE, train_lable);
	//cout << "1" << endl;
	//svm->trainAuto(trainData);	//kFold = 10

	//保存结果
	cout << "saving svm data......" << endl;
	svm->save("D:\\test\\sign\\SVMData\\svm.xml");
	cout << "save svm data successfully!" << endl;

	//float gamma = svm->getGamma();
	//cout << "gamma:" << gamma << endl;

	system("pause");
	return 0;
}



//int main()
int predict()
{
	//创建SVM并且读入训练数据
	cout << "loading svm data......" << endl;
	Ptr<SVM> svm = SVM::create();
	string svmDataPath = "D:\\test\\sign\\SVMData\\svm.xml";
	svm = StatModel::load<SVM>(svmDataPath);
	cout << "load svm data successfully!" << endl;

	//加载图像
	GetImgPath("D:\\test\\sign\\testimg\\a\\");
	GetImgPath("D:\\test\\sign\\testimg\\b\\");
	GetImgPath("D:\\test\\sign\\testimg\\c\\");
	GetImgPath("D:\\test\\sign\\testimg\\d\\");
	GetImgPath("D:\\test\\sign\\testimg\\e\\");

	//载入目标图像
	for (vector<string>::iterator imgIter = imgPath.begin(); imgIter != imgPath.end(); imgIter++)
	{
		cout << *imgIter << ":";

		Mat src = imread(*imgIter, 0);
		//如果加载失败，则跳过
		if (src.empty())
			break;

		//计算目标图像的hog
		CvMat *hogData;
		hogData = cvCreateMat(1, dims, CV_32FC1);
		cvSetZero(hogData);

		HOGDescriptor *hog = new HOGDescriptor(winSize, blockSize, blockStride, cellSize, nbins);
		vector <float> descriptors;//HOG特征向量
		hog->compute(src, descriptors, winStride, Size(0, 0));
		int j = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			cvmSet(hogData, 0, j, *iter);
			j++;
		}
		cvSave("D:\\test\\sign\\HOGData\\testData.xml", hogData);


		Mat testData;
		FileStorage finData("D:\\test\\sign\\HOGData\\testData.xml", FileStorage::READ);
		finData["testData"] >> testData;
		testData.convertTo(testData, CV_32FC1);
		finData.release();

		int response = (int)svm->predict(testData);
		switch (response)
		{
		case 1:
			cout << "张笛" << endl;
			break;
		case 2:
			cout << "谢悦" << endl;
			break;
		case 3:
			cout << "陈晓婧" << endl;
			break;
		case 4:
			cout << "王浩青" << endl;
			break;
		case 5:
			cout << "肖璇" << endl;
			break;
		default:
			break;
		}

	}
	system("pause");
	return 0;
}


void GetImgPath(string filePath)
{
	string imgList = "dataList.txt";
	ifstream fin;
	fin.open(filePath + imgList);
	char name[10];
	while(!fin.eof())
	{
		fin.getline(name, 10);
		string imgName = name;
		string load = filePath + imgName;
		imgPath.push_back(load);
	}
}