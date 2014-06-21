/*
 * PID.h
 *
 *  Created on: Jun 14, 2014
 *      Author: Jiaxin
 */

#ifndef PID_H_
#define PID_H_

typedef struct {
    double windup_guard;
    double proportional_gain;
    double integral_gain;
    double derivative_gain;
    double prev_error;
    double int_error;
    double control;
} PID;

void pid_update(PID* pid, double curr_error, double dt);
void pid_zeroize(PID* pid);

#endif /* PID_H_ */
