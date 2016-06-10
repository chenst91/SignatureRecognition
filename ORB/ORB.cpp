/**************************************************************************
**	File name:	ORB.cpp
**	Created on:	2016.5.22
**  Revised on: 2016.5.27
**	@Author:	MatthewBit
**	Info:		OpenCV3.1版本中实现的ORB和RANSAC匹配
**				在ORB的匹配中,可以使用KNN进行最近邻匹配
***************************************************************************/

#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace cv;

const int HAMMING_THRESHOLD = 5;	// 汉明距离阈值
const int FILE_NUMBER = 10;			// 每组取10张作为样本

// 将矩形分割成两个三角形计算面积
double CalcArea(Point2f a, Point2f b, Point2f c, Point2f d);

// 利用海伦公式计算三角形面积
double Heron(Point2f a, Point2f b, Point2f c);

// 平方
double pow(double x);

// 计算特征向量,输入两张图像,计算匹配点数量,平均匹配距离,匹配面积
void CalcFeature(const Mat sampleImg, const Mat testImg, int &matchCount, double &lengthAverage, double &area);

// 获取图像集
void GetImgPath(string filePath, string imgList, vector<string> &imgPath, int fileNumber);

// 计算图像和样本集的匹配结果
void CalcMatch(const Mat testImg, vector<string> &imgPath, double &averArea, int &count, double &percent, double &showPercent);

// 根据可信度确定签名分类
void Predict(const double a, const double b, const double c, const double d, const double e,
	const double showA, const double showB, const double showC, const double showD, const double showE);


int main()
{
	// 样本图像
	vector<string> aImgPath;
	vector<string> bImgPath;
	vector<string> cImgPath;
	vector<string> dImgPath;
	vector<string> eImgPath;

	string sampleList = "ORBList.txt";
	GetImgPath("D:\\test\\sign\\samples\\a\\", sampleList, aImgPath, 10);
	GetImgPath("D:\\test\\sign\\samples\\b\\", sampleList, bImgPath, 10);
	GetImgPath("D:\\test\\sign\\samples\\c\\", sampleList, cImgPath, 10);
	GetImgPath("D:\\test\\sign\\samples\\d\\", sampleList, dImgPath, 10);
	GetImgPath("D:\\test\\sign\\samples\\e\\", sampleList, eImgPath, 10);


	// 待测图像
	vector<string> testImgPath;
	string testList = "ORBList.txt";
	GetImgPath("D:\\test\\sign\\testimg\\a\\", testList, testImgPath, 60);
	//GetImgPath("D:\\test\\sign\\testimg\\b\\", testList, testImgPath, 75);
	//GetImgPath("D:\\test\\sign\\testimg\\c\\", testList, testImgPath, 75);
	//GetImgPath("D:\\test\\sign\\testimg\\d\\", testList, testImgPath, 60);
	//GetImgPath("D:\\test\\sign\\testimg\\e\\", testList, testImgPath, 60);

	//载入目标图像
	for (vector<string>::iterator imgIter = testImgPath.begin(); imgIter != testImgPath.end(); imgIter++)
	{
		double areaA = 0, areaB = 0, areaC = 0, areaD = 0, areaE = 0;
		double percentA = 0, percentB = 0, percentC = 0, percentD = 0, percentE = 0;
		double showPercentA = 0, showPercentB = 0, showPercentC = 0, showPercentD = 0, showPercentE = 0;
		int countA = 0, countB = 0, countC = 0, countD = 0, countE = 0;


		cout << *imgIter << ":" << endl;
		Mat src = imread(*imgIter, 0);
		//如果加载失败，则跳过
		if (src.empty())
			break;

		Mat testImg = imread(*imgIter, 0);

		cout << "A组：张笛" << endl;
		CalcMatch(testImg, aImgPath, areaA, countA, percentA, showPercentA);

		cout << "B组：谢悦" << endl;
		CalcMatch(testImg, bImgPath, areaB, countB, percentB, showPercentB);

		cout << "C组：陈晓婧" << endl;
		CalcMatch(testImg, cImgPath, areaC, countC, percentC, showPercentC);

		cout << "D组：王皓青" << endl;
		CalcMatch(testImg, dImgPath, areaD, countD, percentD, showPercentD);

		cout << "E组：肖璇" << endl;
		CalcMatch(testImg, eImgPath, areaE, countE, percentE, showPercentE);

		Predict(percentA, percentB, percentC, percentD, percentE,
			showPercentA, showPercentB, showPercentC, showPercentD, showPercentE);

		cout << endl;
	}

	system("pause");
	return 0;
}


void CalcMatch(Mat testImg, vector<string> &imgPath, double &averArea, int &count, double &percent, double &showPercent)
{
	int errorCount = 0;
	double areaSum = 0;

	for (vector<string>::iterator imgIter = imgPath.begin(); imgIter != imgPath.end(); imgIter++)
	{
		//std::cout << *imgIter << ":" << std::endl;

		Mat sampleImg = imread(*imgIter, 0);
		//如果加载失败，则跳过
		if (sampleImg.empty())
			break;

		int matchCount = 0;
		double lengthAverage = 0;
		double area = 0;

		CalcFeature(testImg, sampleImg, matchCount, lengthAverage, area);

		//cout << "  area:" << area << endl;

		// 认为有效匹配范围是1W2 ~ 4W
		if (area >= 10000 && area <= 50000)
			count++;

		// 大于4W认为是错误匹配
		if (area > 50000)
			errorCount++;

		if (area <= 50000)
			areaSum += area;
	}

	averArea = areaSum / (FILE_NUMBER - errorCount);
	percent = averArea * 100 / 28800 * (1 + double(count) / double(FILE_NUMBER));
	if (percent >= 100)
		showPercent = 100.000;
	else
		showPercent = percent;

	cout.setf(ios::fixed);
	//cout.unsetf(ios::scientific); //取消科学记数法  
	cout << setprecision(3) << "     平均匹配面积：" << averArea << endl;
	cout << setprecision(3) << "     有效匹配个数：" << count << endl;
	cout << setprecision(3) << "     可信度：      " << showPercent << "%" << endl;
}


void CalcFeature(const Mat sampleImg, const Mat testImg, int &matchCount, double &lengthAverage, double &area)
{
	/***************************************************************************
	**	static Ptr<ORB> cv::ORB::create	( int 	nfeatures = 500,
	**									  float scaleFactor = 1.2f,
	**									  int 	nlevels = 8,
	**									  int 	edgeThreshold = 31,
	**									  int 	firstLevel = 0,
	**									  int 	WTA_K = 2,
	**									  int 	scoreType = ORB::HARRIS_SCORE,
	**									  int 	patchSize = 31,
	**									  int 	fastThreshold = 20 )
	***************************************************************************/

	Ptr<ORB> orb = ORB::create(500, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);
	vector<KeyPoint> samplePoints, testPoints;
	Mat sampleDescriptors, testDescriptors;

	orb->detectAndCompute(sampleImg, Mat(), samplePoints, sampleDescriptors);
	orb->detectAndCompute(testImg, Mat(), testPoints, testDescriptors);

	// 匹配器以"汉明距离"为度量
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

	vector<DMatch> matches;

	matcher->match(sampleDescriptors, testDescriptors, matches);


	// 保存匹配对序号
	vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());
	for (size_t i = 0; i < matches.size(); i++)
	{
		queryIdxs[i] = matches[i].queryIdx;
		trainIdxs[i] = matches[i].trainIdx;
	}

	vector<Point2f> points1;
	KeyPoint::convert(samplePoints, points1, queryIdxs);
	vector<Point2f> points2;
	KeyPoint::convert(testPoints, points2, trainIdxs);


	// 求解映射矩阵
	Mat homography;
	homography = findHomography(Mat(points1), Mat(points2), CV_RANSAC, HAMMING_THRESHOLD);

	vector<char> matchesMask(matches.size(), 0);
	Mat points1t;
	perspectiveTransform(Mat(points1), points1t, homography);

	// 匹配点数量
	matchCount = 0;
	lengthAverage = 0;
	double lengthSum = 0;

	// 保存匹配距离在阈值范围内的特征点
	for (size_t i = 0; i < points1.size(); i++)
	{
		int length = norm(points2[i] - points1t.at<Point2f>((int)i, 0));
		if (length <= HAMMING_THRESHOLD)
		{
			matchesMask[i] = 1;
			matchCount++;
			lengthSum += length;
		}
	}
	lengthAverage = lengthSum / (double)matchCount;


	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(sampleImg.cols, 0);
	obj_corners[2] = cvPoint(sampleImg.cols, sampleImg.rows); obj_corners[3] = cvPoint(0, sampleImg.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, homography);

	Point2f pa = scene_corners[0] + Point2f(static_cast<double>(sampleImg.cols), 0);
	Point2f pb = scene_corners[1] + Point2f(static_cast<double>(sampleImg.cols), 0);
	Point2f pc = scene_corners[2] + Point2f(static_cast<double>(sampleImg.cols), 0);
	Point2f pd = scene_corners[3] + Point2f(static_cast<double>(sampleImg.cols), 0);

	area = CalcArea(pa, pb, pc, pd);
}


double CalcArea(Point2f a, Point2f b, Point2f c, Point2f d)
{
	return Heron(a, b, c) + Heron(a, c, d);
}


double Heron(Point2f a, Point2f b, Point2f c)
{
	double ab = sqrt(pow(a.x - b.x) + pow(a.y - b.y));
	double bc = sqrt(pow(b.x - c.x) + pow(b.y - c.y));
	double ca = sqrt(pow(c.x - a.x) + pow(c.y - a.y));
	double z = (ab + bc + ca) / 2;
	double s = sqrt(z * abs(z - ab) * abs(z - bc) * abs(z - ca));
	return s;
}


double pow(double x)
{
	return x * x;
}


void GetImgPath(string filePath, string imgList, vector<string> &imgPath, int fileNumber)
{
	// string imgList = "ORBList.txt";
	ifstream fin;
	fin.open(filePath + imgList);
	char name[10];
	for (int i = 1; i <= fileNumber; i++)
	{
		fin.getline(name, 10);
		string imgName = name;
		string load = filePath + imgName;
		imgPath.push_back(load);
	}
}


void Predict(const double a, const double b, const double c, const double d, const double e,
	const double showA, const double showB, const double showC, const double showD, const double showE)
{
	if (a > b && a > c && a > d && a > e)
		cout << "因此该签名为： 张笛(" << showA << "%)" << endl;
	else if (b > a && b > c && b > d && b > e)
		cout << "因此该签名为： 谢悦(" << showB << "%)" << endl;
	else if (c > a && c > b && c > d && c > e)
		cout << "因此该签名为： 陈晓婧(" << showC << "%)" << endl;
	else if (d > a && d > b && d > c && d > e)
		cout << "因此该签名为： 王浩青(" << showD << "%)" << endl;
	else if (e > a && e > b && e > c && e > d)
		cout << "因此该签名为： 肖璇(" << showE << "%)" << endl;
}