#pragma once
#include "../include/energy_detector.h"

Distance::Distance()
{
}
void Distance::run(vector<Point2f> point)
{
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
void Distance::initPoint3d()
{
  point3d.push_back(Point3f(0, 0, 0));
  point3d.push_back(Point3f(x, 0, 0));
  point3d.push_back(Point3f(x, y, 0));
  point3d.push_back(Point3f(0, y, 0));
  return;
} 
Distance::~Distance()
{
}