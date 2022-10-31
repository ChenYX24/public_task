#pragma once
#include "../include/armor_detector.h"

armor_detector::armor_detector()
{
  cout<<"No video!"<<endl;
}
armor_detector::~armor_detector()
{

}
armor_detector::armor_detector(string path)
{
  
  VideoCapture cap_temp(path);
  cap=cap_temp;
 //调整识别的颜色//
 namedWindow("control", WINDOW_NORMAL);
 createTrackbar("lowH", "control", &lowH, 180);
 createTrackbar("lowS", "control", &lowS, 255);
 createTrackbar("lowV", "control", &lowV, 255);
 createTrackbar("highH", "control", &highH, 180);
 createTrackbar("highS", "control", &highS, 255);
 createTrackbar("highV", "control", &highV, 255);
}
VideoCapture armor_detector::getCap()
{
  return cap;
}
void armor_detector::initVideos()
{
  cap >> src;
  cap >> temp;
  if (temp.empty())
  {
    cout<<"exit!"<<endl;
    exit (0);
  }
  resize(src, src, Size(640, 480), cv::INTER_AREA);
  resize(temp, temp, Size(640, 480), cv::INTER_AREA);
  // cvtColor(temp, temp, COLOR_BGR2GRAY);//灰度化
  // GaussianBlur(temp, temp, Size(5, 5), 0, 0);//高斯滤波
  // threshold(temp, temp, 254, 255, THRESH_BINARY);//二值化
  cvtColor(src, temp, COLOR_BGR2HSV);                                         //转为HSV图
  GaussianBlur(temp, temp, Size(7, 7), 5, 0);                                 //高斯滤波
  inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp); //二值化
}
void armor_detector::show()
{
  imshow("test", temp);
  imshow("src", src);
  int k = waitKey(30);
  if (k == 27)
    exit (0);
  else if (k == 32)
  {
    while (waitKey(0) != 32)
      waitKey(0);
  }
}
void armor_detector::run()
{
  pretreat PRETREAT;
  PRETREAT.initPretreat();
  Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
  int frame_counter = 1; //帧计数器
  int count = cap.get(CAP_PROP_FRAME_COUNT);
  Distance DISTANCE;
  DISTANCE.initPoint3d();
  while(1)
  {

    this->initVideos();
    detect DETECT(src,temp);
    points=DETECT.getPoints();
    if (points.size() == 4)
    {
      cout<<points<<endl;
      match MATCH;
      MATCH.run(src,points,points_l);
      points_l=MATCH.getPoint();
      DISTANCE.run(src,points);
      PRETREAT.run(src,points_l);
    }
    this->show();
  }

}