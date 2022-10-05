#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<vector>
#include<algorithm>
using namespace std;
using namespace cv;
//��ʼ��ʶ����ɫ
int lowH = 93;
int lowS = 44;
int lowV = 46;
int highH = 124;
int highS = 255;
int highV = 255;

void getTarget2dPoints(cv::RotatedRect object_rect, std::vector<Point2f>& object2d_point)
{//��ȡ�������꣬˳ʱ�룬���Ͻ�Ϊ0�ŵ�
	Point2f vertices[4];
	object_rect.points(vertices);
	Point2f lu, ld, ru, rd;
	sort(begin(vertices), end(vertices), [](const cv::Point2f& p1, const cv::Point2f& p2) { return p1.x + p1.y < p2.x + p2.y; });

	object2d_point.clear();
	for (int i = 0; i < 4; i++)
		object2d_point.push_back(vertices[i]);

}
int main()
{
	VideoCapture cap("../resources/test.mp4");
	//����ʶ�����ɫ//
	namedWindow("control", WINDOW_NORMAL);
	createTrackbar("lowH", "control", &lowH, 180);
	createTrackbar("lowS", "control", &lowS, 255);
	createTrackbar("lowV", "control", &lowV, 255);
	createTrackbar("highH", "control", &highH, 180);
	createTrackbar("highS", "control", &highS, 255);
	createTrackbar("highV", "control", &highV, 255);
	Mat src;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	int frame_counter = 1;//֡������
	int count = cap.get(CAP_PROP_FRAME_COUNT);
	while (1)
	{
		//**���**//
		Mat temp;
		frame_counter += 1;
		cap.set(CAP_PROP_POS_FRAMES, frame_counter);//��һ֡
		//�ط�
		if (frame_counter == count - 1)
		{
			frame_counter = 1;
			cap.set(CAP_PROP_POS_FRAMES, 1);
		}
		cap >> src;
		cap >> temp;

		if (temp.empty())
		{
			break;
		}
		cvtColor(temp, temp, COLOR_BGR2HSV);//תΪHSVͼ
		GaussianBlur(temp, temp, Size(7, 7), 0, 0);//��˹�˲�
		inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp);//��ֵ��
		//Mat element = getStructuringElement(MORPH_RECT, Size(2 * 3 + 1, 2 * 3 + 1), Point(3, 3));
		//morphologyEx(temp, temp, MORPH_OPEN, element);

		//������
		vector<vector<Point>> contours;
		vector<Vec4i> hehierarchy;
		findContours(temp, contours, hehierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point());

		
		int son=-1;
		int centre=-1;
		//���α�����
		for (int i = 0; i < contours.size(); i++)
		{
			
			if(hehierarchy[i][2]!=-1)
				son = hehierarchy[i][2];
			if(son!=-1)
			{
				if (hehierarchy[son][0] == -1 && hehierarchy[son][1] == -1)
				{
					RotatedRect rect = minAreaRect(contours[son]);
					if (contourArea(contours.at(i)) / rect.size.area() < 0.5)
					{
						continue;
					}
					vector<Point2f> point(4);
					rect.points(point.data());//����rect�е��ĸ����㵽<Point2f>������.
					for (int k = 0; k < 4; k++)//������
					{
						line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 0), 1, 8);
					}
				}
			}
			if (hehierarchy[i][2] == -1 && hehierarchy[i][3] == -1)
			{
				//if (centre == -1)
				//	centre = i;
				//if (contourArea(contours.at(centre)) > contourArea(contours.at(i)))
				//	centre = i;
				RotatedRect rect = minAreaRect(contours.at(i));
				if (contourArea(contours.at(i)) < 60)
				{
					continue;
				}
				
				if (contourArea(contours.at(i)) / rect.size.area() < 0.8)
				{
					continue;
				}
				vector<Point2f> point(4);
				rect.points(point.data());//����rect�е��ĸ����㵽<Point2f>������.
				for (int k = 0; k < 4; k++)//������
				{
					line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 255), 1, 8);
				}

			}
			
	
		}
		//if (centre != -1)
		//{
			//RotatedRect rect = minAreaRect(contours.at(centre));
			//if (contourArea(contours.at(centre)) < 100)
			//{
			//	continue;
			//}
			//vector<Point2f> point(4);
			//rect.points(point.data());//����rect�е��ĸ����㵽<Point2f>������.
			//for (int k = 0; k < 4; k++)//������
			//{
			//	line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 255), 1, 8);
			//}
		//}








		imshow("src", src);
		imshow("test", temp);
		//esc�˳�
		if (waitKey(1000 / cap.get(CAP_PROP_FPS)) == 27)
		{
			break;
		}
	}
	return 0;

}