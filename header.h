#ifndef HEADER
#define HEADER
// *----------------------------------------------------------------*
class Person {
    private:
        string name;
        int level;
    public:
        Health hp;
        Person(string n) : name(n), level(1){}

        void updateLevel(){
            level++;
        }
};

class Human : public Person {
    private:
        int age;
        string gender;
        WarmWeaponAbility WarmAbility;
        ColdWeaponAbility ColdAbility;
        Backpack backpack;

    public:
        Experience exp;
        Stamina stamina;
        Human(string n,string g,int a):Person(n),age(a),gender(g),exp(this),stamina(this),WarmAbility(??),ColdAbility(??),backpack(??){}
        
};

class Player : public Human {
    private:
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
    Human *humanObj;
public :
    Stamina(Human *h):humanObj(h),maximum(100),value(100){}
    int getValue() const;
    void setValue(int val);
    void decrease(int amount);
    void increase(int amount);
    int getMaximum() const;
    void updateMaximumStamina();

};

class Experience {
    private:
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

class Backpack {
private:
    std::vector<std::pair<std::string, int>> items;
    std::vector<std::string> singleUseItems;
public:
    void addItem(const std::string& itemName, int quantity);
    void removeItem(const std::string& itemName, int quantity);
    int getItemCount(const std::string& itemName) const;
    int getTotalItemsCount() const;
    void clear();
    void useItem(const std::string& itemName);
};

class BankAccount {

};
// *----------------------------------------------------------------*
#endif
