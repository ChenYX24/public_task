#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;
int hue = 168;                      //色调
int saturation = 58;                //饱和度
int contrast = 15;                  //对比度
int brightness = 0;                 //亮度
int exposure = 0;                   //曝光
vector<Point3f> point3d;            //三维坐标
vector<Point2f> object2d_point;     //二维坐标
vector<vector<Point2f>> last_point; //上次的二维坐标
float half_x = 4.21875;
float half_y = 13.25;
float x = 23.0;
float y = 12.7/3;//light,not all
int num = 0; //计数器
//初始化识别颜色
int lowH = 0;
int lowS = 80;
int lowV = 158;
int highH = 64;
int highS = 248;
int highV = 255;
//明确点用以框出物体
Point2f points_l; // centre point
void getTarget2dPoints(cv::RotatedRect object_rect, std::vector<Point2f> &object2d_point)
{ //获取像素坐标，顺时针，左上角为0号点
  Point2f vertices[4];
  object_rect.points(vertices);
  Point2f lu, ld, ru, rd;
  sort(begin(vertices), end(vertices), [](const cv::Point2f &p1, const cv::Point2f &p2)
       { return p1.x + p1.y < p2.x + p2.y; });

  object2d_point.clear();
  for (int i = 0; i < 4; i++)
    object2d_point.push_back(vertices[i]);
}
int main()
{
  //卡尔曼初始化

  KalmanFilter KF(4, 2, 0);                                                                    //定义卡尔曼滤波器
  Mat measurement = Mat::zeros(2, 1, CV_32F);                                         //初始测量值x'(0)，因为后面要更新这个值，所以必须先定义
  KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //转移矩阵A
  setIdentity(KF.measurementMatrix);  //测量矩阵H:单位矩阵化
  setIdentity(KF.processNoiseCov, Scalar::all(1));  //系统噪声方差矩阵Q
  setIdentity(KF.measurementNoiseCov, Scalar::all(1e-60));  //测量噪声方差矩阵R
  setIdentity(KF.errorCovPost, Scalar::all(10000)); //后验错误估计协方差矩阵P


  // // 控制向量都没有，控制矩阵Bk（controlMatrix）更没有。
  // // 初始化后验估计的状态向量P(statePost)：
  // randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
  // /* 定义两个噪声：系统噪声Wk和测量噪声Vk */

  // // 定义系统噪声Wk：二维列向量
  // Mat processNoise(2, 1, CV_32F);

  // /* 两个噪声的协方差矩阵： 系统噪声的协方差矩阵Qk（processNoiseCov）和测量噪声的协方差矩阵Rk（measurementNoiseCov）*/

  // // 系统噪声的协方差矩阵Qk（processNoiseCov）
  // setIdentity(KF.processNoiseCov, Scalar::all(1e-5));

  // // 测量噪声的协方差矩阵Rk（measurementNoiseCov）
  // setIdentity(KF.measurementNoiseCov, Scalar::all(1e-5));
  // //初始化系统噪声
  // randn(processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
  // //  errorCovPost 后验错误估计的协方差矩阵P
  // setIdentity(KF.errorCovPost, Scalar::all(1));

  VideoCapture cap("../resources/test.avi");
  //**设置三维坐标**//
  point3d.push_back(Point3f(0, 0, 0));
  point3d.push_back(Point3f(x, 0, 0));
  point3d.push_back(Point3f(x, y, 0));
  point3d.push_back(Point3f(0, y, 0));

  //调整摄像机参数//
  // namedWindow("camera",(640,200));
  // createTrackbar("帧率", "camera", &fps, 30, on_FPS_trackbar);
  // createTrackbar("色调", "camera", &hue, 1000, on_HUE_trackbar);
  // createTrackbar("饱和度", "camera", &saturation, 1000, on_SATURATION_trackbar);
  // createTrackbar("对比度", "camera", &contrast, 1000, on_CONTRAST_trackbar);
  // createTrackbar("亮度", "camera", &brightness, 1000, on_BRIGHTNESS_trackbar);
  // createTrackbar("曝光", "camera", &exposure, 1000, on_EXPOSURE_trackbar);
  // while (1)
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
  int frame_counter = 1; //帧计数器
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
    //**检测**//
    Mat temp;
    // frame_counter += 1;
    // cap.set(CAP_PROP_POS_FRAMES, frame_counter);//下一帧
    ////重放
    // if (frame_counter == count - 1)
    //{
    //	frame_counter = 1;
    //	cap.set(CAP_PROP_POS_FRAMES, 1);
    // }
    cap >> src;
    cap >> temp;

    if (temp.empty())
    {
      break;
    }
    resize(src, src, Size(640, 480), cv::INTER_AREA);
    resize(temp, temp, Size(640, 480), cv::INTER_AREA);
    // cvtColor(temp, temp, COLOR_BGR2GRAY);//灰度化
    // GaussianBlur(temp, temp, Size(5, 5), 0, 0);//高斯滤波
    // threshold(temp, temp, 254, 255, THRESH_BINARY);//二值化
    cvtColor(src, temp, COLOR_BGR2HSV);                                         //转为HSV图
    GaussianBlur(temp, temp, Size(7, 7), 5, 0);                                 //高斯滤波
    inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp); //二值化
    // for (int i = 0; i < 2; i++)
    //{
    //	morphologyEx(temp, temp, 0, element);//腐蚀
    // }
    // for (int i = 0; i < 2; i++)
    //{
    //	morphologyEx(temp, temp, 1, element);//膨胀
    // }

    /*轮廓检测*/
    vector<vector<Point>> contours;
    vector<Vec4i> hehierarchy;
    findContours(temp, contours, hehierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
    //去掉小矩形
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
      rect.points(point.data());  //返回rect中的四个顶点到<Point2f>数组中.
      for (int k = 0; k < 4; k++) //画矩形
      {
        line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 0), 1, 8);
      }
      getTarget2dPoints(rect, point); //返回二维像素坐标
      Point2f temp_p;
      Point2f temp_p2;
      temp_p.x = (point.at(0).x + point.at(1).x) / 2;
      temp_p.y = (point.at(0).y + point.at(1).y) / 2;
      temp_p2.x = (point.at(2).x + point.at(3).x) / 2;
      temp_p2.y = (point.at(2).y + point.at(3).y) / 2;
      points.push_back(temp_p);
      points.push_back(temp_p2);
    }

    /*画点*/

    // for(int i=0;i<points.size();i++)
    //	for(int k=0;k<4;k++)
    //		points_f.push_back(points.at(i).at(k));

    if (points.size() == 4)
    {

      //上一次的点的坐标statePt：（在圆周上）相对Mat画布的点。
      Point lastPt = points_l;

      //顺时针
      swap(points[0], points[2]);
      swap(points[1], points[2]);
      for (int i = 0; i < points.size(); i++)
      {
        circle(src, points.at(i), 3, Scalar(0, 255, 120), -1); //画点
        putText(src, to_string(i), Point(points.at(i).x, points.at(i).y), 1, 1, Scalar(0, 255, 100), 2);
      }

      line(src, points.at(0), points.at(2), Scalar(0, 0, 255), 2);
      line(src, points.at(1), points.at(3), Scalar(0, 0, 255), 2);
      points_l = Point2f((points.at(0).x + points.at(2).x) / 2, (points.at(0).y + points.at(2).y) / 2);
      circle(src, points_l, 3, Scalar(0, 255, 120), -1);

      /*测试数据*/
      Matx33d cam_matrix = {1.6041191539594568e+03, 0, 6.3983687194220943e+02,
                            0, 1.6047833493341714e+03, 5.1222951297937527e+02,
                            0, 0, 1};
      Matx<double, 5, 1> distortion_coeff = {-6.4910709385278609e-01, 8.6914328787426987e-01,
                                             5.1733428362687644e-03, -4.1111054148847371e-03, 0};
      /*测距*/
      Matx31f tvec;
      Matx33f rotM;
      Matx31f rvec;
      Matx33f rotT;
      Rodrigues(rotM, rvec);                                               //将旋转矩阵变换成旋转向量
      solvePnP(point3d, points, cam_matrix, distortion_coeff, rvec, tvec); // PnP
      Rodrigues(rvec, rotM);                                               //将旋转向量变换成旋转矩阵
      Rodrigues(tvec, rotT);                                               //将平移向量变成平移矩阵

      double tx = tvec(0);
      double ty = tvec(1);
      double tz = tvec(2);
      double dis = sqrt(tx * tx + ty * ty + tz * tz); //计算距离
      putText(src, to_string(dis) + "cm " + to_string(tx) + " " + to_string(ty) + " " + to_string(tz), Point(50, 50), 1, 1, Scalar(0, 255, 100), 2);
      cout << "distance:" << dis << "cm " << endl;
      cout << "----------------" << endl;
      cout << "偏移姿态(vector):" << endl
           << rvec << endl;
      cout << "偏移姿态(matrix):" << endl
           << rotM << endl;
      cout << "----------------" << endl;

      //预测

       measurement.at<float>(0) = (float)points_l.x;
			 measurement.at<float>(1) = (float)points_l.y;                                       //update measurement
       KF.correct(measurement);                                                            //update
       Mat prediction = KF.predict();
       Point predict_pt = Point(prediction.at<float>(0), prediction.at<float>(1));         //预测值(x',y')
       circle(src, predict_pt, 15, Scalar(255, 255, 0) );


      // // 预测点的坐标predictPt：（在圆周上）相对Mat画布的点。
      // Point predictPt(KF.statePre.at<float>(0), KF.statePre.at<float>(1));
      // Mat measurement = Mat::zeros(1, 1, CV_32F);
      // measurement += Scalar(theRNG().uniform(10.0, 40.0));
      // Point measPt(measurement.at<float>(0), measurement.at<float>(1));
      // // 融合更新
      // KF.correct(measurement);
      // Point optimalPt(KF.statePost.at<float>(0), KF.statePost.at<float>(1));
      

    }

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