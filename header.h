#ifndef HEADER
#define HEADER
#include<iostream>
#include<string>
#include<vector>
using namespace std;
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Person {
    protected:
        string name;
        int level = 1;
        Health health = 100;
        Experience exp;
    public:
        Person(string name);
        void updateLevel();
};

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
class Player : public Human {
    protected:
        int age;
        string gender;
        BankAccount bankAccount;
    public:
        Player(string n,string g,int a):Human(n,a,g,this,this,??,??,??),bankAccount(??){}
        
};

class SmartZombie : public Human {
    
};

class Zombie : public Person {

};
class BaseZombie : public Zombie {

};
class AdvZombiee : public Zombie {
    
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Items {
    protected:
        string name;
        double price;
    public:
        Items
};

class Permanent : public Items {


};
class WarmWeapon : public Permanent {
    
};
class ColdWeapon : public Permanent {

};

class Consumption : public Items {

};
class Medicine : public Consumption {

};
class Food : public Consumption {

};

class Throwable : public Items {

};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Health {
    protected:
        int currentHealth;
        int maxHealth;
    public:
        Health();
        int getCurrentHealth();
        int getMaxHealth();
        void decreaseHealth(int damage);
        void increaseHealth (int amount);
};

class Stamina {
    protected:
        int value;
        int maximum;
        Human *humanObj;
    public:
        Stamina(Human *h):humanObj(h),maximum(100),value(100){}
        int getValue() const;
        void setValue(int val);
        void decrease(int amount);
        void increase(int amount);
        int getMaximum() const;
        void updateMaximumStamina();

};

class Experience {
    protected:
        int maximum;
        int currentValue;
        Human *humanObj;
    public:
        Experience(Human *h):humanObj(h),maximum(100),currentValue(0){}

        void updateMaximum();

        void setCurrentValue(int,int,int);
};

class Skills {

};

class WarmWeaponAbility : public Skills {

};

class ColdWeaponAbility : public Skills {

};

class ThrowableWeaponAbility : public Skills {

};

class Backpack {
    protected:
        vector<pair<string, int>> items;
        vector<string> singleUseItems;
    public:
        void addItem(const string& itemName, int quantity);
        void removeItem(const string& itemName, int quantity);
        int getItemCount(const string& itemName) const;
        int getTotalItemsCount() const;
        void clear();
        void useItem(const string& itemName);
};

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
#endif