# ----------------------------------------------------------------------------
# SymForce - Copyright 2022, Skydio, Inc.
# This source code is under the Apache 2.0 license found in the LICENSE file.
# ----------------------------------------------------------------------------

"""
Factors generally useful for visual SLAM problems (or visual odometry or structure-from-motion
problems)

Currently contains reprojection factors for all camera models.  For cameras that have
backprojection, the factor operates on pixel coordinates in the source and target cameras.  For
cameras that do not have backprojection, it instead operates on a ray in the source camera frame and
a pixel in the target camera.
"""
import functools
from pathlib import Path

import symforce.symbolic as sf
from symforce import codegen
from symforce import python_util
from symforce import typing as T
from symforce import util
from symforce.opt.noise_models import BarronNoiseModel
from symforce.opt.noise_models import ScalarNoiseModel


def inverse_range_landmark_prior_residual(
    landmark_inverse_range: sf.Scalar,
    inverse_range_prior: sf.Scalar,
    weight: sf.Scalar,
    sigma: sf.Scalar,
    epsilon: sf.Scalar,
) -> sf.Vector1:
    """
    Factor representing a Gaussian prior on the inverse range of a landmark

    Args:
        landmark_inverse_range: The current inverse range estimate
        inverse_range_prior: The mean of the inverse range prior
        weight: The weight of the prior
        sigma: The standard deviation of the prior
        epsilon: Small positive value

    Outputs:
        res: 1dof residual of the prior
    """
    prior_diff = landmark_inverse_range - inverse_range_prior
    prior_whitened_diff = weight * prior_diff / (sigma + epsilon)
    return sf.V1(prior_whitened_diff)


def reprojection_delta(
    source_pose: sf.Pose3,
    source_calibration: sf.CameraCal,
    target_pose: sf.Pose3,
    target_calibration: sf.CameraCal,
    source_inverse_range: sf.Scalar,
    source_pixel: sf.Vector2,
    target_pixel: sf.Vector2,
    epsilon: sf.Scalar,
) -> T.Tuple[sf.Vector2, sf.Scalar]:
    """
    Reprojects the landmark into the target camera and returns the delta from the correspondence to
    the reprojection.

    The landmark is specified as a pixel in the source camera and an inverse range; this means the
    landmark is fixed in the source camera and always has residual 0 there (this 0 residual is not
    returned, only the residual in the target camera is returned).

    Args:
        source_pose: The pose of the source camera
        source_calibration: The source camera calibration
        target_pose: The pose of the target camera
        target_calibration: The target camera calibration
        source_inverse_range: The inverse range of the landmark in the source camera
        source_pixel: The location of the landmark in the source camera
        target_pixel: The location of the correspondence in the target camera
        epsilon: Small positive value
        camera_model_class: The subclass of CameraCal to use as the camera model

    Outputs:
        res: 2dof pixel reprojection error
        valid: is valid projection or not
    """
    # Warp source coords into target
    source_cam = sf.PosedCamera(pose=source_pose, calibration=source_calibration)
    target_cam = sf.PosedCamera(pose=target_pose, calibration=target_calibration)

    target_pixel_warped, warp_is_valid = source_cam.warp_pixel(
        pixel=source_pixel,
        inverse_range=source_inverse_range,
        target_cam=target_cam,
        epsilon=epsilon,
    )

    reprojection_error = target_pixel_warped - target_pixel
    return reprojection_error, warp_is_valid


def inverse_range_landmark_reprojection_error_residual(
    source_pose: sf.Pose3,
    source_calibration: sf.CameraCal,
    target_pose: sf.Pose3,
    target_calibration: sf.CameraCal,
    source_inverse_range: sf.Scalar,
    source_pixel: sf.Vector2,
    target_pixel: sf.Vector2,
    weight: sf.Scalar,
    epsilon: sf.Scalar,
    noise_model: ScalarNoiseModel,
) -> sf.Vector2:
    """
    Return the 2dof residual of reprojecting the landmark into the target camera and comparing
    against the correspondence in the target camera.

    The landmark is specified as a pixel in the source camera and an inverse range; this means the
    landmark is fixed in the source camera and always has residual 0 there (this 0 residual is not
    returned, only the residual in the target camera is returned).

    The norm of the residual is whitened using a
    :class:`ScalarNoiseModel <symforce.opt.noise_models.ScalarNoiseModel>`.  Whitening each
    component of the reprojection error separately would result in rejecting individual components
    as outliers. Instead, we minimize the whitened norm of the full reprojection error for each
    point.  See
    :meth:`ScalarNoiseModel.whiten_norm <symforce.opt.noise_models.ScalarNoiseModel.whiten_norm>`
    for more information on this.

    Args:
        source_pose: The pose of the source camera
        source_calibration: The source camera calibration
        target_pose: The pose of the target camera
        target_calibration: The target camera calibration
        source_inverse_range: The inverse range of the landmark in the source camera
        source_pixel: The location of the landmark in the source camera
        target_pixel: The location of the correspondence in the target camera
        weight: The weight of the factor
        epsilon: Small positive value
        noise_model: The noise model to use for whitening the residual

    Outputs:
        res: 2dof residual of the reprojection
    """
    reprojection_error, warp_is_valid = reprojection_delta(
        source_pose,
        source_calibration,
        target_pose,
        target_calibration,
        source_inverse_range,
        source_pixel,
        target_pixel,
        epsilon,
    )

    whitened_residual = (
        warp_is_valid * sf.sqrt(weight) * noise_model.whiten_norm(reprojection_error, epsilon)
    )

    return whitened_residual


def inverse_range_landmark_gnc_residual(
    source_pose: sf.Pose3,
    source_calibration: sf.CameraCal,
    target_pose: sf.Pose3,
    target_calibration: sf.CameraCal,
    source_inverse_range: sf.Scalar,
    source_pixel: sf.Vector2,
    target_pixel: sf.Vector2,
    weight: sf.Scalar,
    gnc_mu: sf.Scalar,
    gnc_scale: sf.Scalar,
    epsilon: sf.Scalar,
) -> sf.Vector2:
    """
    Return the 2dof residual of reprojecting the landmark into the target camera and comparing
    against the correspondence in the target camera.

    The landmark is specified as a pixel in the source camera and an inverse range; this means the
    landmark is fixed in the source camera and always has residual 0 there (this 0 residual is not
    returned, only the residual in the target camera is returned).

    The norm of the residual is whitened using the
    :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`.  Whitening each
    component of the reprojection error separately would result in rejecting individual components
    as outliers. Instead, we minimize the whitened norm of the full reprojection error for each
    point.  See
    :meth:`ScalarNoiseModel.whiten_norm <symforce.opt.noise_models.ScalarNoiseModel.whiten_norm>`
    for more information on this, and
    :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>` for more information on
    the noise model.

    Args:
        source_pose: The pose of the source camera
        source_calibration: The source camera calibration
        target_pose: The pose of the target camera
        target_calibration: The target camera calibration
        source_inverse_range: The inverse range of the landmark in the source camera
        source_pixel: The location of the landmark in the source camera
        target_pixel: The location of the correspondence in the target camera
        weight: The weight of the factor
        gnc_mu: The mu convexity parameter for the
            :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`
        gnc_scale: The scale parameter for the
            :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`
        epsilon: Small positive value

    Outputs:
        res: 2dof residual of the reprojection
    """
    noise_model = BarronNoiseModel(
        alpha=BarronNoiseModel.compute_alpha_from_mu(gnc_mu, epsilon),
        scalar_information=1 / gnc_scale**2,
        x_epsilon=epsilon,
    )

    return inverse_range_landmark_reprojection_error_residual(
        source_pose,
        source_calibration,
        target_pose,
        target_calibration,
        source_inverse_range,
        source_pixel,
        target_pixel,
        weight,
        epsilon,
        noise_model,
    )


def ray_reprojection_delta(
    source_pose: sf.Pose3,
    target_pose: sf.Pose3,
    target_calibration: sf.CameraCal,
    source_inverse_range: sf.Scalar,
    p_camera_source: sf.Vector3,
    target_pixel: sf.Vector2,
    epsilon: sf.Scalar,
) -> T.Tuple[sf.Vector2, sf.Scalar]:
    """
    Reprojects the landmark ray into the target camera and returns the delta between the
    correspondence and the reprojection.

    The landmark is specified as a 3D point or ray (will be normalized) in the source
    camera; this means the landmark is fixed in the source camera and always has residual 0 there
    (this 0 residual is not returned, only the residual in the target camera is returned).

    Args:
        source_pose: The pose of the source camera
        target_pose: The pose of the target camera
        target_calibration: The target camera calibration
        source_inverse_range: The inverse range of the landmark in the source camera
        p_camera_source: The location of the landmark in the source camera coordinate, will be
            normalized
        target_pixel: The location of the correspondence in the target camera
        epsilon: Small positive value

    Outputs:
        res: 2dof reprojection delta
        valid: is valid projection or not
    """
    nav_T_target_cam = target_pose
    nav_T_source_cam = source_pose
    p_camera_source_unit_ray = p_camera_source / p_camera_source.norm(epsilon)

    p_cam_target = nav_T_target_cam.R.inverse() * (
        nav_T_source_cam.R * p_camera_source_unit_ray
        + (nav_T_source_cam.t - nav_T_target_cam.t) * source_inverse_range
    )

    target_pixel_reprojection, is_valid = target_calibration.pixel_from_camera_point(
        p_cam_target, epsilon=epsilon
    )
    reprojection_error = target_pixel_reprojection - target_pixel

    return reprojection_error, is_valid


def inverse_range_landmark_ray_reprojection_error_residual(
    source_pose: sf.Pose3,
    target_pose: sf.Pose3,
    target_calibration: sf.CameraCal,
    source_inverse_range: sf.Scalar,
    p_camera_source: sf.Vector3,
    target_pixel: sf.Vector2,
    weight: sf.Scalar,
    epsilon: sf.Scalar,
    noise_model: ScalarNoiseModel,
) -> sf.Vector2:
    """
    Return the 2dof residual of reprojecting the landmark ray into the target spherical camera and
    comparing it against the correspondence.

    The landmark is specified as a camera point in the source camera with an inverse range; this
    means the landmark is fixed in the source camera and always has residual 0 there (this 0
    residual is not returned, only the residual in the target camera is returned).

    The norm of the residual is whitened using a
    :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`.  Whitening each
    component of the reprojection error separately would result in rejecting individual components
    as outliers. Instead, we minimize the whitened norm of the full reprojection error for each
    point.  See
    :meth:`ScalarNoiseModel.whiten_norm <symforce.opt.noise_models.ScalarNoiseModel.whiten_norm>`
    for more information on this, and
    :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>` for more information on
    the noise model.

    Args:
        source_pose: The pose of the source camera
        target_pose: The pose of the target camera
        target_calibration: The target spherical camera calibration
        source_inverse_range: The inverse range of the landmark in the source camera
        p_camera_source: The location of the landmark in the source camera coordinate, will be
            normalized
        target_pixel: The location of the correspondence in the target camera
        weight: The weight of the factor
        gnc_mu: The mu convexity parameter for the
            :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`
        gnc_scale: The scale parameter for the
            :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`
        epsilon: Small positive value

    Outputs:
        res: 2dof whiten residual of the reprojection
    """
    reprojection_error, is_valid = ray_reprojection_delta(
        source_pose,
        target_pose,
        target_calibration,
        source_inverse_range,
        p_camera_source,
        target_pixel,
        epsilon,
    )

    whitened_residual = (
        is_valid * sf.sqrt(weight) * noise_model.whiten_norm(reprojection_error, epsilon)
    )

    return whitened_residual


def inverse_range_landmark_ray_gnc_residual(
    source_pose: sf.Pose3,
    target_pose: sf.Pose3,
    target_calibration: sf.CameraCal,
    source_inverse_range: sf.Scalar,
    p_camera_source: sf.Vector3,
    target_pixel: sf.Vector2,
    weight: sf.Scalar,
    gnc_mu: sf.Scalar,
    gnc_scale: sf.Scalar,
    epsilon: sf.Scalar,
) -> sf.Vector2:
    """
    Return the 2dof residual of reprojecting the landmark ray into the target spherical camera and
    comparing it against the correspondence.

    The landmark is specified as a camera point in the source camera with an inverse range; this
    means the landmark is fixed in the source camera and always has residual 0 there (this 0
    residual is not returned, only the residual in the target camera is returned).

    The norm of the residual is whitened using the
    :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`.  Whitening each
    component of the reprojection error separately would result in rejecting individual components
    as outliers. Instead, we minimize the whitened norm of the full reprojection error for each
    point.  See
    :meth:`ScalarNoiseModel.whiten_norm <symforce.opt.noise_models.ScalarNoiseModel.whiten_norm>`
    for more information on this, and
    :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>` for more information on
    the noise model.

    Args:
        source_pose: The pose of the source camera
        target_pose: The pose of the target camera
        target_calibration: The target spherical camera calibration
        source_inverse_range: The inverse range of the landmark in the source camera
        p_camera_source: The location of the landmark in the source camera coordinate, will be
            normalized
        target_pixel: The location of the correspondence in the target camera
        weight: The weight of the factor
        gnc_mu: The mu convexity parameter for the
            :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`
        gnc_scale: The scale parameter for the
            :class:`BarronNoiseModel <symforce.opt.noise_models.BarronNoiseModel>`
        epsilon: Small positive value

    Outputs:
        res: 2dof whiten residual of the reprojection
    """

    noise_model = BarronNoiseModel(
        alpha=BarronNoiseModel.compute_alpha_from_mu(mu=gnc_mu, epsilon=epsilon),
        scalar_information=1 / gnc_scale**2,
        x_epsilon=epsilon,
    )

    return inverse_range_landmark_ray_reprojection_error_residual(
        source_pose,
        target_pose,
        target_calibration,
        source_inverse_range,
        p_camera_source,
        target_pixel,
        weight,
        epsilon,
        noise_model,
    )


def generate(output_dir: Path, config: T.Optional[codegen.CodegenConfig] = None) -> None:
    """
    Generate the SLAM package for the given language.

    Args:
        output_dir: Directory to generate outputs into
        config: CodegenConfig, defaults to the default C++ config
    """
    # Subdirectory for everything we'll generate
    factors_dir = output_dir / "factors"

    # default config to CppConfig
    if config is None:
        config = codegen.CppConfig()

    # Default cam types to generate
    cam_types = sf.CameraCal.__subclasses__()

    codegen.Codegen.function(
        func=inverse_range_landmark_prior_residual, config=config
    ).with_linearization(which_args=["landmark_inverse_range"]).generate_function(
        output_dir=factors_dir, skip_directory_nesting=True
    )

    for cam_type in cam_types:
        cam_type_name = python_util.camelcase_to_snakecase(
            python_util.str_removesuffix(cam_type.__name__, "CameraCal")
        )

        specialize_cam = functools.partial(
            util.specialize_types, type_replacements={sf.CameraCal: cam_type}
        )

        try:
            codegen.Codegen.function(
                func=specialize_cam(inverse_range_landmark_gnc_residual),
                name=f"inverse_range_landmark_{cam_type_name}_gnc_residual",
                config=config,
            ).with_linearization(
                which_args=["source_pose", "target_pose", "source_inverse_range"]
            ).generate_function(output_dir=factors_dir, skip_directory_nesting=True)

            codegen.Codegen.function(
                func=specialize_cam(reprojection_delta),
                name=f"{cam_type_name}_reprojection_delta",
                config=config,
                output_names=["reprojection_delta", "is_valid"],
            ).generate_function(output_dir=factors_dir, skip_directory_nesting=True)

        except NotImplementedError:
            # Not all cameras implement backprojection
            codegen.Codegen.function(
                func=specialize_cam(inverse_range_landmark_ray_gnc_residual),
                name=f"inverse_range_landmark_{cam_type_name}_gnc_residual",
                config=config,
            ).with_linearization(
                which_args=["source_pose", "target_pose", "source_inverse_range"]
            ).generate_function(output_dir=factors_dir, skip_directory_nesting=True)

            codegen.Codegen.function(
                func=specialize_cam(ray_reprojection_delta),
                name=f"{cam_type_name}_reprojection_delta",
                config=config,
                output_names=["reprojection_delta", "is_valid"],
            ).generate_function(output_dir=factors_dir, skip_directory_nesting=True)
