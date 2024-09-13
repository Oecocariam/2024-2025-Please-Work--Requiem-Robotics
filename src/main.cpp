#include "main.h"
#include <bitset>
using namespace std; 


	pros::Controller master (CONTROLLER_MASTER);

	pros::Motor left1 (1, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	pros::Motor left2 (2, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees); 
	pros::Motor left3 (3, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees); 
	pros::Motor right1 (9, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees); 
	pros::Motor right2 (10, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	pros::Motor right3 (12, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);  

	pros::Motor intake (7, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	bitset<1> intakeState(0);

	/*
		0th value: motor running
	*/

double averageMotorVoltage(){
	double x = (abs(left1.get_voltage()) + abs(left2.get_voltage()) + abs(right1.get_voltage()) + abs(right2.get_voltage()))/4;
	return x;
}

void drive (double distance, double speed ) { 

		double efficiency_modifier = 1.;

		double degreesTurned = (distance*efficiency_modifier/double(31.415))*360;

		pros::delay(2);

		double baseMotorVoltager = averageMotorVoltage();

		left1.move_relative(-degreesTurned, speed);
		left2.move_relative(-degreesTurned, speed);
		left3.move_relative(-degreesTurned, speed);

		right1.move_relative(degreesTurned, speed);
		right2.move_relative(degreesTurned, speed);	
		right3.move_relative(degreesTurned, speed);	

		pros::delay(100);

	while (1) {
    	pros::delay(2);

		if(averageMotorVoltage()<=baseMotorVoltager+1000){
			break;
		}
	}
}

void turn (double robot_degrees, double speed, int negatation) {

	double motor_degrees = (robot_degrees/0.1361)-(7.667*negatation);

	double baseMotorVoltager = averageMotorVoltage();

	left1.move_relative(motor_degrees, speed);
	left2.move_relative(motor_degrees, speed);
	left3.move_relative(motor_degrees, speed);

	right1.move_relative(motor_degrees, speed);
	right2.move_relative(motor_degrees, speed);
	right3.move_relative(motor_degrees, speed);

	pros::delay(100);

	while (1) {

		if(averageMotorVoltage()<=baseMotorVoltager+1000){
			break;
		}
	}
}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

//	wingLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
//	wingRight.set_brake_mode(MOTOR_BRAKE_HOLD);
//	launcher.set_brake_mode(MOTOR_BRAKE_COAST);

//	wingLeft.move_absolute(-40, 200);
//	pros::delay(200);
//	wingLeft.brake();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */	
}

void opcontrol() {



	
//	definition of piston, controller , and motors

		pros::lcd::initialize();

	while (true) {
		

		int leftControl = (master.get_analog(ANALOG_LEFT_X))+(-master.get_analog(ANALOG_LEFT_Y));
		int rightControl = (master.get_analog(ANALOG_LEFT_X))-(-master.get_analog(ANALOG_LEFT_Y));
		

		left1.move(leftControl);
		left2.move(leftControl);
		left3.move(leftControl);

		right1.move(rightControl);
		right2.move(rightControl);
		right3.move(rightControl);

		pros::delay(2);


		//control of intake

		switch(master.get_digital(DIGITAL_A)){
			case true:
			
				switch(intakeState.test(0)){
					
					case 1:

						intake.brake();
						intakeState.reset(0);

						break;

					case 0:

						intake.move(127);
						intakeState.set(0);

						break;
				}

				break;

			case false:

				break;
		}
			

//later use bit integer to simplify process
/*
		switch(master.get_digital(DIGITAL_R1)){

			case true:

			switch(rightWingState == 1){

				case true:

					wingRight.move_absolute(90, 200);

					rightWingState = -1;

					pros::delay(200);

				break;

				case false:

					wingRight.move_absolute(0, 200);

					rightWingState = 1;

					pros::delay(200);

				break;

			wingRight.brake();

			break;
			}
		}




		switch(master.get_digital(DIGITAL_L1)){

			case true:

			switch(leftWingState == 1){

				case true:

					wingLeft.move_absolute(140, 200);

					leftWingState = -1;

					pros::delay(200);

				break;

				case false:

					wingLeft.move_absolute(-20, 200);

					leftWingState = 1;

					pros::delay(200);

				break;

			wingLeft.brake();

			break;

			default:

				wingLeft.brake();

				break;
			}
		}
/*
		switch(master.get_digital(DIGITAL_A)) {

			case true:

			switch((rightWingState || leftWingState) == -1){

				case true:


					wingRight.move_absolute(0, 200);
					wingLeft.move_absolute(0, 200);
					
					leftWingState = 1;
					rightWingState = 1;

					pros::delay(200);

				break;

				case false:

					wingRight.move_absolute(90, 200);
					wingLeft.move_absolute(90, 200);

					leftWingState = -1;
					rightWingState = -1;

					pros::delay(200);

				break;

			};
				wingLeft.brake();
				wingRight.brake();

				break;

				case false:

					break;
				

		}
		*/
	}
}