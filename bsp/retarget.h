//
// Created by wangguan on 24-4-29.
//

#ifndef FOC_RETARGET_H
#define FOC_RETARGET_H

#include "stm32g4xx_hal.h"
#include <sys/stat.h>
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

int _fstat(int fd, struct stat *st);
#endif //FOC_RETARGET_H
