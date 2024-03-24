#ifndef HEADER
#define HEADER

#include <string>
#include <vector>
#include <map>

using namespace std;

class Health;  
class Stamina; 
class Experience;  
class Skills;  
class WarmWeaponAbility;  
class ColdWeaponAbility;   
class ThrowableWeaponAbility; 
class Backpack;  
class BankAccount;  

class Person;  
class Human;  
class Player;  
class HumanEnemy;
class Zombie; 
class BasicZombie; 
class AdvZombiee;
class Factory; 

class Items;  
class Permanent; 
class WarmWeapon; 
class ColdWeapon; 
class Throwable; 
class Medicine; 
class Food; 

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Health {
private:
    int currentHealth;
    int maxHealth;
public:
    Health();
    int getCurrentHealth();
    int getMaxHealth();
    void decreaseHealth(int);
    void increaseHealth(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Stamina {
    private:
        int currentStamina;
        int maximum;
    public:
        Stamina(int);
        void decreaseStamina(int);
        void increaseStamina(int);
        void updateMaximumStamina();
        int getCurrentStamina();
        int getMaximumStamina();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Experience {
    private:
        int maximum;
        int currentExp;
        Player *humanObj;
    public:
        Experience(Player*);
        void updateMaximum();
        void setCurrentExp(int, int, int);
        void increaseExp(int);
        int getCurrentExp();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Skills {
    protected:
        int maximum;
        int currentSkill;
        int upgradePrice;
    public:
        Skills(int, int);
        void upgradeSkill(BankAccount*);
        void setUpgradePrice();
        int getCurrentSkill();
};

// *----------------------------------------------------------------*

class WarmWeaponAbility : public Skills {
    public:
        WarmWeaponAbility(int);
};

// *----------------------------------------------------------------*

class ColdWeaponAbility : public Skills {
    public:
        ColdWeaponAbility(int);
};

// *----------------------------------------------------------------*

class ThrowableWeaponAbility : public Skills {
    public:
        ThrowableWeaponAbility(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Backpack {
    private:
        map <Food , int> FoodItems;
        map <Medicine , int> MedicineItems;
        map <Throwable , int> ThrowableItems;
        vector <WarmWeapon> WarmWeaponItems;
        vector <ColdWeapon> ColdWeaponItems;
        void removeFoodItem(const Food);
        void removeMedicineItem(const Medicine);
        void removeThrowableItem(const Throwable);
    public:
        Backpack(){}
        void addFoodItem(const Food,int);
        void addMedicineItem(const Medicine,int);
        void addThrowableItem(const Throwable,int);
        void addWarmWeaponItem(const WarmWeapon);
        void addColdWeaponItem(const ColdWeapon);
        bool warmWeaponExistence(const WarmWeapon);
        bool coldWeaponExistence(const ColdWeapon);
        void useFoodItemCount(const Food specificItem);
        void useMedicineItemCount(const Medicine specificItem);
        void useThrowableItemCount(const Throwable specificItem);
        void showItems();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class BankAccount {
    private:
        int balance;
    public:
        BankAccount(int);
        int getBalance();
        void deposit(int amount);
        bool withdraw(int amount);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Person {
    protected:
        string name;
        int level;
    public:
        Health hp;
        Person(string n);
        void updateLevel();
        int getLevel();
        string getName();  // Getter for name
        int getHealthPoints();  // Getter for health points
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Human : public Person {
    protected:
        Backpack backpack;
    public:
        Stamina stamina;
        Human(string, int);
        Backpack* getBackpack();
        int getStamina();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Player : public Human {
    private:
        int age;
        string gender;
        string username;
        BankAccount bankAccount;
    public:
        Experience exp;
        Player(string, string, string, int, int, int);
        Player(Human&, string, string, int, int);
        BankAccount* getBankAccount();
        int getAge();  // Getter for age
        string getGender();  // Getter for gender
        int getMoney();  // Getter for money
        int getExperience();  // Getter for experience
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class HumanEnemy : public Human {
    private:

    public:
        HumanEnemy(Human&);
        void updateState();       // Method to update the state of the human enemy
        void takeDamage(int);     // Method to take damage

};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Zombie : public Person {
    public:
        Zombie(string);
        void takeDamage(int);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class BasicZombie : public Zombie {
    public:
        BasicZombie(string);
        void bite(Player& player ,int damage);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class AdvZombie : public Zombie {
    public:
        AdvZombie(string);
        void bite();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Factory {
    public:
        static Human* createCharacter(const string &type);

    private:
        static Human* createJonSnow(const string &name);

        static Human* createJaimeLannister(const string &name);

        static Human* createDaenerys(const string &name);

        static Human* createStannis(const string &name);

        static Human* createJoffrey(const string &name);

        static Human* createTheonGreyjoy(const string &name);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Items {
    protected:
        string type;
        string name;
        int price;
        static vector <Items> shop_items;
    public:
        Items(string,int,string);
        virtual void addToVectors(){}                    //add the bought item to the vector
        bool operator==(const Items&) const;             //check equality of two object based on names
        bool operator<(const Items&) const;
        string getName();
        string getType();
}; 
vector <Items> Items::shop_items;     

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Permanent : public Items {
    protected:
        int harm;
        int exp;
        static vector <Permanent> shop_items_permanent;
        Permanent(string, int, string, int, int);
    public:       
        virtual void buy(Player&){}                      //buy item and add it into player's backpack
        virtual void Attack(Human, Person){}               //calculate the damage of attacker and reduce it from attacked health
};
vector <Permanent> Permanent::shop_items_permanent;

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class WarmWeapon : public Permanent {
    private:
        static vector <WarmWeapon> shop_items_permanent_warmweapon;
        WarmWeaponAbility wwa;
    public:
        WarmWeapon(string,int,int,int,int);
        static void showItems();                     //show the available items to buy
        void buy(Player&) override;
        void addToVectors() override;
        void Attack(Human, Person) override;
        friend ostream& operator<<(ostream&, WarmWeapon&);
        friend void Show_Permanent_Items();
        // Add a public method to get the shop items
        static vector<WarmWeapon>& get_shop_items_permanent_warmweapon();
};
vector <WarmWeapon> WarmWeapon::shop_items_permanent_warmweapon;

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class ColdWeapon : public Permanent {
    private:
        static vector <ColdWeapon> shop_items_permanent_coldweapon;
        ColdWeaponAbility cwa;
    public:
        ColdWeapon(string,int,int,int,int);
        static void showItems();                     //show the available items to buy
        void buy(Player&) override;
        void addToVectors() override;
        void Attack(Human, Person) override;
        friend ostream& operator<<(ostream&, ColdWeapon&);
        friend void Show_Permanent_Items();
        // Add a public method to get the shop items
        static vector<ColdWeapon>& get_shop_items_permanent_coldweapon();
};
vector <ColdWeapon> ColdWeapon::shop_items_permanent_coldweapon;

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Throwable : public Items {
    private:
        int harm;
        int exp;
        static vector <Throwable> shop_items_throwable;
        ThrowableWeaponAbility twa;
    public:
        Throwable(string,int,int,int,int);
        static void showItems();                     //show the available items to buy
        void buy(Player&, int);
        void addToVectors() override;
        void Throw(Human, Human);
        friend ostream& operator<<(ostream&, Throwable&);
        friend void Show_Throwable_Items();
        // Add a public method to get the shop items
        static vector<Throwable>& get_shop_items_throwable();
};
vector <Throwable> Throwable::shop_items_throwable;

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Medicine : public Items {
    private:
        int heal;
        static vector <Medicine> shop_items_medicine;
    public:
        Medicine(string,int,int);
        static void showItems();                     //show the available items to buy
        void buy(Player&, int);
        void addToVectors() override;
        void use(Human&);
        friend ostream& operator<<(ostream&,Medicine&);
        friend void Show_Consumable_Items();
        // Add a public method to get the shop items
        static vector<Medicine>& get_shop_items_medicine();
};
vector <Medicine> Medicine::shop_items_medicine;

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Food : public Items {
    private:
        int strength;
        static vector <Food> shop_items_food;
    public:
        Food(string, int, int);
        static void showItems();                     //show the available items to buy
        void buy(Player&, int);
        void addToVectors() override;
        void use(Human&);
        friend ostream& operator<<(ostream&, Food&);
        friend void Show_Consumable_Items();
        // Add a public method to get the shop items
        static vector<Food>& get_shop_items_food();
};
vector <Food> Food::shop_items_food;

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void printWithDelay(string);
void getUserInfo(int&, string&, string&);
void showPlayerInfo();
void playground();
void Menu();
void ShopMenu();
void Show_Permanent_Items();
void Show_Throwable_Items();
void Show_Consumable_Items();
void createWarmWeapons();
void createColdWeapons();
void createThrowableItems();
void createMedicines();
void createFoods();

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

#endif