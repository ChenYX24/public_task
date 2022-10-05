#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<vector>
#include<algorithm>
using namespace std;
using namespace cv;
int hue = 168;//色调
int saturation = 58;//饱和度
int contrast = 15;//对比度
int brightness = 0;//亮度
int exposure = 0;//曝光
vector<Point3f> point3d;//三维坐标
vector<Point2f> object2d_point;//二维坐标
vector<vector<Point2f>> last_point;//上次的二维坐标
float half_x = 4.21875;
float half_y = 13.25;
int num = 0;//计数器
//初始化识别颜色
int lowH = 0;
int lowS = 80;
int lowV = 158;
int highH = 64;
int highS = 248;
int highV = 255;
//明确点用以框出物体

void getTarget2dPoints(cv::RotatedRect object_rect, std::vector<Point2f>& object2d_point)
{//获取像素坐标，顺时针，左上角为0号点
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
	VideoCapture cap("D:\\Git\\public_task\\Armor_plate\\resources\\test.mp4");
	//**设置三维坐标**//
	point3d.push_back(Point3f(-half_x, half_y, 0));
	point3d.push_back(Point3f(half_x, half_y, 0));
	point3d.push_back(Point3f(half_x, -half_y, 0));
	point3d.push_back(Point3f(-half_x, -half_y, 0));

	//调整摄像机参数//
	//namedWindow("camera",(640,200));
	//createTrackbar("帧率", "camera", &fps, 30, on_FPS_trackbar);
	//createTrackbar("色调", "camera", &hue, 1000, on_HUE_trackbar);
	//createTrackbar("饱和度", "camera", &saturation, 1000, on_SATURATION_trackbar);
	//createTrackbar("对比度", "camera", &contrast, 1000, on_CONTRAST_trackbar);
	//createTrackbar("亮度", "camera", &brightness, 1000, on_BRIGHTNESS_trackbar);
	//createTrackbar("曝光", "camera", &exposure, 1000, on_EXPOSURE_trackbar);
	//while (1)
	//{
	//	cap >> src;
	//	if (src.empty())
	//	{
	//		break;
	//	}
	//	else
	//	{
	//		cap.set(CAP_PROP_HUE, hue);
	//		cap.set(CAP_PROP_SATURATION, saturation);
	//		cap.set(CAP_PROP_BRIGHTNESS, brightness);
	//		cap.set(CAP_PROP_CONTRAST, contrast);
	//		cap.set(CAP_PROP_EXPOSURE, exposure);
	//	}
	//	imshow("src", src);
	//	if (waitKey(1000 / cap.get(CAP_PROP_FPS)) == 27)
	//	{
	//		break;
	//	}
	//}
	/*设置摄像头参数*/
	/*cap.set(CAP_PROP_HUE, hue);
	cap.set(CAP_PROP_SATURATION, saturation);
	cap.set(CAP_PROP_BRIGHTNESS, brightness);
	cap.set(CAP_PROP_CONTRAST, contrast);
	cap.set(CAP_PROP_EXPOSURE, exposure);*/

	//调整识别的颜色//
	namedWindow("control", WINDOW_NORMAL);
	createTrackbar("lowH", "control", &lowH, 180);
	createTrackbar("lowS", "control", &lowS, 255);
	createTrackbar("lowV", "control", &lowV, 255);
	createTrackbar("highH", "control", &highH, 180);
	createTrackbar("highS", "control", &highS, 255);
	createTrackbar("highV", "control", &highV, 255);

	Mat src;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	int frame_counter = 1;//帧计数器
	int count = cap.get(CAP_PROP_FRAME_COUNT);

	while (1)
	{
		vector<Point3f> object;
		double X = 0;
		double Y = 0;
		double Z = 0;
		double DIS = 0;
		int size = 0;
		vector<Point2f> points;
		Matx31f rot;
		//**检测**//
		Mat temp;
		frame_counter += 1;
		cap.set(CAP_PROP_POS_FRAMES, frame_counter);//下一帧
		//重放
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
		//resize(src, src, Size(640, 480), cv::INTER_AREA);
		//resize(temp, temp, Size(640, 480), cv::INTER_AREA);
		//cvtColor(temp, temp, COLOR_BGR2GRAY);//灰度化
		//GaussianBlur(temp, temp, Size(5, 5), 0, 0);//高斯滤波
		//threshold(temp, temp, 254, 255, THRESH_BINARY);//二值化
		cvtColor(temp, temp, COLOR_BGR2HSV);//转为HSV图
		GaussianBlur(temp, temp, Size(7, 7), 0, 0);//高斯滤波
		inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp);//二值化
		//for (int i = 0; i < 2; i++)
		//{
		//	morphologyEx(temp, temp, 0, element);//腐蚀
		//}
		//for (int i = 0; i < 2; i++)
		//{
		//	morphologyEx(temp, temp, 1, element);//膨胀
		//}

		/*轮廓检测*/
		vector<vector<Point>> contours;
		vector<Vec4i> hehierarchy;
		findContours(temp, contours, hehierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
		//去掉小矩形
		int j = 0;
		/*矩形标出轮廓*/
		for (int i = 0; i < contours.size(); i++)
		{
			RotatedRect rect = minAreaRect(contours.at(i));
			//轮廓与矩形之比
			if (contourArea(contours.at(i)) / rect.size.area() < 0.7)
			{
				continue;
			}
			//矩形长宽比
			float h = rect.size.height;
			float w = rect.size.width;
			float radio = (h > w) ? h / w : w / h;
			if (radio > 8 || radio < 2)
			{
				continue;
			}
			if (contourArea(contours.at(i)) < 30)
			{
				continue;
			}

			vector<Point2f> point(4);
			rect.points(point.data());//返回rect中的四个顶点到<Point2f>数组中.
			for (int k = 0; k < 4; k++)//画矩形
			{
				line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 0), 1, 8);
			}
			getTarget2dPoints(rect, object2d_point);//返回二维像素坐标
			getTarget2dPoints(rect, point);//返回二维像素坐标
			Point2f temp_p;
			Point2f temp_p2;
			temp_p.x = (point.at(0).x + point.at(1).x) / 2;
			temp_p.y = (point.at(0).y + point.at(1).y) / 2;
			temp_p2.x = (point.at(2).x + point.at(3).x) / 2;
			temp_p2.y = (point.at(2).y + point.at(3).y) / 2;
			points.push_back(temp_p);
			points.push_back(temp_p2);


			//**测距**//
			/*float Distance = Width / rect.size.width * focal *2.54;
			putText(src, to_string(Distance), point.at(1), 3, 1.2, Scalar(0, 0, 255));*/
			//Mat cam_matrix = (Mat_<double>(3, 3) << 1211.0, -0.9, 342.2, 0, 1211.0, 241.5, 0, 0, 1);//内参矩阵
			//Mat distortion_coeff = (Mat_<double>(4, 1) << -0.0872, 1.0592, -0.0033, 0.0010);//畸变矩阵

			/*测试数据*/
			Matx33d cam_matrix = { 1.2853517927598091e+03,                       0, 3.1944768628958542e+02,
								   0                     , 1.27920339468697937e+03, 2.3929354061292258e+02,
								   0                     ,                       0,                      1 };
			Matx<double, 5, 1> distortion_coeff = { -6.3687295852461456e-01, -1.9748008790347320e+00, 3.0970703651800782e-02, 2.1944646842516919e-03, 0 };
			/*测试数据*/
			Matx31f trans;
			solvePnP(point3d, point, cam_matrix, distortion_coeff, rot, trans);//PnP
			Matx33f rmat;
			Rodrigues(rot, rmat);
			double tx = trans(0);
			double ty = trans(1);
			double tz = trans(2);
			double dis = sqrt(tx * tx + ty * ty + tz * tz);//计算距离

			Point3f a(tx, ty, tz);
			object.push_back(a);

			/*if (j == 0)
			{
				putText(src, to_string(dis) + "cm " + to_string(tx) + " " + to_string(ty) + " " + to_string(tz), Point(50, 100), 1, 1, Scalar(0, 255, 100), 2);
				j++;
			}*/
		}

		/*画点*/
		Point2f points_l;
		//for(int i=0;i<points.size();i++)
		//	for(int k=0;k<4;k++)
		//		points_f.push_back(points.at(i).at(k));

		if (points.size() == 4)
		{
			for (int i = 0; i < points.size(); i++)
			{
				circle(src, points.at(i), 3, Scalar(0, 255, 120), -1);//画点
			}
			line(src, points.at(0), points.at(3), Scalar(0, 0, 255), 2);
			line(src, points.at(1), points.at(2), Scalar(0, 0, 255), 2);
			points_l = Point2f((points.at(0).x + points.at(3).x) / 2, (points.at(0).y + points.at(3).y) / 2);
			circle(src, points_l, 3, Scalar(0, 255, 120), -1);
		}





		//if (points.size() != 1)
		//{
		//	if (points.at(0).at(0).x > points.at(1).at(0).x)
		//		swap(points.at(0), points.at(1));
		//	sort(points.at(0).begin(), points.at(0).end(), [](const cv::Point2f& p1, const cv::Point2f& p2) { return p1.x < p2.x; });
		//	sort(points.at(1).begin(), points.at(1).end(), [](const cv::Point2f& p1, const cv::Point2f& p2) { return p1.x < p2.x; });

		//	circle(src, points.at(0).at(1), 3, Scalar(0, 255, 120), -1);//画点
		//	points_l.at(0) = points.at(0).at(1);
		//	if (pow((points.at(0).at(0).x - points.at(0).at(1).x), 2) + pow((points.at(0).at(0).y - points.at(0).at(1).y), 2) < pow((points.at(0).at(0).x - points.at(0).at(2).x), 2) + pow((points.at(0).at(0).y - points.at(0).at(2).y), 2))
		//	{
		//		circle(src, points.at(0).at(2), 3, Scalar(0, 255, 120), -1);//画点
		//		points_l.at(1) = points.at(0).at(2);
		//	}
		//	else
		//	{
		//		circle(src, points.at(0).at(0), 3, Scalar(0, 255, 120), -1);//画点
		//		points_l.at(1) = points.at(0).at(0);
		//	}
		//	circle(src, points.at(1).at(2), 3, Scalar(0, 255, 120), -1);//画点
		//	points_l.at(3) = points.at(1).at(2);
		//	if (pow((points.at(1).at(2).x - points.at(1).at(3).x), 2) + pow((points.at(1).at(2).y - points.at(1).at(3).y), 2) < pow((points.at(1).at(0).x - points.at(1).at(2).x), 2) + pow((points.at(1).at(0).y - points.at(1).at(2).y), 2))
		//	{
		//		circle(src, points.at(1).at(0), 3, Scalar(0, 255, 120), -1);//画点
		//		points_l.at(2) = points.at(1).at(0);
		//	}
		//	else
		//	{
		//		circle(src, points.at(1).at(3), 3, Scalar(0, 255, 120), -1);//画点
		//		points_l.at(2) = points.at(1).at(3);
		//	}

		//}
		//line(src, points_l.at(0), points_l.at(3), Scalar(0, 0, 255), 2);
		//line(src, points_l.at(1), points_l.at(2), Scalar(0, 0, 255), 2);

		size = object.size();
		for (int i = 0; i < size; i++)
		{
			X += object.at(i).x;
			Y += object.at(i).y;
			Z += object.at(i).z;
		}
		X /= size;
		Y /= size;
		Z /= size;
		DIS = sqrt(X * X + Y * Y + Z * Z);
		putText(src, to_string(DIS) + "cm " + to_string(X) + " " + to_string(Y) + " " + to_string(Z), Point(50, 50), 1, 1, Scalar(0, 255, 100), 2);
		//**预测**//
		//if (object2d_point.empty())
		//{
		//	imshow("src", src);
		//	imshow("test", temp);
		//	if (waitKey(1000 / cap.get(CAP_PROP_FPS)) == 27)
		//	{
		//		break;
		//	}
		//	continue;
		//}
		//if (last_point.size() < 6)
		//{
		//	last_point.push_back(object2d_point);
		//	continue;
		//}
		//if (num == 5)
		//	num = 0;
		////取点
		//Point2f lu(object2d_point.at(0).x - last_point.at(num).at(0).x + object2d_point.at(0).x, (object2d_point.at(0).y - last_point.at(num).at(0).y) + object2d_point.at(0).y);
		//Point2f ru(object2d_point.at(1).x - last_point.at(num).at(1).x + object2d_point.at(1).x, (object2d_point.at(1).y - last_point.at(num).at(1).y) + object2d_point.at(1).y);
		//Point2f rd(object2d_point.at(2).x - last_point.at(num).at(2).x + object2d_point.at(2).x, (object2d_point.at(2).y - last_point.at(num).at(2).y) + object2d_point.at(2).y);
		//Point2f ld(object2d_point.at(3).x - last_point.at(num).at(3).x + object2d_point.at(3).x, (object2d_point.at(3).y - last_point.at(num).at(3).y) + object2d_point.at(3).y);
		////画矩形
		//line(src, lu, ru, Scalar(0, 0, 255), 1, 8);
		//line(src, ru, rd, Scalar(0, 0, 255), 1, 8);
		//line(src, rd, ld, Scalar(0, 0, 255), 1, 8);
		//line(src, ld, lu, Scalar(0, 0, 255), 1, 8);
		//last_point.at(num).assign(object2d_point.begin(), object2d_point.end());
		//object2d_point.clear();
		//num++;

		imshow("src", src);
		imshow("test", temp);
		//esc退出
		if (waitKey(1000 / cap.get(CAP_PROP_FPS)) == 27)
		{
			break;
		}
	}
	return 0;
}