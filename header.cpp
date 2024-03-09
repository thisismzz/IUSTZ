#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include "header.h"
using namespace std;

// Function And Class Method Decleration


    Health ::Health() : maxHealth(100),currentHealth(100){}
    int Health::getCurrentHealth(){
        return currentHealth;
    }
    int Health::getMaxHealth(){
        return maxHealth;
    }
    void Health::decreaseHealth(int damage){
        currentHealth -= damage;
        currentHealth = max(0, min(currentHealth,maxHealth));
    }
    void Health::increaseHealth (int amount){
        currentHealth += amount;
        currentHealth = min(maxHealth,currentHealth); 
    }

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