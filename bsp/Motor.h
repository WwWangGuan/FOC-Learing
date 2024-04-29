//
// Created by wangguan on 24-4-29.
//

#ifndef FOC_MOTOR_H
#define FOC_MOTOR_H

#define COS_PI_6  0.8660254037844386f
#define SQRT_3 1.7320508075688772f
#define VBAT 12

#define CtrlPeriod 10
typedef struct {
    float I_a;
    float I_b;
    float I_c;
} SampCurrent_T;

typedef struct {
    float I_alpha;
    float I_beta;
} ClarkTrans_T;

typedef struct {
    float I_d;
    float I_q;
} ParkTrans_T;

typedef struct {
    float T_a;
    float T_b;
    float T_c;

} SwitchTime_T;

typedef struct {
    float Ua;
    float Ub;
    float Uc;
} TripPhaseVolt_T;
typedef struct {
    float U_alpha;
    float U_beta;
} RevParkTrans_T;

typedef struct {
    float U_d;
    float U_q;
} CtrlVolt_T;

typedef struct Motor {
    float theta;//电机转子角度
    SampCurrent_T Current; //三相采样电流值
    ClarkTrans_T ClarkCur; //Clark变换后的电流
    ParkTrans_T ParkCur; // Park变换后电流
    SwitchTime_T SwitchTime; //三相半桥开关时间
    RevParkTrans_T RevParkVolt; //RevPark变换后的电压
    CtrlVolt_T CtrlVolt;//输出控制电压(d-q坐标系)
    TripPhaseVolt_T TripPhaseVolt;//三相电压值
} Motor_T;

void RevPark(CtrlVolt_T *CtrVoltage, RevParkTrans_T *RevParkVolt, float *theta);

void Clark(SampCurrent_T *Current, ClarkTrans_T *ClarkCurt);

void Park(ClarkTrans_T *ClarkCurt, ParkTrans_T *ParkCurt, float *theta);

void SVPWM(RevParkTrans_T *RevParkVolt, SwitchTime_T *ST, TripPhaseVolt_T *TPVolt);

#endif //FOC_MOTOR_H
