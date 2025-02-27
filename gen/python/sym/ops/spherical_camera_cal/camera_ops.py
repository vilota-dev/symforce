# -----------------------------------------------------------------------------
# This file was autogenerated by symforce from template:
#     cam_package/ops/CLASS/camera_ops.py.jinja
# Do NOT modify by hand.
# -----------------------------------------------------------------------------

import math
import typing as T

import numpy

import sym  # pylint: disable=unused-import


class CameraOps(object):
    """
    Python CameraOps implementation for :py:class:`symforce.cam.spherical_camera_cal.SphericalCameraCal`.
    """

    @staticmethod
    def focal_length(self):
        # type: (sym.SphericalCameraCal) -> numpy.ndarray
        """
        Return the focal length.
        """

        # Total ops: 0

        # Input arrays
        _self = self.data

        # Intermediate terms (0)

        # Output terms
        _focal_length = numpy.zeros(2)
        _focal_length[0] = _self[0]
        _focal_length[1] = _self[1]
        return _focal_length

    @staticmethod
    def principal_point(self):
        # type: (sym.SphericalCameraCal) -> numpy.ndarray
        """
        Return the principal point.
        """

        # Total ops: 0

        # Input arrays
        _self = self.data

        # Intermediate terms (0)

        # Output terms
        _principal_point = numpy.zeros(2)
        _principal_point[0] = _self[2]
        _principal_point[1] = _self[3]
        return _principal_point

    @staticmethod
    def pixel_from_camera_point(self, point, epsilon):
        # type: (sym.SphericalCameraCal, numpy.ndarray, float) -> T.Tuple[numpy.ndarray, float]
        """
        Project a 3D point in the camera frame into 2D pixel coordinates.

        Returns:
            pixel: (x, y) coordinate in pixels if valid
            is_valid: 1 if the operation is within bounds else 0
        """

        # Total ops: 30

        # Input arrays
        _self = self.data
        if point.shape == (3,):
            point = point.reshape((3, 1))
        elif point.shape != (3, 1):
            raise IndexError(
                "point is expected to have shape (3, 1) or (3,); instead had shape {}".format(
                    point.shape
                )
            )

        # Intermediate terms (4)
        _tmp0 = math.sqrt(epsilon + point[0, 0] ** 2 + point[1, 0] ** 2)
        _tmp1 = math.atan2(_tmp0, point[2, 0])
        _tmp2 = min(_tmp1, _self[4] - epsilon)
        _tmp3 = (
            _self[5] * _tmp2**3
            + _self[6] * _tmp2**5
            + _self[7] * _tmp2**7
            + _self[8] * _tmp2**9
            + _tmp2
        ) / _tmp0

        # Output terms
        _pixel = numpy.zeros(2)
        _pixel[0] = _self[0] * _tmp3 * point[0, 0] + _self[2]
        _pixel[1] = _self[1] * _tmp3 * point[1, 0] + _self[3]
        _is_valid = max(0, (0.0 if _self[4] - _tmp1 == 0 else math.copysign(1, _self[4] - _tmp1)))
        return _pixel, _is_valid

    @staticmethod
    def pixel_from_camera_point_with_jacobians(self, point, epsilon):
        # type: (sym.SphericalCameraCal, numpy.ndarray, float) -> T.Tuple[numpy.ndarray, float, numpy.ndarray, numpy.ndarray]
        """
        Project a 3D point in the camera frame into 2D pixel coordinates.

        Returns:
            pixel: (x, y) coordinate in pixels if valid
            is_valid: 1 if the operation is within bounds else 0
            pixel_D_cal: Derivative of pixel with respect to intrinsic calibration parameters
            pixel_D_point: Derivative of pixel with respect to point
        """

        # Total ops: 129

        # Input arrays
        _self = self.data
        if point.shape == (3,):
            point = point.reshape((3, 1))
        elif point.shape != (3, 1):
            raise IndexError(
                "point is expected to have shape (3, 1) or (3,); instead had shape {}".format(
                    point.shape
                )
            )

        # Intermediate terms (40)
        _tmp0 = -epsilon
        _tmp1 = point[1, 0] ** 2
        _tmp2 = point[0, 0] ** 2
        _tmp3 = _tmp1 + _tmp2 + epsilon
        _tmp4 = math.sqrt(_tmp3)
        _tmp5 = math.atan2(_tmp4, point[2, 0])
        _tmp6 = min(_tmp5, _self[4] + _tmp0)
        _tmp7 = _tmp6**5
        _tmp8 = _tmp6**7
        _tmp9 = _tmp6**9
        _tmp10 = _tmp6**3
        _tmp11 = _self[5] * _tmp10 + _self[6] * _tmp7 + _self[7] * _tmp8 + _self[8] * _tmp9 + _tmp6
        _tmp12 = 1 / _tmp4
        _tmp13 = _tmp11 * _tmp12
        _tmp14 = _tmp13 * point[0, 0]
        _tmp15 = _tmp13 * point[1, 0]
        _tmp16 = _self[4] - _tmp5
        _tmp17 = _self[0] * point[0, 0]
        _tmp18 = _tmp12 * _tmp17
        _tmp19 = _self[1] * point[1, 0]
        _tmp20 = _tmp12 * _tmp19
        _tmp21 = _tmp12 * _tmp9
        _tmp22 = (9.0 / 2.0) * _self[8] * _tmp6**8
        _tmp23 = ((0.0 if _tmp0 + _tmp16 == 0 else math.copysign(1, _tmp0 + _tmp16)) + 1) / (
            _tmp3 + point[2, 0] ** 2
        )
        _tmp24 = _tmp12 * point[2, 0]
        _tmp25 = _tmp23 * _tmp24
        _tmp26 = _tmp22 * _tmp25
        _tmp27 = (5.0 / 2.0) * _self[6] * _tmp6**4
        _tmp28 = _tmp25 * _tmp27
        _tmp29 = (7.0 / 2.0) * _self[7] * _tmp6**6
        _tmp30 = _tmp25 * _tmp29
        _tmp31 = (3.0 / 2.0) * _self[5] * _tmp6**2
        _tmp32 = _tmp25 * _tmp31
        _tmp33 = (1.0 / 2.0) * _tmp23
        _tmp34 = _tmp24 * _tmp33
        _tmp35 = (
            _tmp26 * point[0, 0]
            + _tmp28 * point[0, 0]
            + _tmp30 * point[0, 0]
            + _tmp32 * point[0, 0]
            + _tmp34 * point[0, 0]
        )
        _tmp36 = _tmp11 / _tmp3 ** (3.0 / 2.0)
        _tmp37 = (
            _tmp26 * point[1, 0]
            + _tmp28 * point[1, 0]
            + _tmp30 * point[1, 0]
            + _tmp32 * point[1, 0]
            + _tmp34 * point[1, 0]
        )
        _tmp38 = _tmp23 * _tmp4
        _tmp39 = _tmp12 * (
            -_tmp22 * _tmp38 - _tmp27 * _tmp38 - _tmp29 * _tmp38 - _tmp31 * _tmp38 - _tmp33 * _tmp4
        )

        # Output terms
        _pixel = numpy.zeros(2)
        _pixel[0] = _self[0] * _tmp14 + _self[2]
        _pixel[1] = _self[1] * _tmp15 + _self[3]
        _is_valid = max(0, (0.0 if _tmp16 == 0 else math.copysign(1, _tmp16)))
        _pixel_D_cal = numpy.zeros((2, 8))
        _pixel_D_cal[0, 0] = _tmp14
        _pixel_D_cal[1, 0] = 0
        _pixel_D_cal[0, 1] = 0
        _pixel_D_cal[1, 1] = _tmp15
        _pixel_D_cal[0, 2] = 1
        _pixel_D_cal[1, 2] = 0
        _pixel_D_cal[0, 3] = 0
        _pixel_D_cal[1, 3] = 1
        _pixel_D_cal[0, 4] = _tmp10 * _tmp18
        _pixel_D_cal[1, 4] = _tmp10 * _tmp20
        _pixel_D_cal[0, 5] = _tmp18 * _tmp7
        _pixel_D_cal[1, 5] = _tmp20 * _tmp7
        _pixel_D_cal[0, 6] = _tmp18 * _tmp8
        _pixel_D_cal[1, 6] = _tmp20 * _tmp8
        _pixel_D_cal[0, 7] = _tmp17 * _tmp21
        _pixel_D_cal[1, 7] = _tmp19 * _tmp21
        _pixel_D_point = numpy.zeros((2, 3))
        _pixel_D_point[0, 0] = _self[0] * _tmp13 - _self[0] * _tmp2 * _tmp36 + _tmp18 * _tmp35
        _pixel_D_point[1, 0] = -_tmp19 * _tmp36 * point[0, 0] + _tmp20 * _tmp35
        _pixel_D_point[0, 1] = -_tmp17 * _tmp36 * point[1, 0] + _tmp18 * _tmp37
        _pixel_D_point[1, 1] = -_self[1] * _tmp1 * _tmp36 + _self[1] * _tmp13 + _tmp20 * _tmp37
        _pixel_D_point[0, 2] = _tmp17 * _tmp39
        _pixel_D_point[1, 2] = _tmp19 * _tmp39
        return _pixel, _is_valid, _pixel_D_cal, _pixel_D_point
