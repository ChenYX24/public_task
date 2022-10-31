#pragma once
#include "../include/energy_detector.h"

energy_detector::energy_detector()
{
  cout<<"No video!"<<endl;
}
energy_detector::~energy_detector()
{

}
energy_detector::energy_detector(string path)
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
VideoCapture energy_detector::getCap()
{
  return cap;
}
void energy_detector::initVideos()
{
  cap >> src;
  cap >> temp;
  if (temp.empty())
  {
    cout<<"exit!"<<endl;
    exit (0);
  }
  cvtColor(src, temp, COLOR_BGR2HSV);//转为HSV图
      
  GaussianBlur(temp, temp, Size(7, 7), 3, 0);
  inRange(temp, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), temp);//二值化
  /*dilate(temp, temp, Mat(),Point(-1,-1),1);*/
  Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//expand line and just can use odd number
  morphologyEx(temp, temp, MORPH_CLOSE, kernel);
}
void energy_detector::show()
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
void energy_detector::run()
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
    two_centre[0]=DETECT.getPoint1();
    two_centre[1]=DETECT.getPoint2();
    energyAngle ENERGYANGLE(src,two_centre);
    angle=ENERGYANGLE.getA();
    if(two_centre[0]!=Point2f(0,0)&&two_centre[1]!=Point2f(0,0))
      PRETREAT.run(src,two_centre,angle);
    this->show();
  }

}