#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {
	this->is_initialized = false;
}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	//need to tune the pid here... use twiddle 
	//double[] params = {Kp, Ki, Kd}; 
	this->p_error = this->d_error = this->i_error = 0.0;
	this->Kp= Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	this->int_cte=0;
	this->is_initialized=true;
	this->it = 0;
	this->sliding_window_size= 200; 
	this->twiddle_tolerance = 0.2;
	this->twiddler.Init(twiddle_tolerance);
	for(int i=0; i<3; i++){
		this->dp[i]=1;
	}
}

void PID::UpdateError(double cte) {


	this->twiddler.twiddle(this,cte);

	double prev_cte = this->cte;
	this->cte = cte;
	double diff_cte = this->cte - prev_cte;
	this->int_cte += cte;

	this->p_error = - Kp *cte;
	this->i_error = - Ki *int_cte;
	this->d_error = - Kd *diff_cte;
}



double PID::TotalError() {
	return this->p_error+this->i_error+this->d_error;
}

double PID::GetSteering(){
	return TotalError();
}