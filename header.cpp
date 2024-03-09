#include<iostream>
#include<string>
#include<vector>
#include "header.h"

// Function And Class Method Decleration

Experience::Experience(int Level):level(Level),maximum(100+(Level*50)),currentValue(0){};

void Experience::setMaximum(){
    maximum=100+(level*50);
}

void Experience::setCurrentValue(int selfDamage,int enemyDamage,int usedStamina){
    currentValue+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
    if(currentValue>=maximum){
        currentValue=0;
    }
}