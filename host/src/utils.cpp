//
// Project: RobokasiHost
// File: utils.cpp
//
// Copyright (c) 2021 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "utils.hpp"


Mat4d dhToTransformation(double theta, double r, double d, double alpha)
{
    Mat4d t;
    t <<    cos(theta), -sin(theta)*cos(alpha), sin(theta)*sin(alpha),  r*cos(theta),
            sin(theta), cos(theta)*cos(alpha),  -cos(theta)*sin(alpha), r*sin(theta),
            0.0,        sin(alpha),             cos(alpha),             d,
            0.0,        0.0,                    0.0,                    1.0;
    return t;
}

Mat4d zRotation(double theta)
{
    Mat4d rz;
    rz <<   cos(theta), -sin(theta),    0.0,    0.0,
            sin(theta), cos(theta),     0.0,    0.0,
            0.0,        0.0,            1.0,    0.0,
            0.0,        0.0,            0.0,    1.0;
    return rz;
}
