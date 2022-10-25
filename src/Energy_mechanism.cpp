#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<vector>
#include<algorithm>
#include <math.h>
using namespace std;
using namespace cv;
//初始化识别颜色
int lowH = 70;
int lowS = 50;
int lowV = 50;
int highH = 120;
int highS = 255;
int highV = 255;
Point2f two_centre[2];
#define PI acos(-1);
bool flag=false;

float x = 23.0;
float y = 12.7;
vector<Point3f> point3d;//三维坐标

void getTarget2dPoints(cv::RotatedRect object_rect)
{//获取像素坐标，顺时针，左上角为0号点
	Point2f vertices[4];
	object_rect.points(vertices);
	Point2f lu, ld, ru, rd;
	sort(begin(vertices), end(vertices), [](const cv::Point2f& p1, const cv::Point2f& p2) { return p1.x + p1.y < p2.x + p2.y; });
}
int main()
{
  //卡尔曼初始化
  KalmanFilter KF(4, 2, 0);                        //定义卡尔曼滤波器
  Mat measurement = Mat::zeros(2, 1, CV_32F);      //初始测量值x'(0)，因为后面要更新这个值，所以必须先定义
  KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //转移矩阵A
  setIdentity(KF.measurementMatrix);  //测量矩阵H:单位矩阵化
  setIdentity(KF.processNoiseCov, Scalar::all(1));  //系统噪声方差矩阵Q
  setIdentity(KF.measurementNoiseCov, Scalar::all(1e-60));  //测量噪声方差矩阵R
  setIdentity(KF.errorCovPost, Scalar::all(10000)); //后验错误估计协方差矩阵P

	VideoCapture cap("../resources/test2.avi");

	//**设置三维坐标**//
	point3d.push_back(Point3f(0, 0, 0));
	point3d.push_back(Point3f(x, 0, 0));
	point3d.push_back(Point3f(x, y, 0));
	point3d.push_back(Point3f(0, y, 0));

	//调整识别的颜色//
	namedWindow("control", WINDOW_NORMAL);
	createTrackbar("lowH", "control", &lowH, 180);
	createTrackbar("lowS", "control", &lowS, 255);
	createTrackbar("lowV", "control", &lowV, 255);
	createTrackbar("highH", "control", &highH, 180);
	createTrackbar("highS", "control", &highS, 255);
	createTrackbar("highV", "control", &highV, 255);
	Mat src;
	Mat temp;
	int frame_counter = 1;//帧计数器
	int count = cap.get(CAP_PROP_FRAME_COUNT);
	
	while (1)
	{
		//**检测**//
		
		//frame_counter += 1;
		//cap.set(CAP_PROP_POS_FRAMES, frame_counter);//下一帧
		////重放
		//if (frame_counter == count - 1)
		//{
		//	frame_counter = 1;
		//	cap.set(CAP_PROP_POS_FRAMES, 1);
		//}
		cap.read(temp);
		cap.read(src);


		if (temp.empty())
		{
			break;
		}
		cvtColor(src, temp, COLOR_BGR2HSV);//转为HSV图
		
		GaussianBlur(temp, temp, Size(7, 7), 3, 0);
		inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp);//二值化
		/*dilate(temp, temp, Mat(),Point(-1,-1),1);*/
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//expand line and just can use odd number
		morphologyEx(temp, temp, MORPH_CLOSE, kernel);


		//找轮廓
		vector<vector<Point>> contours;
		vector<Vec4i> hehierarchy;
		//Canny(temp, temp, 100, 200, 3);
		findContours(temp, contours, hehierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());

		
		int son=-1;
		int centre=-1;
		//矩形标轮廓
		for (int i = 0; i < contours.size(); i++)
		{
			if (hehierarchy[i][2] == -1 && hehierarchy[i][3] == -1)
			{
				//if (centre == -1)
				//	centre = i;
				//if (contourArea(contours.at(centre)) > contourArea(contours.at(i)))
				//	centre = i;
				RotatedRect rect = minAreaRect(contours.at(i));
				if (contourArea(contours.at(i)) / rect.size.area() < 0.5)
				{
					continue;
				}
				if (contourArea(contours.at(i)) < 160 || contourArea(contours.at(i)) > 700)
				{
					continue;
				}
				//矩形长宽比
				float h = rect.size.height;
				float w = rect.size.width;
				float radio = (h > w) ? h / w : w / h;
				if (radio < 7 && radio >3)
				{
					continue;
				}
				vector<Point2f> point(4);
				rect.points(point.data());//返回rect中的四个顶点到<Point2f>数组中.
				for (int k = 0; k < 4; k++)//画矩形
				{
					line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 255), 1, 8);
				}
				two_centre[0] = rect.center;
			}


      //目标
			if (hehierarchy[i][2] != -1)
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
					if (contourArea(contours.at(i)) < 200)
					{
						continue;
					}				
					if (contourArea(contours.at(i)) > 900)
					{
						continue;
					}
					//矩形长宽比
					float h = rect.size.height;
					float w = rect.size.width;
					float radio = (h > w) ? h / w : w / h;
					if (radio < 7 && radio >3)
					{
						continue;
					}
					vector<Point2f> point(4);
					rect.points(point.data());//返回rect中的四个顶点到<Point2f>数组中.
					for (int k = 0; k < 4; k++)//画矩形
					{
						line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 0, 255), 1, 8);
					}
					two_centre[1] = rect.center;

					/*测试数据*/
					Matx33d cam_matrix = { 1.6041191539594568e+03,                       0, 6.3983687194220943e+02,
										                        0,  1.6047833493341714e+03, 5.1222951297937527e+02,
										                        0,                       0,                      1 };
					Matx<double, 5, 1> distortion_coeff = { -6.4910709385278609e-01,  8.6914328787426987e-01,
															 5.1733428362687644e-03, -4.1111054148847371e-03, 0};
					/*测距*/
					Matx31f tvec;
					Matx31f rvec;
					solvePnP(point3d, point, cam_matrix, distortion_coeff, rvec, tvec);//PnP
					double tx = tvec(0);
					double ty = tvec(1);
					double tz = tvec(2);
					double dis = sqrt(tx * tx + ty * ty + tz * tz);//计算距离
					cout << "distance:" << dis << endl;
					cout << "----------------" << endl;
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
			//rect.points(point.data());//返回rect中的四个顶点到<Point2f>数组中.
			//for (int k = 0; k < 4; k++)//画矩形
			//{
			//	line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 255), 1, 8);
			//}
		//}

		double angle = abs(atan((two_centre[1].y - two_centre[0].y) / (two_centre[1].x - two_centre[0].x)));
		angle = angle * 180 / PI;
		putText(src, "angle:" + to_string(angle), Point(50, 50), 1, 1, Scalar(0, 255, 100), 2);

		//预测

    measurement.at<float>(0) = (float)two_centre[1].x;
		measurement.at<float>(1) = (float)two_centre[1].y;               //update measurement
    KF.correct(measurement);                                         //update
    Mat prediction = KF.predict();
    Point predict_pt = Point(prediction.at<float>(0), prediction.at<float>(1));     //预测值(x',y')
    
    
    circle(src, predict_pt, 3, Scalar(255, 255, 0),-1 );




		imshow("test", temp);
		imshow("src", src);

    int k = waitKey(30);
		if (k == 27)
			break;
		else if (k == 32)
		{
			while (waitKey(0) != 32)
				waitKey(0);
		}
    
    

     
	}
	return 0;

}