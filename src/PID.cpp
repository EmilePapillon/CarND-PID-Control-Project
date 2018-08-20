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
	for(int i=0; i<3; i++){
		this->dp[i]=1;
	}
}

void PID::UpdateError(double cte) {

	/*
	* keep track of a sliding window of squared cte (moving average)
	* if the error is above a threshold, initialize twiddle tuning
	*/

	/*get the moving average of error squared*/
	if (this->sliding_window_size>999){
		throw "UpdateError: sliding window size exceeds maximum allocated size, please set sliding_window_size below 1000";
	}

	this->cte_sliding_window[this->it % this->sliding_window_size] = cte;
	double cte_avg =0;
	for (int i =0; i<this->sliding_window_size; i++){
		cte_avg+= pow([this->cte_sliding_window[i]],2);
	}
	cte_avg /= this->sliding_window_size;
	this->it++;

	if (cte_avg > cte_avg_twiddle_threshold){
		double params[3];
		TuneParameters(params,this->twiddle_tolerance,cte_avg);
		this->Kp = params[0];
		this->Ki = params[1];
		this->Kd = params[2];
	}

	this->Twiddler.twiddle(this,cte);

	double prev_cte = this->cte;
	this->cte = cte;
	double diff_cte = this->cte - prev_cte;
	this->int_cte += cte;

	this->p_error = - Kp *cte;
	this->i_error = - Ki *int_cte;
	this->d_error = - Kd *diff_cte;
}

/**
*This function implements twiddle for PID parameter tuning
*/
double* PID::TuneParameters(double* params, double tolerance, double cte_avg){
	double dp[] = this-> dp; //{1,1,1};
	double p[] = {0,0,0};
	double err = cte_avg;

	double sum=0;
	while (true){
		sum=0;
		for(int i =0; i<3; i++){
			sum+=this->dp[i];
		}
		if(sum>tolerance) break;

		for(int i=0; i<3; i++){
			p[i] += this->dp[i];
			//wait for n iterations ("run robot")
			wait_flag = true; 
			if(err<best_err){

			}

		}
	}
	//reset number of iterations since twiddle run and error moving average
	this->it = 0 ;
	for (int i = 0; i < this->sliding_window_size; i++);
		this->cte_sliding_window[i]=0.0;
}

double PID::TotalError() {
	return this->p_error+this->i_error+this->d_error;
}

double PID::GetSteering(){
	return TotalError();
}