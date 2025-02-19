/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2016-2017, Mitsubishi Electric Research Laboratories (MERL)
 * Copyright (c) 2017-2025, ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the copyright holder(s) nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PCC_DISTORTION_HPP
#define PCC_DISTORTION_HPP

#include <iostream>
#include <fstream>
#include <mutex>

#include "nanoflann/nanoflann.hpp"

using namespace nanoflann;
#include "nanoflann/KDTreeVectorOfVectorsAdaptor.h"

#include "pcc_processing.hpp"

using namespace std;
using namespace pcc_processing;

#define PCC_QUALITY_VERSION "0.14.2"

namespace pcc_quality {

  /**!
   * \brief
   *  Command line parameters
   *
   *  Dong Tian <tian@merl.com>
   */
  class commandPar
  {
  public:
    string file1;
    string file2;

    string normIn;            //! input file name for normals, if it is different from the input file 1

    bool   singlePass;        //! Force to run a single pass algorithm. where the loop is over the original point cloud

    bool   hausdorff;         //! true: output hausdorff metric as well

    bool   c2c_only;          //! skip point-to-plane metric
    bool   bColor;            //! check color distortion as well
    bool   bLidar;            //! report reflectance as well

    float  resolution;        //! intrinsic resolution, imported. for geometric distortion
    int    dropDuplicates;    //! 0(detect) 1(drop) 2(average) subsequent points with same geo coordinates
    int    neighborsProc;     //! 0(undefined), 1(average), 2(weighted average), 3(min), 4(max) neighbors with same geometric distance
    int    mseSpace;          //! 0(RGB), 1(YCbCr), 8(YCoCg-R)
    bool   bAverageNormals;   //! 0(undefined), 1(average normal based on neighbors with same geometric distance)

    int    nbThreads;         //! Number of threads used for parallel processing.

    bool   normalCalcModificationEnable;    //! Enable modification of the normal calculation for D2.

    commandPar();
  };

  /**!
   * \brief
   *  Store the quality metric for point to plane measurements
   */
  class qMetric {

  public:
    // point-2-point ( cloud 2 cloud ), benchmark metric
    double c2c_mse;            //! store symm mse metric
    double c2c_hausdorff;      //! store symm haussdorf
    double c2c_psnr;
    double c2c_hausdorff_psnr; //! store symm haussdorf

    double color_mse[3];       //! color components, root mean square
    double color_psnr[3];      //! psnr
    double color_rgb_hausdorff[3]; //! color components, hausdorff
    double color_rgb_hausdorff_psnr[3]; //! psnr hausdorff

    // point-2-plane ( cloud 2 plane ), proposed metric
    double c2p_mse;            //! store symm mse metric
    double c2p_hausdorff;      //! store symm haussdorf
    double c2p_psnr;
    double c2p_hausdorff_psnr; //! store symm haussdorf

    // point 2 plane ( cloud 2 plane ), proposed metric
    double pPSNR; //! Peak value for PSNR computation. Intrinsic resolution

    // reflectance
    double reflectance_mse;
    double reflectance_psnr;
    double reflectance_hausdorff;
    double reflectance_hausdorff_psnr;

    qMetric();
  };

  /**!
   * \brief
   *
   *  Interface function to compute quality metrics
   *
   *  Dong Tian <tian@merl.com>
   */
  void computeQualityMetric( PccPointCloud &cloudA,
                             PccPointCloud &cloudNormalsA,
                             PccPointCloud &cloudB,
                             commandPar &cPar,
                             qMetric &qual_metric, 
                             const bool verbose = true,
                             const double similarPointThreshold = 1e-8,
                             std::vector<qMetric>* qual_metric_perPointsA = nullptr,
                             std::vector<qMetric>* qual_metric_perPointsB = nullptr);

};

#endif
