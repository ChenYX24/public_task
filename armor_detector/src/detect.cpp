#pragma once
#include "../include/armor_detector.h"


detect::detect(Mat src,Mat temp)
{
  vector<Point2f> points;
  /*轮廓检测*/
    
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
      
      temp_p.x = (point.at(0).x + point.at(1).x) / 2;
      temp_p.y = (point.at(0).y + point.at(1).y) / 2;
      temp_p2.x = (point.at(2).x + point.at(3).x) / 2;
      temp_p2.y = (point.at(2).y + point.at(3).y) / 2;
      points.push_back(temp_p);
      points.push_back(temp_p2);
      this->points=points;
    }
}
void detect::d_image(Mat src,Mat temp)
{
  vector<Point2f> points;
  /*轮廓检测*/
    
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
      
      temp_p.x = (point.at(0).x + point.at(1).x) / 2;
      temp_p.y = (point.at(0).y + point.at(1).y) / 2;
      temp_p2.x = (point.at(2).x + point.at(3).x) / 2;
      temp_p2.y = (point.at(2).y + point.at(3).y) / 2;
      points.push_back(temp_p);
      points.push_back(temp_p2);
      this->points=points;
    }
}
detect::~detect()
{
}

void detect::getTarget2dPoints(cv::RotatedRect object_rect, std::vector<Point2f> &object2d_point)
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
