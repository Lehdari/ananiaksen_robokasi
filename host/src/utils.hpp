//
// Project: RobokasiHost
// File: utils.hpp
//
// Copyright (c) 2021 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef ROBOKASIHOST_UTILS_HPP
#define ROBOKASIHOST_UTILS_HPP

#include <gut_utils/MathTypes.hpp>

#define ROBOKASI_HOST_RES(PATH) (std::string(ROBOKASI_HOST_RES_DIR) + "/" + PATH)

Mat4d dhToTransformation(double theta, double r, double d, double alpha);
Mat4d zRotation(double theta);

#endif //ROBOKASIHOST_UTILS_HPP
