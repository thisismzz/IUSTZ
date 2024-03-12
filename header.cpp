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

int Backpack :: getSpecificFoodItemCount(const Food& specificItem) const {
    int count = 0;                                           
    for (const auto& item : FoodItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;                            // return 
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
        if (item.first == specificItem) {               // item.first.getName()==specificItem.getname()
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificPermanentItemCount(const Permanent& specificItem) const {
    return count(PermanentItems.begin(), PermanentItems.end(), specificItem);
}

// *----------------------------------------------------------------*

void Backpack :: addFoodItems(const Food& foodItem, int quantity){
    for (auto &item : FoodItems) {
        if (item.first == foodItem) {
            item.second += quantity;
            return;
        }
    }
    FoodItems.push_back(make_pair(foodItem, quantity));
}
void Backpack :: addMedicineItems(const Medicine& medicineItem, int quantity){
    for (auto &item : MedicineItems) {
        if (item.first == medicineItem) {
            item.second += quantity;
            return;
        }
    }
    MedicineItems.push_back(make_pair(medicineItem, quantity));
}
void Backpack :: addThrowableItems(const Throwable& throwableItem, int quantity){
    for (auto &item : ThrowableItems) {
        if (item.first == throwableItem) {
            item.second += quantity;
            return;
        }
    }
    ThrowableItems.push_back(make_pair(throwableItem, quantity));
}
void Backpack :: addPermanentItems( const Permanent& permanentItem){
    auto it = find(PermanentItems.begin(), PermanentItems.end(), permanentItem);
    if (it == PermanentItems.end()) {
        PermanentItems.push_back(permanentItem);
    }
}

// *----------------------------------------------------------------*

void Backpack :: removeFoodItems(const Food& foodItem, int quantity) {
    for (auto it = FoodItems.begin(); it != FoodItems.end(); ) {
        if (it->first == foodItem && it->second == quantity) {
            it = FoodItems.erase(it);
        } else {
            ++it;
        }
    }
}
void Backpack :: removeMedicineItems(const Medicine& medicineItem, int quantity) {
    for (auto it = MedicineItems.begin(); it != MedicineItems.end(); ) {
        if (it->first == medicineItem && it->second == quantity) {
            it = MedicineItems.erase(it);
        } else {
            ++it;
        }
    }
}
void Backpack :: removeThrowableItems(const Throwable& throwableItem, int quantity) {
    for (auto it = ThrowableItems.begin(); it != ThrowableItems.end(); ) {
        if (it->first == throwableItem && it->second == quantity) {
            it = ThrowableItems.erase(it);
        } else {
            ++it;
        }
    }
}
void Backpack :: removePermanentItems(const Permanent& permanentItem){
    PermanentItems.erase(remove(PermanentItems.begin(), PermanentItems.end(), permanentItem), PermanentItems.end());
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

int Backpack :: getPermanentItemsCount() {
    return PermanentItems.size();
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

void Backpack :: clearPermanentItems() {
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