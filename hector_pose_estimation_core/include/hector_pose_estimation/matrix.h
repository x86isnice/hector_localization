//=================================================================================================
// Copyright (c) 2011, Johannes Meyer and Martin Nowara, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#ifndef HECTOR_POSE_ESTIMATION_MATRIX_H
#define HECTOR_POSE_ESTIMATION_MATRIX_H

#include <Eigen/Geometry>
#include <stdexcept>

// #define ASSERT_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC
#define ASSERT_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC_PRECISION 1e-5
#define FORCE_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC

// We need this to preallocate memory for vectors and matrices:
#define MAXIMUM_STATE_VARIABLES 24

namespace hector_pose_estimation {
  using Eigen::Dynamic;
  using Eigen::Lower;
  using Eigen::Upper;

  typedef double ScalarType;
  typedef Eigen::DenseIndex IndexType;

  template <int Rows>
  class ColumnVector_ : public Eigen::Matrix<ScalarType,Rows,1,0,(Rows != Dynamic ? Rows : MAXIMUM_STATE_VARIABLES),1> {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(Rows != Dynamic) // is this really required if Eigen::Matrix is the base class?

    typedef ColumnVector_<Rows> Derived;
    typedef Eigen::Matrix<ScalarType,Rows,1,0,(Rows != Dynamic ? Rows : MAXIMUM_STATE_VARIABLES),1> Base;
    typedef typename Eigen::internal::traits<Base>::Scalar Scalar;
    typedef typename Eigen::internal::traits<Base>::Index Index;
    typedef Eigen::Map<Base> Map;
    typedef Eigen::Map<const Base> ConstMap;

    ColumnVector_() { this->setZero(); }
    explicit ColumnVector_(IndexType size) : Base(size) { assert(Rows == Dynamic || size == Rows); this->setZero(); }
//    explicit ColumnVector_(Scalar value) { this->setConstant(value); }
    ColumnVector_(Scalar x, Scalar y, Scalar z) : Base(x, y, z) {}
    template <typename OtherDerived> ColumnVector_(const Eigen::MatrixBase<OtherDerived>& other) : Base(other) {}

    EIGEN_INHERIT_ASSIGNMENT_EQUAL_OPERATOR(Derived)
    Derived& operator=(Scalar scalar) { setConstant(scalar); return *this; }
  };
  typedef ColumnVector_<Dynamic> ColumnVector;
  typedef ColumnVector_<3> ColumnVector3;

  template <int Cols>
  class RowVector_ : public Eigen::Matrix<ScalarType,1,Cols,Eigen::RowMajor,1,MAXIMUM_STATE_VARIABLES> {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(Cols != Dynamic) // is this really required if Eigen::Matrix is the base class?

    typedef RowVector_<Cols> Derived;
    typedef Eigen::Matrix<ScalarType,1,Cols,0,1,MAXIMUM_STATE_VARIABLES> Base;
    typedef typename Eigen::internal::traits<Base>::Scalar Scalar;
    typedef typename Eigen::internal::traits<Base>::Index Index;
    typedef Eigen::Map<Base> Map;
    typedef Eigen::Map<const Base> ConstMap;

    RowVector_() { this->setZero(); }
    explicit RowVector_(IndexType size) : Base(size) { assert(Cols == Dynamic || size == Cols); this->setZero(); }
//    explicit RowVector_(Scalar value) { this->setConstant(value); }
    RowVector_(Scalar x, Scalar y, Scalar z) : Base(x, y, z) {}
    template <typename OtherDerived> RowVector_(const Eigen::MatrixBase<OtherDerived>& other) : Base(other) {}

    EIGEN_INHERIT_ASSIGNMENT_EQUAL_OPERATOR(Derived)
    Derived& operator=(Scalar scalar) { setConstant(scalar); return *this; }
  };
  typedef RowVector_<Dynamic> RowVector;
  typedef RowVector_<3> RowVector3;

  template <int Rows, int Cols>
  class Matrix_ : public Eigen::Matrix<ScalarType,Rows,Cols,(Rows==1 && Cols!=1 ? Eigen::RowMajor : Eigen::ColMajor),(Rows != Dynamic ? Rows : MAXIMUM_STATE_VARIABLES),(Cols != Dynamic ? Cols : MAXIMUM_STATE_VARIABLES)> {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(Rows != Dynamic || Cols != Dynamic) // is this really required if Eigen::Matrix is the base class?

    typedef Matrix_<Rows,Cols> Derived;
    typedef Eigen::Matrix<ScalarType,Rows,Cols,(Rows==1 && Cols!=1 ? Eigen::RowMajor : Eigen::ColMajor),(Rows != Dynamic ? Rows : MAXIMUM_STATE_VARIABLES),(Cols != Dynamic ? Cols : MAXIMUM_STATE_VARIABLES)> Base;
    typedef typename Eigen::internal::traits<Base>::Scalar Scalar;
    typedef typename Eigen::internal::traits<Base>::Index Index;
    typedef Eigen::Map<Base> Map;
    typedef Eigen::Map<const Base> ConstMap;

    Matrix_() { this->setZero(); }
    explicit Matrix_(Index rows, Index cols) : Base(rows, cols) { assert(Rows == Dynamic || rows == Rows); assert(Cols == Dynamic || cols == Cols); this->setZero(); }
//    explicit Matrix_(Scalar value) { this->setConstant(value); }
    template <typename OtherDerived> Matrix_(const Eigen::MatrixBase<OtherDerived>& other) : Base(other) {}

    EIGEN_INHERIT_ASSIGNMENT_EQUAL_OPERATOR(Derived)
    Derived& operator=(Scalar scalar) { setConstant(scalar); return *this; }
  };
  typedef Matrix_<Dynamic,Dynamic> Matrix;
  typedef Matrix_<3,3> Matrix3;

  template <int RowsCols>
  class SymmetricMatrix_ : public Matrix_<RowsCols,RowsCols> {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(RowsCols != Dynamic) // is this really required if Eigen::Matrix is the base class?

    typedef SymmetricMatrix_<RowsCols> Derived;
    typedef Matrix_<RowsCols,RowsCols> Storage;
    typedef typename Storage::Base Base;
    typedef Eigen::SelfAdjointView<typename Storage::Base,Upper> SelfAdjointView;
    typedef typename Eigen::internal::traits<Base>::Scalar Scalar;
    typedef typename Eigen::internal::traits<Base>::Index Index;
    typedef Eigen::Map<Base> Map;
    typedef Eigen::Map<const Base> ConstMap;

    // Constructors
    SymmetricMatrix_() {}
    //explicit SymmetricMatrix_(Scalar value) { this->setConstant(value); }
    explicit SymmetricMatrix_(Index dim) : Storage(dim, dim) { assert(RowsCols == Dynamic || dim == RowsCols); }
    template <typename OtherDerived> SymmetricMatrix_(const Eigen::MatrixBase<OtherDerived>& other) : Storage(other) {
#if defined(FORCE_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC)
      symmetric();
#endif
    }
    SymmetricMatrix_(const Derived& other) : Storage(other) {}

    template <typename OtherDerived> Derived& operator=(const Eigen::MatrixBase<OtherDerived>& other) {
      this->Base::operator=(other);
#if defined(ASSERT_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC)
      eigen_assert(this->isApprox(this->transpose(), ASSERT_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC_PRECISION));
#endif
#if defined(FORCE_SYMMETRIC_MATRIX_TO_BE_SYMMETRIC)
      return symmetric();
#else
      return *this;
#endif
    }

    template <typename OtherDerived> Derived& operator=(const Derived& other) {
      this->Base::operator=(other);
      return *this;
    }

    Derived& symmetric() {
      this->Base::operator=((*this + this->transpose()) / 2);
      return *this;
    }

//    template <typename OtherDerived> SymmetricMatrix_<Derived::RowsAtCompileTime> quadratic(const Eigen::MatrixBase<OtherDerived>& other) {
//      return other * this->template selfadjointView<Upper>() * other.transpose();
//    }

    Derived inverse() const {
//      return this->template selfadjointView<Upper>().inverse();
      return this->Storage::inverse().eval();
    }
  };
  typedef SymmetricMatrix_<3> SymmetricMatrix3;

  class SymmetricMatrix : public SymmetricMatrix_<Dynamic> {
  public:
    typedef SymmetricMatrix Derived;
    typedef SymmetricMatrix_<Dynamic> Storage;
    typedef typename Storage::Base Base;
    typedef Eigen::SelfAdjointView<typename Storage::Base,Upper> SelfAdjointView;
    typedef typename Eigen::internal::traits<Base>::Scalar Scalar;
    typedef typename Eigen::internal::traits<Base>::Index Index;
    typedef Eigen::Map<Base> Map;
    typedef Eigen::Map<const Base> ConstMap;

    // Constructors
    SymmetricMatrix() : Storage() {}
    SymmetricMatrix(Index dim) : Storage(dim) { this->setZero(); }
    // SymmetricMatrix(Index dim, Scalar value) : Storage(dim) { this->setConstant(value); }
    template <int OtherRowsCols> SymmetricMatrix(const SymmetricMatrix_<OtherRowsCols>& other) : Storage(other) {}
    template <typename OtherDerived> SymmetricMatrix(const Eigen::MatrixBase<OtherDerived>& other) : Storage(other) {}

    template <typename OtherDerived> Derived& operator=(const Eigen::MatrixBase<OtherDerived>& other) {
      this->Base::operator=(other);
      return symmetric();
    }

    Derived& symmetric() {
      Storage::symmetric();
      return *this;
    }

    void resize(Index size) {
      Base::resize(size, size);
    }

    void conservativeResize(Index size) {
      Base::conservativeResize(size, size);
    }
  };

  class SkewSymmetricMatrix : public Matrix3 {
  public:
    template <typename OtherDerived> SkewSymmetricMatrix(const Eigen::MatrixBase<OtherDerived>& other) : Matrix3()
    {
      EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Eigen::MatrixBase<OtherDerived>, 3);
      *this <<  0.0,       -other.z(),  other.y(),
                other.z(),  0.0,       -other.x(),
               -other.y(),  other.x(),  0.0;
    }
  };

  typedef Eigen::Quaternion<ScalarType> Quaternion;

  using Eigen::VectorBlock;
  typedef VectorBlock<ColumnVector,3>       VectorBlock3;
  typedef VectorBlock<ColumnVector,4>       VectorBlock4;
  typedef VectorBlock<const ColumnVector,3> ConstVectorBlock3;
  typedef VectorBlock<const ColumnVector,4> ConstVectorBlock4;

  using Eigen::Block;
  typedef Eigen::Block<Matrix,Dynamic,Dynamic> MatrixBlock;

} // namespace hector_pose_estimation

namespace Eigen {
namespace internal {

#define EIGEN_FORWARD_TRAITS_FOR_TYPE(Base) \
  typedef typename traits<Base>::Scalar Scalar; \
  typedef typename traits<Base>::StorageKind StorageKind; \
  typedef typename traits<Base>::Index Index; \
  typedef typename traits<Base>::XprKind XprKind; \
  enum { \
    RowsAtCompileTime = traits<Base>::RowsAtCompileTime, \
    ColsAtCompileTime = traits<Base>::ColsAtCompileTime, \
    MaxRowsAtCompileTime = traits<Base>::MaxRowsAtCompileTime, \
    MaxColsAtCompileTime = traits<Base>::MaxColsAtCompileTime, \
    Flags = traits<Base>::Flags, \
    CoeffReadCost = traits<Base>::CoeffReadCost, \
    Options = traits<Base>::Options, \
    InnerStrideAtCompileTime = traits<Base>::InnerStrideAtCompileTime, \
    OuterStrideAtCompileTime = traits<Base>::OuterStrideAtCompileTime \
  }

template <int Rows> struct traits< hector_pose_estimation::ColumnVector_<Rows> > {
  typedef typename hector_pose_estimation::ColumnVector_<Rows>::Base EigenType;
  EIGEN_FORWARD_TRAITS_FOR_TYPE(EigenType);
};

template <int Cols> struct traits< hector_pose_estimation::RowVector_<Cols> > {
  typedef typename hector_pose_estimation::RowVector_<Cols>::Base EigenType;
  EIGEN_FORWARD_TRAITS_FOR_TYPE(EigenType);
};

template <int Rows, int Cols> struct traits< hector_pose_estimation::Matrix_<Rows,Cols> > {
  typedef typename hector_pose_estimation::Matrix_<Rows,Cols>::Base EigenType;
  EIGEN_FORWARD_TRAITS_FOR_TYPE(EigenType);
};

template <int RowsCols> struct traits< hector_pose_estimation::SymmetricMatrix_<RowsCols> > {
  typedef typename hector_pose_estimation::SymmetricMatrix_<RowsCols>::Base EigenType;
  EIGEN_FORWARD_TRAITS_FOR_TYPE(EigenType);
};

template <> struct traits< hector_pose_estimation::SymmetricMatrix > {
  typedef hector_pose_estimation::SymmetricMatrix::Base EigenType;
  EIGEN_FORWARD_TRAITS_FOR_TYPE(EigenType);
};

} // namespace internal
} // namespace Eigen

#endif // HECTOR_POSE_ESTIMATION_MATRIX_H
