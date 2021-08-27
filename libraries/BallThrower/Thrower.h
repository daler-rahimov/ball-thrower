/*
  Thrower.cpp - Library for controling ball Thrower.
  Created by Daler Rahimov
*/
#include <DCMotor.h>

#ifndef Thrower_h 
#define Thrower_h 

class Thrower 
{
public:

	/*!
		@brief	Constructor 
		@param	upMotor top motor 
		@param	downMotro bottom motor  

	*/
	Thrower(Motor &upMotor, Motor &downMotor);
	~Thrower();

	// Only use positive integer with +1 incroment 
	enum Constants
	{
		E_First,// this is used for interating over values
		E_None = E_First,// this is used for interating over values
		FLAT_SHOT =		 		 1,
		TOPSPIN_SHOT =			 2,
		DROP_SHOT =				 3,
		PASSING_SHOT =			 4,
		SLICE_SHOT =			 5,
		LOB_SHOT =			 	 6,
		SERVE_SHOT =			 7,
		STOP_SHOT = 			 8, 
		E_Last, // this is used for interating over values
	};

	/*!
		@brief	Set a shot that needs to be palyed. Call playShot() to actually play it
		@return	shot number if set, -1 if not implemented 
	*/
	int setShot(int shot);

	/*!
		@brief	play currently set shot 
	*/
	void playShot();

	/*!
		@brief	Return current shot being played 
		@returns int value of current shot being played 
	*/
	int getPlayingShot();
	
	/*!
		@brief	 Check if SHOT is implemented 
		@note	 There are some SHOTs that are listed in the docs 
		but not implemeted 
	*/
	bool isModeImplemented(int shot);

	/*!
		@brief Set power of all the shots given in presentile e.g. 50% 0.5 
	*/
	void setShotPower(double shotPowerPersentile = 1.0);
	
	/*!
		@brief Get power of all the shots returned in presentile e.g. 50% 0.5 
	*/
	double getShotPower();

	/*!
		@brief Set spin given in presentile from -1.0(max backspin) to +1.0 (max top spin)
	*/
	void setSpin(double spinPersentile = 0.0);
	
	/*!
		@brief Get spin returned in presentile from -1.0(max backspin) to +1.0 (max top spin)
	*/
	double getSpin();

private:
	Motor *_downMotorThrower;
	Motor *_upMotorThrower;
	int _currentShot=-1;
	double shotPowerInPersentile = 1.0;
	uint8_t ballMotorPower = DCMotor::MAX_SPEED;


	/*!
		@brief stop all shot and stop all motors 
	*/
	void stopShot();

	/*!
		@brief Set spin for the ball  
		@param spin from -255 to 255, backspin to topspin 
	*/
	void setSpin(int spin);

	/*!
		@brief play flat shot 
	*/
	void flatShot();
	
	/*!
		@brief play topspin shot  
	*/
	void topspinShot();

	// /*!
	// 	@brief play drop shot  
	// */
	// void dropShot();

	// /*!
	// 	@brief play passing shot  
	// */
	// void passingShot();

	// /*!
	// 	@brief play slice shot  
	// */
	// void sliceShot();

	// /*!
	// 	@brief play lob shot  
	// */
	// void lobShot();

	// /*!
	// 	@brief play serve shot  
	// */
	// void serveShot();

};
#endif
