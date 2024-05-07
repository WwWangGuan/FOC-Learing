//
// Created by wangguan on 24-4-29.
//

#include "Motor.h"

#include "math.h"
#include "stdint.h"


void Clark(SampCurrent_T *Current, ClarkTrans_T *ClarkCurt) {
    ClarkCurt->I_alpha = Current->I_a;
    ClarkCurt->I_beta = SQRT3_BY_3 * (2 * Current->I_b + Current->I_a);
    //与原视频中的代码不符，更换系数为直接推导出公式的系数
}

void Park(ClarkTrans_T *ClarkCurt, ParkTrans_T *ParkCurt, float *theta) {
    ParkCurt->I_d = ClarkCurt->I_alpha * cosf(*theta) + ClarkCurt->I_beta * sinf(*theta);
    ParkCurt->I_q = ClarkCurt->I_beta * cosf(*theta) - ClarkCurt->I_alpha * sinf(*theta);
}

void RevPark(CtrlVolt_T *CtrVoltage, RevParkTrans_T *RevParkVolt, float *theta) {
    RevParkVolt->U_alpha = CtrVoltage->U_d * cosf(*theta) - CtrVoltage->U_q * sinf(*theta);
    RevParkVolt->U_beta = CtrVoltage->U_d * sinf(*theta) + CtrVoltage->U_q * cosf(*theta);
}

void SVPWM(RevParkTrans_T *RevParkVolt, SwitchTime_T *ST, TripPhaseVolt_T *TPVolt) {
    float Ua = RevParkVolt->U_alpha;
    float Ub = -0.5f * RevParkVolt->U_alpha + SQRT3_2 * RevParkVolt->U_beta;
    float Uc = -0.5f * RevParkVolt->U_alpha - SQRT3_2 * RevParkVolt->U_beta;
    float Umax = Max(Ua, Max(Ub, Uc));
    float Umin = Min(Ua, Min(Ub, Uc));
    float U0 = -0.5f * (Umax + Umin);
    Ua += U0;
    Ub += U0;
    Uc += U0;
    float Ta = Ua / VBAT * 27200;
    float Tb = Ub / VBAT * 27200;
    float Tc = Uc / VBAT * 27200;
    ST->T_a1 = 27200 - Ta;
    ST->T_a2 = 54400 - ST->T_a1;
    ST->T_b1 = 27200 - Tb;
    ST->T_b2 = 54400 - ST->T_b1;
    ST->T_c1 = 27200 - Tc;
    ST->T_c2 = 54400 - ST->T_c1;
//    TPVolt->Ua = Ta - 0.5f * (Tb + Tc);
//    TPVolt->Ub = Tb - 0.5f * (Ta + Tc);
//    TPVolt->Uc = -(TPVolt->Ua + TPVolt->Ub);
    TPVolt->Ua = Ua;
    TPVolt->Ub = Ub;
    TPVolt->Uc = Uc;

}

float Max(float a, float b) {
    return (a > b) ? a : b;
}

float Min(float a, float b) {
    return (a < b) ? a : b;
}
