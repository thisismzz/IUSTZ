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
class SmartZumbi : public Human {
    
};

class Zumbi : public Person {

};
class BaseZumbi : public Zumbi {

};
class AdvZumbie : public Zumbi {
    
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
