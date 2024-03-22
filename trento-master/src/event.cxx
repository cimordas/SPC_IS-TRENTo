// TRENTO: Reduced Thickness Event-by-event Nuclear Topology
// Copyright 2015 Jonah E. Bernhard, J. Scott Moreland
// MIT License

#include "event.h"

#include <algorithm>
#include <cmath>

#include <boost/program_options/variables_map.hpp>

#include "nucleus.h"

namespace trento {

namespace {

constexpr double TINY = 1e-12;

// Generalized mean for p > 0.
// M_p(a, b) = (1/2*(a^p + b^p))^(1/p)
inline double positive_pmean(double p, double a, double b) {
  return std::pow(.5*(std::pow(a, p) + std::pow(b, p)), 1./p);
}

// Generalized mean for p < 0.
// Same as the positive version, except prevents division by zero.
inline double negative_pmean(double p, double a, double b) {
  if (a < TINY || b < TINY)
    return 0.;
  return positive_pmean(p, a, b);
}

// Generalized mean for p == 0.
inline double geometric_mean(double a, double b) {
  return std::sqrt(a*b);
}

}  // unnamed namespace

// Determine the grid parameters like so:
//   1. Read and set step size from the configuration.
//   2. Read grid max from the config, then set the number of steps as
//      nsteps = ceil(2*max/step).
//   3. Set the actual grid max as max = nsteps*step/2.  Hence if the step size
//      does not evenly divide the config max, the actual max will be marginally
//      larger (by at most one step size).
Event::Event(const VarMap& var_map)
    : norm_(var_map["normalization"].as<double>()),
      longOutputCalc_(var_map["long-output"].as<bool>()),
      dxy_(var_map["grid-step"].as<double>()),
      nsteps_(std::ceil(2.*var_map["grid-max"].as<double>()/dxy_)),
      xymax_(.5*nsteps_*dxy_),
      TA_(boost::extents[nsteps_][nsteps_]),
      TB_(boost::extents[nsteps_][nsteps_]),
      TR_(boost::extents[nsteps_][nsteps_]) {
  // Choose which version of the generalized mean to use based on the
  // configuration.  The possibilities are defined above.  See the header for
  // more information.
  auto p = var_map["reduced-thickness"].as<double>();

  if (std::fabs(p) < TINY) {
    compute_reduced_thickness_ = [this]() {
      compute_reduced_thickness(geometric_mean);
    };
  } else if (p > 0.) {
    compute_reduced_thickness_ = [this, p]() {
      compute_reduced_thickness(
        [p](double a, double b) { return positive_pmean(p, a, b); });
    };
  } else {
    compute_reduced_thickness_ = [this, p]() {
      compute_reduced_thickness(
        [p](double a, double b) { return negative_pmean(p, a, b); });
    };
  }
}

void Event::compute(const Nucleus& nucleusA, const Nucleus& nucleusB,
                    const NucleonCommon& nucleon_common) {
  // Reset npart; compute_nuclear_thickness() increments it.
  npart_ = 0;
  compute_nuclear_thickness(nucleusA, nucleon_common, TA_);
  compute_nuclear_thickness(nucleusB, nucleon_common, TB_);
  compute_reduced_thickness_();
  compute_observables();
}

namespace {

// Limit a value to a range.
// Used below to constrain grid indices.
template <typename T>
inline const T& clip(const T& value, const T& min, const T& max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

}  // unnamed namespace

void Event::compute_nuclear_thickness(
    const Nucleus& nucleus, const NucleonCommon& nucleon_common, Grid& TX) {
  // Construct the thickness grid by looping over participants and adding each
  // to a small subgrid within its radius.  Compared to the other possibility
  // (grid cells as the outer loop and participants as the inner loop), this
  // reduces the number of required distance-squared calculations by a factor of
  // ~20 (depending on the nucleon size).  The Event unit test verifies that the
  // two methods agree.

  // Wipe grid with zeros.
  std::fill(TX.origin(), TX.origin() + TX.num_elements(), 0.);

  // Deposit each participant onto the grid.
  for (const auto& nucleon : nucleus) {
    if (!nucleon.is_participant())
      continue;

    ++npart_;

    // Get nucleon subgrid boundary {xmin, xmax, ymin, ymax}.
    const auto boundary = nucleon_common.boundary(nucleon);

    // Determine min & max indices of nucleon subgrid.
    int ixmin = clip(static_cast<int>((boundary[0]+xymax_)/dxy_), 0, nsteps_-1);
    int ixmax = clip(static_cast<int>((boundary[1]+xymax_)/dxy_), 0, nsteps_-1);
    int iymin = clip(static_cast<int>((boundary[2]+xymax_)/dxy_), 0, nsteps_-1);
    int iymax = clip(static_cast<int>((boundary[3]+xymax_)/dxy_), 0, nsteps_-1);

    // Add profile to grid.
    for (auto iy = iymin; iy <= iymax; ++iy) {
      for (auto ix = ixmin; ix <= ixmax; ++ix) {
        TX[iy][ix] += nucleon_common.thickness(
          nucleon, (ix+.5)*dxy_ - xymax_, (iy+.5)*dxy_ - xymax_
        );
      }
    }
  }
}

template <typename GenMean>
void Event::compute_reduced_thickness(GenMean gen_mean) {
  double sum = 0.;
  double ixcm = 0.;
  double iycm = 0.;

  for (int iy = 0; iy < nsteps_; ++iy) {
    for (int ix = 0; ix < nsteps_; ++ix) {
      auto t = norm_ * gen_mean(TA_[iy][ix], TB_[iy][ix]);
      TR_[iy][ix] = t;
      sum += t;
      // Center of mass grid indices.
      // No need to multiply by dxy since it would be canceled later.
      ixcm += t * static_cast<double>(ix);
      iycm += t * static_cast<double>(iy);
    }
  }

  multiplicity_ = dxy_ * dxy_ * sum;
  ixcm_ = ixcm / sum;
  iycm_ = iycm / sum;
}

void Event::compute_observables() {
  // Compute eccentricity.

  // Simple helper class for use in the following loop.
//  struct EccentricityAccumulator {
//    double re = 0.;  // real part
//    double im = 0.;  // imaginary part
//    double wt = 0.;  // weight
//    double finish() const  // compute final eccentricity
//    { return std::sqrt(re*re + im*im) / std::fmax(wt, TINY); }
//  } e2, e3, e4, e5;


//! <<<<<<<<<<<<<<<  Farid Added >>>>>>>>>>>>>>>>>>
  struct EccentricityAccumulator {
    double re = 0.;  // real part
    double im = 0.;  // imaginary part
    double wt = 0.;  // weight
    double finish() const  // compute final eccentricity
    { return std::sqrt(re*re + im*im) / std::fmax(wt, TINY); }
    //! ============= Farid Added ===============
//    double farid_denom() const  // compute final eccentricity
//    { return std::fmax(wt, TINY); }
    //! =========================================
  } farid_norm, e2, e3, e4, e5, e6, e7, e8;
//! <<<<<<<<<<<<<<<  =========== >>>>>>>>>>>>>>>>>>


  for (int iy = 0; iy < nsteps_; ++iy) {
    for (int ix = 0; ix < nsteps_; ++ix) {

 //    std::cout << "*****" << xymax_ << "------" << dxy_ << "          " << nsteps_ << std::endl;
      const auto& t = TR_[iy][ix];
      if (t < TINY)
        continue;

      // Compute (x, y) relative to the CM and cache powers of x, y, r.
      auto x = static_cast<double>(ix) - ixcm_;
      auto x2 = x*x;
      auto x3 = x2*x;
      auto x4 = x2*x2;

      auto y = static_cast<double>(iy) - iycm_;
      auto y2 = y*y;
      auto y3 = y2*y;
      auto y4 = y2*y2;

      auto r2 = x2 + y2;
      auto r = std::sqrt(r2);
      auto r4 = r2*r2;

      auto xy = x*y;
      auto x2y2 = x2*y2;

      // The eccentricity harmonics are weighted averages of r^n*exp(i*n*phi)
      // over the entropy profile (reduced thickness).  The naive way to compute
      // exp(i*n*phi) at a given (x, y) point is essentially:
      //
      //   phi = arctan2(y, x)
      //   real = cos(n*phi)
      //   imag = sin(n*phi)
      //
      // However this implementation uses three unnecessary trig functions; a
      // much faster method is to express the cos and sin directly in terms of x
      // and y.  For example, it is trivial to show (by drawing a triangle and
      // using rudimentary trig) that
      //
      //   cos(arctan2(y, x)) = x/r = x/sqrt(x^2 + y^2)
      //   sin(arctan2(y, x)) = y/r = x/sqrt(x^2 + y^2)
      //
      // This is easily generalized to cos and sin of (n*phi) by invoking the
      // multiple angle formula, e.g. sin(2x) = 2sin(x)cos(x), and hence
      //
      //   sin(2*arctan2(y, x)) = 2*sin(arctan2(y, x))*cos(arctan2(y, x))
      //                        = 2*x*y / r^2
      //
      // Which not only eliminates the trig functions, but also naturally
      // cancels the r^2 weight.  This cancellation occurs for all n.
      //
      // The Event unit test verifies that the two methods agree.
      farid_norm.re += t;

      e2.re += t * (y2 - x2);
      e2.im += t * (-2.*xy);
      e2.wt += t * r2;

      e3.re += t * (3.*x*y2 - x3);
      e3.im += t * (y3 - 3.*y*x2);
      e3.wt += t * r2*r;

      e4.re += t * (-x4 - y4 + 6.*x2y2);
      e4.im += t * 4.*xy*(y2 - x2);
      e4.wt += t * r4;

      e5.re += t * x*(-x4 + 10.*x2y2 - 5.*y4);
      e5.im += t * y*(-5.*x4 + 10.*x2y2 - y4);
      e5.wt += t * r4*r;

      if (longOutputCalc_){
        
        auto x6 = x2*x4;
        auto x8 = x4*x4;

        auto y6 = y2*y4;
        auto y8 = y4*y4;

        auto x3y3 = x3*y3;
        auto x4y2 = x4*y2;
        auto x2y4 = x2*y4;
        auto x5y = x4*xy;
        auto xy5 = xy*y4;

        auto x4y4 = x4*y4;
        auto x2y6 = x2*y6;
        auto x6y2 = x6*y2;        

        auto r6 = r2*r4;
        auto r8 = r4*r4;

        e6.re += t * (-x6 + 15 * x4y2 - 15 * x2y4 + y6);
        e6.im += t * (-6 * x5y + 20 * x3y3 - 6 * xy5);
        e6.wt += t * r6;


        e7.re += t * x * (-x6 + 21 * x4y2 - 35 * x2y4 + 7 * y6);
        e7.im += t * y * (-7 * x6 + 35 * x4y2 - 21 * x2y4 + y6);
        e7.wt += t * r6 * r;


        e8.re += t * (-x8 + 28 * x6y2 - 70 * x4y4 + 28 * x2y6 - y8);
        e8.im += t * (8 * xy) * (-x6 + 7 * x4y2 - 7 * x2y4 + y6);
        e8.wt += t * r8;        

      }

    }
  }

  eccentricity_[2] = e2.finish();
  eccentricity_[3] = e3.finish();
  eccentricity_[4] = e4.finish();
  eccentricity_[5] = e5.finish();

  //! <<<<<<<<<<<<<<<< Farid Added >>>>>>>>>>>>>>>>>

  //! To compute eps_n the code runs 0,1,2,..., nsteps-1. For epsilons it is not important. The actual scale of ix and iy is important for r^n.

  //! I do not want to modify the code too much, so I just find the numerical value needed to convert correct value for r^n. It is r_converter in the following:


  double r_converted;

  r_converted = (2 * xymax_) / (nsteps_);

//  std::cout << "-------->" << r_converted << std::endl;

  eccx_[2] = e2.re  / std::fmax(e2.wt, TINY);
  eccy_[2] = e2.im / std::fmax(e2.wt, TINY);
  rm_[2] = e2.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,2);

//  std::cout << "========>" << e2.wt * pow(r_converted,2) << "          " << std::fmax(farid_norm.re, TINY) << std::endl;

//    std::cout << "========>" << std::sqrt(pow(e2.re,2) + pow(e2.im,2)) / std::fmax(e2.wt, TINY) << "          " << std::fmax(farid_norm.re, TINY) << std::endl;
//    std::cout << "===***==>" << std::sqrt(e2.re*e2.re + e2.im*e2.im) / std::fmax(e2.wt, TINY) << "          " << std::fmax(farid_norm.re, TINY) << std::endl;
//  std::cout << e2.wt << "          " << rm2.re << std::endl;
//  std::cout << "++++++++>" << e2.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,2) << std::endl;
//  std::cout << e3.wt << "          " << std::fmax(farid_norm.re, TINY) << std::endl;
//  std::cout << e4.wt << "          " << std::fmax(farid_norm.re, TINY) << std::endl;

  eccx_[3] = e3.re / std::fmax(e3.wt, TINY);
  eccy_[3] = e3.im / std::fmax(e3.wt, TINY);
  rm_[3] = e3.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,3);

  eccx_[4] = e4.re / std::fmax(e4.wt, TINY);
  eccy_[4] = e4.im / std::fmax(e4.wt, TINY);
  rm_[4] = e4.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,4);

  eccx_[5] = e5.re / std::fmax(e5.wt, TINY);
  eccy_[5] = e5.im / std::fmax(e5.wt, TINY);
  rm_[5] = e5.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,5);
  //! ==============================================

  if (longOutputCalc_){
    eccentricity_[6] = e6.finish();
    eccentricity_[7] = e7.finish();
    eccentricity_[8] = e8.finish();

    eccx_[6] = e6.re / std::fmax(e6.wt, TINY);
    eccy_[6] = e6.im / std::fmax(e6.wt, TINY);
    rm_[6] = e6.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,6);

    eccx_[7] = e7.re / std::fmax(e7.wt, TINY);
    eccy_[7] = e7.im / std::fmax(e7.wt, TINY);
    rm_[7] = e7.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,7);

    eccx_[8] = e8.re / std::fmax(e8.wt, TINY);
    eccy_[8] = e8.im / std::fmax(e8.wt, TINY);
    rm_[8] = e8.wt / std::fmax(farid_norm.re, TINY) * pow(r_converted,8);

  }

}

}  // namespace trento
