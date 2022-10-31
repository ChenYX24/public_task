#pragma once
#include "../include/energy_detector.h"

pretreat::pretreat()
{}
void pretreat::initPretreat()
{
  //卡尔曼初始化
  KF.init(4, 2, 0);
  KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //转移矩阵A
  setIdentity(KF.measurementMatrix);  //测量矩阵H:单位矩阵化
  setIdentity(KF.processNoiseCov, Scalar::all(1));  //系统噪声方差矩阵Q
  setIdentity(KF.measurementNoiseCov, Scalar::all(1e-60));  //测量噪声方差矩阵R
  setIdentity(KF.errorCovPost, Scalar::all(10000)); //后验错误估计协方差矩阵P
}
void pretreat::run(Mat src,Point2f two_centre[],double angle)
{
  measurement.at<float>(0) = (float)two_centre[1].x;
  measurement.at<float>(1) = (float)two_centre[1].y;               //update measurement
  KF.correct(measurement);                                         //update
  Mat prediction = KF.predict();
  Point predict_pt = Point(prediction.at<float>(0), prediction.at<float>(1));     //预测值(x',y')
  circle(src, predict_pt, 10, Scalar(255, 255, 0) );

  energyAngle preA;
  preA.preAngle(src,two_centre,predict_pt,angle);

}

pretreat::~pretreat()
{
}