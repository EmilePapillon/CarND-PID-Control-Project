#include <numeric>

Twiddler::Twiddler() {}

Twiddler::~Twiddler() {}

void Twiddler::Init(){
	TWIDDLE_INCREASE_FACTOR  = 1.1; 
	TWIDDLE_DECREASE_FACTOR = 0.9; 
	this->best_err = numeric_limits<double>::max();
	for (int i=0; i<3; i++){
		this->dp[i]=1.0;
	}
	settle_iterations=200; 
	twiddle_iterations=1000;
	tolerance = 0.2;

	this->current_tuning_phase=p_tuning;
	this->current_direction=increase; 
}

bool Twiddler::refreshError(double err){
	if (err<this->best_err){
		this->best_err=err;
		return true;
	}
	return false;
}

void Twiddler::twiddle(PID*, cte){
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
			PID->params[current_tuning_phase] +=  dp[current_tuning_phase];
			this->iteration=0; //reset iterations, allowing for recalculation of error
			updating_phase=false; 
			return;
		} else {

			if ( err < best_err ) {
				best_err = err; 
				dp[current_tuning_phase]*= TWIDDLE_INCREASE_FACTOR;

				/* then switch to next parameter */
				current_tuning_phase = (current_tuning_phase + 1) % 3;
				updating_phase=true;
				this->iteration=0;
				return;

			} else {

				PID->params[current_tuning_phase] -=  2*dp[current_tuning_phase]; 
				this->iteration=0;
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
