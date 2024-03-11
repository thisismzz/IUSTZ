#ifndef HEADER
#define HEADER
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

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

        int getSpecificFoodItemCount(const Food& specificItem) const;
        int getSpecificMedicineItemCount(vector<pair<Medicine , int>>& MedicineItems, const Medicine& specificItem) const;
        int getSpecificThrowableItemCount(vector<pair<Throwable , int>>& ThrowableItems, const Throwable& specificItem) const;
        int getSpecificPermanentItemCount(vector<Permanent>& PermanentItems, const Permanent& specificItem) const;
        
        void clearFoodItems(vector<pair<Food , int>>& FoodItems);
        void clearMedicineItems(vector<pair<Medicine , int>>& MedicineItems);
        void clearThrowableItems(vector<pair<Throwable , int>>& ThrowableItems);
        void clearPermanentItems(vector<Permanent>& PermanentItems);

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
};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Permanent : public Items {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class WarmWeapon : public Permanent {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class ColdWeapon : public Permanent {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Consumption : public Items {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Medicine : public Consumption {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Food : public Consumption {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
class Throwable : public Items {};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
#endif