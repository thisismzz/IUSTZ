#ifndef HEADER
#define HEADER
// *----------------------------------------------------------------*
class Person {
    Health health;
    Damage damage;
    string name;
};

class Human : public Person {
    
};
class Player : public Human {
    Stamina stamina;
    BankAccount bankAccount;
};
class SmartZombie : public Human {
    
};

class Zombie : public Person {

};
class BaseZombie : public Zombie {

};
class AdvZombie : public Zombie {
    
};

// *----------------------------------------------------------------*
class Shop {

};

class Permanent : public Shop {

};
class WarmWeapon : public Permanent {
    
};
class ColdWeapon : public Permanent {

};

class Consumption : public Shop {

};
class Medicine : public Consumption {

};
class Food : public Consumption {

};

class Throwable : public Shop {

};
// *----------------------------------------------------------------*
class Health {

};

class Damage {

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
