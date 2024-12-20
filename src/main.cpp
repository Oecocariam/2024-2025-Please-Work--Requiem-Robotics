#include "main.h"
#include <bitset>
#include <iostream>
using namespace std; 



	pros::Controller master (CONTROLLER_MASTER);


	pros::MotorGroup lefter ({17, 13,}, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	pros::MotorGroup righter ({11, -12}, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
	

	pros::MotorGroup intake ({7, -6}, pros::v5::MotorGears::red, pros::v5::MotorUnits::degrees);

	pros::Motor Braker(10, pros::v5::MotorGears::red, pros::v5::MotorUnits::degrees);


	pros::ADIDigitalOut pistonCapture ('A');

	string intakeState("00");
	/*
		0th value: motors running
		1st value: swing state
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
		Braker.set_brake_mode(MOTOR_BRAKE_COAST);
	while (true) {

		pros::lcd::clear_line(1);
		pros::lcd::set_text(1, intakeState);


		int leftControl = (master.get_analog(ANALOG_LEFT_X))+(-master.get_analog(ANALOG_LEFT_Y));
		int rightControl = (master.get_analog(ANALOG_LEFT_X))-(-master.get_analog(ANALOG_LEFT_Y));
		

		lefter.move(leftControl);

		righter.move((0.8)*rightControl);
		

		pros::delay(2);

		//control of intake

		if(master.get_digital(DIGITAL_B)){
			
				switch(intakeState.at(1)){
					
					case int('1'):

						Braker.set_brake_mode(MOTOR_BRAKE_COAST);
						intake.set_brake_mode_all(MOTOR_BRAKE_COAST);

						intakeState.replace(1, 1, 1, '0');
						pros::delay(300);
						break;

					case int('0'):

						Braker.set_brake_mode(MOTOR_BRAKE_HOLD);
						intake.set_brake_mode_all(MOTOR_BRAKE_HOLD);
						intake.brake();
						pros::delay(300);
						intakeState.replace(0, 1, 1, '0');
						intakeState.replace(1, 1, 1, '1');	
						break;

					default:
						intakeState.replace(1, 1, 1, 'f');
						break;
				}

			}



		if(master.get_digital(DIGITAL_A)){
				pistonCapture.set_value(!capture);
				capture = !capture;
				break;
		}
			
		if(master.get_digital(DIGITAL_L1)){


				switch(int((100*int(intakeState.at(0)))+int(intakeState.at(1)))){

					case 100*int('1')+ int('0'):

						intake.brake();
						intakeState.replace(0, 1, 1, '0');	
						pros::delay(300);
						break;

					case 100*int('0')+ int('0'):

						intake.move_voltage(6000);
						Braker.move_voltage(3000);
						intakeState.replace(0, 1, 1, '1');	
						pros::delay(300);
						break;

					case 100*int('0')+ int('1'):
						
						
						Braker.move_relative(50, 200);
						intakeState.replace(0, 1, 1, '1');		
						pros::delay(10);
						Braker.brake();
						break;

					case 100*int('1')+ int('1'):
						
						pros::delay(10);
						intakeState.replace(0, 1, 1, '0');	
						break;

					default:
						intakeState.replace(0, 1, 1, 'f');
						break;
				}
			}


		if(master.get_digital(DIGITAL_R1)){

				switch(int((100*int(intakeState.at(0)))+int(intakeState.at(1)))){

					case 100*int('0')+ int('1'):

						Braker.move_relative(-10, 200);
						intake.move_relative(100, 200);
						intakeState.replace(0, 1, 1, '1');		
						pros::delay(10);
						Braker.set_brake_mode(MOTOR_BRAKE_HOLD);
						break;

					case 100*int('1')+ int('1'):
						
						pros::delay(10);
						intakeState.replace(0, 1, 1, '0');	
						break;

					default:
						break;
					

			}

		}

		if(master.get_digital(DIGITAL_UP)){
			
			switch(int((100*int(intakeState.at(0)))+int(intakeState.at(1)))){

					case 100*int('0')+ int('0'):

						intake.move_relative(10, 200);
						intakeState.replace(0, 1, 1, '1');		
						pros::delay(5);
						break;

					case 100*int('1')+ int('0'):
						
						pros::delay(10);
						break;

					default:
						break;

			}

		}

		if(master.get_digital(DIGITAL_DOWN)){
			
			switch(int((100*int(intakeState.at(0)))+int(intakeState.at(1)))){

					case 100*int('0')+ int('0'):

						intake.move_relative(10, 200);
						intakeState.replace(0, 1, 1, '1');		
						pros::delay(5);
						break;

					case 100*int('1')+ int('0'):
						
						pros::delay(10);
						break;

					default:
						break;

			}

		}

	}		
		
}
