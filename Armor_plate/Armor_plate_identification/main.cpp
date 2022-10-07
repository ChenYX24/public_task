#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<vector>
#include<algorithm>
using namespace std;
using namespace cv;
int hue = 168;//ɫ��
int saturation = 58;//���Ͷ�
int contrast = 15;//�Աȶ�
int brightness = 0;//����
int exposure = 0;//�ع�
vector<Point3f> point3d;//��ά����
vector<Point2f> object2d_point;//��ά����
vector<vector<Point2f>> last_point;//�ϴεĶ�ά����
float half_x = 4.21875;
float half_y = 13.25;
int num = 0;//������
//��ʼ��ʶ����ɫ
int lowH = 0;
int lowS = 80;
int lowV = 158;
int highH = 64;
int highS = 248;
int highV = 255;
//��ȷ�����Կ������

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
	VideoCapture cap("D:\\Git\\public_task\\Armor_plate\\resources\\test.mp4");
	//**������ά����**//
	point3d.push_back(Point3f(-half_x, half_y, 0));
	point3d.push_back(Point3f(half_x, half_y, 0));
	point3d.push_back(Point3f(half_x, -half_y, 0));
	point3d.push_back(Point3f(-half_x, -half_y, 0));

	//�������������//
	//namedWindow("camera",(640,200));
	//createTrackbar("֡��", "camera", &fps, 30, on_FPS_trackbar);
	//createTrackbar("ɫ��", "camera", &hue, 1000, on_HUE_trackbar);
	//createTrackbar("���Ͷ�", "camera", &saturation, 1000, on_SATURATION_trackbar);
	//createTrackbar("�Աȶ�", "camera", &contrast, 1000, on_CONTRAST_trackbar);
	//createTrackbar("����", "camera", &brightness, 1000, on_BRIGHTNESS_trackbar);
	//createTrackbar("�ع�", "camera", &exposure, 1000, on_EXPOSURE_trackbar);
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
	/*��������ͷ����*/
	/*cap.set(CAP_PROP_HUE, hue);
	cap.set(CAP_PROP_SATURATION, saturation);
	cap.set(CAP_PROP_BRIGHTNESS, brightness);
	cap.set(CAP_PROP_CONTRAST, contrast);
	cap.set(CAP_PROP_EXPOSURE, exposure);*/

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
		vector<Point3f> object;
		double X = 0;
		double Y = 0;
		double Z = 0;
		double DIS = 0;
		int size = 0;
		vector<Point2f> points;
		//**���**//
		Mat temp;
		//frame_counter += 1;
		//cap.set(CAP_PROP_POS_FRAMES, frame_counter);//��һ֡
		////�ط�
		//if (frame_counter == count - 1)
		//{
		//	frame_counter = 1;
		//	cap.set(CAP_PROP_POS_FRAMES, 1);
		//}
		cap >> src;
		cap >> temp;

		if (temp.empty())
		{
			break;
		}
		//resize(src, src, Size(640, 480), cv::INTER_AREA);
		//resize(temp, temp, Size(640, 480), cv::INTER_AREA);
		//cvtColor(temp, temp, COLOR_BGR2GRAY);//�ҶȻ�
		//GaussianBlur(temp, temp, Size(5, 5), 0, 0);//��˹�˲�
		//threshold(temp, temp, 254, 255, THRESH_BINARY);//��ֵ��
		cvtColor(src, temp, COLOR_BGR2HSV);//תΪHSVͼ
		GaussianBlur(temp, temp, Size(7, 7), 5, 0);//��˹�˲�
		inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp);//��ֵ��
		//for (int i = 0; i < 2; i++)
		//{
		//	morphologyEx(temp, temp, 0, element);//��ʴ
		//}
		//for (int i = 0; i < 2; i++)
		//{
		//	morphologyEx(temp, temp, 1, element);//����
		//}

		/*�������*/
		vector<vector<Point>> contours;
		vector<Vec4i> hehierarchy;
		findContours(temp, contours, hehierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
		//ȥ��С����
		/*���α������*/
		for (int i = 0; i < contours.size(); i++)
		{
			RotatedRect rect = minAreaRect(contours.at(i));
			//���������֮��
			if (contourArea(contours.at(i)) / rect.size.area() < 0.7)
			{
				continue;
			}
			//���γ����
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
			rect.points(point.data());//����rect�е��ĸ����㵽<Point2f>������.
			for (int k = 0; k < 4; k++)//������
			{
				line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 0), 1, 8);
			}
			getTarget2dPoints(rect, object2d_point);//���ض�ά��������
			getTarget2dPoints(rect, point);//���ض�ά��������
			Point2f temp_p;
			Point2f temp_p2;
			temp_p.x = (point.at(0).x + point.at(1).x) / 2;
			temp_p.y = (point.at(0).y + point.at(1).y) / 2;
			temp_p2.x = (point.at(2).x + point.at(3).x) / 2;
			temp_p2.y = (point.at(2).y + point.at(3).y) / 2;
			points.push_back(temp_p);
			points.push_back(temp_p2);


			//**���**//
			/*float Distance = Width / rect.size.width * focal *2.54;
			putText(src, to_string(Distance), point.at(1), 3, 1.2, Scalar(0, 0, 255));*/
			//Mat cam_matrix = (Mat_<double>(3, 3) << 1211.0, -0.9, 342.2, 0, 1211.0, 241.5, 0, 0, 1);//�ڲξ���
			//Mat distortion_coeff = (Mat_<double>(4, 1) << -0.0872, 1.0592, -0.0033, 0.0010);//�������



		}

		/*����*/
		Point2f points_l;
		//for(int i=0;i<points.size();i++)
		//	for(int k=0;k<4;k++)
		//		points_f.push_back(points.at(i).at(k));

		if (points.size() == 4)
		{

			
			//˳ʱ��
			swap(points[0], points[2]);
			swap(points[1], points[2]);
			for (int i = 0; i < points.size(); i++)
			{
				circle(src, points.at(i), 3, Scalar(0, 255, 120), -1);//����
			}
			
			line(src, points.at(0), points.at(2), Scalar(0, 0, 255), 2);
			line(src, points.at(1), points.at(3), Scalar(0, 0, 255), 2);
			points_l = Point2f((points.at(0).x + points.at(2).x) / 2, (points.at(0).y + points.at(2).y) / 2);
			circle(src, points_l, 3, Scalar(0, 255, 120), -1);
			
			
			/*��������*/
			Matx33d cam_matrix = { 1.6041191539594568e+03,                       0, 6.3983687194220943e+02,
														0,  1.6047833493341714e+03, 5.1222951297937527e+02,
														0,                       0,                      1 };
			Matx<double, 5, 1> distortion_coeff = { -6.4910709385278609e-01,  8.6914328787426987e-01,
													 5.1733428362687644e-03, -4.1111054148847371e-03, 0 };
			/*���*/
			Matx31f tvec;
			Matx33f rotM;
			Matx31f rvec;
			Matx33f rotT;
			Rodrigues(rotM, rvec);//����ת����任����ת����
			solvePnP(point3d, points, cam_matrix, distortion_coeff, rvec, tvec);//PnP
			Rodrigues(rvec, rotM);//����ת�����任����ת����
			Rodrigues(tvec, rotT);//��ƽ���������ƽ�ƾ���
			

			double tx = tvec(0);
			double ty = tvec(1);
			double tz = tvec(2);
			double dis = sqrt(tx * tx + ty * ty + tz * tz);//�������
			putText(src, to_string(dis) + "cm " + to_string(tx) + " " + to_string(ty) + " " + to_string(tz), Point(50, 50), 1, 1, Scalar(0, 255, 100), 2);
			cout << "distance:" << dis << "cm " << endl;
			cout << "----------------" << endl;
			cout << "ƫ����̬(vector):" << endl << rvec << endl;
			cout << "ƫ����̬(matrix):" << endl << rotM << endl;
			cout << "----------------" << endl;
		}
		//if (points.size() != 1)
		//{
		//	if (points.at(0).at(0).x > points.at(1).at(0).x)
		//		swap(points.at(0), points.at(1));
		//	sort(points.at(0).begin(), points.at(0).end(), [](const cv::Point2f& p1, const cv::Point2f& p2) { return p1.x < p2.x; });
		//	sort(points.at(1).begin(), points.at(1).end(), [](const cv::Point2f& p1, const cv::Point2f& p2) { return p1.x < p2.x; });

		//	circle(src, points.at(0).at(1), 3, Scalar(0, 255, 120), -1);//����
		//	points_l.at(0) = points.at(0).at(1);
		//	if (pow((points.at(0).at(0).x - points.at(0).at(1).x), 2) + pow((points.at(0).at(0).y - points.at(0).at(1).y), 2) < pow((points.at(0).at(0).x - points.at(0).at(2).x), 2) + pow((points.at(0).at(0).y - points.at(0).at(2).y), 2))
		//	{
		//		circle(src, points.at(0).at(2), 3, Scalar(0, 255, 120), -1);//����
		//		points_l.at(1) = points.at(0).at(2);
		//	}
		//	else
		//	{
		//		circle(src, points.at(0).at(0), 3, Scalar(0, 255, 120), -1);//����
		//		points_l.at(1) = points.at(0).at(0);
		//	}
		//	circle(src, points.at(1).at(2), 3, Scalar(0, 255, 120), -1);//����
		//	points_l.at(3) = points.at(1).at(2);
		//	if (pow((points.at(1).at(2).x - points.at(1).at(3).x), 2) + pow((points.at(1).at(2).y - points.at(1).at(3).y), 2) < pow((points.at(1).at(0).x - points.at(1).at(2).x), 2) + pow((points.at(1).at(0).y - points.at(1).at(2).y), 2))
		//	{
		//		circle(src, points.at(1).at(0), 3, Scalar(0, 255, 120), -1);//����
		//		points_l.at(2) = points.at(1).at(0);
		//	}
		//	else
		//	{
		//		circle(src, points.at(1).at(3), 3, Scalar(0, 255, 120), -1);//����
		//		points_l.at(2) = points.at(1).at(3);
		//	}

		//}
		//line(src, points_l.at(0), points_l.at(3), Scalar(0, 0, 255), 2);
		//line(src, points_l.at(1), points_l.at(2), Scalar(0, 0, 255), 2);

		//size = object.size();
		//for (int i = 0; i < size; i++)
		//{
		//	X += object.at(i).x;
		//	Y += object.at(i).y;
		//	Z += object.at(i).z;
		//}
		//X /= size;
		//Y /= size;
		//Z /= size;
		//DIS = sqrt(X * X + Y * Y + Z * Z);
		//putText(src, to_string(DIS) + "cm " + to_string(X) + " " + to_string(Y) + " " + to_string(Z), Point(50, 50), 1, 1, Scalar(0, 255, 100), 2);
		
		//**Ԥ��**//
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
		////ȡ��
		//Point2f lu(object2d_point.at(0).x - last_point.at(num).at(0).x + object2d_point.at(0).x, (object2d_point.at(0).y - last_point.at(num).at(0).y) + object2d_point.at(0).y);
		//Point2f ru(object2d_point.at(1).x - last_point.at(num).at(1).x + object2d_point.at(1).x, (object2d_point.at(1).y - last_point.at(num).at(1).y) + object2d_point.at(1).y);
		//Point2f rd(object2d_point.at(2).x - last_point.at(num).at(2).x + object2d_point.at(2).x, (object2d_point.at(2).y - last_point.at(num).at(2).y) + object2d_point.at(2).y);
		//Point2f ld(object2d_point.at(3).x - last_point.at(num).at(3).x + object2d_point.at(3).x, (object2d_point.at(3).y - last_point.at(num).at(3).y) + object2d_point.at(3).y);
		////������
		//line(src, lu, ru, Scalar(0, 0, 255), 1, 8);
		//line(src, ru, rd, Scalar(0, 0, 255), 1, 8);
		//line(src, rd, ld, Scalar(0, 0, 255), 1, 8);
		//line(src, ld, lu, Scalar(0, 0, 255), 1, 8);
		//last_point.at(num).assign(object2d_point.begin(), object2d_point.end());
		//object2d_point.clear();
		//num++;

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