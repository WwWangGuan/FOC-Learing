//
// Created by wangguan on 24-4-29.
//

#include "Motor.h"

#include "math.h"
#include "stdint.h"

void RevPark(CtrlVolt_T *CtrVoltage, RevParkTrans_T *RevParkVolt, float *theta) {
    RevParkVolt->U_alpha = CtrVoltage->U_d * cosf(*theta) - CtrVoltage->U_q * sinf(*theta);
    RevParkVolt->U_beta = CtrVoltage->U_d * sinf(*theta) + CtrVoltage->U_q * cosf(*theta);
}

void Clark(SampCurrent_T *Current, ClarkTrans_T *ClarkCurt) {
    ClarkCurt->I_alpha = 1.5f * Current->I_a;
    ClarkCurt->I_beta = COS_PI_6 * (2 * Current->I_b + Current->I_a);
    //与原视频中的代码不符，更换系数为直接推导出公式的系数
}

void Park(ClarkTrans_T *ClarkCurt, ParkTrans_T *ParkCurt, float *theta) {
    ParkCurt->I_d = ClarkCurt->I_alpha * cosf(*theta) + ClarkCurt->I_beta * sinf(*theta);
    ParkCurt->I_q = ClarkCurt->I_beta * cosf(*theta) - ClarkCurt->I_alpha * sinf(*theta);
}

void SVPWM(RevParkTrans_T *RevParkVolt, SwitchTime_T *ST, TripPhaseVolt_T *TPVolt) {
    float T0, T1, T2, T3, T4, T5, T6, T7;
    float U1 = RevParkVolt->U_beta;
    float U2 = COS_PI_6 * RevParkVolt->U_alpha - 0.5f * RevParkVolt->U_beta;
    float U3 = -0.5f * RevParkVolt->U_beta - COS_PI_6 * RevParkVolt->U_alpha;
    uint8_t sector = 4 * (U3 > 0) + 2 * (U2 > 0) + (U1 > 0);
    float k = SQRT_3 * CtrlPeriod / VBAT;
    switch (sector) {
        case 1: {
            T4 = k * U3;
            T6 = k * U1;
            T0 = (CtrlPeriod - T4 - T6) / 2;
            T7 = (CtrlPeriod - T4 - T6) / 2;
            ST->T_a = T4 + T6 + T7;
            ST->T_b = T6 + T7;
            ST->T_c = T7;
            break;
        }
        case 2: {
            T2 = -k * U3;
            T6 = -k * U2;
            T0 = (CtrlPeriod - T2 - T6) / 2;
            T7 = (CtrlPeriod - T2 - T6) / 2;
            ST->T_a = T6 + T7;
            ST->T_b = T2 + T6 + T7;
            ST->T_c = T7;
            break;
        }
        case 3: {
            T2 = k * U1;
            T3 = k * U2;
            T0 = (CtrlPeriod - T2 - T3) / 2;
            T7 = (CtrlPeriod - T2 - T3) / 2;
            ST->T_a = T7;
            ST->T_b = T2 + T3 + T7;
            ST->T_c = T3 + T7;
            break;
        }
        case 4: {
            T1 = -k * U1;
            T3 = -k * U3;
            T0 = (CtrlPeriod - T1 - T3) / 2;
            T7 = (CtrlPeriod - T1 - T3) / 2;
            ST->T_a = T7;
            ST->T_b = T3 + T7;
            ST->T_c = T1 + T3 + T7;
            break;
        }
        case 5: {
            T1 = k * U2;
            T5 = k * U3;
            T0 = (CtrlPeriod - T1 - T5) / 2;
            T7 = (CtrlPeriod - T1 - T5) / 2;
            ST->T_a = T5 + T7;
            ST->T_b = T7;
            ST->T_c = T1 + T5 + T7;
            break;
        }
        case 6: {
            T4 = -k * U2;
            T5 = -k * U1;
            T0 = (CtrlPeriod - T4 - T5) / 2;
            T7 = (CtrlPeriod - T4 - T5) / 2;
            ST->T_a = T1 + T5 + T7;
            ST->T_b = T7;
            ST->T_c = T5 + T7;
            break;
        }
    }
    TPVolt->Ua = ST->T_a - 0.5f * (ST->T_b + ST->T_c);
    TPVolt->Ua = ST->T_b - 0.5f * (ST->T_a + ST->T_c);
    TPVolt->Uc = -(TPVolt->Ua + TPVolt->Ub);

}