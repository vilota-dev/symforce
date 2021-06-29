//  -----------------------------------------------------------------------------
// This file was autogenerated by symforce. Do NOT modify by hand.
//
// Instead modify:
//     symforce/codegen/cpp_templates/example/cam_package_cpp_test.cc.jinja
//
// And then run `symforce_cam_codegen_test --update`.
// -----------------------------------------------------------------------------
/**
 * Tests for C++ camera types. Mostly checking all the templates compile since
 * the math is tested comprehensively in symbolic form.
 */

#include <iostream>
#include <random>

#include <Eigen/Dense>
#include <sym/atan_camera_cal.h>
#include <sym/camera.h>
#include <sym/double_sphere_camera_cal.h>
#include <sym/equidistant_epipolar_camera_cal.h>
#include <sym/linear_camera_cal.h>
#include <sym/pose3.h>
#include <sym/posed_camera.h>
#include <sym/rot3.h>
#include <sym/spherical_camera_cal.h>

#include "catch.hpp"

template <typename T>
T CalFromData(std::initializer_list<typename T::Scalar> data) {
  Eigen::Matrix<typename T::Scalar, sym::StorageOps<T>::StorageDim(), 1> data_vec;
  size_t i = 0;
  for (const auto& x : data) {
    data_vec[i] = x;
    i++;
  }
  return T(data_vec);
}

template <typename T>
struct CamCals {
  static std::vector<T> Get();
};

template <typename Scalar>
struct CamCals<sym::LinearCameraCal<Scalar>> {
  using T = sym::LinearCameraCal<Scalar>;
  static std::vector<T> Get() {
    return {
        CalFromData<T>({90, 90, 60, 80}),
        CalFromData<T>({380, 380, 320, 240}),
        CalFromData<T>({500, 500, 1000, 800}),
    };
  }
};

template <typename Scalar>
struct CamCals<sym::DoubleSphereCameraCal<Scalar>> {
  using T = sym::DoubleSphereCameraCal<Scalar>;
  static std::vector<T> Get() {
    return {
        CalFromData<T>({233, 233, 388, 388, 1.0, 0.0}),
        CalFromData<T>({833, 834, 388, 388, 2.559, -1.328}),
        CalFromData<T>({313, 313, 638, 514, -0.18, 0.59}),
    };
  }
};

template <typename Scalar>
struct CamCals<sym::SphericalCameraCal<Scalar>> {
  using T = sym::SphericalCameraCal<Scalar>;
  static std::vector<T> Get() {
    return {
        CalFromData<T>({234, 234, 388, 391, M_PI, 0, 0, 0, 0}),
        CalFromData<T>({234, 234, 388, 391, M_PI, 0.035, -0.025, 0.0070, -0.0015}),
        CalFromData<T>({250, 250, 700, 500, M_PI, -0.01, -0.016, 0.0020, 0.0030}),
    };
  }
};

template <typename Scalar>
struct CamCals<sym::EquidistantEpipolarCameraCal<Scalar>> {
  using T = sym::EquidistantEpipolarCameraCal<Scalar>;
  static std::vector<T> Get() {
    return {
        CalFromData<T>({90, 90, 60, 80}),
        CalFromData<T>({380, 380, 320, 240}),
        CalFromData<T>({500, 500, 1000, 800}),
    };
  }
};

template <typename Scalar>
struct CamCals<sym::ATANCameraCal<Scalar>> {
  using T = sym::ATANCameraCal<Scalar>;
  static std::vector<T> Get() {
    return {
        CalFromData<T>({90, 90, 60, 80, 0.68}),
        CalFromData<T>({380, 380, 320, 240, 0.35}),
        CalFromData<T>({500, 500, 1000, 800, 0.21}),
    };
  }
};

TEMPLATE_TEST_CASE("Test storage ops", "[cam_package]", sym::LinearCameraCal<double>,
                   sym::LinearCameraCal<float>, sym::DoubleSphereCameraCal<double>,
                   sym::DoubleSphereCameraCal<float>, sym::SphericalCameraCal<double>,
                   sym::SphericalCameraCal<float>, sym::EquidistantEpipolarCameraCal<double>,
                   sym::EquidistantEpipolarCameraCal<float>, sym::ATANCameraCal<double>,
                   sym::ATANCameraCal<float>) {
  using T = TestType;
  const T& cam_cal = GENERATE(from_range(CamCals<T>::Get()));

  using Scalar = typename T::Scalar;

  std::cout << "*** Testing StorageOps: " << cam_cal << " ***" << std::endl;

  constexpr int32_t storage_dim = sym::StorageOps<T>::StorageDim();
  CHECK(cam_cal.Data().rows() == storage_dim);
  CHECK(cam_cal.Data().cols() == 1);

  std::array<Scalar, storage_dim> arr;
  cam_cal.ToStorage(arr.data());
  for (int i = 0; i < arr.size(); ++i) {
    CHECK(arr[i] == cam_cal.Data()[i]);
  }

  const T cam_cal2 = sym::StorageOps<T>::FromStorage(arr.data());
  CHECK(cam_cal.Data() == cam_cal2.Data());
  arr[0] = 2.1;
  const T cam_cal3 = sym::StorageOps<T>::FromStorage(arr.data());
  CHECK(cam_cal.Data() != cam_cal3.Data());
}

TEMPLATE_TEST_CASE("Test project and deproject", "[cam_package]", sym::LinearCameraCal<double>,
                   sym::LinearCameraCal<float>, sym::DoubleSphereCameraCal<double>,
                   sym::DoubleSphereCameraCal<float>, sym::EquidistantEpipolarCameraCal<double>,
                   sym::EquidistantEpipolarCameraCal<float>, sym::ATANCameraCal<double>,
                   sym::ATANCameraCal<float>) {
  using T = TestType;
  const T& cam_cal = GENERATE(from_range(CamCals<T>::Get()));

  using Scalar = typename T::Scalar;
  const Scalar epsilon = 1e-6;  // For preventing degenerate numerical cases (e.g. division by zero)
  const Scalar tolerance = 10.0 * epsilon;  // For checking approx. equality

  std::cout << "*** Testing projection model: " << cam_cal << " ***" << std::endl;

  std::mt19937 gen(42);
  // Generate pixels around principal point
  std::uniform_real_distribution<Scalar> pixel_x_dist(0.0, 2.0 * cam_cal.Data()[2]);
  std::uniform_real_distribution<Scalar> pixel_y_dist(0.0, 2.0 * cam_cal.Data()[3]);
  for (int i = 0; i < 10; i++) {
    Eigen::Matrix<Scalar, 2, 1> pixel;
    pixel << pixel_x_dist(gen), pixel_y_dist(gen);

    Scalar is_valid_camera_ray;
    Scalar is_valid_pixel;
    const Eigen::Matrix<Scalar, 3, 1> camera_ray =
        cam_cal.CameraRayFromPixel(pixel, epsilon, &is_valid_camera_ray);
    const Eigen::Matrix<Scalar, 2, 1> pixel_reprojected =
        cam_cal.PixelFromCameraPoint(camera_ray, epsilon, &is_valid_pixel);
    if (is_valid_camera_ray == 1 && is_valid_pixel == 1) {
      CHECK(pixel.isApprox(pixel_reprojected, tolerance));
    }
  }
}

TEMPLATE_TEST_CASE("Test Camera class", "[cam_package]", sym::LinearCameraCal<double>,
                   sym::LinearCameraCal<float>, sym::DoubleSphereCameraCal<double>,
                   sym::DoubleSphereCameraCal<float>, sym::EquidistantEpipolarCameraCal<double>,
                   sym::EquidistantEpipolarCameraCal<float>, sym::ATANCameraCal<double>,
                   sym::ATANCameraCal<float>) {
  using T = TestType;
  const T& cam_cal = GENERATE(from_range(CamCals<T>::Get()));

  using Scalar = typename T::Scalar;
  const Scalar epsilon = 1e-6;  // For preventing degenerate numerical cases (e.g. division by zero)
  const Scalar tolerance = 10.0 * epsilon;  // For checking approx. equality

  std::cout << "*** Testing Camera class with calibration: " << cam_cal << " ***" << std::endl;

  // Assume the principal point is at the center of the image
  Eigen::Matrix<int, 2, 1> image_size;
  image_size << int(2.0 * cam_cal.Data()[2]), int(2.0 * cam_cal.Data()[3]);

  const sym::Camera<T> cam(cam_cal, image_size);
  Scalar is_valid;

  CHECK(cam.Calibration() == cam_cal);
  CHECK(cam.ImageSize() == image_size);

  CHECK(cam.FocalLength() == cam_cal.FocalLength());
  CHECK(cam.PrincipalPoint() == cam_cal.PrincipalPoint());
  const auto focal_length_expected = cam_cal.Data().template head<2>();
  CHECK(cam.FocalLength() == focal_length_expected);
  const auto principal_point_expected = cam_cal.Data().template segment<2>(2);
  CHECK(cam.PrincipalPoint() == principal_point_expected);

  // Check a pixel that's out of the image
  Eigen::Matrix<Scalar, 2, 1> invalid_pixel;
  invalid_pixel << -1, -1;
  cam.CameraRayFromPixel(invalid_pixel, epsilon, &is_valid);
  CHECK(is_valid == 0);
  CHECK(cam.MaybeCheckInView(invalid_pixel) == 0);
  CHECK(sym::Camera<T>::InView(invalid_pixel, image_size) == 0);

  // Check a point that's at the center of the image
  Eigen::Matrix<Scalar, 2, 1> valid_pixel;
  valid_pixel << image_size[0] / 2.0, image_size[1] / 2.0;
  const Eigen::Matrix<Scalar, 3, 1> valid_camera_point =
      cam.CameraRayFromPixel(valid_pixel, epsilon, &is_valid);
  CHECK(is_valid == 1);
  CHECK(cam.MaybeCheckInView(valid_pixel) == 1);
  CHECK(sym::Camera<T>::InView(valid_pixel, image_size) == 1);

  // Project a point into the camera and check validity
  cam.PixelFromCameraPoint(valid_camera_point, epsilon, &is_valid);
  CHECK(is_valid == 1);
}

TEMPLATE_TEST_CASE("Test PosedCamera class", "[cam_package]", sym::LinearCameraCal<double>,
                   sym::LinearCameraCal<float>, sym::DoubleSphereCameraCal<double>,
                   sym::DoubleSphereCameraCal<float>, sym::EquidistantEpipolarCameraCal<double>,
                   sym::EquidistantEpipolarCameraCal<float>, sym::ATANCameraCal<double>,
                   sym::ATANCameraCal<float>) {
  using T = TestType;
  const T& cam_cal = GENERATE(from_range(CamCals<T>::Get()));

  using Scalar = typename T::Scalar;
  const Scalar epsilon = 1e-6;  // For preventing degenerate numerical cases (e.g. division by zero)
  const Scalar tolerance = 10.0 * epsilon;  // For assessing approximate equality

  std::cout << "*** Testing PosedCamera class with calibration: " << cam_cal << " ***" << std::endl;

  std::mt19937 gen(42);
  // Generate pixels around principal point
  std::uniform_real_distribution<Scalar> pixel_x_dist(0.0, 2.0 * cam_cal.Data()[2]);
  std::uniform_real_distribution<Scalar> pixel_y_dist(0.0, 2.0 * cam_cal.Data()[3]);
  std::uniform_real_distribution<Scalar> range_dist(1.0, 5.0);
  for (int i = 0; i < 10; i++) {
    const sym::Pose3<Scalar> pose = sym::Random<sym::Pose3<Scalar>>(gen);
    const sym::PosedCamera<T> cam(pose, cam_cal);

    CHECK(cam.Pose() == pose);

    Eigen::Matrix<Scalar, 2, 1> pixel;
    pixel << pixel_x_dist(gen), pixel_y_dist(gen);
    const Scalar range_to_point = range_dist(gen);

    Scalar is_valid_global_point;
    Scalar is_valid_pixel;
    const Eigen::Matrix<Scalar, 3, 1> global_point =
        cam.GlobalPointFromPixel(pixel, range_to_point, epsilon, &is_valid_global_point);
    const Eigen::Matrix<Scalar, 2, 1> pixel_reprojected =
        cam.PixelFromGlobalPoint(global_point, epsilon, &is_valid_pixel);
    if (is_valid_global_point == 1 && is_valid_pixel == 1) {
      CHECK(pixel.isApprox(pixel_reprojected, tolerance));
    }

    const sym::Pose3<Scalar> pose2 = sym::Random<sym::Pose3<Scalar>>(gen);
    const sym::PosedCamera<T> cam2(pose2, cam_cal);
    Scalar is_valid_warped_pixel;
    const Scalar inverse_range = 1 / (range_to_point + epsilon);
    const Eigen::Matrix<Scalar, 2, 1> warped_pixel =
        cam.WarpPixel(pixel, inverse_range, cam2, epsilon, &is_valid_warped_pixel);
    const Scalar range2 = (cam2.Pose().Inverse() * global_point).norm();
    const Scalar inverse_range2 = 1 / (range2 + epsilon);
    Scalar is_valid_rewarped_pixel;
    const Eigen::Matrix<Scalar, 2, 1> rewarped_pixel =
        cam2.WarpPixel(warped_pixel, inverse_range2, cam, epsilon, &is_valid_rewarped_pixel);
    if (is_valid_warped_pixel == 1 && is_valid_rewarped_pixel == 1) {
      CHECK(pixel.isApprox(rewarped_pixel, std::sqrt(epsilon)));
    }
  }
}

TEMPLATE_TEST_CASE("Test LinearCameraCal custom methods", "[cam_package]", double, float) {
  using Scalar = TestType;

  const Eigen::Matrix<Scalar, 2, 1> focal_length = Eigen::Matrix<Scalar, 2, 1>(100, 200);
  const Eigen::Matrix<Scalar, 2, 1> principal_point = Eigen::Matrix<Scalar, 2, 1>(300, 400);
  sym::LinearCameraCal<Scalar> cal(focal_length, principal_point);
  CHECK(cal.FocalLength() == focal_length);
  CHECK(cal.PrincipalPoint() == principal_point);
}

TEMPLATE_TEST_CASE("Test SphericalCameraCal custom methods", "[cam_package]", double, float) {
  using Scalar = TestType;

  const Eigen::Matrix<Scalar, 2, 1> focal_length = Eigen::Matrix<Scalar, 2, 1>(100, 200);
  const Eigen::Matrix<Scalar, 2, 1> principal_point = Eigen::Matrix<Scalar, 2, 1>(300, 400);
  const Scalar critical_theta = 0.5;
  const Eigen::Matrix<Scalar, 4, 1> distortion_coeffs =
      Eigen::Matrix<Scalar, 4, 1>(0.1, 0.2, 0.3, 0.4);
  sym::SphericalCameraCal<Scalar> cal(focal_length, principal_point, critical_theta,
                                      distortion_coeffs);
  CHECK(cal.FocalLength() == focal_length);
  CHECK(cal.PrincipalPoint() == principal_point);
  CHECK(cal.Data()[4] == critical_theta);
  CHECK(cal.Data().template tail<4>() == distortion_coeffs);
}

TEMPLATE_TEST_CASE("Test DoubleSphereCameraCal custom methods", "[cam_package]", double, float) {
  using Scalar = TestType;

  const Eigen::Matrix<Scalar, 2, 1> focal_length = Eigen::Matrix<Scalar, 2, 1>(100, 200);
  const Eigen::Matrix<Scalar, 2, 1> principal_point = Eigen::Matrix<Scalar, 2, 1>(300, 400);
  const Scalar xi = 0.1;
  const Scalar alpha = 0.2;
  sym::DoubleSphereCameraCal<Scalar> cal(focal_length, principal_point, xi, alpha);
  CHECK(cal.FocalLength() == focal_length);
  CHECK(cal.PrincipalPoint() == principal_point);
  CHECK(cal.Data()[4] == xi);
  CHECK(cal.Data()[5] == alpha);
}
