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

};

class Experience {
    private:
        int maximum;
        int currentValue;
    public:
        void setMaximum(){
            maximum=100+(*50);
        }

        void setCurrentValue(int selfDamage,int enemyDamage,int usedStamina){
            currentValue+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
            if(currentValue>=maximum){
                currentValue=0;

            }
        }





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
