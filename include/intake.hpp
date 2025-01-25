#include "main.h"
#include <iostream>

#ifndef _INTAKE_HPP_
#define _INTAKE_HPP_

using namespace std; 

namespace pros {
    inline namespace v5 {
        class Intake{
            private:

                string intakeState;
                /*
                    0th value: motor running
                */
//                pros::Motor intaker;
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

                Intake(string iIntakeState, int iChainPosition){
                    intakeState = iIntakeState;
//
                    chainPosition = iChainPosition;
                };

                Intake(string iIntakeState){
                    intakeState = iIntakeState;
//                    intaker = iIntaker;
                    chainPosition = 1;
                };

                Intake(){
                    intakeState = "000";
//                    intaker = iIntaker;

                    chainPosition = 1;
                };




                string getIntakeState(){
                    return intakeState;
                };

                int getChainPosition(){
                    return chainPosition;
                }

                void runContinous(int velocity,  pros::Motor iIntaker){

                    intakeState.replace(0, 1, 1, '1');
                    while((intakeState.at(0)=  1)){
                        iIntaker.move_relative(1080, velocity);
                            if(chainPosition<72){
                                chainPosition += 24;
                                
                            }else{
                                chainPosition = 0;
                            }
                        pros::delay(1000);
                    }
                    
                    iIntaker.brake();
                }

                void stopRunning(){
                    intakeState.replace(0, 1, 1, '0');
                }

                void movePosition(int velocity, int position,  pros::Motor iIntaker){
                    void stopRunning();
                    int chainsToMove;

                    if(chainPosition > position){
                        chainsToMove = (71-chainPosition) + position;
                        
                    }else if(position = chainPosition){
                        chainsToMove = 0;
                    }else{
                        chainsToMove = position-chainPosition;
                   }

                   iIntaker.move_relative(22.5*chainsToMove, 200);
                   if(chainsToMove +chainPosition > 71){
                    chainPosition = (chainPosition + chainsToMove) -71;
                   }else{
                        chainPosition += chainsToMove;
                   }       
                   pros::delay(100);
                }

                void readyHook(int velocity, bool wall,  pros::Motor iIntaker){
                    void stopRunning();
                    if(wall){

                        if(71>chainPosition>47){
                            movePosition(velocity, 71,   iIntaker); 
                        }else if(47>chainPosition>22){
                            movePosition(velocity, 47,  iIntaker);
                        }else{
                            movePosition(velocity, 22,  iIntaker);
                        }

                    }else{

                        if(39>chainPosition>60){
                            movePosition(velocity, 60,  iIntaker);
                        }else if(60>chainPosition>3){
                            movePosition(velocity, 3,  iIntaker);
                        }else{
                            movePosition(velocity, 39,  iIntaker);
                        }
                        
                    }

                    pros::delay(1000);
                }
        };
    }
}
#endif 