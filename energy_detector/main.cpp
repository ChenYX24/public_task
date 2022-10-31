#include <iostream>
#include "./include/energy_detector.h"
//#include "./src/armor_detector.cpp"
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
  energy_detector ENERGY("../../resources/test2.avi");
  ENERGY.run();
  return 0;
}