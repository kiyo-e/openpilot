#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with sympy 1.9                        *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1876979179817506794) {
   out_1876979179817506794[0] = delta_x[0] + nom_x[0];
   out_1876979179817506794[1] = delta_x[1] + nom_x[1];
   out_1876979179817506794[2] = delta_x[2] + nom_x[2];
   out_1876979179817506794[3] = delta_x[3] + nom_x[3];
   out_1876979179817506794[4] = delta_x[4] + nom_x[4];
   out_1876979179817506794[5] = delta_x[5] + nom_x[5];
   out_1876979179817506794[6] = delta_x[6] + nom_x[6];
   out_1876979179817506794[7] = delta_x[7] + nom_x[7];
   out_1876979179817506794[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_738824714567483071) {
   out_738824714567483071[0] = -nom_x[0] + true_x[0];
   out_738824714567483071[1] = -nom_x[1] + true_x[1];
   out_738824714567483071[2] = -nom_x[2] + true_x[2];
   out_738824714567483071[3] = -nom_x[3] + true_x[3];
   out_738824714567483071[4] = -nom_x[4] + true_x[4];
   out_738824714567483071[5] = -nom_x[5] + true_x[5];
   out_738824714567483071[6] = -nom_x[6] + true_x[6];
   out_738824714567483071[7] = -nom_x[7] + true_x[7];
   out_738824714567483071[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1020147720070790775) {
   out_1020147720070790775[0] = 1.0;
   out_1020147720070790775[1] = 0;
   out_1020147720070790775[2] = 0;
   out_1020147720070790775[3] = 0;
   out_1020147720070790775[4] = 0;
   out_1020147720070790775[5] = 0;
   out_1020147720070790775[6] = 0;
   out_1020147720070790775[7] = 0;
   out_1020147720070790775[8] = 0;
   out_1020147720070790775[9] = 0;
   out_1020147720070790775[10] = 1.0;
   out_1020147720070790775[11] = 0;
   out_1020147720070790775[12] = 0;
   out_1020147720070790775[13] = 0;
   out_1020147720070790775[14] = 0;
   out_1020147720070790775[15] = 0;
   out_1020147720070790775[16] = 0;
   out_1020147720070790775[17] = 0;
   out_1020147720070790775[18] = 0;
   out_1020147720070790775[19] = 0;
   out_1020147720070790775[20] = 1.0;
   out_1020147720070790775[21] = 0;
   out_1020147720070790775[22] = 0;
   out_1020147720070790775[23] = 0;
   out_1020147720070790775[24] = 0;
   out_1020147720070790775[25] = 0;
   out_1020147720070790775[26] = 0;
   out_1020147720070790775[27] = 0;
   out_1020147720070790775[28] = 0;
   out_1020147720070790775[29] = 0;
   out_1020147720070790775[30] = 1.0;
   out_1020147720070790775[31] = 0;
   out_1020147720070790775[32] = 0;
   out_1020147720070790775[33] = 0;
   out_1020147720070790775[34] = 0;
   out_1020147720070790775[35] = 0;
   out_1020147720070790775[36] = 0;
   out_1020147720070790775[37] = 0;
   out_1020147720070790775[38] = 0;
   out_1020147720070790775[39] = 0;
   out_1020147720070790775[40] = 1.0;
   out_1020147720070790775[41] = 0;
   out_1020147720070790775[42] = 0;
   out_1020147720070790775[43] = 0;
   out_1020147720070790775[44] = 0;
   out_1020147720070790775[45] = 0;
   out_1020147720070790775[46] = 0;
   out_1020147720070790775[47] = 0;
   out_1020147720070790775[48] = 0;
   out_1020147720070790775[49] = 0;
   out_1020147720070790775[50] = 1.0;
   out_1020147720070790775[51] = 0;
   out_1020147720070790775[52] = 0;
   out_1020147720070790775[53] = 0;
   out_1020147720070790775[54] = 0;
   out_1020147720070790775[55] = 0;
   out_1020147720070790775[56] = 0;
   out_1020147720070790775[57] = 0;
   out_1020147720070790775[58] = 0;
   out_1020147720070790775[59] = 0;
   out_1020147720070790775[60] = 1.0;
   out_1020147720070790775[61] = 0;
   out_1020147720070790775[62] = 0;
   out_1020147720070790775[63] = 0;
   out_1020147720070790775[64] = 0;
   out_1020147720070790775[65] = 0;
   out_1020147720070790775[66] = 0;
   out_1020147720070790775[67] = 0;
   out_1020147720070790775[68] = 0;
   out_1020147720070790775[69] = 0;
   out_1020147720070790775[70] = 1.0;
   out_1020147720070790775[71] = 0;
   out_1020147720070790775[72] = 0;
   out_1020147720070790775[73] = 0;
   out_1020147720070790775[74] = 0;
   out_1020147720070790775[75] = 0;
   out_1020147720070790775[76] = 0;
   out_1020147720070790775[77] = 0;
   out_1020147720070790775[78] = 0;
   out_1020147720070790775[79] = 0;
   out_1020147720070790775[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1837887467846292217) {
   out_1837887467846292217[0] = state[0];
   out_1837887467846292217[1] = state[1];
   out_1837887467846292217[2] = state[2];
   out_1837887467846292217[3] = state[3];
   out_1837887467846292217[4] = state[4];
   out_1837887467846292217[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1837887467846292217[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1837887467846292217[7] = state[7];
   out_1837887467846292217[8] = state[8];
}
void F_fun(double *state, double dt, double *out_5787251712374232048) {
   out_5787251712374232048[0] = 1;
   out_5787251712374232048[1] = 0;
   out_5787251712374232048[2] = 0;
   out_5787251712374232048[3] = 0;
   out_5787251712374232048[4] = 0;
   out_5787251712374232048[5] = 0;
   out_5787251712374232048[6] = 0;
   out_5787251712374232048[7] = 0;
   out_5787251712374232048[8] = 0;
   out_5787251712374232048[9] = 0;
   out_5787251712374232048[10] = 1;
   out_5787251712374232048[11] = 0;
   out_5787251712374232048[12] = 0;
   out_5787251712374232048[13] = 0;
   out_5787251712374232048[14] = 0;
   out_5787251712374232048[15] = 0;
   out_5787251712374232048[16] = 0;
   out_5787251712374232048[17] = 0;
   out_5787251712374232048[18] = 0;
   out_5787251712374232048[19] = 0;
   out_5787251712374232048[20] = 1;
   out_5787251712374232048[21] = 0;
   out_5787251712374232048[22] = 0;
   out_5787251712374232048[23] = 0;
   out_5787251712374232048[24] = 0;
   out_5787251712374232048[25] = 0;
   out_5787251712374232048[26] = 0;
   out_5787251712374232048[27] = 0;
   out_5787251712374232048[28] = 0;
   out_5787251712374232048[29] = 0;
   out_5787251712374232048[30] = 1;
   out_5787251712374232048[31] = 0;
   out_5787251712374232048[32] = 0;
   out_5787251712374232048[33] = 0;
   out_5787251712374232048[34] = 0;
   out_5787251712374232048[35] = 0;
   out_5787251712374232048[36] = 0;
   out_5787251712374232048[37] = 0;
   out_5787251712374232048[38] = 0;
   out_5787251712374232048[39] = 0;
   out_5787251712374232048[40] = 1;
   out_5787251712374232048[41] = 0;
   out_5787251712374232048[42] = 0;
   out_5787251712374232048[43] = 0;
   out_5787251712374232048[44] = 0;
   out_5787251712374232048[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_5787251712374232048[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_5787251712374232048[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5787251712374232048[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5787251712374232048[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_5787251712374232048[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_5787251712374232048[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_5787251712374232048[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_5787251712374232048[53] = -9.8000000000000007*dt;
   out_5787251712374232048[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_5787251712374232048[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_5787251712374232048[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5787251712374232048[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5787251712374232048[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_5787251712374232048[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_5787251712374232048[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_5787251712374232048[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5787251712374232048[62] = 0;
   out_5787251712374232048[63] = 0;
   out_5787251712374232048[64] = 0;
   out_5787251712374232048[65] = 0;
   out_5787251712374232048[66] = 0;
   out_5787251712374232048[67] = 0;
   out_5787251712374232048[68] = 0;
   out_5787251712374232048[69] = 0;
   out_5787251712374232048[70] = 1;
   out_5787251712374232048[71] = 0;
   out_5787251712374232048[72] = 0;
   out_5787251712374232048[73] = 0;
   out_5787251712374232048[74] = 0;
   out_5787251712374232048[75] = 0;
   out_5787251712374232048[76] = 0;
   out_5787251712374232048[77] = 0;
   out_5787251712374232048[78] = 0;
   out_5787251712374232048[79] = 0;
   out_5787251712374232048[80] = 1;
}
void h_25(double *state, double *unused, double *out_2627157697858734525) {
   out_2627157697858734525[0] = state[6];
}
void H_25(double *state, double *unused, double *out_3076857351056495453) {
   out_3076857351056495453[0] = 0;
   out_3076857351056495453[1] = 0;
   out_3076857351056495453[2] = 0;
   out_3076857351056495453[3] = 0;
   out_3076857351056495453[4] = 0;
   out_3076857351056495453[5] = 0;
   out_3076857351056495453[6] = 1;
   out_3076857351056495453[7] = 0;
   out_3076857351056495453[8] = 0;
}
void h_24(double *state, double *unused, double *out_235510140924072653) {
   out_235510140924072653[0] = state[4];
   out_235510140924072653[1] = state[5];
}
void H_24(double *state, double *unused, double *out_1669651975910013581) {
   out_1669651975910013581[0] = 0;
   out_1669651975910013581[1] = 0;
   out_1669651975910013581[2] = 0;
   out_1669651975910013581[3] = 0;
   out_1669651975910013581[4] = 1;
   out_1669651975910013581[5] = 0;
   out_1669651975910013581[6] = 0;
   out_1669651975910013581[7] = 0;
   out_1669651975910013581[8] = 0;
   out_1669651975910013581[9] = 0;
   out_1669651975910013581[10] = 0;
   out_1669651975910013581[11] = 0;
   out_1669651975910013581[12] = 0;
   out_1669651975910013581[13] = 0;
   out_1669651975910013581[14] = 1;
   out_1669651975910013581[15] = 0;
   out_1669651975910013581[16] = 0;
   out_1669651975910013581[17] = 0;
}
void h_30(double *state, double *unused, double *out_4077789090487617692) {
   out_4077789090487617692[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5595190309563744080) {
   out_5595190309563744080[0] = 0;
   out_5595190309563744080[1] = 0;
   out_5595190309563744080[2] = 0;
   out_5595190309563744080[3] = 0;
   out_5595190309563744080[4] = 1;
   out_5595190309563744080[5] = 0;
   out_5595190309563744080[6] = 0;
   out_5595190309563744080[7] = 0;
   out_5595190309563744080[8] = 0;
}
void h_26(double *state, double *unused, double *out_543163231077975709) {
   out_543163231077975709[0] = state[7];
}
void H_26(double *state, double *unused, double *out_664645967817560771) {
   out_664645967817560771[0] = 0;
   out_664645967817560771[1] = 0;
   out_664645967817560771[2] = 0;
   out_664645967817560771[3] = 0;
   out_664645967817560771[4] = 0;
   out_664645967817560771[5] = 0;
   out_664645967817560771[6] = 0;
   out_664645967817560771[7] = 1;
   out_664645967817560771[8] = 0;
}
void h_27(double *state, double *unused, double *out_8160908663677993179) {
   out_8160908663677993179[0] = state[3];
}
void H_27(double *state, double *unused, double *out_7818784380747687297) {
   out_7818784380747687297[0] = 0;
   out_7818784380747687297[1] = 0;
   out_7818784380747687297[2] = 0;
   out_7818784380747687297[3] = 1;
   out_7818784380747687297[4] = 0;
   out_7818784380747687297[5] = 0;
   out_7818784380747687297[6] = 0;
   out_7818784380747687297[7] = 0;
   out_7818784380747687297[8] = 0;
}
void h_29(double *state, double *unused, double *out_5768236925223298652) {
   out_5768236925223298652[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6105421653878136264) {
   out_6105421653878136264[0] = 0;
   out_6105421653878136264[1] = 1;
   out_6105421653878136264[2] = 0;
   out_6105421653878136264[3] = 0;
   out_6105421653878136264[4] = 0;
   out_6105421653878136264[5] = 0;
   out_6105421653878136264[6] = 0;
   out_6105421653878136264[7] = 0;
   out_6105421653878136264[8] = 0;
}
void h_28(double *state, double *unused, double *out_6004271568787897929) {
   out_6004271568787897929[0] = state[0];
}
void H_28(double *state, double *unused, double *out_1023022636808605690) {
   out_1023022636808605690[0] = 1;
   out_1023022636808605690[1] = 0;
   out_1023022636808605690[2] = 0;
   out_1023022636808605690[3] = 0;
   out_1023022636808605690[4] = 0;
   out_1023022636808605690[5] = 0;
   out_1023022636808605690[6] = 0;
   out_1023022636808605690[7] = 0;
   out_1023022636808605690[8] = 0;
}
void h_31(double *state, double *unused, double *out_3764190290286879080) {
   out_3764190290286879080[0] = state[8];
}
void H_31(double *state, double *unused, double *out_1290854070050912247) {
   out_1290854070050912247[0] = 0;
   out_1290854070050912247[1] = 0;
   out_1290854070050912247[2] = 0;
   out_1290854070050912247[3] = 0;
   out_1290854070050912247[4] = 0;
   out_1290854070050912247[5] = 0;
   out_1290854070050912247[6] = 0;
   out_1290854070050912247[7] = 0;
   out_1290854070050912247[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_1876979179817506794) {
  err_fun(nom_x, delta_x, out_1876979179817506794);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_738824714567483071) {
  inv_err_fun(nom_x, true_x, out_738824714567483071);
}
void car_H_mod_fun(double *state, double *out_1020147720070790775) {
  H_mod_fun(state, out_1020147720070790775);
}
void car_f_fun(double *state, double dt, double *out_1837887467846292217) {
  f_fun(state,  dt, out_1837887467846292217);
}
void car_F_fun(double *state, double dt, double *out_5787251712374232048) {
  F_fun(state,  dt, out_5787251712374232048);
}
void car_h_25(double *state, double *unused, double *out_2627157697858734525) {
  h_25(state, unused, out_2627157697858734525);
}
void car_H_25(double *state, double *unused, double *out_3076857351056495453) {
  H_25(state, unused, out_3076857351056495453);
}
void car_h_24(double *state, double *unused, double *out_235510140924072653) {
  h_24(state, unused, out_235510140924072653);
}
void car_H_24(double *state, double *unused, double *out_1669651975910013581) {
  H_24(state, unused, out_1669651975910013581);
}
void car_h_30(double *state, double *unused, double *out_4077789090487617692) {
  h_30(state, unused, out_4077789090487617692);
}
void car_H_30(double *state, double *unused, double *out_5595190309563744080) {
  H_30(state, unused, out_5595190309563744080);
}
void car_h_26(double *state, double *unused, double *out_543163231077975709) {
  h_26(state, unused, out_543163231077975709);
}
void car_H_26(double *state, double *unused, double *out_664645967817560771) {
  H_26(state, unused, out_664645967817560771);
}
void car_h_27(double *state, double *unused, double *out_8160908663677993179) {
  h_27(state, unused, out_8160908663677993179);
}
void car_H_27(double *state, double *unused, double *out_7818784380747687297) {
  H_27(state, unused, out_7818784380747687297);
}
void car_h_29(double *state, double *unused, double *out_5768236925223298652) {
  h_29(state, unused, out_5768236925223298652);
}
void car_H_29(double *state, double *unused, double *out_6105421653878136264) {
  H_29(state, unused, out_6105421653878136264);
}
void car_h_28(double *state, double *unused, double *out_6004271568787897929) {
  h_28(state, unused, out_6004271568787897929);
}
void car_H_28(double *state, double *unused, double *out_1023022636808605690) {
  H_28(state, unused, out_1023022636808605690);
}
void car_h_31(double *state, double *unused, double *out_3764190290286879080) {
  h_31(state, unused, out_3764190290286879080);
}
void car_H_31(double *state, double *unused, double *out_1290854070050912247) {
  H_31(state, unused, out_1290854070050912247);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_init(car);
