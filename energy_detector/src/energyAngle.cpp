#pragma once
#include "../include/energy_detector.h"

energyAngle::energyAngle(Mat src,Point2f two_centre[])
{
  double angle = abs(atan(double(two_centre[1].y - two_centre[0].y) / double(two_centre[1].x - two_centre[0].x)));
  if(two_centre[1].y - two_centre[0].y>0)
    angle*=-1;
  angle = angle * 180 / PI;
  putText(src, "angle:" + to_string(angle), Point(50, 50), 1, 1, Scalar(0, 255, 100), 2);
  A=angle;
}



void energyAngle::preAngle(Mat src ,Point2f two_centre[],Point predict_pt,double angle)
{
  if((predict_pt.x - two_centre[0].x)==0)
    predict_pt.x+=1;
  double p_angle = abs(atan(double(predict_pt.y - two_centre[0].y) / double(predict_pt.x - two_centre[0].x)));
  if(predict_pt.y - two_centre[0].y>0)
    p_angle*=-1;
  p_angle = p_angle * 180 / PI;
  circle(src, getNextPoint(predict_pt,two_centre[0]), 10, Scalar(100, 255, 23),-1 );
  circle(src, getNextPoint(getNextPoint(predict_pt,two_centre[0]),two_centre[0]), 10, Scalar(100, 255, 23),-1 );
  circle(src, getNextPoint(getNextPoint(getNextPoint(predict_pt,two_centre[0]),two_centre[0]),two_centre[0]), 10, Scalar(100, 255, 23),-1 );
  circle(src, getNextPoint(getNextPoint(getNextPoint(getNextPoint(predict_pt,two_centre[0]),two_centre[0]),two_centre[0]),two_centre[0]), 10, Scalar(100, 255, 23),-1 );
  
  cout<<"angle:"<<angle<<endl;
  cout<<"p_angle1:"<<out_angle(predict_pt,two_centre[0])<<endl;
  cout<<"p_angle2:"<<out_angle(getNextPoint(predict_pt,two_centre[0]),two_centre[0])<<endl;
  cout<<"p_angle3:"<<out_angle(getNextPoint(getNextPoint(predict_pt,two_centre[0]),two_centre[0]),two_centre[0])<<endl;
  cout<<"p_angle4:"<<out_angle(getNextPoint(getNextPoint(getNextPoint(predict_pt,two_centre[0]),two_centre[0]),two_centre[0]),two_centre[0])<<endl;
  cout<<"p_angle5:"<<out_angle(getNextPoint(getNextPoint(getNextPoint(getNextPoint(predict_pt,two_centre[0]),two_centre[0]),two_centre[0]),two_centre[0]),two_centre[0])<<endl;
  cout<<"---------------------------------------------------"<<endl;
}



Point energyAngle::getNextPoint(Point p,Point c_p)
{
  Point t_p,n_p,f_p;
  t_p.x=p.x-c_p.x;
  t_p.y=p.y-c_p.y;
  n_p.x=t_p.x*cos((72*PI)/180.0)-t_p.y*sin((72*PI)/180.0);
  n_p.y=t_p.x*sin((72*PI)/180.0)+t_p.y*cos((72*PI)/180.0);
  f_p.x=n_p.x+c_p.x;
  f_p.y=n_p.y+c_p.y;
  return f_p;
}
double energyAngle::out_angle(Point p,Point c_p)
{
  if((p.x - c_p.x)==0)
    p.x+=1;
  double p_angle = abs(atan(double(p.y - c_p.y) / double(p.x - c_p.x)));
  if(p.y - c_p.y>0)
    p_angle*=-1;
	p_angle = p_angle * 180 / PI;
  return p_angle;
}