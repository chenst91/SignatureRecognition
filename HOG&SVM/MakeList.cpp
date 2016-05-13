#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int data_list(string adress, char group);
int preprocess(string adress);
int MakeDataList(string adress, char group)
{
	adress = "D:\\test\\sign\\b\\data_list.txt";
	ofstream fout;
	fout.open(adress);

	if (!fout)
	{
		cout << "file open error!" << endl;
		return -1;
	}

	int num = 0;
	int sum = 75;
	//group = 'A';
	group = 'B';

	for (int i = 1; i <= sum; i++)
	{
		if (i < 10)
			fout << group << "0" << i << ".bmp" << endl;
		else
			fout << group << i << ".bmp" << endl;
	}
	fout.close();
	cout << "save data list successfully!" << endl;

	system("pause");
	return 0;
}

