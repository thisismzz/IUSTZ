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

void Stamina ::updateMaximumStamina(){
        maximum+=100;
}

void Experience::updateMaximum(){
    maximum+=50;
}

void Experience::setCurrentValue(int selfDamage,int enemyDamage,int usedStamina){
    currentValue+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
    if(currentValue>=maximum){
        currentValue=0;
        updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
    }
}

void Backpack::removeItem(const std::string& itemName, int quantity) {
    for (auto& item : items) {
        if (item.first == itemName) {
            item.second -= quantity;
            if (item.second <= 0) {
                items.erase(std::remove_if(items.begin(), items.end(),
                                           [&](const auto& pair) { return pair.first == itemName; }), items.end());
            }
            break;
        }
    }

    if (std::find(singleUseItems.begin(), singleUseItems.end(), itemName) != singleUseItems.end()) {
        singleUseItems.erase(std::remove(singleUseItems.begin(), singleUseItems.end(), itemName), singleUseItems.end());
    }
}


int Backpack::getItemCount(const std::string& itemName) const {
    for (const auto& item : items) {
        if (item.first == itemName) {
            return item.second;
        }
    }
    return 0;
}
void Backpack::addItem(const std::string& itemName, int quantity) {
    for (auto &item: items) {
        if (item.first == itemName) {
            item.second += quantity;
            return;
        }
    }
}
int Backpack::getTotalItemsCount() const {
    int totalCount = 0;
    for (const auto& item : items) {
        totalCount += item.second;
    }
    return totalCount;
}

void Backpack::clear() {
    items.clear();
    singleUseItems.clear();
}
void Backpack::useItem(const std::string& itemName) {

    if (std::find(singleUseItems.begin(), singleUseItems.end(), itemName) != singleUseItems.end()) {
        removeItem(itemName, 1);
    }
    else {
        removeItem(itemName, 1);
    }
}