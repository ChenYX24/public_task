#pragma once
#include "../include/energy_detector.h"


detect::detect(Mat src,Mat temp)
{
  vector<Point2f> points;
  /*轮廓检测*/
    
		findContours(temp, contours, hehierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    //去掉小矩形
    /*矩形标出轮廓*/
    int son=-1;
		int centre=-1;
		//矩形标轮廓
		for (int i = 0; i < contours.size(); i++)
		{
			if (hehierarchy[i][2] == -1 && hehierarchy[i][3] == -1)
			{
				RotatedRect rect = minAreaRect(contours.at(i));
				if (contourArea(contours.at(i)) / rect.size.area() < 0.5)
				{
					continue;
				}
				if (contourArea(contours.at(i)) < 160 || contourArea(contours.at(i)) > 700)
				{
					continue;
				}
				//矩形长宽比
				float h = rect.size.height;
				float w = rect.size.width;
				float radio = (h > w) ? h / w : w / h;
				if (radio < 7 && radio >3)
				{
					continue;
				}
				vector<Point2f> point(4);
				rect.points(point.data());//返回rect中的四个顶点到<Point2f>数组中.
				for (int k = 0; k < 4; k++)//画矩形
				{
					line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 255, 255), 1, 8);
				}
				two_centre[0] = rect.center;
			}
      //目标
			if (hehierarchy[i][2] != -1)
				son = hehierarchy[i][2];
			if(son!=-1)
			{
				if (hehierarchy[son][0] == -1 && hehierarchy[son][1] == -1)
				{
					RotatedRect rect = minAreaRect(contours[son]);
					if (contourArea(contours.at(i)) / rect.size.area() < 0.5)
					{
						continue;
					}
					if (contourArea(contours.at(i)) < 200)
					{
						continue;
					}				
					if (contourArea(contours.at(i)) > 900)
					{
						continue;
					}
					//矩形长宽比
					float h = rect.size.height;
					float w = rect.size.width;
					float radio = (h > w) ? h / w : w / h;
					if (radio < 7 && radio >3)
					{
						continue;
					}
					vector<Point2f> point(4);
					rect.points(point.data());//返回rect中的四个顶点到<Point2f>数组中.
					for (int k = 0; k < 4; k++)//画矩形
					{
						line(src, point.at(k), point.at((k + 1) % 4), Scalar(0, 0, 255), 1, 8);
					}
					two_centre[1] = rect.center;
          Distance DISTANCE;
          DISTANCE.initPoint3d();
          DISTANCE.run(point);
				}
				
			}

			
	
		}
}

detect::~detect()
{
}


