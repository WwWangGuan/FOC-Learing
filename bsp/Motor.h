//
// Created by wangguan on 24-4-29.
//

#ifndef FOC_MOTOR_H
#define FOC_MOTOR_H

#define COS_PI_6  0.8660254037844386f
#define SQRT3_2 0.8660254037844386f
#define VBAT 0.000001f
#define SQRT3_BY_3 0.5773502691896257f

#define Period 54400

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
    float T_a1;
    float T_a2;
    float T_b1;
    float T_b2;
    float T_c1;
    float T_c2;

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

    float BusVoltage; //母线电压

    float RawAngle; //磁编原始角度值
    float Angle; // 转子角度值


} Motor_T;

void RevPark(CtrlVolt_T *CtrVoltage, RevParkTrans_T *RevParkVolt, float *theta);

void Clark(SampCurrent_T *Current, ClarkTrans_T *ClarkCurt);

void Park(ClarkTrans_T *ClarkCurt, ParkTrans_T *ParkCurt, float *theta);

void SVPWM(RevParkTrans_T *RevParkVolt, SwitchTime_T *ST, TripPhaseVolt_T *TPVolt);

float Max(float a, float b);

float Min(float a, float b);

#endif //FOC_MOTOR_H
