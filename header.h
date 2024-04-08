#ifndef HEADER
#define HEADER

// #include <string>
// #include <vector>
// #include <map>

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
class AdvZombie;
class Factory; 

class Items;  
class Permanent; 
class WarmWeapon; 
class ColdWeapon; 
class Throwable; 
class Medicine; 
class Food;

class HE_View;
class HE_Controller;
class BZ_Controller;
class BZ_View;
class AZ_Controller;
class AZ_View;

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
        int getMaxStamina();
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
        int getMaxExp();
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
        friend class HE_Controller;
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
        void showWarmWeaponItems();
        void showColdWeaponItems();
        void showThrowableItems();
        void showMedicineItems();
        void showFoodItems();
        void ConsumeMedForSurvival();
        void ConsumeFoodForSurvival();
        Items* useWeapons();
        Items* useConsumableItems();
        Items* upgradeWeapons();
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
        Person(string,int);
        void updateLevel();
        int getLevel();
        string getName();  // Getter for name
        int getHealthPoints();  // Getter for health points
        int getMaxHealth();
        virtual void takeDamage(int){}  //show detail of entry damage
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Human : public Person {
    protected:
        Backpack backpack;
    public:
        Stamina stamina;
        Human(string,int,int);
        Backpack* getBackpack();
        int getStamina();
        int getMaxStamina();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

enum class PlayerState{
    DEFEATED,ALIVE
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Player : public Human {
    private:
        int age;
        string gender;
        string username;
        BankAccount bankAccount;
        PlayerState state;
    public:
        Experience exp;
        Player(string, string, string, int, int, int);
        Player(Human&, string, string, int, int);
        BankAccount* getBankAccount();
        int getAge();  // Getter for age
        string getGender();  // Getter for gender
        int getMoney();  // Getter for money
        int getExperience();  // Getter for current experience
        int getMaxExperience();
        string getUsername();  //Getter for username
        void takeDamage(int) override;  //show detail of entry damage
        void newLife();     //set the state ALIVE
        PlayerState getState();
        void setState(PlayerState);
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
enum class HumanEnemyState {
    LOW_HEALTH,
    LOW_POWER,
    FIGHT
};

enum class HumanEnemyStatus{
    DEFEATED,ALIVE
};
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class HumanEnemy : public Human {
    private:
        HumanEnemyState state;
        HumanEnemyStatus status;
    public:
        HumanEnemy(Human&,int);
        void setState(HumanEnemyState);
        HumanEnemyState getState();
        HumanEnemyStatus getStatus();
        void takeDamage(int) override;        //show damage amount
};

class HE_View {
    public:
        HE_View();
        void showInfo(HumanEnemy&);
        void updateHealth(string,int);     //prints the amount of heal
        void updateStamina(string,int);    //prints the amount of increased strength
        void attackView(string,Items);     //prints the weapon name that used to attack player
        void showBackpackItems();
};

class HE_Controller {
    private:
        HumanEnemy  *model;
        HE_View view;
        Backpack* backpack;
        void Attack(Items*);
    public:
        HE_Controller(HumanEnemy*);
        void updateState();      // Method to update the state of the human enemy
        HumanEnemyState getState();
        HumanEnemyStatus getStatus();
        Items* chooseWeapon();
        Food* chooseFood();
        Medicine* chooseMedicine();
        void transferItems();
        void decision();
        void showInfo();
        void showBackpackItems();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

enum class ZombieState{
    DEFEATED,ALIVE
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Zombie : public Person {
    private:
        ZombieState state;
    public:
        Zombie(string,int);
        void takeDamage(int) override;     //show damage amount
        ZombieState getState();
        virtual void bite(){}
        virtual void scratch(){}
        ~Zombie(){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class BasicZombie : public Zombie {
    public:
        BasicZombie(string,int);
        BasicZombie(Zombie&);
        void bite() override;
        ~BasicZombie(){}
};

class BZ_View {
    public:
        BZ_View();
        void showInfo(BasicZombie&);
};

class BZ_Controller {
    private:
        BasicZombie *model;
        BZ_View view;
    public:
        BZ_Controller(BasicZombie*);
        ZombieState getState();
        void bite();
        void showInfo();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class AdvZombie : public Zombie {
    public:
        AdvZombie(string,int);
        AdvZombie(Zombie&);
        void bite() override;
        void scratch() override;
};

class AZ_View {
    public:
    AZ_View();
    void showInfo(AdvZombie&);
};

class AZ_Controller {
    private:
        AdvZombie * model;
        AZ_View view;
    public:
        AZ_Controller(AdvZombie*);
        ZombieState getState();
        void Attack();
        void showInfo();
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Factory {
    public:
        static Human* createCharacter(const string&);

        static BasicZombie* createBasicZombie(const int);

        static AdvZombie* createAdvanceZombie(const int);

    private:
        static Human* createJonSnow(const string&);

        static Human* createJaimeLannister(const string&);

        static Human* createDaenerys(const string&);

        static Human* createStannis(const string&);

        static Human* createJoffrey(const string&);

        static Human* createTheonGreyjoy(const string&);

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
        int getPrice();
        ~Items(){}
}; 

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Permanent : public Items {
    protected:
        int harm;
        int exp;
        static vector <Permanent> shop_items_permanent;
        Permanent(string, int, string, int, int);
    public:       
        virtual void buy(){}                      //buy item and add it into player's backpack
        virtual void Attack(Human&, Person&){}               //calculate the damage of attacker and reduce it from attacked health
        ~Permanent(){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class WarmWeapon : public Permanent {
    private:
        static vector <WarmWeapon> shop_items_permanent_warmweapon;
        WarmWeaponAbility wwa;
    public:
        WarmWeapon(string,int,int,int,int);
        static void showItems();                     //show the available items to buy
        void buy() override;
        void addToVectors() override;
        void Attack(Human&, Person&) override;
        friend ostream& operator<<(ostream&, WarmWeapon&);
        friend void Show_Permanent_Items();
        friend void playground();
        friend void Shop_PermanentItems_Menu();
        WarmWeaponAbility getwwa();
        ~WarmWeapon(){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class ColdWeapon : public Permanent {
    private:
        static vector <ColdWeapon> shop_items_permanent_coldweapon;
        ColdWeaponAbility cwa;
    public:
        ColdWeapon(string,int,int,int,int);
        static void showItems();                     //show the available items to buy
        void buy() override;
        void addToVectors() override;
        void Attack(Human&, Person&) override;
        friend ostream& operator<<(ostream&, ColdWeapon&);
        friend void Show_Permanent_Items();
        friend void playground();
        friend void Shop_PermanentItems_Menu();
        ColdWeaponAbility getcwa();
        ~ColdWeapon(){}
};

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
        void buy(int);
        void addToVectors() override;
        void Throw(Human&, Person&);
        friend ostream& operator<<(ostream&, Throwable&);
        friend void Show_Throwable_Items();
        friend void playground();
        ThrowableWeaponAbility gettwa();
        ~Throwable(){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Medicine : public Items {
    private:
        int heal;
        static vector <Medicine> shop_items_medicine;
    public:
        Medicine(string,int,int);
        static void showItems();                     //show the available items to buy
        void buy(int);
        void addToVectors() override;
        void use(Human&);
        int getHeal();
        friend ostream& operator<<(ostream&,Medicine&);
        friend void Show_Consumable_Items();
        friend void playground();
        friend void medicineMenu();
        ~Medicine(){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

class Food : public Items {
    private:
        int strength;
        static vector <Food> shop_items_food;
    public:
        Food(string, int, int);
        static void showItems();                     //show the available items to buy
        void buy(int);
        void addToVectors() override;
        void use(Human&);
        int getStrength();
        friend ostream& operator<<(ostream&, Food&);
        friend void Show_Consumable_Items();
        friend void playground();
        friend void foodMenu();
        ~Food(){}
};

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void printWithDelay(string);
void getUserInfo(int&, string&, string&);
void showPlayerInfo();
void createItem(); 
void playground();
void Menu();
void ShopMenu();
void Show_Permanent_Items();
void Show_Throwable_Items();
void Show_Consumable_Items();
void Shop_PermanentItems_Menu();
void goodbye();
void battleGround_humanEnemy();
void battleGround_basicZombie();
void battleGround_advZombie();
void BattleMenu();
void createWarmWeapons();
void createColdWeapons();
void createThrowableItems();
void createMedicines();
void createFoods();
void medicineMenu();
void foodMenu();

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

#endif