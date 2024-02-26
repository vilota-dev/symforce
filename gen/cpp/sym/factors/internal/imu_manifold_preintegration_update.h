// -----------------------------------------------------------------------------
// This file was autogenerated by symforce from template:
//     function/FUNCTION.h.jinja
// Do NOT modify by hand.
// -----------------------------------------------------------------------------

#pragma once

#include <Eigen/Core>

#include <sym/rot3.h>

namespace sym {

/**
 * An internal helper function to update a set of preintegrated IMU measurements with a new pair of
 * gyroscope and accelerometer measurements. Returns the updated preintegrated measurements.
 *
 * NOTE: If you are interested in this function, you should likely be using the
 * ``IntegrateMeasurement`` method of the ``PreintegratedImuMeasurements`` class located in
 * ``symforce/slam/imu_preintegration/preintegrated_imu_measurements.h``.
 *
 * When integrating the first measurement, DR should be the identity, Dv, Dp, covariance, and the
 * derivatives w.r.t. to bias should all be 0.
 *
 * Args:
 *     DR: Preintegrated change in orientation
 *     Dv: Preintegrated change in velocity
 *     Dp: Preintegrated change in position
 *     covariance: Covariance [DR, Dv, Dp] in local coordinates of mean
 *     DR_D_gyro_bias: Derivative of DR w.r.t. gyro_bias
 *     Dv_D_accel_bias: Derivative of Dv w.r.t. accel_bias
 *     Dv_D_gyro_bias: Derivative of Dv w.r.t. gyro_bias
 *     Dp_D_accel_bias: Derivative of Dp w.r.t. accel_bias
 *     Dp_D_gyro_bias: Derivative of Dp w.r.t. gyro_bias
 *     accel_bias: Initial guess for the accelerometer measurement bias
 *     gyro_bias: Initial guess for the gyroscope measurement bias
 *     accel_cov_diagonal: The diagonal of the covariance of the accelerometer measurement
 *     gyro_cov_diagonal: The diagonal of the covariance of the gyroscope measurement
 *     accel_measurement: The accelerometer measurement
 *     gyro_measurement: The gyroscope measurement
 *     dt: The time between the new measurements and the last
 *     epsilon: epsilon for numerical stability
 *
 * Returns:
 *     T.Tuple[sf.Rot3, sf.V3, sf.V3, sf.M99, sf.M33, sf.M33, sf.M33, sf.M33, sf.M33]:
 *         new_DR,
 *         new_Dv,
 *         new_Dp,
 *         new_covariance,
 *         new_DR_D_gyro_bias,
 *         new_Dv_D_accel_bias,
 *         new_Dv_D_gyro_bias,
 *         new_Dp_D_accel_bias
 *         new_Dp_D_gyro_bias,
 */
template <typename Scalar>
void ImuManifoldPreintegrationUpdate(
    const sym::Rot3<Scalar>& DR, const Eigen::Matrix<Scalar, 3, 1>& Dv,
    const Eigen::Matrix<Scalar, 3, 1>& Dp, const Eigen::Matrix<Scalar, 9, 9>& covariance,
    const Eigen::Matrix<Scalar, 3, 3>& DR_D_gyro_bias,
    const Eigen::Matrix<Scalar, 3, 3>& Dv_D_accel_bias,
    const Eigen::Matrix<Scalar, 3, 3>& Dv_D_gyro_bias,
    const Eigen::Matrix<Scalar, 3, 3>& Dp_D_accel_bias,
    const Eigen::Matrix<Scalar, 3, 3>& Dp_D_gyro_bias,
    const Eigen::Matrix<Scalar, 3, 1>& accel_bias, const Eigen::Matrix<Scalar, 3, 1>& gyro_bias,
    const Eigen::Matrix<Scalar, 3, 1>& accel_cov_diagonal,
    const Eigen::Matrix<Scalar, 3, 1>& gyro_cov_diagonal,
    const Eigen::Matrix<Scalar, 3, 1>& accel_measurement,
    const Eigen::Matrix<Scalar, 3, 1>& gyro_measurement, const Scalar dt, const Scalar epsilon,
    sym::Rot3<Scalar>* const new_DR = nullptr, Eigen::Matrix<Scalar, 3, 1>* const new_Dv = nullptr,
    Eigen::Matrix<Scalar, 3, 1>* const new_Dp = nullptr,
    Eigen::Matrix<Scalar, 9, 9>* const new_covariance = nullptr,
    Eigen::Matrix<Scalar, 3, 3>* const new_DR_D_gyro_bias = nullptr,
    Eigen::Matrix<Scalar, 3, 3>* const new_Dv_D_accel_bias = nullptr,
    Eigen::Matrix<Scalar, 3, 3>* const new_Dv_D_gyro_bias = nullptr,
    Eigen::Matrix<Scalar, 3, 3>* const new_Dp_D_accel_bias = nullptr,
    Eigen::Matrix<Scalar, 3, 3>* const new_Dp_D_gyro_bias = nullptr) {
  // Total ops: 1257

  // Input arrays
  const Eigen::Matrix<Scalar, 4, 1>& _DR = DR.Data();

  // Intermediate terms (339)
  const Scalar _tmp0 = std::pow(dt, Scalar(2));
  const Scalar _tmp1 = -gyro_bias(2, 0) + gyro_measurement(2, 0);
  const Scalar _tmp2 = _tmp0 * std::pow(_tmp1, Scalar(2));
  const Scalar _tmp3 = -gyro_bias(1, 0) + gyro_measurement(1, 0);
  const Scalar _tmp4 = _tmp0 * std::pow(_tmp3, Scalar(2));
  const Scalar _tmp5 = -gyro_bias(0, 0) + gyro_measurement(0, 0);
  const Scalar _tmp6 = _tmp0 * std::pow(_tmp5, Scalar(2));
  const Scalar _tmp7 = _tmp2 + _tmp4 + _tmp6;
  const Scalar _tmp8 = _tmp7 + std::pow(epsilon, Scalar(2));
  const Scalar _tmp9 = std::sqrt(_tmp8);
  const Scalar _tmp10 = (Scalar(1) / Scalar(2)) * _tmp9;
  const Scalar _tmp11 = std::sin(_tmp10);
  const Scalar _tmp12 = _tmp11 / _tmp9;
  const Scalar _tmp13 = _DR[3] * _tmp12;
  const Scalar _tmp14 = _tmp5 * dt;
  const Scalar _tmp15 = _tmp1 * dt;
  const Scalar _tmp16 = _DR[1] * _tmp12;
  const Scalar _tmp17 = _tmp3 * dt;
  const Scalar _tmp18 = _DR[2] * _tmp12;
  const Scalar _tmp19 = std::cos(_tmp10);
  const Scalar _tmp20 = _DR[0] * _tmp12;
  const Scalar _tmp21 = -2 * std::pow(_DR[2], Scalar(2));
  const Scalar _tmp22 = 1 - 2 * std::pow(_DR[1], Scalar(2));
  const Scalar _tmp23 = _tmp21 + _tmp22;
  const Scalar _tmp24 = -accel_bias(0, 0) + accel_measurement(0, 0);
  const Scalar _tmp25 = 2 * _DR[1];
  const Scalar _tmp26 = _DR[0] * _tmp25;
  const Scalar _tmp27 = 2 * _DR[2] * _DR[3];
  const Scalar _tmp28 = _tmp26 - _tmp27;
  const Scalar _tmp29 = -accel_bias(1, 0) + accel_measurement(1, 0);
  const Scalar _tmp30 = _DR[3] * _tmp25;
  const Scalar _tmp31 = 2 * _DR[0];
  const Scalar _tmp32 = _DR[2] * _tmp31;
  const Scalar _tmp33 = _tmp30 + _tmp32;
  const Scalar _tmp34 = -accel_bias(2, 0) + accel_measurement(2, 0);
  const Scalar _tmp35 = _tmp23 * _tmp24 + _tmp28 * _tmp29 + _tmp33 * _tmp34;
  const Scalar _tmp36 = -2 * std::pow(_DR[0], Scalar(2));
  const Scalar _tmp37 = _tmp21 + _tmp36 + 1;
  const Scalar _tmp38 = _tmp26 + _tmp27;
  const Scalar _tmp39 = _DR[3] * _tmp31;
  const Scalar _tmp40 = _DR[2] * _tmp25;
  const Scalar _tmp41 = -_tmp39 + _tmp40;
  const Scalar _tmp42 = _tmp24 * _tmp38 + _tmp29 * _tmp37 + _tmp34 * _tmp41;
  const Scalar _tmp43 = _tmp22 + _tmp36;
  const Scalar _tmp44 = -_tmp30 + _tmp32;
  const Scalar _tmp45 = _tmp39 + _tmp40;
  const Scalar _tmp46 = _tmp24 * _tmp44 + _tmp29 * _tmp45 + _tmp34 * _tmp43;
  const Scalar _tmp47 = (Scalar(1) / Scalar(2)) * _tmp0;
  const Scalar _tmp48 = 2 * std::pow(_tmp11, Scalar(2)) / _tmp8;
  const Scalar _tmp49 = -_tmp2 * _tmp48;
  const Scalar _tmp50 = -_tmp4 * _tmp48 + 1;
  const Scalar _tmp51 = _tmp49 + _tmp50;
  const Scalar _tmp52 = 2 * _tmp12 * _tmp19;
  const Scalar _tmp53 = _tmp17 * _tmp52;
  const Scalar _tmp54 = _tmp0 * _tmp5;
  const Scalar _tmp55 = _tmp48 * _tmp54;
  const Scalar _tmp56 = _tmp1 * _tmp55;
  const Scalar _tmp57 = -_tmp53 + _tmp56;
  const Scalar _tmp58 = _tmp15 * _tmp52;
  const Scalar _tmp59 = _tmp3 * _tmp55;
  const Scalar _tmp60 = _tmp58 + _tmp59;
  const Scalar _tmp61 = -_tmp2;
  const Scalar _tmp62 = -_tmp4;
  const Scalar _tmp63 = _tmp7 + std::sqrt(epsilon);
  const Scalar _tmp64 = std::sqrt(_tmp63);
  const Scalar _tmp65 = (_tmp64 - std::sin(_tmp64)) / (_tmp63 * std::sqrt(_tmp63));
  const Scalar _tmp66 = _tmp65 * (_tmp61 + _tmp62) + 1;
  const Scalar _tmp67 = dt * gyro_cov_diagonal(0, 0);
  const Scalar _tmp68 = _tmp54 * _tmp65;
  const Scalar _tmp69 = _tmp1 * _tmp68;
  const Scalar _tmp70 = (1 - std::cos(_tmp64)) / _tmp63;
  const Scalar _tmp71 = _tmp17 * _tmp70;
  const Scalar _tmp72 = _tmp69 - _tmp71;
  const Scalar _tmp73 = dt * gyro_cov_diagonal(2, 0);
  const Scalar _tmp74 = _tmp3 * _tmp68;
  const Scalar _tmp75 = _tmp70 * dt;
  const Scalar _tmp76 = _tmp1 * _tmp75;
  const Scalar _tmp77 = _tmp74 + _tmp76;
  const Scalar _tmp78 = dt * gyro_cov_diagonal(1, 0);
  const Scalar _tmp79 = _tmp0 * _tmp1 * _tmp3;
  const Scalar _tmp80 = _tmp65 * _tmp79;
  const Scalar _tmp81 = _tmp5 * _tmp75;
  const Scalar _tmp82 = _tmp80 + _tmp81;
  const Scalar _tmp83 = _tmp72 * dt;
  const Scalar _tmp84 = -_tmp48 * _tmp6;
  const Scalar _tmp85 = _tmp49 + _tmp84 + 1;
  const Scalar _tmp86 = _tmp14 * _tmp52;
  const Scalar _tmp87 = _tmp48 * _tmp79;
  const Scalar _tmp88 = _tmp86 + _tmp87;
  const Scalar _tmp89 = -_tmp58 + _tmp59;
  const Scalar _tmp90 =
      _tmp85 * covariance(1, 0) + _tmp88 * covariance(2, 0) + _tmp89 * covariance(0, 0);
  const Scalar _tmp91 =
      _tmp85 * covariance(2, 1) + _tmp88 * covariance(2, 2) + _tmp89 * covariance(2, 0);
  const Scalar _tmp92 =
      _tmp85 * covariance(1, 1) + _tmp88 * covariance(2, 1) + _tmp89 * covariance(1, 0);
  const Scalar _tmp93 = -_tmp6;
  const Scalar _tmp94 = _tmp65 * (_tmp61 + _tmp93) + 1;
  const Scalar _tmp95 = _tmp94 * dt;
  const Scalar _tmp96 = _tmp95 * gyro_cov_diagonal(1, 0);
  const Scalar _tmp97 = _tmp74 - _tmp76;
  const Scalar _tmp98 = _tmp66 * dt;
  const Scalar _tmp99 = _tmp98 * gyro_cov_diagonal(0, 0);
  const Scalar _tmp100 = _tmp51 * _tmp90 + _tmp57 * _tmp91 + _tmp60 * _tmp92 + _tmp77 * _tmp96 +
                         _tmp82 * _tmp83 * gyro_cov_diagonal(2, 0) + _tmp97 * _tmp99;
  const Scalar _tmp101 = _tmp65 * (_tmp62 + _tmp93) + 1;
  const Scalar _tmp102 = _tmp101 * gyro_cov_diagonal(2, 0);
  const Scalar _tmp103 = _tmp50 + _tmp84;
  const Scalar _tmp104 = -_tmp86 + _tmp87;
  const Scalar _tmp105 = _tmp53 + _tmp56;
  const Scalar _tmp106 =
      _tmp103 * covariance(2, 0) + _tmp104 * covariance(1, 0) + _tmp105 * covariance(0, 0);
  const Scalar _tmp107 = _tmp69 + _tmp71;
  const Scalar _tmp108 =
      _tmp103 * covariance(2, 1) + _tmp104 * covariance(1, 1) + _tmp105 * covariance(1, 0);
  const Scalar _tmp109 = _tmp80 - _tmp81;
  const Scalar _tmp110 = _tmp77 * dt;
  const Scalar _tmp111 =
      _tmp103 * covariance(2, 2) + _tmp104 * covariance(2, 1) + _tmp105 * covariance(2, 0);
  const Scalar _tmp112 = _tmp102 * _tmp83 + _tmp106 * _tmp51 + _tmp107 * _tmp99 + _tmp108 * _tmp60 +
                         _tmp109 * _tmp110 * gyro_cov_diagonal(1, 0) + _tmp111 * _tmp57;
  const Scalar _tmp113 = _tmp33 * dt;
  const Scalar _tmp114 = _tmp28 * dt;
  const Scalar _tmp115 = _tmp113 * _tmp29 - _tmp114 * _tmp34;
  const Scalar _tmp116 = _tmp115 * covariance(2, 0);
  const Scalar _tmp117 = _tmp23 * dt;
  const Scalar _tmp118 = -_tmp113 * _tmp24 + _tmp117 * _tmp34;
  const Scalar _tmp119 = _tmp118 * covariance(2, 1);
  const Scalar _tmp120 = _tmp114 * _tmp24 - _tmp117 * _tmp29;
  const Scalar _tmp121 = _tmp120 * covariance(2, 2);
  const Scalar _tmp122 = _tmp116 + _tmp119 + _tmp121 + covariance(3, 2);
  const Scalar _tmp123 = _tmp115 * covariance(0, 0);
  const Scalar _tmp124 = _tmp118 * covariance(1, 0);
  const Scalar _tmp125 = _tmp120 * covariance(2, 0);
  const Scalar _tmp126 = _tmp123 + _tmp124 + _tmp125 + covariance(3, 0);
  const Scalar _tmp127 = _tmp115 * covariance(1, 0);
  const Scalar _tmp128 = _tmp118 * covariance(1, 1);
  const Scalar _tmp129 = _tmp120 * covariance(2, 1);
  const Scalar _tmp130 = _tmp127 + _tmp128 + _tmp129 + covariance(3, 1);
  const Scalar _tmp131 = _tmp122 * _tmp57 + _tmp126 * _tmp51 + _tmp130 * _tmp60;
  const Scalar _tmp132 = _tmp41 * dt;
  const Scalar _tmp133 = _tmp38 * dt;
  const Scalar _tmp134 = -_tmp132 * _tmp24 + _tmp133 * _tmp34;
  const Scalar _tmp135 = _tmp134 * covariance(1, 0);
  const Scalar _tmp136 = _tmp37 * dt;
  const Scalar _tmp137 = _tmp132 * _tmp29 - _tmp136 * _tmp34;
  const Scalar _tmp138 = _tmp137 * covariance(0, 0);
  const Scalar _tmp139 = -_tmp133 * _tmp29 + _tmp136 * _tmp24;
  const Scalar _tmp140 = _tmp139 * covariance(2, 0);
  const Scalar _tmp141 = _tmp135 + _tmp138 + _tmp140 + covariance(4, 0);
  const Scalar _tmp142 = _tmp134 * covariance(2, 1);
  const Scalar _tmp143 = _tmp137 * covariance(2, 0);
  const Scalar _tmp144 = _tmp139 * covariance(2, 2);
  const Scalar _tmp145 = _tmp142 + _tmp143 + _tmp144 + covariance(4, 2);
  const Scalar _tmp146 = _tmp134 * covariance(1, 1);
  const Scalar _tmp147 = _tmp137 * covariance(1, 0);
  const Scalar _tmp148 = _tmp139 * covariance(2, 1);
  const Scalar _tmp149 = _tmp146 + _tmp147 + _tmp148 + covariance(4, 1);
  const Scalar _tmp150 = _tmp141 * _tmp51 + _tmp145 * _tmp57 + _tmp149 * _tmp60;
  const Scalar _tmp151 = _tmp45 * dt;
  const Scalar _tmp152 = _tmp43 * dt;
  const Scalar _tmp153 = -_tmp151 * _tmp34 + _tmp152 * _tmp29;
  const Scalar _tmp154 = _tmp153 * covariance(1, 0);
  const Scalar _tmp155 = _tmp44 * dt;
  const Scalar _tmp156 = -_tmp152 * _tmp24 + _tmp155 * _tmp34;
  const Scalar _tmp157 = _tmp156 * covariance(1, 1);
  const Scalar _tmp158 = _tmp151 * _tmp24 - _tmp155 * _tmp29;
  const Scalar _tmp159 = _tmp158 * covariance(2, 1);
  const Scalar _tmp160 = _tmp154 + _tmp157 + _tmp159 + covariance(5, 1);
  const Scalar _tmp161 = _tmp153 * covariance(2, 0);
  const Scalar _tmp162 = _tmp156 * covariance(2, 1);
  const Scalar _tmp163 = _tmp158 * covariance(2, 2);
  const Scalar _tmp164 = _tmp161 + _tmp162 + _tmp163 + covariance(5, 2);
  const Scalar _tmp165 = _tmp153 * covariance(0, 0);
  const Scalar _tmp166 = _tmp156 * covariance(1, 0);
  const Scalar _tmp167 = _tmp158 * covariance(2, 0);
  const Scalar _tmp168 = _tmp165 + _tmp166 + _tmp167 + covariance(5, 0);
  const Scalar _tmp169 = _tmp160 * _tmp60 + _tmp164 * _tmp57 + _tmp168 * _tmp51;
  const Scalar _tmp170 = (Scalar(1) / Scalar(2)) * dt;
  const Scalar _tmp171 = covariance(3, 1) * dt;
  const Scalar _tmp172 =
      _tmp127 * _tmp170 + _tmp128 * _tmp170 + _tmp129 * _tmp170 + _tmp171 + covariance(6, 1);
  const Scalar _tmp173 = _tmp116 * _tmp170 + _tmp119 * _tmp170 + _tmp121 * _tmp170 +
                         covariance(3, 2) * dt + covariance(6, 2);
  const Scalar _tmp174 = covariance(3, 0) * dt;
  const Scalar _tmp175 =
      _tmp123 * _tmp170 + _tmp124 * _tmp170 + _tmp125 * _tmp170 + _tmp174 + covariance(6, 0);
  const Scalar _tmp176 = _tmp172 * _tmp60 + _tmp173 * _tmp57 + _tmp175 * _tmp51;
  const Scalar _tmp177 = covariance(4, 2) * dt;
  const Scalar _tmp178 =
      _tmp142 * _tmp170 + _tmp143 * _tmp170 + _tmp144 * _tmp170 + _tmp177 + covariance(7, 2);
  const Scalar _tmp179 = covariance(4, 1) * dt;
  const Scalar _tmp180 =
      _tmp146 * _tmp170 + _tmp147 * _tmp170 + _tmp148 * _tmp170 + _tmp179 + covariance(7, 1);
  const Scalar _tmp181 = covariance(4, 0) * dt;
  const Scalar _tmp182 =
      _tmp135 * _tmp170 + _tmp138 * _tmp170 + _tmp140 * _tmp170 + _tmp181 + covariance(7, 0);
  const Scalar _tmp183 = _tmp178 * _tmp57 + _tmp180 * _tmp60 + _tmp182 * _tmp51;
  const Scalar _tmp184 = covariance(5, 1) * dt;
  const Scalar _tmp185 =
      _tmp154 * _tmp170 + _tmp157 * _tmp170 + _tmp159 * _tmp170 + _tmp184 + covariance(8, 1);
  const Scalar _tmp186 = covariance(5, 2) * dt;
  const Scalar _tmp187 =
      _tmp161 * _tmp170 + _tmp162 * _tmp170 + _tmp163 * _tmp170 + _tmp186 + covariance(8, 2);
  const Scalar _tmp188 = covariance(5, 0) * dt;
  const Scalar _tmp189 =
      _tmp165 * _tmp170 + _tmp166 * _tmp170 + _tmp167 * _tmp170 + _tmp188 + covariance(8, 0);
  const Scalar _tmp190 = _tmp185 * _tmp60 + _tmp187 * _tmp57 + _tmp189 * _tmp51;
  const Scalar _tmp191 = _tmp82 * dt;
  const Scalar _tmp192 = _tmp107 * dt;
  const Scalar _tmp193 = _tmp102 * _tmp191 + _tmp106 * _tmp89 + _tmp108 * _tmp85 +
                         _tmp109 * _tmp96 + _tmp111 * _tmp88 +
                         _tmp192 * _tmp97 * gyro_cov_diagonal(0, 0);
  const Scalar _tmp194 = _tmp122 * _tmp88 + _tmp126 * _tmp89 + _tmp130 * _tmp85;
  const Scalar _tmp195 = _tmp141 * _tmp89 + _tmp145 * _tmp88 + _tmp149 * _tmp85;
  const Scalar _tmp196 = _tmp160 * _tmp85 + _tmp164 * _tmp88 + _tmp168 * _tmp89;
  const Scalar _tmp197 = _tmp172 * _tmp85 + _tmp173 * _tmp88 + _tmp175 * _tmp89;
  const Scalar _tmp198 = _tmp178 * _tmp88 + _tmp180 * _tmp85 + _tmp182 * _tmp89;
  const Scalar _tmp199 = _tmp185 * _tmp85 + _tmp187 * _tmp88 + _tmp189 * _tmp89;
  const Scalar _tmp200 = _tmp103 * _tmp122 + _tmp104 * _tmp130 + _tmp105 * _tmp126;
  const Scalar _tmp201 = _tmp103 * _tmp145 + _tmp104 * _tmp149 + _tmp105 * _tmp141;
  const Scalar _tmp202 = _tmp103 * _tmp164 + _tmp104 * _tmp160 + _tmp105 * _tmp168;
  const Scalar _tmp203 = _tmp103 * _tmp173 + _tmp104 * _tmp172 + _tmp105 * _tmp175;
  const Scalar _tmp204 = _tmp103 * _tmp178 + _tmp104 * _tmp180 + _tmp105 * _tmp182;
  const Scalar _tmp205 = _tmp103 * _tmp187 + _tmp104 * _tmp185 + _tmp105 * _tmp189;
  const Scalar _tmp206 = _tmp118 * covariance(3, 1);
  const Scalar _tmp207 = std::pow(_tmp33, Scalar(2));
  const Scalar _tmp208 = accel_cov_diagonal(2, 0) * dt;
  const Scalar _tmp209 = std::pow(_tmp28, Scalar(2));
  const Scalar _tmp210 = accel_cov_diagonal(1, 0) * dt;
  const Scalar _tmp211 = std::pow(_tmp23, Scalar(2));
  const Scalar _tmp212 = accel_cov_diagonal(0, 0) * dt;
  const Scalar _tmp213 = _tmp120 * covariance(3, 2);
  const Scalar _tmp214 = _tmp33 * accel_cov_diagonal(2, 0);
  const Scalar _tmp215 = _tmp37 * accel_cov_diagonal(1, 0);
  const Scalar _tmp216 = _tmp133 * accel_cov_diagonal(0, 0);
  const Scalar _tmp217 = _tmp139 * covariance(3, 2);
  const Scalar _tmp218 = _tmp114 * _tmp215 + _tmp115 * _tmp141 + _tmp118 * _tmp149 +
                         _tmp120 * _tmp145 + _tmp132 * _tmp214 + _tmp134 * covariance(3, 1) +
                         _tmp137 * covariance(3, 0) + _tmp216 * _tmp23 + _tmp217 + covariance(4, 3);
  const Scalar _tmp219 = _tmp151 * accel_cov_diagonal(1, 0);
  const Scalar _tmp220 = _tmp156 * covariance(3, 1);
  const Scalar _tmp221 = _tmp23 * accel_cov_diagonal(0, 0);
  const Scalar _tmp222 = _tmp158 * covariance(3, 2);
  const Scalar _tmp223 = _tmp115 * _tmp168 + _tmp118 * _tmp160 + _tmp120 * _tmp164 +
                         _tmp152 * _tmp214 + _tmp153 * covariance(3, 0) + _tmp155 * _tmp221 +
                         _tmp219 * _tmp28 + _tmp220 + _tmp222 + covariance(5, 3);
  const Scalar _tmp224 = _tmp120 * _tmp173;
  const Scalar _tmp225 = _tmp207 * accel_cov_diagonal(2, 0);
  const Scalar _tmp226 = _tmp209 * accel_cov_diagonal(1, 0);
  const Scalar _tmp227 = _tmp118 * _tmp172;
  const Scalar _tmp228 = _tmp211 * accel_cov_diagonal(0, 0);
  const Scalar _tmp229 = _tmp115 * _tmp175;
  const Scalar _tmp230 = (Scalar(1) / Scalar(2)) * _tmp115;
  const Scalar _tmp231 = _tmp170 * _tmp206 + _tmp170 * _tmp213 + _tmp174 * _tmp230 +
                         covariance(3, 3) * dt + covariance(6, 3);
  const Scalar _tmp232 = _tmp224 + _tmp225 * _tmp47 + _tmp226 * _tmp47 + _tmp227 +
                         _tmp228 * _tmp47 + _tmp229 + _tmp231;
  const Scalar _tmp233 = _tmp115 * _tmp182;
  const Scalar _tmp234 = _tmp118 * _tmp180;
  const Scalar _tmp235 = _tmp120 * _tmp178;
  const Scalar _tmp236 = (Scalar(1) / Scalar(2)) * _tmp134;
  const Scalar _tmp237 = covariance(4, 3) * dt;
  const Scalar _tmp238 = (Scalar(1) / Scalar(2)) * _tmp137;
  const Scalar _tmp239 =
      _tmp170 * _tmp217 + _tmp171 * _tmp236 + _tmp174 * _tmp238 + _tmp237 + covariance(7, 3);
  const Scalar _tmp240 = _tmp33 * _tmp47;
  const Scalar _tmp241 = _tmp41 * accel_cov_diagonal(2, 0);
  const Scalar _tmp242 = _tmp28 * _tmp47;
  const Scalar _tmp243 = _tmp38 * _tmp47;
  const Scalar _tmp244 = _tmp215 * _tmp242 + _tmp221 * _tmp243 + _tmp240 * _tmp241;
  const Scalar _tmp245 = _tmp233 + _tmp234 + _tmp235 + _tmp239 + _tmp244;
  const Scalar _tmp246 = _tmp115 * _tmp189;
  const Scalar _tmp247 = _tmp118 * _tmp185;
  const Scalar _tmp248 = _tmp120 * _tmp187;
  const Scalar _tmp249 = (Scalar(1) / Scalar(2)) * _tmp153;
  const Scalar _tmp250 = covariance(5, 3) * dt;
  const Scalar _tmp251 =
      _tmp170 * _tmp220 + _tmp170 * _tmp222 + _tmp174 * _tmp249 + _tmp250 + covariance(8, 3);
  const Scalar _tmp252 = _tmp45 * accel_cov_diagonal(1, 0);
  const Scalar _tmp253 = _tmp44 * _tmp47;
  const Scalar _tmp254 = _tmp43 * _tmp47;
  const Scalar _tmp255 = _tmp214 * _tmp254 + _tmp221 * _tmp253 + _tmp242 * _tmp252;
  const Scalar _tmp256 = _tmp246 + _tmp247 + _tmp248 + _tmp251 + _tmp255;
  const Scalar _tmp257 = std::pow(_tmp38, Scalar(2));
  const Scalar _tmp258 = std::pow(_tmp37, Scalar(2));
  const Scalar _tmp259 = _tmp139 * covariance(4, 2);
  const Scalar _tmp260 = std::pow(_tmp41, Scalar(2));
  const Scalar _tmp261 = _tmp156 * covariance(4, 1);
  const Scalar _tmp262 = _tmp158 * covariance(4, 2);
  const Scalar _tmp263 = _tmp134 * _tmp160 + _tmp137 * _tmp168 + _tmp139 * _tmp164 +
                         _tmp152 * _tmp241 + _tmp153 * covariance(4, 0) + _tmp216 * _tmp44 +
                         _tmp219 * _tmp37 + _tmp261 + _tmp262 + covariance(5, 4);
  const Scalar _tmp264 = (Scalar(1) / Scalar(2)) * _tmp118;
  const Scalar _tmp265 = (Scalar(1) / Scalar(2)) * _tmp120;
  const Scalar _tmp266 = _tmp134 * _tmp172 + _tmp137 * _tmp175 + _tmp139 * _tmp173 +
                         _tmp177 * _tmp265 + _tmp179 * _tmp264 + _tmp181 * _tmp230 + _tmp237 +
                         _tmp244 + covariance(6, 4);
  const Scalar _tmp267 = _tmp139 * _tmp178;
  const Scalar _tmp268 = _tmp134 * _tmp180;
  const Scalar _tmp269 = _tmp257 * accel_cov_diagonal(0, 0);
  const Scalar _tmp270 = _tmp258 * accel_cov_diagonal(1, 0);
  const Scalar _tmp271 = _tmp260 * accel_cov_diagonal(2, 0);
  const Scalar _tmp272 = _tmp137 * _tmp182;
  const Scalar _tmp273 = _tmp170 * _tmp259 + _tmp179 * _tmp236 + _tmp181 * _tmp238 +
                         covariance(4, 4) * dt + covariance(7, 4);
  const Scalar _tmp274 = _tmp267 + _tmp268 + _tmp269 * _tmp47 + _tmp270 * _tmp47 +
                         _tmp271 * _tmp47 + _tmp272 + _tmp273;
  const Scalar _tmp275 = _tmp134 * _tmp185;
  const Scalar _tmp276 = _tmp139 * _tmp187;
  const Scalar _tmp277 = _tmp137 * _tmp189;
  const Scalar _tmp278 = covariance(5, 4) * dt;
  const Scalar _tmp279 =
      _tmp170 * _tmp261 + _tmp170 * _tmp262 + _tmp181 * _tmp249 + _tmp278 + covariance(8, 4);
  const Scalar _tmp280 = _tmp44 * accel_cov_diagonal(0, 0);
  const Scalar _tmp281 = _tmp45 * _tmp47;
  const Scalar _tmp282 = _tmp215 * _tmp281 + _tmp241 * _tmp254 + _tmp243 * _tmp280;
  const Scalar _tmp283 = _tmp275 + _tmp276 + _tmp277 + _tmp279 + _tmp282;
  const Scalar _tmp284 = _tmp156 * covariance(5, 1);
  const Scalar _tmp285 = std::pow(_tmp44, Scalar(2));
  const Scalar _tmp286 = std::pow(_tmp43, Scalar(2));
  const Scalar _tmp287 = _tmp158 * covariance(5, 2);
  const Scalar _tmp288 = std::pow(_tmp45, Scalar(2));
  const Scalar _tmp289 = _tmp153 * _tmp175 + _tmp156 * _tmp172 + _tmp158 * _tmp173 +
                         _tmp184 * _tmp264 + _tmp186 * _tmp265 + _tmp188 * _tmp230 + _tmp250 +
                         _tmp255 + covariance(6, 5);
  const Scalar _tmp290 = (Scalar(1) / Scalar(2)) * _tmp139 * _tmp186 + _tmp153 * _tmp182 +
                         _tmp156 * _tmp180 + _tmp158 * _tmp178 + _tmp184 * _tmp236 +
                         _tmp188 * _tmp238 + _tmp278 + _tmp282 + covariance(7, 5);
  const Scalar _tmp291 = _tmp156 * _tmp185;
  const Scalar _tmp292 = _tmp158 * _tmp187;
  const Scalar _tmp293 = _tmp153 * _tmp189;
  const Scalar _tmp294 = _tmp285 * accel_cov_diagonal(0, 0);
  const Scalar _tmp295 = _tmp286 * accel_cov_diagonal(2, 0);
  const Scalar _tmp296 = _tmp288 * accel_cov_diagonal(1, 0);
  const Scalar _tmp297 = _tmp170 * _tmp284 + _tmp170 * _tmp287 + _tmp188 * _tmp249 +
                         covariance(5, 5) * dt + covariance(8, 5);
  const Scalar _tmp298 = _tmp291 + _tmp292 + _tmp293 + _tmp294 * _tmp47 + _tmp295 * _tmp47 +
                         _tmp296 * _tmp47 + _tmp297;
  const Scalar _tmp299 = _tmp170 * covariance(6, 1);
  const Scalar _tmp300 = _tmp170 * covariance(6, 2);
  const Scalar _tmp301 = (Scalar(1) / Scalar(4)) * [&]() {
    const Scalar base = dt;
    return base * base * base;
  }();
  const Scalar _tmp302 = _tmp28 * _tmp301;
  const Scalar _tmp303 = _tmp301 * _tmp38;
  const Scalar _tmp304 = _tmp137 * _tmp170;
  const Scalar _tmp305 = _tmp134 * _tmp299 + _tmp139 * _tmp300 + _tmp170 * _tmp233 +
                         _tmp170 * _tmp234 + _tmp170 * _tmp235 + _tmp214 * _tmp301 * _tmp41 +
                         _tmp215 * _tmp302 + _tmp221 * _tmp303 + _tmp239 * dt +
                         _tmp304 * covariance(6, 0) + covariance(6, 4) * dt + covariance(7, 6);
  const Scalar _tmp306 = _tmp153 * _tmp170;
  const Scalar _tmp307 = _tmp156 * _tmp170;
  const Scalar _tmp308 = _tmp158 * _tmp170;
  const Scalar _tmp309 = _tmp301 * _tmp43;
  const Scalar _tmp310 = _tmp170 * _tmp246 + _tmp170 * _tmp247 + _tmp170 * _tmp248 +
                         _tmp214 * _tmp309 + _tmp221 * _tmp301 * _tmp44 + _tmp251 * dt +
                         _tmp252 * _tmp302 + _tmp306 * covariance(6, 0) +
                         _tmp307 * covariance(6, 1) + _tmp308 * covariance(6, 2) +
                         covariance(6, 5) * dt + covariance(8, 6);
  const Scalar _tmp311 = _tmp170 * _tmp275 + _tmp170 * _tmp276 + _tmp170 * _tmp277 +
                         _tmp215 * _tmp301 * _tmp45 + _tmp241 * _tmp309 + _tmp279 * dt +
                         _tmp280 * _tmp303 + _tmp306 * covariance(7, 0) +
                         _tmp307 * covariance(7, 1) + _tmp308 * covariance(7, 2) +
                         covariance(7, 5) * dt + covariance(8, 7);
  const Scalar _tmp312 = DR_D_gyro_bias(2, 0) * _tmp120;
  const Scalar _tmp313 = DR_D_gyro_bias(1, 0) * _tmp118;
  const Scalar _tmp314 = DR_D_gyro_bias(0, 0) * _tmp115;
  const Scalar _tmp315 = DR_D_gyro_bias(2, 0) * _tmp139;
  const Scalar _tmp316 = DR_D_gyro_bias(1, 0) * _tmp134;
  const Scalar _tmp317 = DR_D_gyro_bias(0, 0) * _tmp137;
  const Scalar _tmp318 = DR_D_gyro_bias(2, 0) * _tmp158;
  const Scalar _tmp319 = DR_D_gyro_bias(1, 0) * _tmp156;
  const Scalar _tmp320 = DR_D_gyro_bias(0, 0) * _tmp153;
  const Scalar _tmp321 = DR_D_gyro_bias(2, 1) * _tmp120;
  const Scalar _tmp322 = DR_D_gyro_bias(1, 1) * _tmp118;
  const Scalar _tmp323 = DR_D_gyro_bias(0, 1) * _tmp115;
  const Scalar _tmp324 = DR_D_gyro_bias(2, 1) * _tmp139;
  const Scalar _tmp325 = DR_D_gyro_bias(1, 1) * _tmp134;
  const Scalar _tmp326 = DR_D_gyro_bias(0, 1) * _tmp137;
  const Scalar _tmp327 = DR_D_gyro_bias(2, 1) * _tmp158;
  const Scalar _tmp328 = DR_D_gyro_bias(1, 1) * _tmp156;
  const Scalar _tmp329 = DR_D_gyro_bias(0, 1) * _tmp153;
  const Scalar _tmp330 = DR_D_gyro_bias(2, 2) * _tmp120;
  const Scalar _tmp331 = DR_D_gyro_bias(1, 2) * _tmp118;
  const Scalar _tmp332 = DR_D_gyro_bias(0, 2) * _tmp115;
  const Scalar _tmp333 = DR_D_gyro_bias(2, 2) * _tmp139;
  const Scalar _tmp334 = DR_D_gyro_bias(1, 2) * _tmp134;
  const Scalar _tmp335 = DR_D_gyro_bias(0, 2) * _tmp137;
  const Scalar _tmp336 = DR_D_gyro_bias(2, 2) * _tmp158;
  const Scalar _tmp337 = DR_D_gyro_bias(1, 2) * _tmp156;
  const Scalar _tmp338 = DR_D_gyro_bias(0, 2) * _tmp153;

  // Output terms (9)
  if (new_DR != nullptr) {
    Eigen::Matrix<Scalar, 4, 1> _new_DR;

    _new_DR[0] = _DR[0] * _tmp19 + _tmp13 * _tmp14 + _tmp15 * _tmp16 - _tmp17 * _tmp18;
    _new_DR[1] = _DR[1] * _tmp19 + _tmp13 * _tmp17 + _tmp14 * _tmp18 - _tmp15 * _tmp20;
    _new_DR[2] = _DR[2] * _tmp19 + _tmp13 * _tmp15 - _tmp14 * _tmp16 + _tmp17 * _tmp20;
    _new_DR[3] = _DR[3] * _tmp19 - _tmp14 * _tmp20 - _tmp15 * _tmp18 - _tmp16 * _tmp17;

    *new_DR = sym::Rot3<Scalar>(_new_DR);
  }

  if (new_Dv != nullptr) {
    Eigen::Matrix<Scalar, 3, 1>& _new_Dv = (*new_Dv);

    _new_Dv(0, 0) = Dv(0, 0) + _tmp35 * dt;
    _new_Dv(1, 0) = Dv(1, 0) + _tmp42 * dt;
    _new_Dv(2, 0) = Dv(2, 0) + _tmp46 * dt;
  }

  if (new_Dp != nullptr) {
    Eigen::Matrix<Scalar, 3, 1>& _new_Dp = (*new_Dp);

    _new_Dp(0, 0) = Dp(0, 0) + Dv(0, 0) * dt + _tmp35 * _tmp47;
    _new_Dp(1, 0) = Dp(1, 0) + Dv(1, 0) * dt + _tmp42 * _tmp47;
    _new_Dp(2, 0) = Dp(2, 0) + Dv(2, 0) * dt + _tmp46 * _tmp47;
  }

  if (new_covariance != nullptr) {
    Eigen::Matrix<Scalar, 9, 9>& _new_covariance = (*new_covariance);

    _new_covariance(0, 0) =
        _tmp51 *
            (_tmp51 * covariance(0, 0) + _tmp57 * covariance(2, 0) + _tmp60 * covariance(1, 0)) +
        _tmp57 *
            (_tmp51 * covariance(2, 0) + _tmp57 * covariance(2, 2) + _tmp60 * covariance(2, 1)) +
        _tmp60 *
            (_tmp51 * covariance(1, 0) + _tmp57 * covariance(2, 1) + _tmp60 * covariance(1, 1)) +
        std::pow(_tmp66, Scalar(2)) * _tmp67 + std::pow(_tmp72, Scalar(2)) * _tmp73 +
        std::pow(_tmp77, Scalar(2)) * _tmp78;
    _new_covariance(1, 0) = _tmp100;
    _new_covariance(2, 0) = _tmp112;
    _new_covariance(3, 0) = _tmp131;
    _new_covariance(4, 0) = _tmp150;
    _new_covariance(5, 0) = _tmp169;
    _new_covariance(6, 0) = _tmp176;
    _new_covariance(7, 0) = _tmp183;
    _new_covariance(8, 0) = _tmp190;
    _new_covariance(0, 1) = _tmp100;
    _new_covariance(1, 1) =
        _tmp67 * std::pow(_tmp97, Scalar(2)) + _tmp73 * std::pow(_tmp82, Scalar(2)) +
        _tmp78 * std::pow(_tmp94, Scalar(2)) + _tmp85 * _tmp92 + _tmp88 * _tmp91 + _tmp89 * _tmp90;
    _new_covariance(2, 1) = _tmp193;
    _new_covariance(3, 1) = _tmp194;
    _new_covariance(4, 1) = _tmp195;
    _new_covariance(5, 1) = _tmp196;
    _new_covariance(6, 1) = _tmp197;
    _new_covariance(7, 1) = _tmp198;
    _new_covariance(8, 1) = _tmp199;
    _new_covariance(0, 2) = _tmp112;
    _new_covariance(1, 2) = _tmp193;
    _new_covariance(2, 2) = std::pow(_tmp101, Scalar(2)) * _tmp73 + _tmp103 * _tmp111 +
                            _tmp104 * _tmp108 + _tmp105 * _tmp106 +
                            std::pow(_tmp107, Scalar(2)) * _tmp67 +
                            std::pow(_tmp109, Scalar(2)) * _tmp78;
    _new_covariance(3, 2) = _tmp200;
    _new_covariance(4, 2) = _tmp201;
    _new_covariance(5, 2) = _tmp202;
    _new_covariance(6, 2) = _tmp203;
    _new_covariance(7, 2) = _tmp204;
    _new_covariance(8, 2) = _tmp205;
    _new_covariance(0, 3) = _tmp131;
    _new_covariance(1, 3) = _tmp194;
    _new_covariance(2, 3) = _tmp200;
    _new_covariance(3, 3) = _tmp115 * _tmp126 + _tmp115 * covariance(3, 0) + _tmp118 * _tmp130 +
                            _tmp120 * _tmp122 + _tmp206 + _tmp207 * _tmp208 + _tmp209 * _tmp210 +
                            _tmp211 * _tmp212 + _tmp213 + covariance(3, 3);
    _new_covariance(4, 3) = _tmp218;
    _new_covariance(5, 3) = _tmp223;
    _new_covariance(6, 3) = _tmp232;
    _new_covariance(7, 3) = _tmp245;
    _new_covariance(8, 3) = _tmp256;
    _new_covariance(0, 4) = _tmp150;
    _new_covariance(1, 4) = _tmp195;
    _new_covariance(2, 4) = _tmp201;
    _new_covariance(3, 4) = _tmp218;
    _new_covariance(4, 4) = _tmp134 * _tmp149 + _tmp134 * covariance(4, 1) + _tmp137 * _tmp141 +
                            _tmp137 * covariance(4, 0) + _tmp139 * _tmp145 + _tmp208 * _tmp260 +
                            _tmp210 * _tmp258 + _tmp212 * _tmp257 + _tmp259 + covariance(4, 4);
    _new_covariance(5, 4) = _tmp263;
    _new_covariance(6, 4) = _tmp266;
    _new_covariance(7, 4) = _tmp274;
    _new_covariance(8, 4) = _tmp283;
    _new_covariance(0, 5) = _tmp169;
    _new_covariance(1, 5) = _tmp196;
    _new_covariance(2, 5) = _tmp202;
    _new_covariance(3, 5) = _tmp223;
    _new_covariance(4, 5) = _tmp263;
    _new_covariance(5, 5) = _tmp153 * _tmp168 + _tmp153 * covariance(5, 0) + _tmp156 * _tmp160 +
                            _tmp158 * _tmp164 + _tmp208 * _tmp286 + _tmp210 * _tmp288 +
                            _tmp212 * _tmp285 + _tmp284 + _tmp287 + covariance(5, 5);
    _new_covariance(6, 5) = _tmp289;
    _new_covariance(7, 5) = _tmp290;
    _new_covariance(8, 5) = _tmp298;
    _new_covariance(0, 6) = _tmp176;
    _new_covariance(1, 6) = _tmp197;
    _new_covariance(2, 6) = _tmp203;
    _new_covariance(3, 6) = _tmp232;
    _new_covariance(4, 6) = _tmp266;
    _new_covariance(5, 6) = _tmp289;
    _new_covariance(6, 6) = _tmp115 * _tmp170 * covariance(6, 0) + _tmp118 * _tmp299 +
                            _tmp120 * _tmp300 + _tmp170 * _tmp224 + _tmp170 * _tmp227 +
                            _tmp170 * _tmp229 + _tmp225 * _tmp301 + _tmp226 * _tmp301 +
                            _tmp228 * _tmp301 + _tmp231 * dt + covariance(6, 3) * dt +
                            covariance(6, 6);
    _new_covariance(7, 6) = _tmp305;
    _new_covariance(8, 6) = _tmp310;
    _new_covariance(0, 7) = _tmp183;
    _new_covariance(1, 7) = _tmp198;
    _new_covariance(2, 7) = _tmp204;
    _new_covariance(3, 7) = _tmp245;
    _new_covariance(4, 7) = _tmp274;
    _new_covariance(5, 7) = _tmp290;
    _new_covariance(6, 7) = _tmp305;
    _new_covariance(7, 7) = _tmp134 * _tmp170 * covariance(7, 1) +
                            _tmp139 * _tmp170 * covariance(7, 2) + _tmp170 * _tmp267 +
                            _tmp170 * _tmp268 + _tmp170 * _tmp272 + _tmp269 * _tmp301 +
                            _tmp270 * _tmp301 + _tmp271 * _tmp301 + _tmp273 * dt +
                            _tmp304 * covariance(7, 0) + covariance(7, 4) * dt + covariance(7, 7);
    _new_covariance(8, 7) = _tmp311;
    _new_covariance(0, 8) = _tmp190;
    _new_covariance(1, 8) = _tmp199;
    _new_covariance(2, 8) = _tmp205;
    _new_covariance(3, 8) = _tmp256;
    _new_covariance(4, 8) = _tmp283;
    _new_covariance(5, 8) = _tmp298;
    _new_covariance(6, 8) = _tmp310;
    _new_covariance(7, 8) = _tmp311;
    _new_covariance(8, 8) = _tmp170 * _tmp291 + _tmp170 * _tmp292 + _tmp170 * _tmp293 +
                            _tmp294 * _tmp301 + _tmp295 * _tmp301 + _tmp296 * _tmp301 +
                            _tmp297 * dt + _tmp306 * covariance(8, 0) + _tmp307 * covariance(8, 1) +
                            _tmp308 * covariance(8, 2) + covariance(8, 5) * dt + covariance(8, 8);
  }

  if (new_DR_D_gyro_bias != nullptr) {
    Eigen::Matrix<Scalar, 3, 3>& _new_DR_D_gyro_bias = (*new_DR_D_gyro_bias);

    _new_DR_D_gyro_bias(0, 0) = DR_D_gyro_bias(0, 0) * _tmp51 + DR_D_gyro_bias(1, 0) * _tmp60 +
                                DR_D_gyro_bias(2, 0) * _tmp57 - _tmp98;
    _new_DR_D_gyro_bias(1, 0) = DR_D_gyro_bias(0, 0) * _tmp89 + DR_D_gyro_bias(1, 0) * _tmp85 +
                                DR_D_gyro_bias(2, 0) * _tmp88 - _tmp97 * dt;
    _new_DR_D_gyro_bias(2, 0) = DR_D_gyro_bias(0, 0) * _tmp105 + DR_D_gyro_bias(1, 0) * _tmp104 +
                                DR_D_gyro_bias(2, 0) * _tmp103 - _tmp192;
    _new_DR_D_gyro_bias(0, 1) = DR_D_gyro_bias(0, 1) * _tmp51 + DR_D_gyro_bias(1, 1) * _tmp60 +
                                DR_D_gyro_bias(2, 1) * _tmp57 - _tmp110;
    _new_DR_D_gyro_bias(1, 1) = DR_D_gyro_bias(0, 1) * _tmp89 + DR_D_gyro_bias(1, 1) * _tmp85 +
                                DR_D_gyro_bias(2, 1) * _tmp88 - _tmp95;
    _new_DR_D_gyro_bias(2, 1) = DR_D_gyro_bias(0, 1) * _tmp105 + DR_D_gyro_bias(1, 1) * _tmp104 +
                                DR_D_gyro_bias(2, 1) * _tmp103 - _tmp109 * dt;
    _new_DR_D_gyro_bias(0, 2) = DR_D_gyro_bias(0, 2) * _tmp51 + DR_D_gyro_bias(1, 2) * _tmp60 +
                                DR_D_gyro_bias(2, 2) * _tmp57 - _tmp83;
    _new_DR_D_gyro_bias(1, 2) = DR_D_gyro_bias(0, 2) * _tmp89 + DR_D_gyro_bias(1, 2) * _tmp85 +
                                DR_D_gyro_bias(2, 2) * _tmp88 - _tmp191;
    _new_DR_D_gyro_bias(2, 2) = DR_D_gyro_bias(0, 2) * _tmp105 + DR_D_gyro_bias(1, 2) * _tmp104 +
                                DR_D_gyro_bias(2, 2) * _tmp103 - _tmp101 * dt;
  }

  if (new_Dv_D_accel_bias != nullptr) {
    Eigen::Matrix<Scalar, 3, 3>& _new_Dv_D_accel_bias = (*new_Dv_D_accel_bias);

    _new_Dv_D_accel_bias(0, 0) = Dv_D_accel_bias(0, 0) - _tmp117;
    _new_Dv_D_accel_bias(1, 0) = Dv_D_accel_bias(1, 0) - _tmp133;
    _new_Dv_D_accel_bias(2, 0) = Dv_D_accel_bias(2, 0) - _tmp155;
    _new_Dv_D_accel_bias(0, 1) = Dv_D_accel_bias(0, 1) - _tmp114;
    _new_Dv_D_accel_bias(1, 1) = Dv_D_accel_bias(1, 1) - _tmp136;
    _new_Dv_D_accel_bias(2, 1) = Dv_D_accel_bias(2, 1) - _tmp151;
    _new_Dv_D_accel_bias(0, 2) = Dv_D_accel_bias(0, 2) - _tmp113;
    _new_Dv_D_accel_bias(1, 2) = Dv_D_accel_bias(1, 2) - _tmp132;
    _new_Dv_D_accel_bias(2, 2) = Dv_D_accel_bias(2, 2) - _tmp152;
  }

  if (new_Dv_D_gyro_bias != nullptr) {
    Eigen::Matrix<Scalar, 3, 3>& _new_Dv_D_gyro_bias = (*new_Dv_D_gyro_bias);

    _new_Dv_D_gyro_bias(0, 0) = Dv_D_gyro_bias(0, 0) + _tmp312 + _tmp313 + _tmp314;
    _new_Dv_D_gyro_bias(1, 0) = Dv_D_gyro_bias(1, 0) + _tmp315 + _tmp316 + _tmp317;
    _new_Dv_D_gyro_bias(2, 0) = Dv_D_gyro_bias(2, 0) + _tmp318 + _tmp319 + _tmp320;
    _new_Dv_D_gyro_bias(0, 1) = Dv_D_gyro_bias(0, 1) + _tmp321 + _tmp322 + _tmp323;
    _new_Dv_D_gyro_bias(1, 1) = Dv_D_gyro_bias(1, 1) + _tmp324 + _tmp325 + _tmp326;
    _new_Dv_D_gyro_bias(2, 1) = Dv_D_gyro_bias(2, 1) + _tmp327 + _tmp328 + _tmp329;
    _new_Dv_D_gyro_bias(0, 2) = Dv_D_gyro_bias(0, 2) + _tmp330 + _tmp331 + _tmp332;
    _new_Dv_D_gyro_bias(1, 2) = Dv_D_gyro_bias(1, 2) + _tmp333 + _tmp334 + _tmp335;
    _new_Dv_D_gyro_bias(2, 2) = Dv_D_gyro_bias(2, 2) + _tmp336 + _tmp337 + _tmp338;
  }

  if (new_Dp_D_accel_bias != nullptr) {
    Eigen::Matrix<Scalar, 3, 3>& _new_Dp_D_accel_bias = (*new_Dp_D_accel_bias);

    _new_Dp_D_accel_bias(0, 0) =
        Dp_D_accel_bias(0, 0) + Dv_D_accel_bias(0, 0) * dt - _tmp23 * _tmp47;
    _new_Dp_D_accel_bias(1, 0) = Dp_D_accel_bias(1, 0) + Dv_D_accel_bias(1, 0) * dt - _tmp243;
    _new_Dp_D_accel_bias(2, 0) = Dp_D_accel_bias(2, 0) + Dv_D_accel_bias(2, 0) * dt - _tmp253;
    _new_Dp_D_accel_bias(0, 1) = Dp_D_accel_bias(0, 1) + Dv_D_accel_bias(0, 1) * dt - _tmp242;
    _new_Dp_D_accel_bias(1, 1) =
        Dp_D_accel_bias(1, 1) + Dv_D_accel_bias(1, 1) * dt - _tmp37 * _tmp47;
    _new_Dp_D_accel_bias(2, 1) = Dp_D_accel_bias(2, 1) + Dv_D_accel_bias(2, 1) * dt - _tmp281;
    _new_Dp_D_accel_bias(0, 2) = Dp_D_accel_bias(0, 2) + Dv_D_accel_bias(0, 2) * dt - _tmp240;
    _new_Dp_D_accel_bias(1, 2) =
        Dp_D_accel_bias(1, 2) + Dv_D_accel_bias(1, 2) * dt - _tmp41 * _tmp47;
    _new_Dp_D_accel_bias(2, 2) = Dp_D_accel_bias(2, 2) + Dv_D_accel_bias(2, 2) * dt - _tmp254;
  }

  if (new_Dp_D_gyro_bias != nullptr) {
    Eigen::Matrix<Scalar, 3, 3>& _new_Dp_D_gyro_bias = (*new_Dp_D_gyro_bias);

    _new_Dp_D_gyro_bias(0, 0) = Dp_D_gyro_bias(0, 0) + Dv_D_gyro_bias(0, 0) * dt +
                                _tmp170 * _tmp312 + _tmp170 * _tmp313 + _tmp170 * _tmp314;
    _new_Dp_D_gyro_bias(1, 0) = Dp_D_gyro_bias(1, 0) + Dv_D_gyro_bias(1, 0) * dt +
                                _tmp170 * _tmp315 + _tmp170 * _tmp316 + _tmp170 * _tmp317;
    _new_Dp_D_gyro_bias(2, 0) = Dp_D_gyro_bias(2, 0) + Dv_D_gyro_bias(2, 0) * dt +
                                _tmp170 * _tmp318 + _tmp170 * _tmp319 + _tmp170 * _tmp320;
    _new_Dp_D_gyro_bias(0, 1) = Dp_D_gyro_bias(0, 1) + Dv_D_gyro_bias(0, 1) * dt +
                                _tmp170 * _tmp321 + _tmp170 * _tmp322 + _tmp170 * _tmp323;
    _new_Dp_D_gyro_bias(1, 1) = Dp_D_gyro_bias(1, 1) + Dv_D_gyro_bias(1, 1) * dt +
                                _tmp170 * _tmp324 + _tmp170 * _tmp325 + _tmp170 * _tmp326;
    _new_Dp_D_gyro_bias(2, 1) = Dp_D_gyro_bias(2, 1) + Dv_D_gyro_bias(2, 1) * dt +
                                _tmp170 * _tmp327 + _tmp170 * _tmp328 + _tmp170 * _tmp329;
    _new_Dp_D_gyro_bias(0, 2) = Dp_D_gyro_bias(0, 2) + Dv_D_gyro_bias(0, 2) * dt +
                                _tmp170 * _tmp330 + _tmp170 * _tmp331 + _tmp170 * _tmp332;
    _new_Dp_D_gyro_bias(1, 2) = Dp_D_gyro_bias(1, 2) + Dv_D_gyro_bias(1, 2) * dt +
                                _tmp170 * _tmp333 + _tmp170 * _tmp334 + _tmp170 * _tmp335;
    _new_Dp_D_gyro_bias(2, 2) = Dp_D_gyro_bias(2, 2) + Dv_D_gyro_bias(2, 2) * dt +
                                _tmp170 * _tmp336 + _tmp170 * _tmp337 + _tmp170 * _tmp338;
  }
}  // NOLINT(readability/fn_size)

// NOLINTNEXTLINE(readability/fn_size)
}  // namespace sym
