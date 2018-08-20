#include <numeric>
#include "twiddler.h"
using namespace std;

Twiddler::Twiddler() {}

Twiddler::~Twiddler() {}

void Twiddler::Init(double tol){
	TWIDDLE_INCREASE_FACTOR  = 1.1; 
	TWIDDLE_DECREASE_FACTOR = 0.9; 
	this->best_err = numeric_limits<double>::max();
	for (int i=0; i<3; i++){
		this->dp[i]=1.0;
	}
	settle_iterations=200; 
	twiddle_iterations=1000;
	tolerance = tol;

	this->current_tuning_phase=p_tuning;
	this->current_direction=increase; 
	bool tried_subtraction = false;
}

bool Twiddler::refreshError(double err){
	if (err<this->best_err){
		this->best_err=err;
		return true;
	}
	return false;
}

void Twiddler::twiddle(PID* pid, double cte){
	if(this->iteration < settle_iterations){
		this->iteration++;
		return; //do nothing

	} else if(this->iteration < twiddle_iterations){
		/* calculate error */
		err += cte*cte;
		this->iteration++;
		return; //stop execution of function here, move to next iteration
	}

	double sum = 0;
	for (int i=0; i<3; i++){
		sum+=this->dp[i]
	}
	while (sum<this->tolerance){
	//implement here
		
		if (updating_phase)
		{
			pid->params[current_tuning_phase] +=  dp[current_tuning_phase];
			this->iteration=0; //reset iterations, allowing for recalculation of error
			updating_phase=false; 
			return;
		} else {

			if ( err < best_err ) {
				cout << "Error has lowered to "<< err << endl;
				best_err = err; 
				dp[current_tuning_phase]*= TWIDDLE_INCREASE_FACTOR;

				/* then switch to next parameter */
				current_tuning_phase = (current_tuning_phase + 1) % 3;
				tried_subtraction=false; //reset the tried_subtraction flag state
				updating_phase=true; //next iteration will increase the parameter value by dp
				this->iteration=0; //reset iterations so the error will be evaluated
				return;

			} else if (!tried_subtraction){

				pid->params[current_tuning_phase] -=  2*dp[current_tuning_phase];  //try subtracting dp
				this->iteration=0; //reset iterations so the error will be evaluated
				tried_subtraction = true; //next failure to decrease error will make parameter increments (dp) decrease
				return;

			} else {
				params[current_tuning_phase] += dp[current_tuning_phase];
				dp[current_tuning_phase] *= TWIDDLE_DECREASE_FACTOR; //decrease dp
				updating_phase=true; //we go back to the begining (adding dp to the parameter)
				this->iteration=0; //reset iterations so the error will be evaluated
				return;
			}
		}

		//recalculate the sum
		sum = 0;
		for (int i=0; i<3; i++){
			sum+=this->dp[i]
		}

	}
}
