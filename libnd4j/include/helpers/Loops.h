/* ******************************************************************************
 *
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 *  See the NOTICE file distributed with this work for additional
 *  information regarding copyright ownership.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// @author Yurii Shyrma (iuriish@yahoo.com), created on 14.03.2019
//

#ifndef LIBND4J_LOOPS_H
#define LIBND4J_LOOPS_H
#include <array/DataTypeUtils.h>
#include <execution/Threads.h>
#include <helpers/ConstantTadHelper.h>
#include <helpers/LoopKind.h>
#include <helpers/OmpLaunchHelper.h>
#include <helpers/shape.h>
#include <loops/indexreduce.h>
#include <ops/ops.h>

#include <functional>

namespace sd {

template <typename X, typename Z, typename E>
class SD_LIB_HIDDEN ReductionLoops {
 protected:
 public:
  template <typename OpType>
  static SD_INLINE void loopReduce(sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                                   const sd::LongType* zShapeInfo, const int* dims, E* extraParams);
};

template <typename X, typename Z>
class SD_LIB_HIDDEN ReductionFloatLoops : public ReductionLoops<X, Z, Z> {
 public:
  static void wrapper(int opNum, sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                      const sd::LongType* zShapeInfo, const int* dims, Z* extraParams);

  template <typename OpType>
  static void innerloopReduce(sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                              const sd::LongType* zShapeInfo, const int* dims, Z* extraParams);
};

template <typename X, typename Z>
class SD_LIB_HIDDEN ReductionBoolLoops : public ReductionLoops<X, Z, X> {
 public:
  static void wrapper(int opNum, sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                      const sd::LongType* zShapeInfo, const int* dims, X* extraParams);

  template <typename OpType>
  static void innerloopReduce(sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                              const sd::LongType* zShapeInfo, const int* dims, X* extraParams);
};

template <typename X, typename Z>
class SD_LIB_HIDDEN ReductionLongLoops : public ReductionLoops<X, Z, X> {
 public:
  static void wrapper(int opNum, sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                      const sd::LongType* zShapeInfo, const int* dims, X* extraParams);

  template <typename OpType>
  static void innerloopReduce(sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                              const sd::LongType* zShapeInfo, const int* dims, X* extraParams);
};

template <typename X>
class SD_LIB_HIDDEN ReductionSameLoops : public ReductionLoops<X, X, X> {
 public:
  static void wrapper(int opNum, sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, X* z,
                      const sd::LongType* zShapeInfo, const int* dims, X* extraParams);

  template <typename OpType>
  static void innerloopReduce(sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, X* z,
                              const sd::LongType* zShapeInfo, const int* dims, X* extraParams);
};

template <typename X, typename Z>
class SD_LIB_HIDDEN IndexReductionLoops {
 private:
 public:
  static void wrapIndexReduce(int opNum, const void* x, const sd::LongType* xShapeInfo, void* z,
                              const sd::LongType* zShapeInfo, const sd::LongType* tadShapeInfo,
                              const sd::LongType* tadOffsets, void* extraParams);

  template <typename OpType>
  static void loopIndexReduce(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                              const sd::LongType* tadShapeInfo, const sd::LongType* tadOffsets, X* extraParams);
};

template <typename X, typename Z, typename E>
class SD_LIB_HIDDEN TransformLoops {
 public:
  template <typename OpType>
  static SD_INLINE void loopTransform(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                      E* extraParams, uint64_t threadId, uint64_t numThreads);
};

template <typename X, typename Z>
class SD_LIB_HIDDEN Reduction3Loops {
 public:
  template <typename OpType>
  static SD_INLINE void loopReduce3(const X* x, const sd::LongType* xShapeInfo, const X* y,
                                    const sd::LongType* yShapeInfo, Z* z, const sd::LongType* zShapeInfo, int* dims,
                                    int dimsLen, Z* extraParams, int64_t start, int64_t stop);

  template <typename OpType>
  static SD_INLINE void loopReduce3All(const X* x, const sd::LongType* xShapeInfo, const X* y,
                                       const sd::LongType* yShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                       const sd::LongType* xTadShapeInfo, const sd::LongType* xTadOffsets,
                                       const sd::LongType* yTadShapeInfo, const sd::LongType* yTadOffsets,
                                       Z* extraParams, int64_t start, int64_t stop);

  static void wrapper(int opNum, const X* x, const sd::LongType* xShapeInfo, const X* y, const sd::LongType* yShapeInfo,
                      Z* z, const sd::LongType* zShapeInfo, int* dims, int dimsLen, Z* extraParams, int64_t start,
                      int64_t stop);

  static void wrapperAll(int opNum, const X* x, const sd::LongType* xShapeInfo, const X* y,
                         const sd::LongType* yShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                         const sd::LongType* xTadShapeInfo, const sd::LongType* xTadOffsets,
                         const sd::LongType* yTadShapeInfo, const sd::LongType* yTadOffsets, Z* extraParams,
                         int64_t start, int64_t stop);

  template <typename OpType>
  static void innerloopReduce3(const X* x, const sd::LongType* xShapeInfo, const X* y, const sd::LongType* yShapeInfo,
                               Z* z, const sd::LongType* zShapeInfo, int* dims, int dimsLen, Z* extraParams,
                               int64_t start, int64_t stop);

  template <typename OpType>
  static void innerloopReduce3All(const X* x, const sd::LongType* xShapeInfo, const X* y,
                                  const sd::LongType* yShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                  const sd::LongType* xTadShapeInfo, const sd::LongType* xTadOffsets,
                                  const sd::LongType* yTadShapeInfo, const sd::LongType* yTadOffsets, Z* extraParams,
                                  int64_t start, int64_t stop);
};

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
static void reduceExec21(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                         const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, dims[0]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, dims[0]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, 0);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, dims[1]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, dims[1]);

  auto func = PRAGMA_THREADS_FOR {
    for (auto i0 = start; i0 < stop; ++i0) {
      auto x0 = x + i0 * xStrd0;
      auto z0 = z + i0 * zStrd0;

      auto s = OpType::startingValue(x0);

      if (xStrd1 == 1)
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          s = OpType::update(s, OpType::op(x0[i1], extraParams), extraParams);
      else
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          s = OpType::update(s, OpType::op(x0[i1 * xStrd1], extraParams), extraParams);

      *z0 = OpType::postProcess(s, static_cast<sd::LongType>(xAxis1), extraParams);
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
static void reduceExec31(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                         const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, dims[0]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, dims[0]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, 0);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, dims[1]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, dims[1]);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, dims[2]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, dims[2]);

  const sd::LongType tadLen = static_cast<sd::LongType>(xAxis1 * xAxis2);

  auto func = PRAGMA_THREADS_FOR {
    for (auto i0 = start; i0 < stop; ++i0) {
      auto x0 = x + i0 * xStrd0;
      auto z0 = z + i0 * zStrd0;

      auto s = OpType::startingValue(x0);

      if (xStrd1 == 1)
        for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
          for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
            s = OpType::update(s, OpType::op(x0[i1 + i2 * xStrd2], extraParams), extraParams);
      else if (xStrd2 == 1)
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2], extraParams), extraParams);
      else
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 * xStrd2], extraParams), extraParams);

      *z0 = OpType::postProcess(s, tadLen, extraParams);
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec32(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[1]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[1]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 0 : 1);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[0]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[0]);
  const sd::LongType zStrd1 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 1 : 0);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, dims[2]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, dims[2]);

  auto func = PRAGMA_THREADS_FOR_2D {
    for (auto i0 = start_x; i0 < stop_x; ++i0) {
      for (auto i1 = start_y; i1 < stop_y; ++i1) {
        auto x1 = x + i0 * xStrd0 + i1 * xStrd1;
        auto z1 = z + i0 * zStrd0 + i1 * zStrd1;

        auto s = OpType::startingValue(x1);

        if (xStrd2 == 1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            s = OpType::update(s, OpType::op(x1[i2], extraParams), extraParams);
        else
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            s = OpType::update(s, OpType::op(x1[i2 * xStrd2], extraParams), extraParams);

        *z1 = OpType::postProcess(s, static_cast<sd::LongType>(xAxis2), extraParams);
      }
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0, 1, 0, xAxis1, 1);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec41(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, dims[0]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, dims[0]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, 0);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, dims[1]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, dims[1]);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, dims[2]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, dims[2]);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, dims[3]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, dims[3]);

  const sd::LongType tadLen = static_cast<sd::LongType>(xAxis1 * xAxis2 * xAxis3);

  auto func = PRAGMA_THREADS_FOR {
    for (auto i0 = start; i0 < stop; ++i0) {
      auto x0 = x + i0 * xStrd0;
      auto z0 = z + i0 * zStrd0;

      auto s = OpType::startingValue(x0);

      if (xStrd1 == 1)
        for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
              s = OpType::update(s, OpType::op(x0[i1 + i2 * xStrd2 + i3 * xStrd3], extraParams), extraParams);
      else if (xStrd2 == 1)
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
            for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
              s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 + i3 * xStrd3], extraParams), extraParams);
      else if (xStrd3 == 1)
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 * xStrd2 + i3], extraParams), extraParams);
      else
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 * xStrd2 + i3 * xStrd3], extraParams), extraParams);

      *z0 = OpType::postProcess(s, tadLen, extraParams);
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec42(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[1]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[1]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 0 : 1);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[0]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[0]);
  const sd::LongType zStrd1 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 1 : 0);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, dims[2]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, dims[2]);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, dims[3]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, dims[3]);

  const sd::LongType tadLen = static_cast<sd::LongType>(xAxis2 * xAxis3);

  auto func = PRAGMA_THREADS_FOR_2D {
    for (auto i0 = start_x; i0 < stop_x; ++i0) {
      for (auto i1 = start_y; i1 < stop_y; ++i1) {
        auto x1 = x + i0 * xStrd0 + i1 * xStrd1;
        auto z1 = z + i0 * zStrd0 + i1 * zStrd1;

        auto s = OpType::startingValue(x1);

        if (xStrd2 == 1)
          for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
            for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
              s = OpType::update(s, OpType::op(x1[i2 + i3 * xStrd3], extraParams), extraParams);
        else if (xStrd3 == 1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              s = OpType::update(s, OpType::op(x1[i2 * xStrd2 + i3], extraParams), extraParams);
        else
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              s = OpType::update(s, OpType::op(x1[i2 * xStrd2 + i3 * xStrd3], extraParams), extraParams);

        *z1 = OpType::postProcess(s, tadLen, extraParams);
      }
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0, 1, 0, xAxis1, 1);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec43(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[2]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[2]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 0 : 2);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, dims[1]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, dims[1]);
  const sd::LongType zStrd1 = shape::strideAt(zShapeInfo, 1);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[2] : dims[0]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[2] : dims[0]);
  const sd::LongType zStrd2 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 2 : 0);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, dims[3]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, dims[3]);

  auto func = PRAGMA_THREADS_FOR_3D {
    for (auto i0 = start_x; i0 < stop_x; ++i0) {
      for (auto i1 = start_y; i1 < stop_y; ++i1) {
        for (auto i2 = start_z; i2 < stop_z; ++i2) {
          auto x2 = x + i0 * xStrd0 + i1 * xStrd1 + i2 * xStrd2;
          auto z2 = z + i0 * zStrd0 + i1 * zStrd1 + i2 * zStrd2;

          auto s = OpType::startingValue(x2);

          if (xStrd3 == 1)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              s = OpType::update(s, OpType::op(x2[i3], extraParams), extraParams);
          else
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              s = OpType::update(s, OpType::op(x2[i3 * xStrd3], extraParams), extraParams);

          *z2 = OpType::postProcess(s, static_cast<sd::LongType>(xAxis3), extraParams);
        }
      }
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0, 1, 0, xAxis1, 1, 0, xAxis2, 1);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec51(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, dims[0]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, dims[0]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, 0);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, dims[1]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, dims[1]);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, dims[2]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, dims[2]);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, dims[3]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, dims[3]);

  const sd::Unsigned xAxis4 = shape::sizeAt(xShapeInfo, dims[4]);
  const sd::LongType xStrd4 = shape::strideAt(xShapeInfo, dims[4]);

  const sd::LongType tadLen = static_cast<sd::LongType>(xAxis1 * xAxis2 * xAxis3 * xAxis4);

  auto func = PRAGMA_THREADS_FOR {
    for (auto i0 = start; i0 < stop; ++i0) {
      auto x0 = x + i0 * xStrd0;
      auto z0 = z + i0 * zStrd0;

      auto s = OpType::startingValue(x0);

      if (xStrd1 == 1)
        for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
          for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
            for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
              for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
                s = OpType::update(s, OpType::op(x0[i1 + i2 * xStrd2 + i3 * xStrd3 + i4 * xStrd4], extraParams),
                                   extraParams);
      else if (xStrd2 == 1)
        for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
          for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
            for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
              for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
                s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 + i3 * xStrd3 + i4 * xStrd4], extraParams),
                                   extraParams);
      else if (xStrd3 == 1)
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
              for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
                s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 * xStrd2 + i3 + i4 * xStrd4], extraParams),
                                   extraParams);
      else if (xStrd4 == 1)
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x0[i1 * xStrd1 + i2 * xStrd2 + i3 * xStrd3 + i4], extraParams),
                                   extraParams);
      else
        for (sd::Unsigned i1 = 0; i1 < xAxis1; ++i1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(
                    s, OpType::op(x0[i1 * xStrd1 + i2 * xStrd2 + i3 * xStrd3 + i4 * xStrd4], extraParams), extraParams);

      *z0 = OpType::postProcess(s, tadLen, extraParams);
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec52(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[1]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[1]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 0 : 1);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[0]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[0]);
  const sd::LongType zStrd1 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 1 : 0);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, dims[2]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, dims[2]);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, dims[3]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, dims[3]);

  const sd::Unsigned xAxis4 = shape::sizeAt(xShapeInfo, dims[4]);
  const sd::LongType xStrd4 = shape::strideAt(xShapeInfo, dims[4]);

  const sd::LongType tadLen = static_cast<sd::LongType>(xAxis2 * xAxis3 * xAxis4);

  auto func = PRAGMA_THREADS_FOR_2D {
    for (auto i0 = start_x; i0 < stop_x; ++i0) {
      for (auto i1 = start_y; i1 < stop_y; ++i1) {
        auto x1 = x + i0 * xStrd0 + i1 * xStrd1;
        auto z1 = z + i0 * zStrd0 + i1 * zStrd1;

        auto s = OpType::startingValue(x1);

        if (xStrd2 == 1)
          for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
                s = OpType::update(s, OpType::op(x1[i2 + i3 * xStrd3 + i4 * xStrd4], extraParams), extraParams);
        else if (xStrd3 == 1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
              for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
                s = OpType::update(s, OpType::op(x1[i2 * xStrd2 + i3 + i4 * xStrd4], extraParams), extraParams);
        else if (xStrd4 == 1)
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x1[i2 * xStrd2 + i3 * xStrd3 + i4], extraParams), extraParams);
        else
          for (sd::Unsigned i2 = 0; i2 < xAxis2; ++i2)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x1[i2 * xStrd2 + i3 * xStrd3 + i4 * xStrd4], extraParams),
                                   extraParams);

        *z1 = OpType::postProcess(s, tadLen, extraParams);
      }
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0, 1, 0, xAxis1, 1);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec53(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[2]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[2]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 0 : 2);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, dims[1]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, dims[1]);
  const sd::LongType zStrd1 = shape::strideAt(zShapeInfo, 1);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[2] : dims[0]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[2] : dims[0]);
  const sd::LongType zStrd2 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 2 : 0);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, dims[3]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, dims[3]);

  const sd::Unsigned xAxis4 = shape::sizeAt(xShapeInfo, dims[4]);
  const sd::LongType xStrd4 = shape::strideAt(xShapeInfo, dims[4]);

  const sd::LongType tadLen = static_cast<sd::LongType>(xAxis3 * xAxis4);

  auto func = PRAGMA_THREADS_FOR_3D {
    for (auto i0 = start_x; i0 < stop_x; ++i0) {
      for (auto i1 = start_y; i1 < stop_y; ++i1) {
        for (auto i2 = start_z; i2 < stop_z; ++i2) {
          auto x2 = x + i0 * xStrd0 + i1 * xStrd1 + i2 * xStrd2;
          auto z2 = z + i0 * zStrd0 + i1 * zStrd1 + i2 * zStrd2;

          auto s = OpType::startingValue(x2);

          if (xStrd3 == 1)
            for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
              for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
                s = OpType::update(s, OpType::op(x2[i3 + i4 * xStrd4], extraParams), extraParams);
          else if (xStrd4 == 1)
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x2[i3 * xStrd3 + i4], extraParams), extraParams);
          else
            for (sd::Unsigned i3 = 0; i3 < xAxis3; ++i3)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x2[i3 * xStrd3 + i4 * xStrd4], extraParams), extraParams);

          *z2 = OpType::postProcess(s, tadLen, extraParams);
        }
      }
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0, 1, 0, xAxis1, 1, 0, xAxis2, 1);
}

//////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceExec54(const X* x, const sd::LongType* xShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                const int* dims, E* extraParams) {
  const sd::Unsigned xAxis0 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[3]);
  const sd::LongType xStrd0 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[0] : dims[3]);
  const sd::LongType zStrd0 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 0 : 3);

  const sd::Unsigned xAxis1 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[2]);
  const sd::LongType xStrd1 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[1] : dims[2]);
  const sd::LongType zStrd1 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 1 : 2);

  const sd::Unsigned xAxis2 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[2] : dims[1]);
  const sd::LongType xStrd2 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[2] : dims[1]);
  const sd::LongType zStrd2 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 2 : 1);

  const sd::Unsigned xAxis3 = shape::sizeAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[3] : dims[0]);
  const sd::LongType xStrd3 = shape::strideAt(xShapeInfo, shape::order(zShapeInfo) == 'c' ? dims[3] : dims[0]);
  const sd::LongType zStrd3 = shape::strideAt(zShapeInfo, shape::order(zShapeInfo) == 'c' ? 3 : 0);

  const sd::Unsigned xAxis4 = shape::sizeAt(xShapeInfo, dims[4]);
  const sd::LongType xStrd4 = shape::strideAt(xShapeInfo, dims[4]);

  auto func = PRAGMA_THREADS_FOR_3D {
    for (auto i0 = start_x; i0 < stop_x; ++i0) {
      for (auto i1 = start_y; i1 < stop_y; ++i1) {
        for (auto i2 = start_z; i2 < stop_z; ++i2) {
          for (auto i3 = 0; i3 < xAxis3; ++i3) {
            auto x3 = x + i0 * xStrd0 + i1 * xStrd1 + i2 * xStrd2 + i3 * xStrd3;
            auto z3 = z + i0 * zStrd0 + i1 * zStrd1 + i2 * zStrd2 + i3 * zStrd3;

            auto s = OpType::startingValue(x3);

            if (xStrd4 == 1)
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x3[i4], extraParams), extraParams);
            else
              for (sd::Unsigned i4 = 0; i4 < xAxis4; ++i4)
                s = OpType::update(s, OpType::op(x3[i4 * xStrd4], extraParams), extraParams);

            *z3 = OpType::postProcess(s, static_cast<sd::LongType>(xAxis4), extraParams);
          }
        }
      }
    }
  };

  samediff::Threads::parallel_for(func, 0, xAxis0, 1, 0, xAxis1, 1, 0, xAxis2, 1);
}

////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E, typename OpType>
SD_LIB_HIDDEN void reduceDefault(sd::memory::Workspace* workspace, const X* x, const sd::LongType* xShapeInfo, Z* z,
                                 const sd::LongType* zShapeInfo, const int* dims, E* extraParams) {
  const int zRank = shape::rank(zShapeInfo);
  const int tadRank = shape::rank(xShapeInfo) - zRank;

  sd::LongType* outerXTadShapeInfo = sd::ShapeBuilders::createSubArrShapeInfo(xShapeInfo, dims, zRank);
  sd::LongType* innerXTadShapeInfo = sd::ShapeBuilders::createSubArrShapeInfo(xShapeInfo, dims + zRank, tadRank);

  const bool sameOffsets1 = shape::haveSameShapeAndStrides(zShapeInfo, outerXTadShapeInfo);
  const bool sameOffsets2 = shape::haveSameShapeAndStrides(zShapeInfo, innerXTadShapeInfo);

  const sd::LongType zLen = shape::length(zShapeInfo);
  const sd::LongType tadLen = shape::length(innerXTadShapeInfo);

  sd::LongType* zOffsets = nullptr;
  ALLOCATE(zOffsets, workspace, zLen, sd::LongType);
  shape::calcOffsets(zShapeInfo, zOffsets);

  sd::LongType* outerXTadOffsets = zOffsets;
  if (!sameOffsets1) {
    ALLOCATE(outerXTadOffsets, workspace, zLen, sd::LongType);
    shape::calcOffsets(outerXTadShapeInfo, outerXTadOffsets);
  }

  sd::LongType* innerXTadOffsets = zOffsets;
  if (!sameOffsets2) {
    ALLOCATE(innerXTadOffsets, workspace, tadLen, sd::LongType);
    shape::calcOffsets(innerXTadShapeInfo, innerXTadOffsets);
  }

  auto func = PRAGMA_THREADS_FOR {
    for (auto i = start; i < stop; ++i) {
      const auto tad = x + outerXTadOffsets[i];
      auto s = OpType::startingValue(tad);

      for (sd::LongType j = 0; j < tadLen; j++)
        s = OpType::update(s, OpType::op(tad[innerXTadOffsets[j]], extraParams), extraParams);

      z[zOffsets[i]] = OpType::postProcess(s, tadLen, extraParams);
    }
  };

  samediff::Threads::parallel_for(func, 0, shape::length(zShapeInfo));

  RELEASE(outerXTadShapeInfo, workspace);
  RELEASE(innerXTadShapeInfo, workspace);
  RELEASE(zOffsets, workspace);
  if (!sameOffsets1) RELEASE(outerXTadOffsets, workspace);
  if (!sameOffsets2) RELEASE(innerXTadOffsets, workspace);
}

//////////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E>
template <typename OpType>
SD_LIB_HIDDEN void sd::ReductionLoops<X, Z, E>::loopReduce(sd::memory::Workspace* workspace, const X* x,
                                                           const sd::LongType* xShapeInfo, Z* z,
                                                           const sd::LongType* zShapeInfo, const int* dims,
                                                           E* extraParams) {
  const int xRank = shape::rank(xShapeInfo);
  const int zRank = shape::rank(zShapeInfo);

  // shape::printShapeInfoLinear(xShapeInfo);
  // shape::printShapeInfoLinear(zShapeInfo);
  // shape::printIntArray(dims, shape::rank(xShapeInfo));

  if (xRank == 2 && zRank == 1)
    reduceExec21<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 3 && zRank == 1)
    reduceExec31<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 3 && zRank == 2)
    reduceExec32<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 4 && zRank == 1)
    reduceExec41<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 4 && zRank == 2)
    reduceExec42<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 4 && zRank == 3)
    reduceExec43<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 5 && zRank == 1)
    reduceExec51<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 5 && zRank == 2)
    reduceExec52<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 5 && zRank == 3)
    reduceExec53<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else if (xRank == 5 && zRank == 4)
    reduceExec54<X, Z, E, OpType>(x, xShapeInfo, z, zShapeInfo, dims, extraParams);
  else
    reduceDefault<X, Z, E, OpType>(workspace, x, xShapeInfo, z, zShapeInfo, dims, extraParams);
}

//////////////////////////////////////////////////////////////////////////////
template <typename X, typename Z, typename E>
template <typename OpType>
SD_LIB_HIDDEN void sd::TransformLoops<X, Z, E>::loopTransform(const X* x, const sd::LongType* xShapeInfo, Z* z,
                                                              const sd::LongType* zShapeInfo, E* extraParams,
                                                              uint64_t threadId, uint64_t numThreads) {
  const LoopKind::Kind kindOfLoop = LoopKind::deduceKindOfLoopXZ(xShapeInfo, zShapeInfo);
  if(xShapeInfo == nullptr) {
    throw std::runtime_error("Input x shape info was null!");
  }

  if(xShapeInfo[0] > SD_MAX_RANK || xShapeInfo[0] < 0) {
    throw std::runtime_error("x shape info appears to be corrupt. This is likely due to deallocation.");
  }

  if(zShapeInfo[0] > SD_MAX_RANK || zShapeInfo[0] < 0) {
    throw std::runtime_error("z shape info appears to be corrupt. This is likely due to deallocation.");
  }

  if(zShapeInfo == nullptr) {
    throw std::runtime_error("Input z shape info was null!");
  }




  const sd::LongType* xShape = shape::shapeOf(const_cast<sd::LongType*>(xShapeInfo));
  const sd::LongType* xStride = shape::stride(const_cast<sd::LongType*>(xShapeInfo));
  const sd::LongType* zStride = shape::stride(const_cast<sd::LongType*>(zShapeInfo));
  const sd::LongType len = shape::length(xShapeInfo);

  if (len == 0) return;

  switch (kindOfLoop) {
    //*********************************************//
    case LoopKind::EWS1: {
      auto span = samediff::Span::build(threadId, numThreads, 0, len, 1);
      int64_t start = span.startX(), stop = span.stopX();

      for (auto i = start; i < stop; i++) z[i] = OpType::op(x[i], extraParams);

    } break;

      //*********************************************//
    case LoopKind::EWSNONZERO: {
      const sd::Unsigned xEws = shape::elementWiseStride(xShapeInfo);
      const sd::Unsigned zEws = shape::elementWiseStride(zShapeInfo);

      auto span = samediff::Span::build(threadId, numThreads, 0, len, 1);
      int64_t start = span.startX(), stop = span.stopX();

      for (auto i = start; i < stop; i++) z[i * zEws] = OpType::op(x[i * xEws], extraParams);

    } break;

      //*********************************************//
    case LoopKind::Z_EWSNONZERO: {

      const sd::Unsigned zEws = shape::elementWiseStride(zShapeInfo);
      sd::Unsigned castXShapeInfo[SD_MAX_RANK];
      const bool canCastX = sd::DataTypeUtils::castShapeInfo<sd::Unsigned>(xShapeInfo, castXShapeInfo);

      auto span = samediff::Span::build(threadId, numThreads, 0, len, 1);
      int64_t start = span.startX(), stop = span.stopX();

      if (zEws > 1) {
        for (auto i = start; i < stop; i++) {
          const auto xOffset = shape::indexOffset(i, xShapeInfo, castXShapeInfo, canCastX);
          z[i * zEws] = OpType::op(x[xOffset], extraParams);
        }
      } else {
        for (auto i = start; i < stop; i++) {
          const auto xOffset = shape::indexOffset(i, xShapeInfo, castXShapeInfo, canCastX);
          z[i] = OpType::op(x[xOffset], extraParams);
        }
      }

    } break;

      //*********************************************//
    case LoopKind::RANK1: {
      auto span = samediff::Span::build(threadId, numThreads, 0, len, 1);

      for (auto i0 = span.startX(); i0 < span.stopX(); i0++) {
        z[i0 * zStride[0]] = OpType::op(x[i0 * xStride[0]], extraParams);
      }
    } break;

      //*********************************************//
    case LoopKind::RANK2: {
      auto uXShape0 = static_cast<sd::Unsigned>(xShape[0]);
      auto uXShape1 = static_cast<sd::Unsigned>(xShape[1]);

      auto loop = samediff::ThreadsHelper::pickLoop2d(numThreads, uXShape0, uXShape1);

      auto span = samediff::Span2::build(loop, threadId, numThreads, 0, uXShape0, 1, 0, uXShape1, 1);

      for (auto i0 = span.startX(); i0 < span.stopX(); i0++) {
        auto z0 = i0 * zStride[0];
        auto x0 = i0 * xStride[0];

        for (auto i1 = span.startY(); i1 < span.stopY(); ++i1) {
          z[z0 + i1 * zStride[1]] = OpType::op(x[x0 + i1 * xStride[1]], extraParams);

        }
      }

    } break;

      //*********************************************//
    case LoopKind::RANK3: {
      auto uXShape0 = xShape[0];
      auto uXShape1 = xShape[1];
      auto uXShape2 = xShape[2];

      auto loop = samediff::ThreadsHelper::pickLoop2d(numThreads, uXShape0, uXShape1);
      auto span = samediff::Span2::build(loop, threadId, numThreads, 0, uXShape0, 1, 0, uXShape1, 1);

      for (auto i0 = span.startX(); i0 < span.stopX(); i0++)
        for (auto i1 = span.startY(); i1 < span.stopY(); i1++) {
          auto z0 = i0 * zStride[0] + i1 * zStride[1];
          auto x0 = i0 * xStride[0] + i1 * xStride[1];

          for (sd::LongType i2 = 0; i2 < uXShape2; ++i2)
            z[z0 + i2 * zStride[2]] = OpType::op(x[x0 + i2 * xStride[2]], extraParams);
        }

    } break;

      //*********************************************//
    case LoopKind::RANK4: {

      auto uXShape0 = xShape[0];
      auto uXShape1 = xShape[1];
      auto uXShape2 = xShape[2];
      auto uXShape3 = xShape[3];

      auto loop = samediff::ThreadsHelper::pickLoop3d(numThreads, uXShape0, uXShape1, uXShape2);
      auto span = samediff::Span3::build(loop, threadId, numThreads, 0, uXShape0, 1, 0, uXShape1, 1, 0, uXShape2, 1);

      for (auto i0 = span.startX(); i0 < span.stopX(); i0++)
        for (auto i1 = span.startY(); i1 < span.stopY(); i1++)
          for (auto i2 = span.startZ(); i2 < span.stopZ(); i2++) {
            auto x0 = i0 * xStride[0] + i1 * xStride[1] + i2 * xStride[2];
            auto z0 = i0 * zStride[0] + i1 * zStride[1] + i2 * zStride[2];

            for (sd::LongType i3 = 0; i3 < uXShape3; ++i3)
              z[z0 + i3 * zStride[3]] = OpType::op(x[x0 + i3 * xStride[3]], extraParams);
          }

    } break;

      //*********************************************//
    case LoopKind::RANK5: {
      auto uXShape0 = xShape[0];
      auto uXShape1 = xShape[1];
      auto uXShape2 = xShape[2];
      auto uXShape3 = xShape[3];
      auto uXShape4 = xShape[4];

      auto loop = samediff::ThreadsHelper::pickLoop3d(numThreads, uXShape0, uXShape1, uXShape2);
      auto span = samediff::Span3::build(loop, threadId, numThreads, 0, uXShape0, 1, 0, uXShape1, 1, 0, uXShape2, 1);

      for (auto i0 = span.startX(); i0 < span.stopX(); i0++)
        for (auto i1 = span.startY(); i1 < span.stopY(); i1++)
          for (auto i2 = span.startZ(); i2 < span.stopZ(); i2++) {
            auto z0 = i0 * zStride[0] + i1 * zStride[1] + i2 * zStride[2];
            auto x0 = i0 * xStride[0] + i1 * xStride[1] + i2 * xStride[2];

            for (sd::LongType i3 = 0; i3 < uXShape3; ++i3) {
              auto z1 = z0 + i3 * zStride[3];
              auto x1 = x0 + i3 * xStride[3];

              for (sd::LongType i4 = 0; i4 < uXShape4; ++i4)
                z[z1 + i4 * zStride[4]] = OpType::op(x[x1 + i4 * xStride[4]], extraParams);
            }
          }

    } break;

      //*********************************************//
    default: {

      sd::Unsigned xShapeInfoCast[SD_MAX_RANK];
      sd::Unsigned zShapeInfoCast[SD_MAX_RANK];

      bool canCastX = DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);
      bool canCastZ = DataTypeUtils::castShapeInfo(zShapeInfo, zShapeInfoCast);

      auto span = samediff::Span::build(threadId, numThreads, 0, len, 1);

      for (auto i = span.startX(); i < span.stopX(); i++) {
        auto xOffset = shape::indexOffset(i, xShapeInfo, xShapeInfoCast, canCastX);
        auto zOffset = shape::indexOffset(i, zShapeInfo, zShapeInfoCast, canCastZ);
        z[zOffset] = OpType::op(x[xOffset], extraParams);
      }


    }
  }
}

//////////////////////////////////////////////////////////////////////////////
template <typename X, typename Z>
template <typename OpType>
void sd::Reduction3Loops<X, Z>::loopReduce3(const X* x, const sd::LongType* xShapeInfo, const X* y,
                                            const sd::LongType* yShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                            int* dims, int dimsLen, Z* extraParameters, int64_t start, int64_t stop) {
  // both tads have same shape, however strides and ews may differ

  Z param0(OpType::startingValue(x)), param1(OpType::startingValue(x)),
      param2(extraParameters ? extraParameters[0] : OpType::startingValue(x));

  const sd::LongType xLen = shape::length(xShapeInfo);
  const sd::LongType yLen = shape::length(yShapeInfo);

  const sd::LongType *xTadShapeInfo = nullptr, *yTadShapeInfo = nullptr, *xTadOffsets = nullptr, *yTadOffsets = nullptr;
  TadPack tadPackX, tadPackY;
  std::vector<sd::LongType> zeroOffsets;

  if (xLen == yLen) {
    tadPackX = sd::ConstantTadHelper::getInstance().tadForDimensions(xShapeInfo, dims, dimsLen);
    tadPackY = sd::ConstantTadHelper::getInstance().tadForDimensions(yShapeInfo, dims, dimsLen);
    xTadShapeInfo = tadPackX.primaryShapeInfo();
    yTadShapeInfo = tadPackY.primaryShapeInfo();
    xTadOffsets = tadPackX.primaryOffsets();
    yTadOffsets = tadPackY.primaryOffsets();
  } else if (yLen > xLen) {
    tadPackY = sd::ConstantTadHelper::getInstance().tadForDimensions(yShapeInfo, dims, dimsLen);
    xTadShapeInfo = xShapeInfo;
    yTadShapeInfo = tadPackY.primaryShapeInfo();
    yTadOffsets = tadPackY.primaryOffsets();
  } else {
    tadPackX = sd::ConstantTadHelper::getInstance().tadForDimensions(xShapeInfo, dims, dimsLen);
    yTadShapeInfo = yShapeInfo;
    xTadShapeInfo = tadPackX.primaryShapeInfo();
    xTadOffsets = tadPackX.primaryOffsets();
  }

  const LoopKind::Kind kindOfLoop = LoopKind::deduceKindOfLoopTadXYZ(xTadShapeInfo, yTadShapeInfo, zShapeInfo);

  const auto xTadEws = shape::elementWiseStride(xTadShapeInfo);
  const auto yTadEws = shape::elementWiseStride(yTadShapeInfo);
  const auto zEws = shape::elementWiseStride(zShapeInfo);

  const auto zLen = shape::length(zShapeInfo);
  const auto tadLen = shape::length(xTadShapeInfo);

  const auto tadShape = shape::shapeOf(xTadShapeInfo);
  const auto xTadStride = shape::stride(xTadShapeInfo);
  const auto yTadStride = shape::stride(xTadShapeInfo);

  int numThreads = OmpLaunchHelper::tadThreads(tadLen, zLen);

  switch (kindOfLoop) {
    //*********************************************//
    case LoopKind::EWS1: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType j = 0; j < tadLen; ++j)
          s = OpType::update(s, OpType::op(xTad[j], yTad[j], extraParams), extraParams);

        z[i] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    case LoopKind::EWSNONZERO: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType j = 0; j < tadLen; ++j)
          s = OpType::update(s, OpType::op(xTad[j * xTadEws], yTad[j * yTadEws], extraParams), extraParams);

        z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    case LoopKind::RANK1: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType i0 = 0; i0 < tadLen; ++i0) {
          const auto xTadOffset = i0 * xTadStride[0];
          const auto yTadOffset = i0 * yTadStride[0];
          s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
        }

        z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    case LoopKind::RANK2: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
          for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
            const auto xTadOffset = i0 * xTadStride[0] + i1 * xTadStride[1];
            const auto yTadOffset = i0 * yTadStride[0] + i1 * yTadStride[1];
            s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
          }
        }
        z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    case LoopKind::RANK3: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
          for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
            for (sd::LongType i2 = 0; i2 < tadShape[2]; ++i2) {
              const auto xTadOffset = i0 * xTadStride[0] + i1 * xTadStride[1] + i2 * xTadStride[2];
              const auto yTadOffset = i0 * yTadStride[0] + i1 * yTadStride[1] + i2 * yTadStride[2];
              s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
            }
          }
        }
        z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    case LoopKind::RANK4: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
          for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
            for (sd::LongType i2 = 0; i2 < tadShape[2]; ++i2) {
              for (sd::LongType i3 = 0; i3 < tadShape[3]; ++i3) {
                const auto xTadOffset =
                    i0 * xTadStride[0] + i1 * xTadStride[1] + i2 * xTadStride[2] + i3 * xTadStride[3];
                const auto yTadOffset =
                    i0 * yTadStride[0] + i1 * yTadStride[1] + i2 * yTadStride[2] + i3 * yTadStride[3];
                s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
              }
            }
          }
        }
        z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    case LoopKind::RANK5: {
      Z extraParams[3];
      for (auto i = start; i < stop; i++) {
        extraParams[0] = param0;
        extraParams[1] = param1;
        extraParams[2] = param2;

        const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
        const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
        auto s = OpType::startingValue(xTad);

        for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
          for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
            for (sd::LongType i2 = 0; i2 < tadShape[2]; ++i2) {
              for (sd::LongType i3 = 0; i3 < tadShape[3]; ++i3) {
                for (sd::LongType i4 = 0; i4 < tadShape[4]; ++i4) {
                  const auto xTadOffset = i0 * xTadStride[0] + i1 * xTadStride[1] + i2 * xTadStride[2] +
                                          i3 * xTadStride[3] + i4 * xTadStride[4];
                  const auto yTadOffset = i0 * yTadStride[0] + i1 * yTadStride[1] + i2 * yTadStride[2] +
                                          i3 * yTadStride[3] + i4 * yTadStride[4];
                  s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
                }
              }
            }
          }
        }
        z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
      };
    } break;

      //*********************************************//
    default: {
      sd::Unsigned castXTadShapeInfo[SD_MAX_RANK];
      const bool canCastXTad = sd::DataTypeUtils::castShapeInfo<sd::Unsigned>(xTadShapeInfo, castXTadShapeInfo);

      if (shape::haveSameShapeAndStrides(xTadShapeInfo, yTadShapeInfo)) {
        Z extraParams[3];
        for (auto i = start; i < stop; i++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
          const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
          auto s = OpType::startingValue(xTad);

          for (sd::LongType j = 0; j < tadLen; ++j) {
            const auto tadOffset = shape::indexOffset(j, xTadShapeInfo, castXTadShapeInfo, canCastXTad);
            s = OpType::update(s, OpType::op(xTad[tadOffset], yTad[tadOffset], extraParams), extraParams);
          }

          z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
        };
      } else {
        sd::Unsigned castYTadShapeInfo[SD_MAX_RANK];
        const bool canCastYTad = sd::DataTypeUtils::castShapeInfo<sd::Unsigned>(yTadShapeInfo, castYTadShapeInfo);

        Z extraParams[3];
        for (auto i = start; i < stop; i++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = xTadOffsets ? x + xTadOffsets[i] : x;
          const auto yTad = yTadOffsets ? y + yTadOffsets[i] : y;
          auto s = OpType::startingValue(xTad);

          for (sd::LongType j = 0; j < tadLen; ++j) {
            const auto xTadOffset = shape::indexOffset(j, xTadShapeInfo, castXTadShapeInfo, canCastXTad);
            const auto yTadOffset = shape::indexOffset(j, yTadShapeInfo, castYTadShapeInfo, canCastYTad);
            s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
          }
          z[i * zEws] = OpType::postProcess(s, tadLen, extraParams);
        };
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
template <typename X, typename Z>
template <typename OpType>
void sd::Reduction3Loops<X, Z>::loopReduce3All(const X* x, const sd::LongType* xShapeInfo, const X* y,
                                               const sd::LongType* yShapeInfo, Z* z, const sd::LongType* zShapeInfo,
                                               const sd::LongType* xTadShapeInfo, const sd::LongType* xTadOffsets,
                                               const sd::LongType* yTadShapeInfo, const sd::LongType* yTadOffsets,
                                               Z* extraParameters, int64_t start, int64_t stop) {
  // both tads have same shape, however strides and ews may differ

  Z param0(OpType::startingValue(x)), param1(OpType::startingValue(x)),
      param2(extraParameters ? extraParameters[0] : OpType::startingValue(x));

  const LoopKind::Kind kindOfLoop = LoopKind::deduceKindOfLoopTadXYZ(xTadShapeInfo, yTadShapeInfo, zShapeInfo);

  const auto xTadEws = shape::elementWiseStride(xTadShapeInfo);
  const auto yTadEws = shape::elementWiseStride(yTadShapeInfo);
  const auto zEws = shape::elementWiseStride(zShapeInfo);

  const auto zLen = shape::length(zShapeInfo);
  const auto tadLen = shape::length(xTadShapeInfo);

  const auto numXTads = shape::length(xShapeInfo) / tadLen;
  const auto numYTads = shape::length(yShapeInfo) / tadLen;

  const auto tadShape = shape::shapeOf(xTadShapeInfo);
  const auto xTadStride = shape::stride(xTadShapeInfo);
  const auto yTadStride = shape::stride(yTadShapeInfo);

  const auto startVal = OpType::startingValue(x);

  int numThreads = OmpLaunchHelper::tadThreads(tadLen, numXTads * numYTads);

  switch (kindOfLoop) {
    //*********************************************//
    case LoopKind::EWS1: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType j = 0; j < tadLen; ++j)
            s = OpType::update(s, OpType::op(xTad[j], yTad[j], extraParams), extraParams);

          z[zInd] = OpType::postProcess(s, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    case LoopKind::EWSNONZERO: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType j = 0; j < tadLen; ++j)
            s = OpType::update(s, OpType::op(xTad[j * xTadEws], yTad[j * yTadEws], extraParams), extraParams);

          z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    case LoopKind::RANK1: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType i0 = 0; i0 < tadLen; ++i0) {
            const auto xTadOffset = i0 * xTadStride[0];
            const auto yTadOffset = i0 * yTadStride[0];
            s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
          }
          z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    case LoopKind::RANK2: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
            for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
              const auto xTadOffset = i0 * xTadStride[0] + i1 * xTadStride[1];
              const auto yTadOffset = i0 * yTadStride[0] + i1 * yTadStride[1];
              s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
            }
          }
          z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    case LoopKind::RANK3: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
            for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
              for (sd::LongType i2 = 0; i2 < tadShape[2]; ++i2) {
                const auto xTadOffset = i0 * xTadStride[0] + i1 * xTadStride[1] + i2 * xTadStride[2];
                const auto yTadOffset = i0 * yTadStride[0] + i1 * yTadStride[1] + i2 * yTadStride[2];
                s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
              }
            }
          }
          z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    case LoopKind::RANK4: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
            for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
              for (sd::LongType i2 = 0; i2 < tadShape[2]; ++i2) {
                for (sd::LongType i3 = 0; i3 < tadShape[3]; ++i3) {
                  const auto xTadOffset =
                      i0 * xTadStride[0] + i1 * xTadStride[1] + i2 * xTadStride[2] + i3 * xTadStride[3];
                  const auto yTadOffset =
                      i0 * yTadStride[0] + i1 * yTadStride[1] + i2 * yTadStride[2] + i3 * yTadStride[3];
                  s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
                }
              }
            }
          }
          z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    case LoopKind::RANK5: {
      Z extraParams[3];
      for (sd::LongType ix = 0; ix < numXTads; ix++) {
        for (sd::LongType iy = 0; iy < numYTads; iy++) {
          extraParams[0] = param0;
          extraParams[1] = param1;
          extraParams[2] = param2;

          const auto xTad = x + xTadOffsets[ix];
          const auto yTad = y + yTadOffsets[iy];
          const auto zInd = ix * numYTads + iy;
          auto s = startVal;

          for (sd::LongType i0 = 0; i0 < tadShape[0]; ++i0) {
            for (sd::LongType i1 = 0; i1 < tadShape[1]; ++i1) {
              for (sd::LongType i2 = 0; i2 < tadShape[2]; ++i2) {
                for (sd::LongType i3 = 0; i3 < tadShape[3]; ++i3) {
                  for (sd::LongType i4 = 0; i4 < tadShape[4]; ++i4) {
                    const auto xTadOffset = i0 * xTadStride[0] + i1 * xTadStride[1] + i2 * xTadStride[2] +
                                            i3 * xTadStride[3] + i4 * xTadStride[4];
                    const auto yTadOffset = i0 * yTadStride[0] + i1 * yTadStride[1] + i2 * yTadStride[2] +
                                            i3 * yTadStride[3] + i4 * yTadStride[4];
                    s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
                  }
                }
              }
            }
          }
          z[zInd * zEws] = OpType::postProcess(start, tadLen, extraParams);
        }
      };
    } break;

      //*********************************************//
    default: {
      sd::Unsigned castXTadShapeInfo[SD_MAX_RANK];
      const bool canCastXTad = sd::DataTypeUtils::castShapeInfo<sd::Unsigned>(xTadShapeInfo, castXTadShapeInfo);

      if (shape::haveSameShapeAndStrides(xTadShapeInfo, yTadShapeInfo)) {
        Z extraParams[3];
        for (sd::LongType ix = 0; ix < numXTads; ix++) {
          for (sd::LongType iy = 0; iy < numYTads; iy++) {
            extraParams[0] = param0;
            extraParams[1] = param1;
            extraParams[2] = param2;

            const auto xTad = x + xTadOffsets[ix];
            const auto yTad = y + yTadOffsets[iy];
            const auto zInd = ix * numYTads + iy;
            auto s = startVal;

            for (sd::LongType j = 0; j < tadLen; ++j) {
              const auto tadOffset = shape::indexOffset(j, xTadShapeInfo, castXTadShapeInfo, canCastXTad);
              s = OpType::update(s, OpType::op(xTad[tadOffset], yTad[tadOffset], extraParams), extraParams);
            }
            z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
          }
        };
      } else {
        sd::Unsigned castYTadShapeInfo[SD_MAX_RANK];
        const bool canCastYTad = sd::DataTypeUtils::castShapeInfo<sd::Unsigned>(yTadShapeInfo, castYTadShapeInfo);

        Z extraParams[3];
        for (sd::LongType ix = 0; ix < numXTads; ix++) {
          for (sd::LongType iy = 0; iy < numYTads; iy++) {
            extraParams[0] = param0;
            extraParams[1] = param1;
            extraParams[2] = param2;

            const auto xTad = x + xTadOffsets[ix];
            const auto yTad = y + yTadOffsets[iy];
            const auto zInd = ix * numYTads + iy;
            auto s = startVal;

            for (sd::LongType j = 0; j < tadLen; ++j) {
              const auto xTadOffset = shape::indexOffset(j, xTadShapeInfo, castXTadShapeInfo, canCastXTad);
              const auto yTadOffset = shape::indexOffset(j, yTadShapeInfo, castYTadShapeInfo, canCastYTad);
              s = OpType::update(s, OpType::op(xTad[xTadOffset], yTad[yTadOffset], extraParams), extraParams);
            }

            z[zInd * zEws] = OpType::postProcess(s, tadLen, extraParams);
          }
        };
      }
    }
  }
}

}  // namespace sd

#endif  // LIBND4J_LOOPS_H
