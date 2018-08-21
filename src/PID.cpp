#include "PID.h"
#include "twiddler.h"

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
	this->int_cte=0;
	this->is_initialized=true;
	this->it = 0;
	this->twiddle_tolerance = 0.2;
	this->twiddler.Init(twiddle_tolerance);
	for(int i=0; i<3; i++){
		this->dp[i]=1;
	}
	for(int i=0; i<3; i++){
		switch(i) {
			case 0: params[i] = Kp;
					break;
			case 1: params[i] = Ki;
					break;
			case 2: params[i] = Kd;
					break;
		}
		this->params[i]=0;
	}
}

void PID::UpdateError(double cte) {

	cout << "Updating error, parameters : " << endl;
	for(int i=0; i<3; i++){
		cout << params[i]<<endl;
	}
	this->twiddler.twiddle(this,cte); //this is gonna refresh parameters

	double prev_cte = this->cte;
	this->cte = cte;
	double diff_cte = this->cte - prev_cte;
	this->int_cte += cte;

	this->p_error = - params[0] *cte;
	this->i_error = - params[1] *int_cte;
	this->d_error = - params[2] *diff_cte;
}



double PID::TotalError() {
	return this->p_error+this->i_error+this->d_error;
}

double PID::GetSteering(){
	return TotalError();
}