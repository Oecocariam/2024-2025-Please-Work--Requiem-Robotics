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
                    1st value: position
                    2nd value: motors running continous
                */
            public:

                Intake(string iIntakeState){
                    intakeState = iIntakeState;
                };

                Intake(){
                    intakeState = "000";
                };



        };
    }
}