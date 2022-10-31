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

#define PI acos(-1)


class energyAngle
{
private:
  double A;
public:
  energyAngle(){};
  energyAngle(Mat src,Point2f two_centre[]);
  void preAngle(Mat src ,Point2f two_centre[],Point predict_pt,double angle);
  double getA(){return this->A;};
  Point getNextPoint(Point p,Point c_p);
  double out_angle(Point p,Point c_p);
  ~energyAngle(){};
};

class detect
{
private:
  vector<vector<Point>> contours;
  vector<Vec4i> hehierarchy;

  Point2f two_centre[2];


public:
  detect(){};
  Point2f getPoint1(){return this->two_centre[0];};
  Point2f getPoint2(){return this->two_centre[1];};
  void clearPoints(){};
  detect(Mat src,Mat temp);
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
  float y = 12.7;//light,not all
  vector<Point3f> point3d;            //三维坐标
public:
  Distance();
  void run(vector<Point2f> point);
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
  void run(Mat src,Point2f two_centre[],double angle);
  void initPretreat();
  void lastPoint();
  ~pretreat();
};


class energy_detector
{
  public:
  energy_detector();
  energy_detector(string path);//load
  VideoCapture getCap();
  void initVideos();
  void run();
  void show();
  
  ~energy_detector();
  private:
  //初始化识别颜色
  int lowH = 70;
  int lowS = 50;
  int lowV = 50;
  int highH = 120;
  int highS = 255;
  int highV = 255;
  //明确点用以框出物体
  Point2f two_centre[2];



  VideoCapture cap;
  Mat src,temp;
  
  vector<Point2f> points;
  
  Point lastPt;
  
  double angle;
};