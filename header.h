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
        Backpack* getBackpack();
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
        vector<pair<WarmWeapon , int>> WarmWeaponItems;
        vector<pair<ColdWeapon , int>> ColdWeaponItems;
    public:
        void addFoodItem(const Food& foodItem, int quantity);
        void addMedicineItem(const Medicine& medicineItem, int quantity);
        void addThrowableItem(const Throwable& throwableItem, int quantity);
        void addWarmWeaponItem(const WarmWeapon& WarmWeaponItem);
        void addColdWeaponItem(const ColdWeapon& ColdWeaponItem);

        void removeFoodItem(const Food& foodItem);
        void removeMedicineItem(const Medicine& medicineItem);
        void removeThrowableItem(const Throwable& throwableItem);
        void removeWarmWeaponItem(const WarmWeapon& WarmWeaponItem);
        void removeColdWeaponItem(const ColdWeapon& ColdWeaponItem);

        int getFoodItemsCount();
        int getMedicineItemsCount();
        int getThrowableItemsCount();
        int getWarmWeaponItemsCount();
        int getColdWeaponItemsCount();

        int getSpecificFoodItemCount(const Food& specificItem) const;
        int getSpecificMedicineItemCount(const Medicine& specificItem) const;
        int getSpecificThrowableItemCount(const Throwable& specificItem) const;
        int getSpecificWarmWeaponItemCount(const WarmWeapon& WarmWeaponItem) const;
        int getSpecificColdWeaponItemCount(const ColdWeapon& ColdWeaponItem) const;
        
        void clearFoodItems();
        void clearMedicineItems();
        void clearThrowableItems();
        void clearWarmWeaponItems();
        void clearColdWeaponItems();

        void useFoodItemCount(const Food& specificItem, int quantity);
        void useMedicineItemCount(const Medicine& specificItem, int quantity);
        void useThrowableItemCount(const Throwable& specificItem, int quantity);

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
        string type;
        string name;
        int price;
        vector <Items> shop_items;
    public:
        Items(string,int);
        virtual void buy(Player& player);        //buy Item and add it into player's backpack
        virtual void showItems();
        virtual bool operator==(const Items& other) const;
        void addToShop(Items);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Permanent : public Items {
    private:
        int damage;
    public:
        Permanent(string,int);
        void showItems() override;
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class WarmWeapon : public Permanent {
    private:
        WarmWeaponAbility wwa;

};

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