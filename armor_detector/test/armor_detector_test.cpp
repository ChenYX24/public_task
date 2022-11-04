#pragma once
#include <iostream>
#include <gtest/gtest.h>
#include "../include/armor_detector.h"

struct detectTest:public testing::Test
{
  armor_detector a1,a2;
  detect d1,d2;
  void SetUp(){
  a1.readImg("../../resources/a1.jpeg");
  a2.readImg("../../resources/a.jpeg");
  Mat src=a1.getSrc();
  Mat temp=a1.getTemp();
  Mat src2=a2.getSrc();
  Mat temp2=a2.getTemp();
  d1.d_image(src,temp);
  d2.d_image(src2,temp2);
  }
  void TearDown(){}
};
TEST_F(detectTest,colorTest_yellow)
{
  vector<Point2f> T_p=d1.getPoints();
  cout<<T_p.size();
  EXPECT_EQ(T_p.size(),4);
}
TEST_F(detectTest,colorTest_blue)
{
  vector<Point2f> T_p=d2.getPoints();
  cout<<T_p.size();
  EXPECT_EQ(T_p.size(),4);
}