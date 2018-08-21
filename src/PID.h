#ifndef PID_H
#define PID_H

#include <iostream>
#include "twiddler.h"


class PID{
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double cte_sliding_window[1000];
  int sliding_window_size;
  int it;
  double twiddle_tolerance;
  double dp[3];
  double params[3];
  Twiddler twiddler;


  /*
  * CTE memory
  */
  double cte; 
  double int_cte;

  /*
  * initialization
  */
  bool is_initialized;
  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

/*
* Calculate the steering angle
*/
  double GetSteering();

};

#endif /* PID_H */
