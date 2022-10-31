#pragma once
#include "../include/armor_detector.h"

void match::run(Mat src,vector<Point2f> points,Point2f points_l)
{
  //上一次的点的坐标statePt：（在圆周上）相对Mat画布的点。
  lastPt = points_l;
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
  this->pl=points_l;
}

match::~match()
{
}