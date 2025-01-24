#include "main.h"
#include <iostream>



using namespace std; 

namespace pros {
    inline namespace v5 {
        class Intake{
            private:

                string intakeState;
                /*
                    0th value: motor running
                */
                pros::Motor intaker;
                int chainPosition;
                /*
                    total length of chain: 72

                    hook one at chain: 1
                        hook one ready positon: 39
                        hook one wall position: 71
                    hook two at chain: 24
                        hook two ready position: 60
                        hook one wall position: 22
                    hook three at chain: 49
                        hook three ready position: 3
                        hook one wall position: 47
                */

            public:

                Intake(string iIntakeState, pros::Motor iIntaker, int iChainPosition){
                    intakeState = iIntakeState;
                    intaker = iIntaker;
                    intaker.set_voltage_limit(5500);
                    chainPosition = iChainPosition;
                    intaker.set_encoder_units(pros::v5::MotorUnits::degrees);
                };

                Intake(string iIntakeState, pros::Motor iIntaker){
                    intakeState = iIntakeState;
                    intaker = iIntaker;
                    intaker.set_voltage_limit(5500);
                    chainPosition = 1;
                    intaker.set_encoder_units(pros::v5::MotorUnits::degrees);
                };

                Intake(pros::Motor iIntaker){
                    intakeState = "000";
                    intaker = iIntaker;
                    intaker.set_voltage_limit(5500);
                    chainPosition = 1;
                };

                Intake(){
                    intakeState = "000";
                    intaker.set_voltage_limit(5500);
                    chainPosition = 1;
                };


                string getIntakeState(){
                    return intakeState;
                };

                int getChainPosition(){
                    return chainPosition;
                }

                void runContinous(int velocity){
                    while(intaker.get_actual_velocity() > 10){
                        pros::delay(10);
                    }
                    intakeState.replace(0, 1, 1, '1');
                    while(intakeState.at(0)){
                        intaker.move_relative(22.5, velocity);
                            if(chainPosition<72){
                                chainPosition += 1;
                            }else{
                                chainPosition = 0;
                            }
                        pros::delay(100);
                    }
                    intaker.brake();
                }

                void stopRunning(){
                    intakeState.replace(0, 1, 1, '0');
                }

                void movePosition(int velocity, int position){
                    void stopRunning();
                    int chainsToMove;

                    if(chainPosition > position){
                        chainsToMove = (72-chainPosition) + position;
                        
                    }else if(position = chainPosition){
                        chainsToMove = 0;
                    }else{
                        chainsToMove = position-chainPosition;
                   }

                   intaker.move_relative(22.5*chainsToMove, 100);
                   if(chainsToMove +chainPosition > 72){
                    chainPosition = (chainPosition + chainsToMove) -72;
                   }else{
                        chainPosition += chainsToMove;
                   }       
                   pros::delay(100);
                }

                void readyHook(int velocity, bool wall){
                    void stopRunning();
                    if(wall){
                        if(71>chainPosition>47){
                            movePosition(velocity, 71);
                            
                        }else if(47>chainPosition>22){
                            movePosition(velocity, 47);
                        }else{
                            movePosition(velocity, 22);
                        }
                    }else{
                        if(39>chainPosition>60){
                            movePosition(velocity, 60);
                        }else if(60>chainPosition>3){
                            movePosition(velocity, 3);
                        }else{
                            movePosition(velocity, 39);
                        }
                    }

                    pros::delay(1000);
                }
        };
    }
}
