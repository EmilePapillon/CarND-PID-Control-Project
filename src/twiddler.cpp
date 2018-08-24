#include <numeric>
#include "PID.h"
#include "twiddler.h"
using namespace std;

#define TWIDDLE_INCREASE_FACTOR 1.1
#define TWIDDLE_DECREASE_FACTOR 0.9

Twiddler::Twiddler() {}

Twiddler::~Twiddler() {}

void Twiddler::Init(double tol){
	this->best_err = numeric_limits<double>::max();
	for (int i=0; i<3; i++){
		this->dp[i]=1.0;
	}
	settle_iterations=50; 
	twiddle_iterations=150;
	tolerance = tol;

	this->current_tuning_phase=p_tuning;
	this->current_direction=increase; 
	tried_subtraction = false;
	updating_phase = true;
	err=0;
}


void Twiddler::twiddle(PID* pid, double cte){
	if(this->iteration < settle_iterations){
		cout << "settle iteration : "<< 100*iteration/settle_iterations << endl;
		this->iteration++;
		return; //do nothing

	} else if(this->iteration < twiddle_iterations){
		cout << "error calculation : "<< 100*(iteration-settle_iterations)/twiddle_iterations << endl;
		/* calculate error */
		err += cte*cte;
		this->iteration++;
		return; //stop execution of function here, move to next iteration
	}

	double sum = 0;
	for (int i=0; i<3; i++){
		sum+=this->dp[i];
	}
	while (sum<this->tolerance){  //having a while loop is not right... it needs to be a for ... rethink this part
	//implement here
		cout << "total iterations: "<< iteration << endl;
		if (updating_phase)
		{
			pid->params[current_tuning_phase] +=  dp[current_tuning_phase];
			this->iteration=0; //reset iterations, allowing for recalculation of error
			updating_phase=false; 
			return;
		} else {

			if ( err < best_err ) {
				cout << "Error has lowered to: "<< err << endl;
				best_err = err; 
				dp[current_tuning_phase]*= TWIDDLE_INCREASE_FACTOR;

				/* then switch to next parameter */
				current_tuning_phase = static_cast<twiddle_phase>((current_tuning_phase + 1) % 3);
				tried_subtraction=false; //reset the tried_subtraction flag state
				updating_phase=true; //next iteration will increase the parameter value by dp
				this->iteration=0; //reset iterations so the error will be evaluated
				return;

			} else if (!tried_subtraction){
				cout << "Error has not lowered, trying to subtract. Error: "<< err << endl;

				pid->params[current_tuning_phase] -=  2*dp[current_tuning_phase];  //try subtracting dp
				this->iteration=0; //reset iterations so the error will be evaluated
				tried_subtraction = true; //next failure to decrease error will make parameter increments (dp) decrease
				return;

			} else {
				cout << "Error not lowered, decreasing twiddle factor: "<< err << endl;
				pid->params[current_tuning_phase] += dp[current_tuning_phase];
				dp[current_tuning_phase] *= TWIDDLE_DECREASE_FACTOR; //decrease dp
				updating_phase=true; //we go back to the begining (adding dp to the parameter)
				this->iteration=0; //reset iterations so the error will be evaluated
				return;
			}
		}

		//recalculate the sum
		sum = 0;
		for (int i=0; i<3; i++){
			sum+=this->dp[i];
		}

	}
}
