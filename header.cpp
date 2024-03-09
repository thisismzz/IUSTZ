#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include "header.h"
using namespace std;

// Function And Class Method Decleration

    Health ::Health(int maxHP) : maxHealth(maxHP),currentHealth(maxHP){}
    int Health::getCurrentHealth(){
        return currentHealth;
    }
    int Health::getMaxHealth(){
        return maxHealth;
    }
    void Health::setCurrentHealt(int health){
        currentHealth = health;
        currentHealth = max(0, min(currentHealth,maxHealth));
    }
    void Health::increaseHealth (int amount){
        currentHealth += amount;
        currentHealth = min(maxHealth,currentHealth); 
    }