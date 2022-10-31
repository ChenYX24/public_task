#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;












class match
{
private:
  Point lastPt;
  Point2f pl;
public:
  match(){};
  void run(Mat src,vector<Point2f> points,Point2f points_l);
  Point getPoint(){return this->pl;};
  ~match();
};

class detect
{
private:
  vector<vector<Point>> contours;
  vector<Vec4i> hehierarchy;

  vector<Point2f> points;

  Point2f temp_p;
  Point2f temp_p2;
public:
  detect(){};
  vector<Point2f> getPoints(){return this->points;};
  void clearPoints(){};
  detect(Mat src,Mat temp);
  void getTarget2dPoints(cv::RotatedRect object_rect, std::vector<Point2f> &object2d_point);
  ~detect();
};

class Distance
{
private:
  /*测试数据*/
Matx33d cam_matrix = {1.6041191539594568e+03, 0., 6.3983687194220943e+02, 0.,
       1.6047833493341714e+03, 5.1222951297937527e+02, 0., 0., 1.};
Matx<double, 5, 1> distortion_coeff = {-6.4910709385278609e-01, 8.6914328787426987e-01,
       5.1733428362687644e-03, -4.1111054148847371e-03, 0.};

  float x = 23.0;
  float y = 12.7/3;//light,not all
  vector<Point3f> point3d;            //三维坐标
public:
  Distance();
  void run(Mat src,vector<Point2f> points);
  void initPoint3d();
  ~Distance();
};
class pretreat
{
private:
  KalmanFilter KF;                     //定义卡尔曼滤波器
  Mat measurement = Mat::zeros(2, 1, CV_32F);   //初始测量值x'(0)，因为后面要更新这个值，所以必须先定义
public:
  pretreat();
  void run(Mat src,Point2f points_l);
  void initPretreat();
  void lastPoint();
  ~pretreat();
};





class armor_detector
{
  public:
  armor_detector();
  armor_detector(string path);//load
  VideoCapture getCap();
  void initVideos();
  void run();
  void show();
  
  ~armor_detector();
  private:
  //初始化
  vector<Point2f> object2d_point;     //二维坐标
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


  VideoCapture cap;
  Mat src,temp;
  
  double X = 0;
  double Y = 0;
  double Z = 0;
  double DIS = 0;
  int size = 0;
  vector<Point2f> points;
  
  Point lastPt;
  
};




