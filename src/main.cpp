#include "main.h"
#include <bitset>
using namespace std; 



	pros::Controller master (CONTROLLER_MASTER);


	pros::MotorGroup lefter ({1, 2, 3}, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	pros::MotorGroup righter ({9, 10, 12}, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	

	pros::MotorGroup intake ({7, 8}, pros::v5::MotorGears::red, pros::v5::MotorUnits::degrees);

	pros::Motor Braker(9, pros::v5::MotorGears::red, pros::v5::MotorUnits::degrees);

	pros::ADIDigitalOut pistonIntake ('A');
	pros::ADIDigitalOut pistonCapture ('B');

	bitset<1> intakeState(0);
	/*
		0th value: motors running
		1st value: Brake state
	*/

	bool capture = false;
	


double averageMotorVoltage(pros::MotorGroup& motors){
	double x;
	int motorCount = motors.get_port_all().size();
	std::int32_t voltage;

	voltage = motors.get_voltage();
	x = voltage/motorCount;
	
	return x;
}

void drive (double distance, double speed ) { 

		double efficiency_modifier = 1.;

		double degreesTurned = (distance*efficiency_modifier/double(31.415))*360;

		pros::delay(2);

		double baseMotorVoltager = (averageMotorVoltage(lefter)+averageMotorVoltage(righter))/2;

		lefter.move_relative(-degreesTurned, speed);

		righter.move_relative(degreesTurned, speed);
		
		pros::delay(100);

	while (1) {
    	pros::delay(2);

		if(((averageMotorVoltage(lefter)+averageMotorVoltage(righter))/2)<=baseMotorVoltager+1000){
			break;
		}
	}
}

void turn (double robot_degrees, double speed, int negatation) {

	double motor_degrees = (robot_degrees/0.1361)-(7.667*negatation);

	double baseMotorVoltager = (averageMotorVoltage(lefter)+averageMotorVoltage(righter))/2;

	lefter.move_relative(motor_degrees, speed);

	righter.move_relative(motor_degrees, speed);

	pros::delay(100);

	while (1) {

		if(((averageMotorVoltage(lefter)+averageMotorVoltage(righter))/2)<=baseMotorVoltager+1000){
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
	intake.set_brake_mode_all(MOTOR_BRAKE_HOLD);
	Braker.brake();
	Braker.set_brake_mode(MOTOR_BRAKE_COAST);
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
	Braker.brake();

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
		Braker.brake();
	while (true) {


		//control of intake

		switch(master.get_digital(DIGITAL_A)){
			case true:
			
				switch(intakeState.test(1)){
					
					case 1:

						Braker.set_brake_mode(MOTOR_BRAKE_COAST);

						intakeState.reset(1);
						pros::delay(10);
						break;

					case 0:

						Braker.set_brake_mode(MOTOR_BRAKE_HOLD);
						intake.brake();
						pros::delay(10);
						intakeState.reset(0);
						intakeState.set(1);	
						break;
				}

				break;

			case false:

				break;
		}

		switch(master.get_digital(DIGITAL_B)){
			case true:
				pistonCapture.set_value(!capture);
				capture = !capture;
				break;

			case false:

				break;
		}
			
		switch(master.get_digital(DIGITAL_L1)){
			case true:

				switch((10*intakeState.test(0))+(intakeState.test(1))){

					case 10:

						intake.brake();
						intakeState.reset(0);	
						pros::delay(10);
						break;

					case 00:

						intake.move_voltage(6000);
						intakeState.set(0);	
						pros::delay(10);
						break;

					case 01:

						intake.move_relative(1, 200);
						intakeState.set(0);	
						pros::delay(10);
						break;

					case 11:
						
						pros::delay(10);
						break;

				}
			case false:
				break;
		}
	}
}