#include <iostream>
#include "./include/armor_detector.h"
//#include "./src/armor_detector.cpp"
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
  armor_detector ARMOR("../../resources/test.avi");
  ARMOR.run();
  return 0;
}
