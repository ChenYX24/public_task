#pragma once
#include "../include/armor_detector.h"

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
void pretreat::run(Mat src,Point2f points_l)
{
  //预测
  measurement.at<float>(0) = (float)points_l.x;
  measurement.at<float>(1) = (float)points_l.y;                                       //update measurement
  KF.correct(measurement);                                                            //update
  Mat prediction = KF.predict();
  Point predict_pt = Point(prediction.at<float>(0), prediction.at<float>(1));         //预测值(x',y')
  circle(src, predict_pt, 15, Scalar(255, 255, 0) );
}

pretreat::~pretreat()
{
}