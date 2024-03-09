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
        if(currentHealth <= 0)
            currentHealth = 0;
    }
    void Health::increaseHealth (int amount){
        currentHealth += amount;
        if(currentHealth >= maxHealth)
            currentHealth = maxHealth;
    }

Stamina ::Stamina(int Level) : level(Level), maximum(100*level),value(100*level) {}
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
}
    

Experience::Experience(int Level) : level(Level),maximum(100+(Level*50)),currentValue(0){};

void Experience::setMaximum(){
    maximum=100+(level*50);
}

void Experience::setCurrentValue(int selfDamage,int enemyDamage,int usedStamina){
    currentValue+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
    if(currentValue>=maximum){
        currentValue=0;
    }
}

BankAccount::BankAccount() : balance(0.0) {}
double BankAccount::getBalance() {
    return balance;
}
void BankAccount::deposit(double amount) { 
    balance += amount;
}
void BankAccount::withdraw(double amount) {
    if (balance >= amount)
        balance -= amount;
    else
        std::cout << "Insufficient funds" << std::endl;
}