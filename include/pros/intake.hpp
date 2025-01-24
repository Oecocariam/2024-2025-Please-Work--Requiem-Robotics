#include "main.h"
#include <iostream>



using namespace std; 

namespace pros {
    inline namespace v5 {
        class Intake{
            private:

                string intakeState;
                pros::Motor intaker;
                /*
                    0th value: motor running
                    1st value: position
                    2nd value: motors running continous
                */
            public:

                Intake(string iIntakeState, pros::Motor iIntaker){
                    intakeState = iIntakeState;
                    intaker = iIntaker;
                    intaker.set_voltage_limit(5500);
                };

                Intake(){
                    intakeState = "000";
                    intaker.set_voltage_limit(5500);
                };


                string getIntakeState(){
                    return intakeState;
                };

                void runContinous(int voltage){
                        intaker.move(voltage);
                }
        };
    }
}
