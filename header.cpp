#include<iostream>
#include<string>
#include <algorithm>
#include<vector>
#include<algorithm>
#include "header.h"
using namespace std;

<<<<<<< HEAD
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
=======
Stamina ::Stamina(int val , int max) : value(val) , maximum(max) {}
int Stamina ::getValue() const {
    return value;
}
void Stamina ::setValue(int val) {
    value = val;
}
void Stamina ::decrease(int amount) {
    value -= amount;
    if (value < 0)
        value = 0;
}
void Stamina ::increase(int amount) {
    value += amount;
    if ( value > maximum)
        value = maximum;
}
int Stamina ::getMaximum() const {
    return maximum;
>>>>>>> 9dbc8b9391765a30b5d754c8ce52d8b3a056a58f
}
