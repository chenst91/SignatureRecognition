/***************************************************************************************
** ǩ������Ԥ����
** 1.��ֵ��
** 2.��һ��
***************************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

const int THRESHOLD_VALUE = 220;
const int FILE_NUMBER = 75;

//�����е�ͼ���ַ����������
vector<string> imgPath;
vector<string> dstPath;

//��ȡͼ���ַ
void GetImgPath()
{
	string filePath = "D:\\test\\threshold\\e\\";
	string savePath = "D:\\test\\dst\\e\\";
	string imgList = "dataList.txt";
	ifstream fin;
	fin.open(filePath + imgList);
	char name[10];
	for (int i = 1; i <= FILE_NUMBER; i++)
	{
		fin.getline(name, 10);
		string imgName = name;
		string load = filePath + imgName;
		string save = savePath + imgName;
		imgPath.push_back(load);
		dstPath.push_back(save);
	}
}

//��ֵ������
Mat Threshold(Mat src)
{
	Mat dst = Mat(src.cols, src.rows, CV_8UC1);
	threshold(src, dst, THRESHOLD_VALUE, 255, CV_THRESH_BINARY);
	return dst;
}

//ͼ���һ��������ǩ����һ��ͼ������λ��
Mat Normalize(Mat src)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	bool flag_x1 = false, flag_x2 = false, flag_y1 = false, flag_y2 = false;

	//x1����������Ѱ��
	for (int i = 0; i < src.cols - 1; i++)
	{
		//����ҵ��˺�ɫ���أ��˳�ѭ��
		if (flag_x1 == true)
			break;
		else
			for (int j = 0; j < src.rows - 1; j++)
			{
				//��������˺�ɫ����
				if (src.at<uchar>(j, i) == 0)
				{
					x1 = i;
					flag_x1 = true;
					break;
				}
			}
	}

	//x2����������Ѱ��
	for (int i = src.cols - 1; i >= 0; i--)
	{
		//����ҵ��˺�ɫ���أ��˳�ѭ��
		if (flag_x2 == true)
			break;
		else
			for (int j = 0; j < src.rows - 1; j++)
			{
				//��������˺�ɫ����
				if (src.at<uchar>(j, i) == 0)
				{
					x2 = i;
					flag_x2 = true;
					break;
				}
			}
	}

	//y1����������Ѱ��
	for (int j = 0; j < src.rows - 1; j++)
	{
		//����ҵ��˺�ɫ���أ��˳�ѭ��
		if (flag_y1 == true)
			break;
		else
			for (int i = 0; i < src.cols - 1; i++)
			{
				//��������˺�ɫ����
				if (src.at<uchar>(j, i) == 0)
				{
					y1 = j;
					flag_y1 = true;
					break;
				}
			}
	}

	//y2����������Ѱ��
	for (int j = src.rows - 1; j >= 0; j--)
	{
		//����ҵ��˺�ɫ���أ��˳�ѭ��
		if (flag_y2 == true)
			break;
		else
			for (int i = 0; i < src.cols - 1; i++)
			{
				//��������˺�ɫ����
				if (src.at<uchar>(j, i) == 0)
				{
					y2 = j;
					flag_y2 = true;
					break;
				}
			}
	}

	Rect srcRect;
	srcRect.x = x1;
	srcRect.y = y1;
	srcRect.width = x2 - x1;
	srcRect.height = y2 - y1;

	Rect dstRect;
	dstRect.x = (int)(240 - srcRect.width) / 2;
	dstRect.y = (int)(120 - srcRect.height) / 2;
	dstRect.width = x2 - x1;
	dstRect.height = y2 - y1;

	//��ʼ��dstͼ��
	Mat dst = Mat(src.rows, src.cols, CV_8UC1);
	dst.setTo(Scalar(255, 255, 255));

	//����ROI
	Mat srcRoi = src(srcRect);
	Mat dstRoi = dst(dstRect);
	srcRoi.copyTo(dstRoi);

	return dst;
}

int Preprocess()
{
	GetImgPath();
	for (vector<string>::iterator i = imgPath.begin(), j = dstPath.begin();
	i != imgPath.end(), j != dstPath.end(); i++, j++)
	{
		Mat src = imread(*i, 0);
		Mat thresholdImg = Threshold(src);
		Mat dst = Normalize(thresholdImg);
		cout << *j << endl;
		imwrite(*j, dst);
	}
	return 0;
}

//int main()
//{
//	Preprocess();
//
//	system("pause");
//	return 0;
//}