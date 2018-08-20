class Twiddler{
public: 
	const double TWIDDLE_INCREASE_FACTOR  = 1.1; 
	const double TWIDDLE_DECREASE_FACTOR = 0.9; 

	double best_err;
	double dp[3];
	int settle_iterations; //number of iterations allowed to settle (before calculating error)
	int twiddle_iterations; //number of total iteration for calculating error
	double tolerance;//parameter: tolerance of the twiddler

	enum twiddle_phase 
	{	p_tuning, 
		i_tuning, 
		d_tuning
	} current_tuning_phase; 

	enum increase_decrease_flag
	{
		increase = 1,
		decrease = -1
	} current_direction;

	Twiddle();

	virtual ~Twiddle();

	//methods
	void Init();

	bool refreshError(double err);

	void twiddle(PID*, cte);
}