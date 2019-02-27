#include "MathFunc.h"

double mfnc::deg2rad(double deg) 
{
  return (deg * PI / 180);
}

double mfnc::rad2deg(double rad) 
{
  return (rad * 180 / PI);
}

double mfnc::calcPointsDistance(double x1, double y1, double x2, double y2)
{
    return std::sqrt(std::pow((x2 - x1), 2.0) + std::pow((y2 - y1), 2.0));
}