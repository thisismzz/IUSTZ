#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

#include "header.h"

using namespace std;

// Function And Class Method Decleration

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
Person :: Person(string name){
    this-> name = name;
    this-> level = 1;
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
Human :: Human(){}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
Player :: Player(int age , string userName , string gender){
    this-> age = age;
    this-> userName = userName;
    this-> gender = gender;
}
void Player :: updateLevel(){
    this->level = this->level + 1;
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
Health :: Health(Human &human){
    this->maxHealth = 100;
    this->currentHealth = 100;
    humanObj = &human;
}
int Health :: getCurrentHealth() const{
    return currentHealth;
}
int Health :: getMaxHealth() const{
    return maxHealth;
}
void Health :: setHealth(int newHealth){
    currentHealth = newHealth;
}
void Health :: decrease(int damage){
    currentHealth -= damage;
    if(currentHealth <= 0)
        currentHealth = 0;
}
void Health :: increase (int amount){
    currentHealth += amount;
    if(currentHealth >= maxHealth)
        currentHealth = maxHealth;
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
Stamina :: Stamina(Human &human){
    this->currentValue = 100;
    this->maximum = 100;
    humanObj = &human;
}
int Stamina :: getValue() const {
    return currentValue;
}
int Stamina :: getMaximum() const {
    return maximum;
}
void Stamina :: setValue(int newVal) {
    currentValue = newVal;
}
void Stamina :: decrease(int amount) {
    currentValue -= amount;
    if (currentValue < 0)
        currentValue = 0;
}
void Stamina :: increase(int amount) {
    currentValue += amount;
    if ( currentValue > maximum)
        currentValue = maximum;
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
Experience :: Experience(Human& human) {
    this->currentValue = 0;
    this->maximum = 100;
    humanObj = &human;
}
void Experience :: updateMaximumExperience(){
    this->maximum += 50;
}
int Experience :: getValue() const {
    return currentValue;
}
int Experience :: getMaximum() const {
    return maximum;
}
void Experience::setCurrentValue(int selfDamage, int enemyDamage, int usedStamina) {
    currentValue += (0.5 * selfDamage) + (0.2 * enemyDamage) + (0.3 * usedStamina);
    if (currentValue >= maximum) {
        currentValue = 0;
        updateMaximumExperience();
        humanObj->stamina.updateMaximumStamina();
        humanObj->updateLevel();
    }
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
BankAccount :: BankAccount(){
    this->balance = 0;
}
int BankAccount :: getBalance() {
    return balance;
}
void BankAccount :: deposit(int amount) { 
    this->balance += amount;
}
void BankAccount::withdraw(int amount) {
    if (balance >= amount)
        balance -= amount;
    else
        cout << "Insufficient funds" << endl;
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
int Backpack :: getSpecificFoodItemCount(vector<pair<Food , int>>& FoodItems, const Food& specificItem) const {
    int count = 0;
    for (const auto& item : FoodItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}
int Backpack :: getSpecificMedicineItemCount(vector<pair<Medicine , int>>& MedicineItems, const Medicine& specificItem) const {
    int count = 0;
    for (const auto& item : MedicineItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}
int Backpack :: getSpecificThrowableItemCount(vector<pair<Throwable , int>>& ThrowableItems, const Throwable& specificItem) const {
    int count = 0;
    for (const auto& item : ThrowableItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}
int Backpack :: getSpecificPermanentItemCount(vector<Permanent>& PermanentItems, const Permanent& specificItem) const {
    return count(PermanentItems.begin(), PermanentItems.end(), specificItem);
}
// *----------------------------------------------------------------*
void Backpack :: addFoodItems(vector<pair<Food , int>>& FoodItems ,const Food& foodItem, int quantity){
    for (auto &item : FoodItems) {
        if (item.first == foodItem) {
            item.second += quantity;
            return;
        }
    }
    FoodItems.push_back(make_pair(foodItem, quantity));
}
void Backpack :: addMedicineItems(vector<pair<Medicine , int>>& MedicineItems ,const Medicine& medicineItem, int quantity){
    for (auto &item : MedicineItems) {
        if (item.first == medicineItem) {
            item.second += quantity;
            return;
        }
    }
    MedicineItems.push_back(make_pair(medicineItem, quantity));
}
void Backpack :: addThrowableItems(vector<pair<Throwable , int>>& ThrowableItems ,const Throwable& throwableItem, int quantity){
    for (auto &item : ThrowableItems) {
        if (item.first == throwableItem) {
            item.second += quantity;
            return;
        }
    }
    ThrowableItems.push_back(make_pair(throwableItem, quantity));
}
void Backpack :: addPermanentItems(vector<Permanent>& PermanentItems, const Permanent& permanentItem){
    auto it = find(PermanentItems.begin(), PermanentItems.end(), permanentItem);
    if (it == PermanentItems.end()) {
        PermanentItems.push_back(permanentItem);
    }
}
// *----------------------------------------------------------------*
void Backpack :: removeFoodItems(vector<pair<Food , int>>& FoodItems ,const Food& foodItem, int quantity) {
    for (auto it = FoodItems.begin(); it != FoodItems.end(); ) {
        if (it->first == foodItem && it->second == quantity) {
            it = FoodItems.erase(it);
        } else {
            ++it;
        }
    }
}
void Backpack :: removeMedicineItems(vector<pair<Medicine , int>>& MedicineItems ,const Medicine& medicineItem, int quantity) {
    for (auto it = MedicineItems.begin(); it != MedicineItems.end(); ) {
        if (it->first == medicineItem && it->second == quantity) {
            it = MedicineItems.erase(it);
        } else {
            ++it;
        }
    }
}
void Backpack :: removeThrowableItems(vector<pair<Throwable , int>>& ThrowableItems ,const Throwable& throwableItem, int quantity) {
    for (auto it = ThrowableItems.begin(); it != ThrowableItems.end(); ) {
        if (it->first == throwableItem && it->second == quantity) {
            it = ThrowableItems.erase(it);
        } else {
            ++it;
        }
    }
}
void Backpack :: removePermanentItems(vector<Permanent>& PermanentItems ,const Permanent& permanentItem){
    PermanentItems.erase(remove(PermanentItems.begin(), PermanentItems.end(), permanentItem), PermanentItems.end());
}
// *----------------------------------------------------------------*
int Backpack :: getFoodItemsCount(vector<pair<Food , int>>& FoodItems) {
    int total = 0;
    for (const auto& item : FoodItems) {
        total += item.second;
    }
    return total;
}
int Backpack :: getMedicineItemsCount(vector<pair<Medicine , int>>& MedicineItems) {
    int total = 0;
    for (const auto& item : MedicineItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getThrowableItemsCount(vector<pair<Throwable , int>>& ThrowableItems) {
    int total = 0;
    for (const auto& item : ThrowableItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getPermanentItemsCount(vector<Permanent>& PermanentItems) {
    return PermanentItems.size();
}
// *----------------------------------------------------------------*
void Backpack :: clearFoodItems(vector<pair<Food , int>>& FoodItems) {
    FoodItems.clear();
}

void Backpack :: clearMedicineItems(vector<pair<Medicine , int>>& MedicineItems) {
    MedicineItems.clear();
}

void Backpack :: clearThrowableItems(vector<pair<Throwable , int>>& ThrowableItems) {
    ThrowableItems.clear();
}

void Backpack :: clearPermanentItems(vector<Permanent>& PermanentItems) {
    PermanentItems.clear();
}
// *----------------------------------------------------------------*
// void Backpack::useItem(const string& itemName) {
//     if (find(singleUseItems.begin(), singleUseItems.end(), itemName) != singleUseItems.end()) {
//         removeItem(itemName, 1);
//     }
//     else {
//         removeItem(itemName, 1);
//         }
// }
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
