/**************************************************************************
**	File name:	ORB.cpp
**	Created on:	2016.5.22
**  Revised on: 2016.5.27
**	@Author:	MatthewBit
**	Info:		OpenCV3.1�汾��ʵ�ֵ�ORB��RANSACƥ��
**				��ORB��ƥ����,����ʹ��KNN���������ƥ��
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

const int HAMMING_THRESHOLD = 5;	// ����������ֵ
const int FILE_NUMBER = 10;			// ÿ��ȡ10����Ϊ����

// �����ηָ�����������μ������
double CalcArea(Point2f a, Point2f b, Point2f c, Point2f d);

// ���ú��׹�ʽ�������������
double Heron(Point2f a, Point2f b, Point2f c);

// ƽ��
double pow(double x);

// ������������,��������ͼ��,����ƥ�������,ƽ��ƥ�����,ƥ�����
void CalcFeature(const Mat sampleImg, const Mat testImg, int &matchCount, double &lengthAverage, double &area);

// ��ȡͼ��
void GetImgPath(string filePath, string imgList, vector<string> &imgPath, int fileNumber);

// ����ͼ�����������ƥ����
void CalcMatch(const Mat testImg, vector<string> &imgPath, double &averArea, int &count, double &percent, double &showPercent);

// ���ݿ��Ŷ�ȷ��ǩ������
void Predict(const double a, const double b, const double c, const double d, const double e,
	const double showA, const double showB, const double showC, const double showD, const double showE);


int main()
{
	// ����ͼ��
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


	// ����ͼ��
	vector<string> testImgPath;
	string testList = "ORBList.txt";
	GetImgPath("D:\\test\\sign\\testimg\\a\\", testList, testImgPath, 60);
	//GetImgPath("D:\\test\\sign\\testimg\\b\\", testList, testImgPath, 75);
	//GetImgPath("D:\\test\\sign\\testimg\\c\\", testList, testImgPath, 75);
	//GetImgPath("D:\\test\\sign\\testimg\\d\\", testList, testImgPath, 60);
	//GetImgPath("D:\\test\\sign\\testimg\\e\\", testList, testImgPath, 60);

	//����Ŀ��ͼ��
	for (vector<string>::iterator imgIter = testImgPath.begin(); imgIter != testImgPath.end(); imgIter++)
	{
		double areaA = 0, areaB = 0, areaC = 0, areaD = 0, areaE = 0;
		double percentA = 0, percentB = 0, percentC = 0, percentD = 0, percentE = 0;
		double showPercentA = 0, showPercentB = 0, showPercentC = 0, showPercentD = 0, showPercentE = 0;
		int countA = 0, countB = 0, countC = 0, countD = 0, countE = 0;


		cout << *imgIter << ":" << endl;
		Mat src = imread(*imgIter, 0);
		//�������ʧ�ܣ�������
		if (src.empty())
			break;

		Mat testImg = imread(*imgIter, 0);

		cout << "A�飺�ŵ�" << endl;
		CalcMatch(testImg, aImgPath, areaA, countA, percentA, showPercentA);

		cout << "B�飺л��" << endl;
		CalcMatch(testImg, bImgPath, areaB, countB, percentB, showPercentB);

		cout << "C�飺�����" << endl;
		CalcMatch(testImg, cImgPath, areaC, countC, percentC, showPercentC);

		cout << "D�飺�����" << endl;
		CalcMatch(testImg, dImgPath, areaD, countD, percentD, showPercentD);

		cout << "E�飺Ф�" << endl;
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
		//�������ʧ�ܣ�������
		if (sampleImg.empty())
			break;

		int matchCount = 0;
		double lengthAverage = 0;
		double area = 0;

		CalcFeature(testImg, sampleImg, matchCount, lengthAverage, area);

		//cout << "  area:" << area << endl;

		// ��Ϊ��Чƥ�䷶Χ��1W2 ~ 4W
		if (area >= 10000 && area <= 50000)
			count++;

		// ����4W��Ϊ�Ǵ���ƥ��
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
	//cout.unsetf(ios::scientific); //ȡ����ѧ������  
	cout << setprecision(3) << "     ƽ��ƥ�������" << averArea << endl;
	cout << setprecision(3) << "     ��Чƥ�������" << count << endl;
	cout << setprecision(3) << "     ���Ŷȣ�      " << showPercent << "%" << endl;
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

	// ƥ������"��������"Ϊ����
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

	vector<DMatch> matches;

	matcher->match(sampleDescriptors, testDescriptors, matches);


	// ����ƥ������
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


	// ���ӳ�����
	Mat homography;
	homography = findHomography(Mat(points1), Mat(points2), CV_RANSAC, HAMMING_THRESHOLD);

	vector<char> matchesMask(matches.size(), 0);
	Mat points1t;
	perspectiveTransform(Mat(points1), points1t, homography);

	// ƥ�������
	matchCount = 0;
	lengthAverage = 0;
	double lengthSum = 0;

	// ����ƥ���������ֵ��Χ�ڵ�������
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
		cout << "��˸�ǩ��Ϊ�� �ŵ�(" << showA << "%)" << endl;
	else if (b > a && b > c && b > d && b > e)
		cout << "��˸�ǩ��Ϊ�� л��(" << showB << "%)" << endl;
	else if (c > a && c > b && c > d && c > e)
		cout << "��˸�ǩ��Ϊ�� �����(" << showC << "%)" << endl;
	else if (d > a && d > b && d > c && d > e)
		cout << "��˸�ǩ��Ϊ�� ������(" << showD << "%)" << endl;
	else if (e > a && e > b && e > c && e > d)
		cout << "��˸�ǩ��Ϊ�� Ф�(" << showE << "%)" << endl;
}