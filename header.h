#ifndef HEADER
#define HEADER
#include<iostream>
#include<string>
#include<vector>

using namespace std;
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Health {
    protected:
        int currentHealth;
        int maxHealth;
        Human* humanObj;
    public:
        Health(Human &human);
        int getCurrentHealth() const;
        int getMaxHealth() const;
        void setHealth(int newHealth);
        void decrease(int damage);
        void increase(int amount);
};
// *----------------------------------------------------------------*
class Stamina {
    protected:
        int currentValue;
        int maximum;
        Human* humanObj;
    public:
        Stamina(Human &human);
        int getValue() const;
        int getMaximum() const;
        void setValue(int newVal);
        void decrease(int amount);
        void increase(int amount);
        void updateMaximumStamina();
};
// *----------------------------------------------------------------*
class Experience {
    protected:
        int maximum;
        int currentValue;
        Human* humanObj;
    public:
        Experience(Human &human);
        int getValue() const;
        int getMaximum() const;
        void updateMaximumExperience();
        void setCurrentValue(int,int,int);
};
// *----------------------------------------------------------------*
class Skills {};
// *----------------------------------------------------------------*
class WarmWeaponAbility : public Skills {};
// *----------------------------------------------------------------*
class ColdWeaponAbility : public Skills {};
// *----------------------------------------------------------------*
class ThrowableWeaponAbility : public Skills {};
// *----------------------------------------------------------------*
class Backpack {
    protected:
        vector<pair<Food , int>> FoodItems;
        vector<pair<Medicine , int>> MedicineItems;
        vector<pair<Throwable , int>> ThrowableItems;
        vector<Permanent> PermanentItems;
    public:
        void addFoodItems(vector<pair<Food , int>>& FoodItems ,const Food& foodItem, int quantity);
        void addMedicineItems(vector<pair<Medicine , int>>& MedicineItems ,const Medicine& medicineItem, int quantity);
        void addThrowableItems(vector<pair<Throwable , int>>& ThrowableItems ,const Throwable& throwableItem, int quantity);
        void addPermanentItems(vector<Permanent>& PermanentItems ,const Permanent& permanentItem);

        void removeFoodItems(vector<pair<Food , int>>& FoodItems ,const Food& foodItem, int quantity);
        void removeMedicineItems(vector<pair<Medicine , int>>& MedicineItems ,const Medicine& medicineItem, int quantity);
        void removeThrowableItems(vector<pair<Throwable , int>>& ThrowableItems ,const Throwable& throwableItem, int quantity);
        void removePermanentItems(vector<Permanent>& PermanentItems ,const Permanent& permanentItem);

        int getFoodItemsCount(vector<pair<Food , int>>& FoodItems);
        int getMedicineItemsCount(vector<pair<Medicine , int>>& MedicineItems);
        int getThrowableItemsCount(vector<pair<Throwable , int>>& ThrowableItems);
        int getPermanentItemsCount(vector<Permanent>& PermanentItems);

        int getSpecificFoodItemCount(vector<pair<Food , int>>& FoodItems, const Food& specificItem) const;
        int getSpecificMedicineItemCount(vector<pair<Medicine , int>>& MedicineItems, const Medicine& specificItem) const;
        int getSpecificThrowableItemCount(vector<pair<Throwable , int>>& ThrowableItems, const Throwable& specificItem) const;
        int getSpecificPermanentItemCount(vector<Permanent>& PermanentItems, const Permanent& specificItem) const;
        
        void clearFoodItems(vector<pair<Food , int>>& FoodItems);
        void clearMedicineItems(vector<pair<Medicine , int>>& MedicineItems);
        void clearThrowableItems(vector<pair<Throwable , int>>& ThrowableItems);
        void clearPermanentItems(vector<Permanent>& PermanentItems);

        void useItem(const string& itemName);
};
// *----------------------------------------------------------------*
class BankAccount {
    protected:
        int balance;
    public:
        BankAccount();
        int getBalance();
        void deposit(int amount);
        void withdraw(int amount);
};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Person {
    protected:
        string name;
        int level;
        Health health;
        Experience exp;
    public:
        Person(string name);
};
// *----------------------------------------------------------------*
class Human : public Person {
    protected:
        WarmWeaponAbility WarmAbility;
        ColdWeaponAbility ColdAbility;
        ThrowableWeaponAbility throwableWeaponAbility;
        Backpack backpack;
        Stamina stamina;
    public:
        Human();
};
// *----------------------------------------------------------------*
class Player : public Human {
    protected:
        int age;
        string userName;
        string gender;
        BankAccount bankAccount;
    public:
        Player(int age , string userName , string gender);
        void updateLevel();
};
// *----------------------------------------------------------------*
class SmartZombie : public Human {};
// *----------------------------------------------------------------*
class Zombie : public Person {};
// *----------------------------------------------------------------*
class BaseZombie : public Zombie {};
// *----------------------------------------------------------------*
class AdvZombiee : public Zombie {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Items {
    protected:
        string name;
        int price;
    public:
};
// *----------------------------------------------------------------*
class Permanent : public Items {};
// *----------------------------------------------------------------*
class WarmWeapon : public Permanent {};
// *----------------------------------------------------------------*
class ColdWeapon : public Permanent {};
// *----------------------------------------------------------------*
class Consumption : public Items {};
// *----------------------------------------------------------------*
class Medicine : public Consumption {};
// *----------------------------------------------------------------*
class Food : public Consumption {};
// *----------------------------------------------------------------*
class Throwable : public Items {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
#endif