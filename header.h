#ifndef HEADER
#define HEADER
// *----------------------------------------------------------------*
class Person {
    private:
        Health hp;
        string name;
        int level;
};

class Human : public Person {
    private:
        Experience exp;
        Stamina stamina;
        WarmWeaponAbility WarmAbility;
        ColdWeaponAbility ColdAbility;
        Backpack backpack;

    public:
        friend class Experience;
};

class Player : public Human {
    private:
        BankAccount bankAccount;
    public:
        

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
class Items {

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

class Health {
private :
    int currentHealth;
    int maxHealth;
public :
    Health();
    int getCurrentHealth();
    int getMaxHealth();
    void decreaseHealth(int damage);
    void increaseHealth (int amount);
};

class Experience {
    private:
        int maximum;
        int currentValue;
        int level;

    public:
        Experience(int);
        void setMaximum();
        void setCurrentValue(int,int,int);
};

class Stamina {
private :
    int value;
    int maximum;
public :
    Stamina(int val , int max);
    int getValue() const;
    void setValue(int val);
    void decrease(int amount);
    void increase(int amount);
    int getMaximum() const;

};

class Skills {

};

class WarmWeaponAbility : public Skills {

};

class ColdWeaponAbility : public Skills {

};

class Backpack {

};

class BankAccount {

};
// *----------------------------------------------------------------*
#endif
