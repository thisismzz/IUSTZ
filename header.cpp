#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include <thread>
#include <chrono>

#include "header.h"

using namespace std;

// Function And Class Method Decleration

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Person::Person(string n) : name(n), level(1){}

void Person :: updateLevel(){
    level++;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BankAccount* Player :: getBankAccount(){
    BankAccount* p = &bankAccount;
    return p;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Health :: Health() : maxHealth(100),currentHealth(100){}

void Health :: decreaseHealth(int damage){
    currentHealth -= damage;
    if(currentHealth <= 0)
        currentHealth = 0;
}
void Health :: increaseHealth (int amount){
    currentHealth += amount;
    if(currentHealth >= maxHealth)
        currentHealth = maxHealth;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Stamina :: Stamina():maximum(100),currentStamina(100){}

void Stamina :: decreaseStamina(int amount) {
    currentStamina -= amount;
    if (currentStamina < 0)
        currentStamina = 0;
}
void Stamina :: increaseStamina(int amount) {
    currentStamina += amount;
    if ( currentStamina > maximum)
        currentStamina = maximum;
}
void Stamina :: updateMaximumStamina(){
    maximum+=100;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Experience :: Experience(Human *h):humanObj(h),maximum(100),currentExp(0){}

void Experience :: updateMaximum(){
    maximum+=50;
}
void Experience :: setCurrentExp(int selfDamage,int enemyDamage,int usedStamina){
    currentExp+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
    if(currentExp>=maximum){
        currentExp=0;
        updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
    }
}
void Experience :: increaseExp(int amount){
    currentExp+=amount;
    if(currentExp>=maximum){
        currentExp=0;
        humanObj->exp.updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Skills::Skills(int n,int m):maximum(5),currentSkill(n),upgradePrice(m){}

void Skills::upgradeSkill(BankAccount* creditcard) {
    try{
        if(creditcard->getBalance()>=upgradePrice){
            creditcard->withdraw(upgradePrice);
            currentSkill++;
            setUpgradePrice();
        }
        else{
            throw 0;
        }
    }
    catch(int temp){
        cout<<"not enough money!\n";
    } 
}

void Skills::setUpgradePrice(){
    upgradePrice+=10;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

WarmWeaponAbility::WarmWeaponAbility(int n):Skills(n,n*10){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ColdWeaponAbility::ColdWeaponAbility(int n):Skills(n,n*15){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ThrowableWeaponAbility::ThrowableWeaponAbility(int n):Skills(n,n*20){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack :: addFoodItem(const Food& foodItem, int quantity){
    for (auto &item : FoodItems) {
        if (item.first == foodItem) {
            item.second += quantity;
            return;
        }
    }
    FoodItems.push_back(make_pair(foodItem, quantity));
}

void Backpack :: addMedicineItem(const Medicine& medicineItem, int quantity){
    for (auto &item : MedicineItems) {
        if (item.first == medicineItem) {
            item.second += quantity;
            return;
        }
    }
    MedicineItems.push_back(make_pair(medicineItem, quantity));
}

void Backpack :: addThrowableItem(const Throwable& throwableItem, int quantity){
    for (auto &item : ThrowableItems) {
        if (item.first == throwableItem) {
            item.second += quantity;
            return;
        }
    }
    ThrowableItems.push_back(make_pair(throwableItem, quantity));
}

void Backpack :: addWarmWeaponItem(const WarmWeapon& WarmWeaponItem, int quantity){
    for (auto &item : WarmWeaponItems) {
        if (item.first == WarmWeaponItem) {
            item.second += quantity;
            return;
        }
    }
    WarmWeaponItems.push_back(make_pair(WarmWeaponItem, quantity));
}

void Backpack :: addColdWeaponItem(const ColdWeapon& ColdWeaponItem, int quantity){
    for (auto &item : ColdWeaponItems) {
        if (item.first == ColdWeaponItem) {
            item.second += quantity;
            return;
        }
    }
    ColdWeaponItems.push_back(make_pair(ColdWeaponItem, quantity));
}

// *----------------------------------------------------------------*

void Backpack :: removeFoodItem(const Food& foodItem, int quantity) {
    for (auto it = FoodItems.begin(); it != FoodItems.end(); ) {
        if (it->first == foodItem && it->second == quantity) {
            it = FoodItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeMedicineItem(const Medicine& medicineItem, int quantity) {
    for (auto it = MedicineItems.begin(); it != MedicineItems.end(); ) {
        if (it->first == medicineItem && it->second == quantity) {
            it = MedicineItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeThrowableItem(const Throwable& throwableItem, int quantity) {
    for (auto it = ThrowableItems.begin(); it != ThrowableItems.end(); ) {
        if (it->first == throwableItem && it->second == quantity) {
            it = ThrowableItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeWarmWeaponItem(const WarmWeapon& WarmWeaponItem, int quantity) {
    for (auto it = WarmWeaponItems.begin(); it != WarmWeaponItems.end(); ) {
        if (it->first == WarmWeaponItem && it->second == quantity) {
            it = WarmWeaponItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeColdWeaponItem(const ColdWeapon& ColdWeaponItem, int quantity) {
    for (auto it = ColdWeaponItems.begin(); it != ColdWeaponItems.end(); ) {
        if (it->first == ColdWeaponItem && it->second == quantity) {
            it = ColdWeaponItems.erase(it);
        } else {
            ++it;
        }
    }
}

// *----------------------------------------------------------------*

int Backpack :: getFoodItemsCount() {
    int total = 0;
    for (const auto& item : FoodItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getMedicineItemsCount() {
    int total = 0;
    for (const auto& item : MedicineItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getThrowableItemsCount() {
    int total = 0;
    for (const auto& item : ThrowableItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getWarmWeaponItemsCount() {
    int total = 0;
    for (const auto& item : WarmWeaponItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getColdWeaponItemsCount() {
    int total = 0;
    for (const auto& item : ColdWeaponItems) {
        total += item.second;
    }
    return total;
}

// *----------------------------------------------------------------*

int Backpack :: getSpecificFoodItemCount(const Food& specificItem) const {
    int count = 0;                                           
    for (const auto& item : FoodItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificMedicineItemCount(const Medicine& specificItem) const {
    int count = 0;
    for (const auto& item : MedicineItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificThrowableItemCount(const Throwable& specificItem) const {
    int count = 0;
    for (const auto& item : ThrowableItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificWarmWeaponItemCount(const WarmWeapon& specificItem) const {
    int count = 0;
    for (const auto& item : WarmWeaponItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificColdWeaponItemCount(const ColdWeapon& specificItem) const {
    int count = 0;
    for (const auto& item : ColdWeaponItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

// *----------------------------------------------------------------*

void Backpack :: clearFoodItems() {
    FoodItems.clear();
}

void Backpack :: clearMedicineItems() {
    MedicineItems.clear();
}

void Backpack :: clearThrowableItems() {
    ThrowableItems.clear();
}

void Backpack :: clearWarmWeaponItems() {
    WarmWeaponItems.clear();
}

void Backpack :: clearColdWeaponItems() {
    ColdWeaponItems.clear();
}

// *----------------------------------------------------------------*

void Backpack :: useFoodItemCount(const Food& specificItem, int quantity) {
    for (auto& item : FoodItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

void Backpack :: useMedicineItemCount(const Medicine& specificItem, int quantity) {
    for (auto& item : MedicineItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

void Backpack :: useThrowableItemCount(const Throwable& specificItem, int quantity) {
    for (auto& item : ThrowableItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

void Backpack :: useWarmWeaponItemCount(const WarmWeapon& specificItem, int quantity) {
    for (auto& item : WarmWeaponItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

void Backpack :: useColdWeaponItemCount(const ColdWeapon& specificItem, int quantity) {
    for (auto& item : ColdWeaponItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}


// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BankAccount :: BankAccount(){
    this->balance = 500;
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

Items::Items(string n,int p):name(n),price(p){}

bool Items::operator==(const Items& other) const {
    return name == other.name;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void printWithDelay() {
    int delay_ms = 20;
    string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tincidunt tristique maximus. Sed eget massa nec sem accumsan sagittis in a nisl. Cras pellentesque, est in feugiat consequat, orci magna tempus lorem, id pretium nibh nisl in tellus.";
    for (char c : text) {
        cout << c;
        cout.flush();  // Ensure the character is immediately printed
        this_thread :: sleep_for(chrono::milliseconds(delay_ms));
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*