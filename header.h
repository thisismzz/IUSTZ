#ifndef HEADER
#define HEADER
// *----------------------------------------------------------------*
class Person {
    private:
        Health hp;
        string name;
        int level;
    public:
        Person(string n) : name(n), level(1), hp(100) {}
};

class Human : public Person {
    private:
        int age;
        string gender;
        Experience exp;
        Stamina stamina;
        WarmWeaponAbility WarmAbility;
        ColdWeaponAbility ColdAbility;
        Backpack backpack;

    public:
        Human(string n,string g,int a):Person(n,1,100),age(a),gender(g),exp(??),stamina(??),WarmAbility(??),ColdAbility(??),backpack(??){}
};

class Player : public Human {
    private:
        BankAccount bankAccount;
    public:
        Player(string n,string g,int a):Human(n,1,100,a,g,??,??,??,??,??),bankAccount(??){}
        
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
private:
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

class Stamina {
private :
    int value;
    int maximum;
    int level;
public :
    Stamina(int level);
    int getValue() const;
    void setValue(int val);
    void decrease(int amount);
    void increase(int amount);
    int getMaximum() const;

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
