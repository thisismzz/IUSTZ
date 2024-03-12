#ifndef HEADER
#define HEADER
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include <thread>
#include <chrono>

using namespace std;
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Person {
    private:
        string name;
        int level;
        Health hp;
    public:
        Person(string);
        void updateLevel();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Human : public Person {
    private:
        Backpack backpack;
    public:
        Experience exp;
        Stamina stamina;
        Human(string n):Person(n),exp(this),backpack(??){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Player : public Human {
    private:
        int age;
        string gender;
        BankAccount bankAccount;
    public:
        Player(string n,string g,int a):Human(n,this,??),age(a),gender(g),bankAccount(??){}
        BankAccount* getBankAccount();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class SmartZombie : public Human {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Zombie : public Person {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class BaseZombie : public Zombie {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class AdvZombiee : public Zombie {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Health {
private :
    int currentHealth;
    int maxHealth;
public :
    Health();
    void decreaseHealth(int damage);
    void increaseHealth (int amount);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Stamina {
private :
    int currentStamina;
    int maximum;
public :
    Stamina();
    void decreaseStamina(int amount);
    void increaseStamina(int amount);
    void updateMaximumStamina();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Experience {
    private:
        int maximum;
        int currentExp;
        Human *humanObj;
    public:
        Experience(Human*);
        void updateMaximum();
        void setCurrentExp(int,int,int);
        void increaseExp(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Skills {
    protected:
        int maximum;
        int currentSkill;
        int upgradePrice;
    public:
        Skills(int,int);
        void upgradeSkill(BankAccount*);
        void setUpgradePrice();

};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class WarmWeaponAbility : public Skills {
    public:
        WarmWeaponAbility(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class ColdWeaponAbility : public Skills {
    public:
        ColdWeaponAbility(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class ThrowableWeaponAbility : public Skills {
    public:
        ThrowableWeaponAbility(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Backpack {
    protected:
        vector<pair<Food , int>> FoodItems;
        vector<pair<Medicine , int>> MedicineItems;
        vector<pair<Throwable , int>> ThrowableItems;
        vector<Permanent> PermanentItems;
    public:
        void addFoodItems(const Food& foodItem, int quantity);
        void addMedicineItems(const Medicine& medicineItem, int quantity);
        void addThrowableItems(const Throwable& throwableItem, int quantity);
        void addPermanentItems(const Permanent& permanentItem);

        void removeFoodItems(const Food& foodItem, int quantity);
        void removeMedicineItems(const Medicine& medicineItem, int quantity);
        void removeThrowableItems(const Throwable& throwableItem, int quantity);
        void removePermanentItems(const Permanent& permanentItem);

        int getFoodItemsCount();
        int getMedicineItemsCount();
        int getThrowableItemsCount();
        int getPermanentItemsCount();

        int getSpecificFoodItemCount(const Food& specificItem) const;
        int getSpecificMedicineItemCount(const Medicine& specificItem) const;
        int getSpecificThrowableItemCount(const Throwable& specificItem) const;
        int getSpecificPermanentItemCount(const Permanent& specificItem) const;
        
        void clearFoodItems();
        void clearMedicineItems();
        void clearThrowableItems();
        void clearPermanentItems();

        // void useItem(const string& itemName);
};

// *----------------------------------------------------------------*
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

class Items {
    protected:
        string name;
        int price;
    public:
        Items(string,int);
        void buy();
        virtual void addToBackpack();
        virtual void showItems();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Permanent : public Items {
    private:
        vector<Permanent> permanentItems;
        int damage;
    public:
        Permanent();
        void showItems() override;
        void addToBackpack() override;
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class WarmWeapon : public Permanent {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class ColdWeapon : public Permanent {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Consumable : public Items {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Medicine : public Consumable {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Food : public Consumable {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Throwable : public Items {};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void printWithDelay() {}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
#endif