#pragma once
#include "../include/armor_detector.h"

Distance::Distance()
{
}
void Distance::run(Mat src,vector<Point2f> points)
{
  swap(points[0], points[2]);
  swap(points[1], points[2]);
  /*测距*/
  Matx31f tvec;
  Matx33f rotM;
  Matx31f rvec;
  Matx33f rotT;
  Rodrigues(rotM, rvec);                                               //将旋转矩阵变换成旋转向量
  solvePnP(point3d, points, cam_matrix, distortion_coeff, rvec, tvec); // PnP
  cout<<point3d<<endl<<points<<endl<<cam_matrix<<endl<<distortion_coeff<<endl;
  Rodrigues(rvec, rotM);                                               //将旋转向量变换成旋转矩阵
  Rodrigues(tvec, rotT);                                              //将平移向量变成平移矩阵
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