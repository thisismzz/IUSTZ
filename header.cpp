#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>
#include <conio.h>
#include <windows.h>

#include "header.h"

using namespace std;

//*****************************
Player* player = nullptr;
HumanEnemy* humanEnemy=nullptr;
BasicZombie* basicZombie=nullptr;
AdvZombie* advZombie=nullptr;
vector <string> characterTypes = {"JonSnow", "JaimeLannister", "Daenerys", "Stannis", "Joffrey", "TheonGreyjoy"};
vector <Items> Items::shop_items={};
vector <Permanent> Permanent::shop_items_permanent={};
vector <WarmWeapon> WarmWeapon::shop_items_permanent_warmweapon={};
vector <ColdWeapon> ColdWeapon::shop_items_permanent_coldweapon={};
vector <Throwable> Throwable::shop_items_throwable={};
vector <Medicine> Medicine::shop_items_medicine={};
vector <Food> Food::shop_items_food={};
//*****************************

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Health::Health() : maxHealth(100) , currentHealth(100) {} // Constructor

int Health::getCurrentHealth() {
    return currentHealth; // Returns current health
}

int Health::getMaxHealth() {
    return maxHealth; // Returns max health
}

void Health::decreaseHealth(int damage){
    currentHealth -= damage;
    // Ensures health doesn't go below 0
    if(currentHealth <= 0)
        currentHealth = 0;
}

void Health::increaseHealth (int amount){
    currentHealth += amount; // Increases health
    if(currentHealth >= maxHealth)
        currentHealth = maxHealth; // Ensures health doesn't exceed maxHealth
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Stamina::Stamina(int cs):maximum(100),currentStamina(cs){} // Constructor

void Stamina::decreaseStamina(int amount) {
    currentStamina -= amount; // Decreases stamina
    if (currentStamina < 0)
        currentStamina = 0; // Ensures stamina doesn't go below 0
}

void Stamina::increaseStamina(int amount) {
    currentStamina += amount; // Increases stamina
    if ( currentStamina > maximum)
        currentStamina = maximum; // Ensures stamina doesn't exceed maximum
}

void Stamina::updateMaximumStamina(){
    maximum+=50; // Increases maximum stamina
}

int Stamina::getCurrentStamina(){
    return currentStamina; // Returns current stamina
}

int Stamina::getMaxStamina(){
    return maximum; // Returns maximum stamina
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Experience::Experience(Player *h):humanObj(h),maximum(100),currentExp(0){} // Constructor

void Experience::updateMaximum(){
    maximum+=100; // Increases maximum experience
}

void Experience::setCurrentExp(int selfDamage,int enemyDamage,int usedStamina){
    currentExp+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina); // Updates current experience
    if(currentExp>=maximum){
        currentExp=0; // Resets current experience if it exceeds maximum
        updateMaximum(); // Updates maximum experience
        humanObj->updateLevel(); // Updates player's level
        humanObj->stamina.updateMaximumStamina(); // Updates player's maximum stamina
    }
}

void Experience::increaseExp(int amount){
    currentExp+=amount; // Increases current experience
    if(currentExp>=maximum){
        currentExp=0; // Resets current experience if it exceeds maximum
        humanObj->exp.updateMaximum(); // Updates maximum experience
        humanObj->updateLevel(); // Updates player's level
        humanObj->stamina.updateMaximumStamina(); // Updates player's maximum stamina
    }
}

int Experience::getCurrentExp(){
    return currentExp; // Returns current experience
}

int Experience :: getMaxExp() {
    return maximum;
}
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Skills::Skills(int n,int m):maximum(5),currentSkill(n),upgradePrice(m){} // Constructor

void Skills::upgradeSkill(BankAccount *creditcard){
    try{
        if(creditcard->getBalance()>=upgradePrice){
            creditcard->withdraw(upgradePrice); // Withdraws the upgrade price from the credit card
            currentSkill++; // Increases the current skill level
            setUpgradePrice(); // Sets the new upgrade price
        }
        else{
            throw 0; // Throws an exception if there's not enough money
        }
    }
    catch(int temp){
        cout<<"not enough money!\n"; // Prints an error message if there's not enough money
    } 
}

void Skills::setUpgradePrice(){
    upgradePrice+=10; // Increases the upgrade price by 10
}

int Skills::getCurrentSkill(){
    return currentSkill; // Returns the current skill level
}


// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

WarmWeaponAbility::WarmWeaponAbility(int n):Skills(n,n*10){} // Constructor

ColdWeaponAbility::ColdWeaponAbility(int n):Skills(n,n*15){} // Constructor

ThrowableWeaponAbility::ThrowableWeaponAbility(int n):Skills(n,n*20){} // Constructor

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::addFoodItem(const Food foodItem, int quantity){
    if(FoodItems.find(foodItem)!=FoodItems.end()){
        FoodItems[foodItem]+=quantity; // Increases quantity if food item exists
    }
    else
        FoodItems.insert({foodItem,quantity}); // Inserts new food item
}

void Backpack::addMedicineItem(const Medicine medicineItem, int quantity){
    if(MedicineItems.find(medicineItem)!=MedicineItems.end()){
        MedicineItems[medicineItem]+=quantity; // Increases quantity if medicine item exists
    }
    else
        MedicineItems.insert({medicineItem,quantity}); // Inserts new medicine item
}

void Backpack::addThrowableItem(const Throwable throwableItem, int quantity){
    if(ThrowableItems.find(throwableItem)!=ThrowableItems.end()){
        ThrowableItems[throwableItem]+=quantity; // Increases quantity if throwable item exists
    }
    else
        ThrowableItems.insert({throwableItem,quantity}); // Inserts new throwable item
}

void Backpack::addWarmWeaponItem(const WarmWeapon warmWeaponItem){
    WarmWeaponItems.push_back(warmWeaponItem); // Adds warm weapon item
}

void Backpack::addColdWeaponItem(const ColdWeapon coldWeaponItem){
    ColdWeaponItems.push_back(coldWeaponItem); // Adds cold weapon item
}


// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

bool Backpack::warmWeaponExistence(const WarmWeapon warmWeaponItem){
    for (const auto &item : WarmWeaponItems) {
        if (item== warmWeaponItem) {
            return true; // Returns true if warm weapon item exists
        }
    }
    return false; // Returns false if warm weapon item does not exist
}

bool Backpack::coldWeaponExistence(const ColdWeapon coldWeaponItem){
    for (const auto &item : ColdWeaponItems) {
        if (item== coldWeaponItem) {
            return true; // Returns true if cold weapon item exists
        }
    }
    return false; // Returns false if cold weapon item does not exist
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::removeFoodItem(const Food foodItem) {
    FoodItems.erase(foodItem); // Removes food item
}

void Backpack::removeMedicineItem(const Medicine medicineItem) {
    MedicineItems.erase(medicineItem); // Removes medicine item
}

void Backpack::removeThrowableItem(const Throwable throwableItem) {
    ThrowableItems.erase(throwableItem); // Removes throwable item
}

void Backpack::useFoodItemCount(const Food foodItem) {
    FoodItems[foodItem]-=1; // Decreases food item count
    if(FoodItems[foodItem]==0)
        removeFoodItem(foodItem); // Removes food item if count is 0
}

void Backpack::useMedicineItemCount(const Medicine medicineItem) {
    MedicineItems[medicineItem]-=1; // Decreases medicine item count
    if(MedicineItems[medicineItem]==0)
        removeMedicineItem(medicineItem); // Removes medicine item if count is 0
}

void Backpack::useThrowableItemCount(const Throwable throwableItem) {
    ThrowableItems[throwableItem]-=1; // Decreases throwable item count
    if(ThrowableItems[throwableItem]==0)
        removeThrowableItem(throwableItem); // Removes throwable item if count is 0
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::showItems(){
    int index;
    if(!ColdWeaponItems.empty()){
        index=1;
        for(auto i:ColdWeaponItems){
            cout << index << ")   " << "type : " << i.getType() << "\tname : " << i.getName() << '\n'; // Prints cold weapon items
            index++;
        }
    }

    if(!WarmWeaponItems.empty()){
        index=1;
        for(auto i:WarmWeaponItems){
            cout<<index<<")   "<<"type : "<<i.getType()<<"\tname : "<<i.getName()<<'\n'; // Prints warm weapon items
            index++;
        }
    }

    if(!ThrowableItems.empty()){
        index=1;
        for(auto i:ThrowableItems){
            Throwable item=i.first;
            cout<<index<<")   "<<"type : "<<item.getType()<<"\tname : "<<item.getName()<<"\tstock : "<<i.second<<'\n'; // Prints throwable items
            index++;
        }

        auto p=ThrowableItems.begin();
    }

    if(!MedicineItems.empty()){
        index=1;
        for(auto i:MedicineItems){
            Medicine item=i.first;
            cout<<index<<")   "<<"type : "<<item.getType()<<"\tname : "<<item.getName()<<"\tstock : "<<i.second<<'\n'; // Prints medicine items
            index++;
        }
    }

    if(!FoodItems.empty()){
        index=1;
        for(auto i:FoodItems){
            Food item=i.first;
            cout<<index<<")   "<<"type : "<<item.getType()<<"\tname : "<<item.getName()<<"\tstock : "<<i.second<<'\n'; // Prints food items
            index++;
        }
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::showWarmWeaponItems(){
    int index = 1;
    for(auto i: WarmWeaponItems){
        cout << index << ")" << i.getName() << '\n';
        index++;
    }
}

void Backpack::showColdWeaponItems(){
    int index = 1;
    for(auto i: ColdWeaponItems){
        cout << index << ")" << i.getName() << '\n'; 
        index++;
    }
}

void Backpack::showThrowableItems(){
    int index = 1;
    for(auto pair : ThrowableItems){
    Throwable item = pair.first;
    cout << index << ")" << item.getName() << "(stock : " << pair.second << ")" << '\n';
    index++;
    }

}

void Backpack::showMedicineItems(){
    int index = 1;
    for(auto pair : MedicineItems){
        Medicine item = pair.first;
        cout << index << ")" << item.getName() << "(stock : " << pair.second << ")" << '\n';
        index++;
    }
}

void Backpack::showFoodItems(){
    int index = 1;
    for(auto pair : FoodItems){
        Food item = pair.first;
        cout << index << ")" << item.getName() << "(stock : " << pair.second << ")" << '\n';
        index++;
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::consumeForSurvival() {
    system("cls");

    if(MedicineItems.empty()){
        cout << "No medicine items left in the backpack." << endl;
        medicineMenu();
    }

    cout << "Here are the available medicine items in the backpack:" << endl;
    int index = 1;
    vector<Medicine> medicineVector;
    for(auto i:MedicineItems){
        Medicine item = i.first;
        cout << index << ")   " << "\tname : " << item.getName() << "\tstock : " << i.second << '\n'; // Prints medicine items
        medicineVector.push_back(item);
        index++;
    }

    int choice;
    cout << "Enter the number of the medicine item you want to use: ";
    cin >> choice;

    if(choice < 1 || choice > medicineVector.size()){
        cout << "INVALID CHOICE. PLEASE ENTER A VALID NUMBER." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getch();  // Wait for a key press
        consumeForSurvival();
    }

    int quantity;
    cout << "ENTER THE QUANTITY OF THE MEDICINE ITEM YOU WANT TO USE: ";
    cin >> quantity;

    Medicine chosenMedicine = medicineVector[choice - 1];
    if(MedicineItems[chosenMedicine] < quantity){
        cout << "Not enough stock. You only have " << MedicineItems[chosenMedicine] << "." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getch();  // Wait for a key press
        consumeForSurvival();
    } 
    else {
        for(int i = 0; i < quantity; i++){
            chosenMedicine.use(*player);
        }
        cout << "You used " << quantity << " many " << chosenMedicine.getName() << " items." << endl;
        player->newLife();   //change the state from DEFEATED to ALIVE
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Items* Backpack::useWeapons() {
    int number;
    int index;
    int ChosenWeapon;
    system("cls");
    cout << "Enter the index of the Weapon you want to attack with:" << endl 
    << "[1].WarmWeapons" << endl << "[2].ColdWeapons" << endl << "[3].ThrowableWeapons" << endl << "[4].Back" << endl;
    cin >> number;
    switch(number){
        case 1: 
            if(!WarmWeaponItems.empty()){
                while(true){
                    system("cls");
                    cout << "Choose the WarmWeapon you want to attack with:" << endl;
                    index = 1;
                    showWarmWeaponItems();    // Prints warm weapon items
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= WarmWeaponItems.size()) {
                        WarmWeapon *wweapon=new WarmWeapon(WarmWeaponItems.at(ChosenWeapon-1));
                        //cout << *wweapon;
                        return wweapon;
                    } 
                    else {
                        cout << "Invalid index. Try again." << endl;
                        cout << "Please press any key to continue...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "No WarmWeapon exists!" << endl;
                cout << "Please press any key to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                useWeapons();
            }
            break;

        case 2:
            if(!ColdWeaponItems.empty()){
                while(true){
                    system("cls");
                    cout << "Choose the ColdWeapon you want to attack with:" << endl;
                    index = 1;
                    showColdWeaponItems();      // Prints cold weapon items
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= ColdWeaponItems.size()) {
                        ColdWeapon* cweapon=new ColdWeapon(ColdWeaponItems.at(ChosenWeapon-1));
                        //cout << *cweapon;
                        return cweapon;
                    }
                    else {
                        cout << "Invalid index. Try again." << endl;
                        cout << "Please press any key to continue...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "No ColdWeapon exists!" << endl;
                cout << "Please press any key to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                useWeapons();
            }
            break;

        case 3:
            if(!ThrowableItems.empty()){
                while(true){
                    system("cls");
                    cout << "Choose the ThrowableWeapon you want to attack with:" << endl;
                    index = 1;
                    showThrowableItems();      // Prints throwable items
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= ThrowableItems.size()) {
                        auto iter = next(ThrowableItems.begin(), ChosenWeapon - 1);
                        Throwable* tweapon = new Throwable(iter->first);
                        //cout << *tweapon;
                        return tweapon;
                    }
                    else {
                        cout << "Invalid index. Try again." << endl;
                        cout << "Please press any key to continue...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "No ThrowableWeapon exists!" << endl;
                cout << "Please press any key to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                useWeapons();
            }
            break;

        case 4:
            BattleMenu();
            break;

        default:
            cout << "Invalid choice. Choose a number between 1 and 3." << endl;
            cout << "Please press any key to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            useWeapons();
    }
    return nullptr;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Items* Backpack::useConsumableItems() {
    int number;
    int index;
    int ChosenConsumableItem;
    system("cls");
    cout << "Enter the index of the Consumable Item you want to use:" << endl 
    << "[1].Medicine" << endl << "[2].Food" << endl << "[3].Back" << endl;
    cin >> number;
    switch(number){
        case 1: 
            if(!MedicineItems.empty()){
                while(true){
                    system("cls");
                    cout << "Choose the Medicine you want to use:" << endl;
                    index = 1;
                    showMedicineItems();
                    cin >> ChosenConsumableItem;
                    if (ChosenConsumableItem >= 1 && ChosenConsumableItem <= MedicineItems.size()) {
                        auto iter = next(MedicineItems.begin(), ChosenConsumableItem - 1);
                        Medicine* medicine = new Medicine(iter->first);
                        //cout << *medicine;
                        return medicine;
                    }
                    else {
                        cout << "Invalid index. Try again." << endl;
                        cout << "Please press any key to continue...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "No Medicine exists!" << endl;
                cout << "Please press any key to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                useConsumableItems();
            }
            break;

        case 2:
            if(!FoodItems.empty()){
                while(true){
                    system("cls");
                    cout << "Choose the Food you want to use:" << endl;
                    index = 1;
                    showFoodItems();
                    cin >> ChosenConsumableItem;
                    if (ChosenConsumableItem >= 1 && ChosenConsumableItem <= FoodItems.size()) {
                        auto iter = next(FoodItems.begin(), ChosenConsumableItem - 1);
                        Food* food = new Food(iter->first);
                        //cout << *food;
                        return food;
                    }
                    else{
                        cout << "Invalid index. Try again." << endl;
                        cout << "Please press any key to continue...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "No Food exists!" << endl;
                cout << "Please press any key to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                useConsumableItems();
            }
            break;

        case 3:
            BattleMenu();
            break;

        default:
            cout << "Invalid choice. Choose a number between 1 and 3." << endl;
            cout << "Please press any key to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            useConsumableItems();
    }
    return nullptr;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BankAccount::BankAccount(int n){
    this->balance = n; // Constructor that initializes balance to n
}

int BankAccount::getBalance() {
    return balance; // Returns the current balance
}

void BankAccount::deposit(int amount) { 
    this->balance += amount; // Increases the balance by the deposit amount
}

bool BankAccount::withdraw(int amount) {
    if (balance >= amount){
        balance -= amount; // Decreases the balance by the withdrawal amount if there's enough balance
        return true; // Returns true if withdrawal was successful
    }    
    else
        return false; // Returns false if there's not enough balance for withdrawal
}


// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Person::Person(string n,int l=1):name(n),level(l){} // Constructor that initializes name and level

void Person::updateLevel() {
    level++; // Increases level by 1
}

int Person::getLevel() {
    return level; // Returns the current level
}

string Person::getName() {
    return name; // Returns the name
}

int Person::getHealthPoints() {
    return hp.getCurrentHealth(); // Returns the current health points
}

int Person::getMaxHealth() {
    return hp.getMaxHealth();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Human::Human(string n,int s,int l=1):Person(n,l),stamina(s){} // Constructor that initializes name and stamina

Backpack* Human::getBackpack() {
    Backpack *p=&backpack;
    return p; // Returns a pointer to the backpack
}

int Human::getStamina() {
    return stamina.getCurrentStamina(); // Returns the current stamina
}

int Human::getMaxStamina() {
    return stamina.getMaxStamina();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BankAccount* Player::getBankAccount(){
    BankAccount* p = &bankAccount;
    return p; // Returns a pointer to the bank account
}

Player::Player(string n, string g,string un, int a,int m,int s) : Human(n,s), age(a), gender(g), bankAccount(m) , username(un), exp(this), state(PlayerState::ALIVE){}
// Constructor that initializes name, gender, username, age, money, and stamina

Player::Player(Human & human,string g,string un,int a,int m) : Human(human.getName(), human.getStamina()), age(a), gender(g), bankAccount(m), username(un),exp(this), state(PlayerState::ALIVE) {}
// Constructor that initializes name, gender, username, age, and money from a Human object

int Player::getAge() {
    return age; // Returns the age
}

string Player::getGender() {
    return gender; // Returns the gender
}

int Player::getMoney() {
    return bankAccount.getBalance(); // Returns the balance of the bank account
}

int Player::getExperience() {
    return exp.getCurrentExp(); // Returns the current experience
}

int Player::getMaxExperience() {
    return exp.getMaxExp();
}

string Player::getUsername(){
    return username;
}

void Player::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout << "You have been defeated!" << endl; // Prints a message if the player has been defeated
        state=PlayerState::DEFEATED;
    } 

    else 
        cout << "You took " << amount << " damage. Remaining your HP: " << hp.getCurrentHealth() << endl; // Prints a message if the player takes damage
    
}

void Player::newLife(){
    state=PlayerState::ALIVE;
}

PlayerState Player::getState(){
    return state;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

HumanEnemy::HumanEnemy(Human& human,int l=1): Human(human.getName(), human.getStamina(),l),state(HumanEnemyState::FIGHT),status(HumanEnemyStatus::ALIVE){} // Constructor that initializes name and stamina from a Human object

void HumanEnemy::setState(HumanEnemyState newState){
    state = newState;
}

HumanEnemyState HumanEnemy::getState(){
    return state;
}

HumanEnemyState HumanEnemy::getStatus(){
    return status;
}

void HumanEnemy::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout << name << " has been defeated!" << endl; // Prints a message if the human enemy has been defeated
        status = HumanEnemyStatus :: DEFEATED;
    } 
    else{ 
        cout << name << " takes " << amount << " damage. Remaining enemy HP: " << hp.getCurrentHealth() << endl; // Prints a message if the human enemy takes damage
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

HE_Controller :: HE_Controller (HumanEnemy HE) : model(HE) , backpack(HE.getBackpack()) , view(HE_View())  {}

void HE_Controller::updateState() {
    double healthRatio = model.getHealthPoints() / model.hp.getMaxHealth();
    double staminaRatio = model.stamina.getCurrentStamina() / model.stamina.getMaxStamina();

    // Check if health or stamina is below 0.4
    if (staminaRatio <= 0.4)
        model.setState(HumanEnemyState :: LOW_HEALTH);

    else if (healthRatio <= 0.4)
        model.setState(HumanEnemyState :: LOW_POWER);

    else
        model.setState(HumanEnemyState :: FIGHT);
}

HumanEnemyState HE_Controller::getState(){
    return model.getState();
}

HumanEnemyState HE_Controller::getStatus(){
    return model.getStatus();
}

Items* HE_Controller :: chooseWeapon() {
    srand(time(0)); // use current time as seed for random generator
    // Check if backpack is empty
    if (backpack->ThrowableItems.empty() && backpack->WarmWeaponItems.empty() && 
        backpack->ColdWeaponItems.empty()) {
        return nullptr; // backpack is empty
    }
    else {
        int itemType = rand() % 3; // Randomly choose between 5 types of items
        switch (itemType) {
            case 0: // Throwable
                {if (!backpack->ThrowableItems.empty()) {
                    int index = rand() % backpack->ThrowableItems.size();
                    auto it = next(backpack->ThrowableItems.begin(),index);
                    Throwable *item = new Throwable(it->first);
                    return item;
                }
                break;
                }
            case 1: // WarmWeapon
                {if (!backpack->WarmWeaponItems.empty()) {
                    int index = rand() % backpack->WarmWeaponItems.size();
                    WarmWeapon *item = new WarmWeapon(backpack->WarmWeaponItems[index]);
                    return item;
                }
                break;
                }
            case 2: // ColdWeapon
                {if (!backpack->ColdWeaponItems.empty()) {
                    int index = rand() % backpack->ColdWeaponItems.size();
                    ColdWeapon *item = new ColdWeapon(backpack->ColdWeaponItems[index]);
                    return item;
                }
                break;
                }
        }
    }
    return nullptr;
}

Food* HE_Controller :: chooseFood() {
    if (!backpack->FoodItems.empty()) {
        int index = rand() % backpack->FoodItems.size();
        auto it = next(backpack->FoodItems.begin(),index);;
        Food *item = new Food(it->first);
        return item;
    }

    return nullptr;
}

Medicine* HE_Controller :: chooseMedicine() {
    if (!backpack->MedicineItems.empty()) {
        int index = rand() % backpack->MedicineItems.size();
        auto it = next(backpack->MedicineItems.begin(),index);
        Medicine *item = new Medicine(it->first);
        return item;
    } else {
        return nullptr;
    }
}

void HE_Controller::Attack(Items* weapon){
    if(static_cast<WarmWeapon*>(weapon)){
        WarmWeapon *wweapon = static_cast<WarmWeapon*>(weapon);
        wweapon->Attack(model,*player);
        view.attackView(model.getName(),*wweapon);
    }
    if(static_cast<ColdWeapon*>(weapon)){
        ColdWeapon *cweapon = static_cast<ColdWeapon*>(weapon);

        cweapon->Attack(model,*player);
        view.attackView(model.getName(),*cweapon);
    }
    if(static_cast<Throwable*>(weapon)){
        Throwable *tweapon = static_cast<Throwable*>(weapon);
        tweapon->Throw(model,*player);
        view.attackView(model.getName(),*tweapon);
    }
}

void HE_Controller :: decision() {
    Medicine *mitem;
    Food *fitem;

    switch(model.getState()){
    case HumanEnemyState::LOW_HEALTH:
        mitem = chooseMedicine();
        if(mitem!=nullptr){
            mitem->use(model);
            view.updateStamina(model.getName(),mitem->getHeal());
        }
        break;

    case HumanEnemyState::LOW_POWER:
        fitem = chooseFood();
        if(fitem!=nullptr){
            fitem->use(model);
            view.updateHealth(model.getName(),fitem->getStrength());
        }
        break;

    case HumanEnemyState::FIGHT:
        Attack(chooseWeapon());
        break;

    default:
        break;
    }
}

void HE_Controller :: transferItems() {
    Backpack *playerBackpack = player->getBackpack();
    Backpack *enemyBackpack = model.getBackpack();

    // Transfer Food Items
    if(!enemyBackpack->FoodItems.empty()) {
        for(auto& item : enemyBackpack->FoodItems) {
            playerBackpack->addFoodItem(item.first, item.second);
        }
    }

    // Transfer Medicine Items
    if(!enemyBackpack->MedicineItems.empty()) {
        for(auto& item : enemyBackpack->MedicineItems) {
            playerBackpack->addMedicineItem(item.first, item.second);
        }
    }

    // Transfer Throwable Items
    if(!enemyBackpack->ThrowableItems.empty()) {
        for(auto& item : enemyBackpack->ThrowableItems) {
            playerBackpack->addThrowableItem(item.first, item.second);
        }
    }

    // Transfer Warm Weapon Items
    if(!enemyBackpack->WarmWeaponItems.empty()) {
        for(auto& item : enemyBackpack->WarmWeaponItems) {
            if(!playerBackpack->warmWeaponExistence(item)) {
                playerBackpack->addWarmWeaponItem(item);
            }
        }
    }

    // Transfer Cold Weapon Items
    if(!enemyBackpack->ColdWeaponItems.empty()) {
        for(auto& item : enemyBackpack->ColdWeaponItems) {
            if(!playerBackpack->coldWeaponExistence(item)) {
                playerBackpack->addColdWeaponItem(item);
            }
        }
    }
}

void HE_Controller :: showBackpackItems() {
    view.showBackpackItems();
}

void HE_Controller :: showInfo(){
    view.showInfo(model);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

HE_View::HE_View(){}

void HE_View :: showInfo(HumanEnemy model){
    cout << "Name : " << model.getName() << endl;
    cout << "Level : " << model.getLevel() << endl;
    cout << "Stamina : " << model.getStamina() <<  " / ( " << model.getMaxStamina() << " )" << endl;
    cout << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();  // Wait for a key press
}

void HE_View::updateHealth(string name,int amount) {
    cout << name << "'s health increased for "<< amount << "HP\n";
}

void HE_View::updateStamina(string name,int amount) {
    cout << name << "'s stamina increased for "<< amount << "STM\n";
}

void HE_View::attackView(string name,Items weapon) {
    cout << name << "is attacking you by "<< weapon.getName() << "\n";
}

void HE_View :: showBackpackItems() {
    Backpack *HE_Backpack = humanEnemy->getBackpack();
    HE_Backpack.showItems();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Zombie::Zombie(string n,int l=1) : Person(n,l) , state(ZombieState::ALIVE){} // Constructor that initializes name from a Person object

void Zombie::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout << name << " has been defeated!" << endl; // Prints a message if the zombie has been defeated
        state=ZombieState::DEFEATED;
    } 
    else {
        cout << name << " takes " << amount << " damage. Remaining Zombie HP: " << hp.getCurrentHealth() << endl; // Prints a message if the zombie takes damage
    }
}

ZombieState Zombie::getState(){
    return state;
}

void Zombie::bite(){}

void Zombie::scratch(){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BasicZombie::BasicZombie(Zombie& zombie) : Zombie(zombie.getName(),zombie.getLevel()){}
BasicZombie::BasicZombie(string n,int l) : Zombie(n,l){}

void BasicZombie :: bite() {
    player->hp.decreaseHealth(this->getLevel()*5);
    cout<<"You have been bitten by "<<this->getName();
    player->takeDamage(this->getLevel()*5);
}

// *----------------------------------------------------------------*

BZ_Controller :: BZ_Controller (BasicZombie BZ) : model(BZ) , view(BZ_View())  {}

ZombieState BZ_Controller::getState(){
    return model.getState();
}

void BZ_Controller :: bite() {
    model.bite();
}
 
void BZ_Controller :: showInfo() {
    view.showInfo(model);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BZ_View::BZ_View(){}

void BZ_View :: showInfo(BasicZombie model) {
    cout << "Name : " << model.getName() << endl;
    cout << "Level : " << model.getLevel() << endl;
    cout << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();  // Wait for a key press
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AdvZombie::AdvZombie(Zombie & zombie) : Zombie(zombie.getName(),zombie.getLevel()){}
AdvZombie::AdvZombie(string n,int l) : Zombie(n,l){}

void AdvZombie :: bite() {
    player->hp.decreaseHealth(this->getLevel()*7);
    cout<<"You have been bitten by "<<this->getName();
    player->takeDamage(this->getLevel()*7);
}

void AdvZombie :: scratch() {
    player->hp.decreaseHealth(this->getLevel()*8);
    cout<<"You have been scratched by "<<this->getName();
    player->takeDamage(this->getLevel()*8);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AZ_Controller :: AZ_Controller (AdvZombie AZ) : model(AZ) , view(AZ_View())  {}

ZombieState AZ_Controller::getState(){
    return model.getState();
}
 
void AZ_Controller :: showInfo() {
    view.showInfo(model);
}

void AZ_Controller::Attack(){
    srand(time(0));
    int randomNum = rand();

    if(randomNum % 2 == 0){
        model.bite();
    }

    else{
        model.scratch();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AZ_View::AZ_View(){}

void AZ_View :: showInfo(AdvZombie model) {
    cout << "Name : " << model.getName() << endl;
    cout << "Level : " << model.getLevel() << endl;
    cout << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();  // Wait for a key press
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Items::Items(string n,int p,string t):name(n),price(p),type(t){} // Constructor that initializes name, price, and type

bool Items::operator==(const Items& other) const {
    return (name == other.name); // Checks if two items are equal based on their names
}

bool Items::operator<(const Items& other) const {
    return name < other.name; // Checks if one item is less than another based on their names
}

string Items::getName(){
    return name; // Returns the name of the item
}

string Items::getType(){
    return type; // Returns the type of the item
}

int Items::getPrice(){
    return price;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Permanent::Permanent(string n,int p,string t,int e,int h):Items(n,p,t),exp(e),harm(h){} // Constructor that initializes name, price, type, exp, and harm from an Items object

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

WarmWeapon::WarmWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Warm Weapon",e,h),wwa(x){
    WarmWeapon::addToVectors(); // Adds this warm weapon to vectors
}

void WarmWeapon::showItems(){
    int n=1;
    for(WarmWeapon i : shop_items_permanent_warmweapon){
        cout << "[" << n <<"]. " << i << endl; // Prints each warm weapon item
        n++;
    }
}

void WarmWeapon::buy(){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(backpack->warmWeaponExistence(*this)){
        cout<<"This item already exist in your backpack!\n"; // Prints a message if the item already exists in the backpack
    }

    else if(creditcard->withdraw(price)){
        cout << "Item bought successfully!\n"; // Prints a message if the item was bought successfully
        backpack->addWarmWeaponItem(*this); // Adds the item to the backpack
        cout<<"Item added to your backpack!\n"; // Prints a message that the item was added to the backpack
        player->exp.increaseExp(exp); // Increases the player's experience
        cout<<"EXP increased!\n"; // Prints a message that the experience was increased
    }
    else
        cout<<"Not enough money!\n"; // Prints a message if there's not enough money to buy the item
}

void WarmWeapon::addToVectors(){
    shop_items_permanent_warmweapon.push_back(*this); // Adds this warm weapon to the permanent warm weapon vector
    shop_items_permanent.push_back(*this); // Adds this warm weapon to the permanent items vector
    shop_items.push_back(*this); // Adds this warm weapon to the items vector
}

void WarmWeapon::Attack(Human attacker, Person attacked){
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) + (wwa.getCurrentSkill() * weaponLevelFactor) + (attacker.getLevel() * playerLevelFactor);
    attacker.stamina.decreaseStamina(10*wwa.getCurrentSkill()); // Decreases the attacker's stamina
    attacked.hp.decreaseHealth(damage); // Decreases the attacked person's health
    attacked.takeDamage(damage);  //attacked takes damage 

    if(static_cast<Player*>(&attacker)){
        Player *p=static_cast<Player*>(&attacker);
        p->exp.setCurrentExp(0,damage,10*wwa.getCurrentSkill()); // Updates the player's as attacker experience
    }

    else{
        Player *p=static_cast<Player*>(&attacked);
        p->exp.setCurrentExp(damage,0,0); // Updates the attacked player's as attacked experience
    }
}

ostream& operator<<(ostream &os,WarmWeapon &obj){
    os << obj.name << "(+" << obj.exp << "EXP): " << "lvl : "<< obj.wwa.getCurrentSkill() << " , harm : "<< obj.harm << " , price : " << obj.price << "$";
    return os; // Prints the warm weapon's details
}

WarmWeaponAbility WarmWeapon::getwwa(){
    return wwa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ColdWeapon::ColdWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Cold Weapon",e,h),cwa(x){
    ColdWeapon::addToVectors(); // Adds this cold weapon to vectors
}

void ColdWeapon::showItems(){
    int n=1;
    for (ColdWeapon i : shop_items_permanent_coldweapon){
        cout << "[" << n <<"]. " << i << endl; // Prints each cold weapon item
        n++;
    }
}

void ColdWeapon::buy(){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(backpack->coldWeaponExistence(*this)){
        cout<<"This item already exist in your backpack!\n"; // Prints a message if the item already exists in the backpack
    }

    else if(creditcard->withdraw(price)){
        cout << "Item bought successfully!\n"; // Prints a message if the item was bought successfully
        backpack->addColdWeaponItem(*this); // Adds the item to the backpack
        cout<<"Item added to your backpack!\n"; // Prints a message that the item was added to the backpack
        player->exp.increaseExp(exp); // Increases the player's experience
        cout<<"EXP increased!\n"; // Prints a message that the experience was increased
    }
    else
        cout<<"Not enough money!\n"; // Prints a message if there's not enough money to buy the item
}

void ColdWeapon::addToVectors(){
    shop_items_permanent_coldweapon.push_back(*this); // Adds this cold weapon to the permanent cold weapon vector
    shop_items_permanent.push_back(*this); // Adds this cold weapon to the permanent items vector
    shop_items.push_back(*this); // Adds this cold weapon to the items vector
}

void ColdWeapon::Attack(Human attacker, Person attacked){
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) + (cwa.getCurrentSkill() * weaponLevelFactor) + (attacker.getLevel() * playerLevelFactor);
    attacker.stamina.decreaseStamina(10*cwa.getCurrentSkill()); // Decreases the attacker's stamina
    attacked.hp.decreaseHealth(damage); // Decreases the attacked person's health

    if(static_cast<Player*>(&attacker)){
        Player *p=static_cast<Player*>(&attacker);
        p->exp.setCurrentExp(0,damage,10*cwa.getCurrentSkill()); // Updates the attacker's experience

        if(static_cast<HumanEnemy*>(&attacked)){
            HumanEnemy *he=static_cast<HumanEnemy*>(&attacked);
            he->takeDamage(damage); // The attacked human enemy takes damage
        }

        else if(static_cast<Zombie*>(&attacked)){
            Zombie *z=static_cast<Zombie*>(&attacked);
            z->takeDamage(damage); // The attacked zombie takes damage
        }
    }
    else{
        Player *p=static_cast<Player*>(&attacked);
        p->exp.setCurrentExp(damage,0,0); // Updates the attacked player's experience
    }
}

ostream& operator<<(ostream& os,ColdWeapon& obj){
    os << obj.name << "(+" << obj.exp << "EXP): " << "lvl : "<< obj.cwa.getCurrentSkill() << " , harm : "<< obj.harm << " , price : " << obj.price << "$";
    return os; // Prints the cold weapon's details
}

ColdWeaponAbility ColdWeapon::getcwa(){
    return cwa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Throwable::Throwable(string n,int p,int x,int e,int h):Items(n,p,"Throwable Weapon"),twa(x),exp(e),harm(h){
    Throwable::addToVectors(); // Adds this throwable weapon to vectors
}

void Throwable::showItems(){
    int n=1;
    for (Throwable i : shop_items_throwable){
        cout << "[" << n <<"]. " << i << endl; // Prints each throwable weapon item
        n++;
    }
}

void Throwable::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout << "Item bought successfully!\n"; // Prints a message if the item was bought successfully
        backpack->addThrowableItem(*this,quantity); // Adds the item to the backpack
        cout<<"Item added to your backpack!\n"; // Prints a message that the item was added to the backpack
        player->exp.increaseExp(exp*quantity); // Increases the player's experience
        cout<<"EXP increased!\n"; // Prints a message that the experience was increased
    }
    else
        cout<<"Not enough money!\n"; // Prints a message if there's not enough money to buy the item
}

void Throwable::addToVectors(){
    shop_items_throwable.push_back(*this); // Adds this throwable weapon to the throwable weapon vector
    shop_items.push_back(*this); // Adds this throwable weapon to the items vector
}

void Throwable::Throw(Human attacker, Human attacked){
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) + (twa.getCurrentSkill() * weaponLevelFactor) + (attacker.getLevel() * playerLevelFactor);
    attacker.stamina.decreaseStamina(10*twa.getCurrentSkill()); // Decreases the attacker's stamina
    attacked.hp.decreaseHealth(damage); // Decreases the attacked person's health
    attacked.takeDamage(damage);

    if(static_cast<Player*>(&attacker)){
        Player *p=static_cast<Player*>(&attacker);
        p->exp.setCurrentExp(0,damage,10*twa.getCurrentSkill()); // Updates the attacker's experience
        Backpack *b=p->getBackpack();
        b->useThrowableItemCount(*this); // Uses a throwable item from the attacker's backpack
    }
    else{
        Player *p=static_cast<Player*>(&attacked);
        p->exp.setCurrentExp(damage,0,0); // Updates the attacked player's experience
        Backpack *b=attacker.getBackpack();
        b->useThrowableItemCount(*this); // Uses a throwable item from the attacked person's backpack
    }
}

ostream& operator<<(ostream& os,Throwable& obj) {
    os << obj.name << "(+" << obj.exp << "EXP): " << "lvl : "<< obj.twa.getCurrentSkill() << " , harm : "<< obj.harm << " , price : " << obj.price << "$ (each)";
    return os; // Prints the throwable weapon's details
}

ThrowableWeaponAbility Throwable::gettwa(){
    return twa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Medicine::Medicine(string n,int p,int h):Items(n,p,"Medicine"),heal(h){
    Medicine::addToVectors(); // Adds this medicine to vectors
}

void Medicine::showItems(){
    int n=1;
    for (Medicine i : shop_items_medicine){
        cout << "[" << n <<"]. " << i << endl; // Prints each medicine item
        n++;
    }
}

void Medicine::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout << "Item bought successfully!\n"; // Prints a message if the item was bought successfully
        backpack->addMedicineItem(*this,quantity); // Adds the item to the backpack
        cout<<"Item added to your backpack!\n"; // Prints a message that the item was added to the backpack
    }
    else
        cout<<"Not enough money!\n"; // Prints a message if there's not enough money to buy the item
}

void Medicine::addToVectors(){
    shop_items_medicine.push_back(*this); // Adds this medicine to the medicine vector
    shop_items.push_back(*this); // Adds this medicine to the items vector
}

void Medicine::use(Human human){
    human.hp.increaseHealth(heal); // Increases the human's health
    Backpack *b=human.getBackpack();
    b->useMedicineItemCount(*this); // Uses a medicine item from the human's backpack
}

int Medicine::getHeal() {
    return heal;
}

ostream& operator<<(ostream& os,Medicine& obj) {
    os << obj.name << "(+" << obj.heal << "HP): " << obj.price << "$ (each)";
    return os; // Prints the medicine's details
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Food::Food(string n,int p,int s):Items(n,p,"Food"),strength(s){
    Food::addToVectors(); // Adds this food to vectors
}

void Food::showItems(){
    int n=1;
    for (Food i : shop_items_food){
        cout << "[" << n <<"]. " << i << endl; // Prints each food item
        n++;
    }
}

void Food::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout << "Item bought successfully!\n"; // Prints a message if the item was bought successfully
        backpack->addFoodItem(*this,quantity); // Adds the item to the backpack
        cout<<"Item added to your backpack!\n"; // Prints a message that the item was added to the backpack
    }
    else
        cout<<"Not enough money!\n"; // Prints a message if there's not enough money to buy the item
}

void Food::addToVectors(){
    shop_items_food.push_back(*this); // Adds this food to the food vector
    shop_items.push_back(*this); // Adds this food to the items vector
}

void Food::use(Human human){
    human.stamina.increaseStamina(strength); // Increases the human's stamina
    Backpack *b=human.getBackpack();
    b->useFoodItemCount(*this); // Uses a food item from the human's backpack
}

int Food::getStrength (){
    return strength;
}

ostream& operator<<(ostream& os,Food& obj) {
    os << obj.name << "(+" << obj.strength << "STM): " << obj.price << "$ (each)";
    return os; // Prints the food's details
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Human* Factory::createCharacter(const string& type) {
    // Creates a character of a specific type
    if (type == "JonSnow") {
        return createJonSnow(type);
    }
    else if (type == "JaimeLannister") {
        return createJaimeLannister(type);
    }
    else if (type == "Daenerys") {
        return createDaenerys(type);
    }
    else if (type == "Stannis") {
        return createStannis(type);
    }
    else if (type == "Joffrey") {
        return createJoffrey(type);
    }
    else if (type == "TheonGreyjoy") {
        return createTheonGreyjoy(type);
    }
    else {
        return nullptr; // Returns null if type is not recognized
    }
}

Human* Factory::createJonSnow(const string& type) {
    return new Human(type, /*stamina*/ 100); // Creates a Jon Snow character with 100 stamina
}

Human* Factory::createJaimeLannister(const string& type) {
    return new Human(type, /*stamina*/ 70); // Creates a Jaime Lannister character with 70 stamina
}

Human* Factory::createDaenerys(const string& type) {
    return new Human(type, /*stamina*/ 110); // Creates a Daenerys character with 110 stamina
}

Human* Factory::createStannis(const string& type) {
    return new Human(type, /*stamina*/ 90); // Creates a Stannis character with 90 stamina
}

Human* Factory::createJoffrey(const string& type) {
    return new Human(type, /*stamina*/ 75); // Creates a Joffrey character with 75 stamina
}

Human* Factory::createTheonGreyjoy(const string& type) {
    return new Human(type, /*stamina*/ 85); // Creates a Theon Greyjoy character with 85 stamina
}

BasicZombie* Factory :: createBasicZombie(const int level) {
    return new BasicZombie("BasicZombie",level); // Creates a basic zombie
}

AdvZombie* Factory :: createAdvanceZombie(const int level) {
    return new AdvZombie("AdvanceZombie",level); // Creates a Advance zombie
}


// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void print_with_delay(string text, int delay=4) {
    for (char c : text) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

void getUserInfo(int& age , string& gender , string& username) {
    cout << "Please Enter your Name : ";
    cin >> username;
    cout << "Please Enter your Age : ";
    cin >> age;
    cout << "Please Enter your Gender : ";
    cin >> gender;
    cout << "Thanks for introducing yourself";
    cout << endl << "Please press any key to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();
    system("cls");
}

void showPlayerInfo() {
    cout << "Name : " << player->getName() << endl;
    cout << "Level : " << player->getLevel() << endl;
    cout << "Experience : " << player->getExperience() << " / ( " << player->getMaxExperience() << " )" << endl;
    cout << "Stamina : " << player->getStamina() <<  " / ( " << player->getMaxStamina() << " )" << endl;
    cout << "Health : " << player->getHealthPoints() << " / ( " << player->getMaxHealth() << " )" << endl;
    cout << "Money : " << player->getMoney() << " $" << endl << endl;
}

void createItem() {
    createWarmWeapons();
    createColdWeapons();
    createThrowableItems();
    createMedicines();
    createFoods();
}

void medicineMenu() {
    system("cls");

    if(player->getMoney() >= 15) {
        int item,quantity;
        Medicine *drug;
        cout << "You go to take a look at the Medicines:" << "(your money : " << player->getMoney() << ")" << endl;
        Medicine::showItems(); // Shows medicines
        cout << "which one do you want to buy?" << endl;
        cin >> item;
        cout << "How many?" << endl;
        cin >> quantity;
        drug=new Medicine(Medicine::shop_items_medicine.at(item-1));
        if(player->getMoney() >= drug->getPrice() * quantity) {
            drug->buy(quantity); // Buys a medicine
        }
        else {
            cout << "Not Enough Money To Purchase " << quantity << " Amounts Of " << drug->getName() << " Items. Buy Less Items ... \n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            medicineMenu();
        }
    } 
    else {
        //the Player Looses.
        goodbye();
    }
}

void playground() {
    system("cls");
    srand(time(0));
    Backpack *playerBackpack = player->getBackpack();
    Backpack *enemyBackpack;
    int randomNum = rand();
    //check the player state
    if(player->getState()==PlayerState::DEFEATED){
        cout<<"YOU HAVE BEEN DEFEATED\nYOUR HP IS 0 \nTO CONTINUE YOU NEED TO INCREASE YOUR HP... \n"; 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getch();  // Wait for a key press
        cout<<"going to your backpack...\n";
        playerBackpack->consumeForSurvival();
    }

    int choice;
    if ((randomNum % 100) < 70) {
    //fight ground
        randomNum=rand();

        if (randomNum % 2 == 0) {
        //fight with human enemy

            //create random humanEnemy from characters
            int index = rand() % characterTypes.size();
            Human *character=Factory::createCharacter(characterTypes[index]);
            humanEnemy = new HumanEnemy(*character,player->getLevel());

            // Add items to the humanEnemy's backpack
            Backpack *bp = humanEnemy->getBackpack();
            bp->addWarmWeaponItem(WarmWeapon::shop_items_permanent_warmweapon.at(rand() % WarmWeapon::shop_items_permanent_warmweapon.size()));
            bp->addColdWeaponItem(ColdWeapon::shop_items_permanent_coldweapon.at(rand() % ColdWeapon::shop_items_permanent_coldweapon.size()));
            for (int j = 0; j < 100; j++) {
                bp->addThrowableItem(Throwable::shop_items_throwable.at(rand() % Throwable::shop_items_throwable.size()), 1);
                bp->addFoodItem(Food::shop_items_food.at(rand() % Food::shop_items_food.size()), 1);
                bp->addMedicineItem(Medicine::shop_items_medicine.at(rand() % Medicine::shop_items_medicine.size()), 1);
            }

            //show enemy's info
            cout << "THE HUMAN ENEMY YOU ARE FACING IS : " << endl;
            cout << "Name : " << humanEnemy->getName() << endl;
            cout << "Level : " << humanEnemy->getLevel() << endl;
            cout << "Stamina : " << humanEnemy->getStamina() <<  " / ( " << humanEnemy->getMaxStamina() << " )" << endl;
            cout << "Health : " << humanEnemy->getHealthPoints() << " / ( " << humanEnemy->getMaxHealth() << " )" << endl << endl;
            cout << "PRESS ANY KEY TO ENTER THE FIGHTGROUND...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press

            system("cls");

            battleGround_humanEnemy();
        }

        else{
        //fight with zombie
            randomNum = rand();
            if ((randomNum % 100) < 50) {
                //fight with basic zombie

                //create basic zombie
                basicZombie = Factory::createBasicZombie(player->getLevel());

                //show enemy's info
                cout << "THE BASIC ZOMBIE YOU ARE FACING IS : " << endl;
                cout << "Name : " << basicZombie->getName() << endl;
                cout << "Level : " << basicZombie->getLevel() << endl;
                cout << "Health : " << basicZombie->getHealthPoints() << " / ( " << basicZombie->getMaxHealth() << " )" << endl << endl;
                cout << "Press any key to Enter to fightground...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press

                system("cls");

                battleGround_basicZombie();
            }

            else{
            //fight with advance zombie

                //create advance zombie
                advZombie = Factory::createAdvanceZombie(player->getLevel());

                //show enemy's info
                cout << "THE ADVANCED ZOMBIE YOU ARE FACING IS : " << endl;
                cout << "Name : " << advZombie->getName() << endl;
                cout << "Level : " << advZombie->getLevel() << endl;
                cout << "Health : " << advZombie->getHealthPoints() << " / ( " << advZombie->getMaxHealth() << " )" << endl << endl;
                cout << "Press any key to Enter to fightground...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                
                system("cls");

                battleGround_advZombie();
            }
        }
    }

    else {
    //reach to shop menu
        ShopMenu();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Menu() {
    // just getting user info :)))
    int age;
    string gender,username;

    system("cls");

    print_with_delay("In the land of Westeros, war and tensions among powerful families have always existed. But now, the wrath and uninvited power have cast a harsh shadow over this land.\nYou, a hero, are faced with an important mission. You must navigate through the dangers and immense obstacles ahead and confront the looming threats that menace the land of Westeros.\nSo who are you?\n");
    getUserInfo(age , gender , username);

    print_with_delay("In this journey, you must choose your character. Will Jon Snow, the strong and just commander, seize the fate of the land? Or will you, instead of him, travel with Jaime Lannister, the intelligent knight and seasoned strategist, and overcome all obstacles? Or perhaps with Daenerys Targaryen, the dangerous and powerful queen, you seek to rule over Westeros?\n\nYour decision can change the fate of the land. Are you ready?");
    cout << endl << "Please press any key to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();

    system("cls");
    
    // Creating the Player's Character Choices
    int chosenIndex;
    int money[6]={10000, 18000, 12000, 13000, 20000, 11000};

    // Show all the different options a user has for the characters
    Human *character;
    
    for (int i = 0; i < 6; i++) {
        character = Factory::createCharacter(characterTypes[i]);
        cout << i+1 << ". " << characterTypes[i] << endl;
        cout << "   " << "Stamina : " << character->getStamina() << endl;
        cout << "   " << "Money : " << money[i] << " $" << endl;
    }
    cout << "CHOOSE THE INDEX OF THE CHARACTER YOU WANTED : " << endl;
    
    // Get the user's choice
    cin >> chosenIndex;
    chosenIndex--;  // Adjust for 0-based indexing

    system("cls");

    // Create Player Character
    character = Factory::createCharacter(characterTypes[chosenIndex]);
    player = new Player(*character,gender,username,age,money[chosenIndex]);
    characterTypes.erase(characterTypes.begin()+chosenIndex);

    // Show the details of the Player
    cout << "CHARACTER YOU HAVE CHOSEN IS : " << endl;
    cout << "\t" << "Name : " << player->getName() << endl;
    cout << "\t" << "Level : " << player->getLevel() << endl;
    cout << "\t" << "Experience : " << player->getExperience() << endl;
    cout << "\t" << "Stamina : " << player->getStamina() << endl;
    cout << "\t" << "Health : " << player->getHealthPoints() << endl;
    cout << "\t" << "Money : " << money[chosenIndex] << " $" << endl << endl;

    print_with_delay("Now that you have chosen your CHARACTER, you will go to SHOP to buy WEAPONS to fight with.\n");
    cout << "Press any key to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();  // Wait for a key press

    createItem();
    Shop_PermanentItems_Menu();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void ShopMenu() {
    while(true){
        system("cls");
        int number;
        cout << "You enter the shop." << endl << "What do you want to buy?" << "(your money : " << player->getMoney() << ")" << endl 
        << "[1].Permanent Items" << endl << "[2].Throwable Items" << endl << "[3].Consumable Items" << endl << "[4].Exit" << endl;
        cin >> number;
        switch(number){
            case 1: Show_Permanent_Items(); // Shows permanent items
                break;
        
            case 2: Show_Throwable_Items(); // Shows throwable items
                break;
        
            case 3: Show_Consumable_Items(); // Shows consumable items
                break;
        
            case 4: 
                cout << "You left the shop." << endl; // Exits the shop
                cout << "Please press any key to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch();  // Wait for a key press
                playground();

            default: 
                cout << "Wrong number!" << endl << "Please press enter to continue..." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getch(); // Handles invalid input
                break;
        }
    }
}

void Show_Permanent_Items() {
    system("cls");
    int number,item;
    WarmWeapon *wweapon;
    ColdWeapon *cweapon;
    cout << "Permanent Items:" << endl << "[1].WarmWeapons" << endl << "[2].ColdWeapons" << endl << "[3].back" << endl;
    cin>>number;
    system("cls");
    switch(number){
        case 1: 
            cout << "You go to take a look at the WarmWeapons:" << "(your money : " << player->getMoney() << ")" << endl;
            WarmWeapon::showItems(); // Shows warm weapons
            cout << "[0] : Back" << endl;
            cout << "which one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Show_Permanent_Items();
            }
            wweapon=new WarmWeapon(WarmWeapon::shop_items_permanent_warmweapon.at(item-1));
            wweapon->buy(); // Buys a warm weapon
            cout << "Please press any key to continue shopping...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            break;
        
        case 2:
            cout << "You go to take a look at the ColdWeapons:" << "(your money : " << player->getMoney() << ")" << endl;
            ColdWeapon::showItems(); // Shows cold weapons
            cout << "[0] : Back" << endl;
            cout << "which one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Show_Permanent_Items();
            }
            cweapon=new ColdWeapon(ColdWeapon::shop_items_permanent_coldweapon.at(item-1));
            cweapon->buy(); // Buys a cold weapon
            cout << "Please press any key to continue shopping...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            break;
        
        case 3: 
            ShopMenu(); // Returns to the previous menu
    }
}

void Show_Throwable_Items() {
    system("cls");
    int item,quantity;
    Throwable *tweapon;
    cout << "You go to take a look at the Throwable Items:" << "(your money : " << player->getMoney() << ")" << endl;
    Throwable::showItems(); // Shows throwable items
    cout << "[0] : Back" << endl;
    cout<<"which one do you want to buy?" << endl;
    cin >> item;
    if ( item == 0) {
        ShopMenu();
    }
    cout << "How many?" << endl;
    cin >> quantity;
    tweapon=new Throwable(Throwable::shop_items_throwable.at(item-1));
    tweapon->buy(quantity); // Buys a throwable item
    cout << "Please press any key to continue shopping...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getch();  // Wait for a key press
}

void Show_Consumable_Items() {
    system("cls");
    int number,item,quantity;
    Medicine *drug;
    Food *food;
    cout << "Consumable Items:" << endl << "[1].Medicines" << endl << "[2].Foods" << endl << "[3].back" << endl;
    cin>>number;
    system("cls");
    switch(number){
        case 1: 
            cout << "You go to take a look at the Medicines:" << "(your money : " << player->getMoney() << ")" << endl;
            Medicine::showItems(); // Shows medicines
            cout << "[0] : Back" << endl;
            cout << "which one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Show_Consumable_Items();
            }
            cout << "How many?" << endl;
            cin >> quantity;
            drug=new Medicine(Medicine::shop_items_medicine.at(item-1));
            drug->buy(quantity); // Buys a medicine
            cout << "Please press any key to continue shopping...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            break;
            
        case 2:
            cout << "You go to take a look at the Foods:" << "(your money : " << player->getMoney() << ")" << endl;
            Food::showItems(); // Shows foods
            cout << "[0] : Back" << endl;
            cout << "which one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Show_Consumable_Items();
            }
            cout << "How many?" << endl;
            cin >> quantity;
            food=new Food(Food::shop_items_food.at(item-1));
            food->buy(quantity); // Buys a food
            cout << "Please press any key to continue shopping...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            break;
        case 3: 
            ShopMenu(); // Returns to the previous menu
    }
}

void Shop_PermanentItems_Menu() {
    system("cls");
    int number,item;
    WarmWeapon *wweapon;
    ColdWeapon *cweapon;
    cout << "You enter the shop to buy atleast one Permanent Item to fight with." << endl << "What do you want to buy?" << "(your money : " << player->getMoney() << ")" << endl
    << "Permanent Items:" << endl << "[1].WarmWeapons" << endl << "[2].ColdWeapons" << endl;
    cin >> number;
    system("cls");
    switch(number){
        case 1: 
            cout << "You go to take a look at the WarmWeapons:" << "(your money : " << player->getMoney() << ")" << endl;
            WarmWeapon::showItems(); // Shows warm weapons
            cout << "[0] : Back" << endl;
            cout << "which one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Shop_PermanentItems_Menu();
            }
            wweapon=new WarmWeapon(WarmWeapon::shop_items_permanent_warmweapon.at(item-1));
            wweapon->buy(); // Buys a warm weapon
            print_with_delay("Ok, Now that you have bought a WarmWeapon, you can continue shopping and buy other Items that you want.\n");
            cout << "Please press any key to continue shopping...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            ShopMenu();
        
        case 2:
            cout << "You go to take a look at the ColdWeapons:" << "(your money : " << player->getMoney() << ")" << endl;
            ColdWeapon::showItems(); // Shows cold weapons
            cout << "[0] : Back" << endl;
            cout << "which one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Shop_PermanentItems_Menu();
            }
            cweapon=new ColdWeapon(ColdWeapon::shop_items_permanent_coldweapon.at(item-1));
            cweapon->buy(); // Buys a cold weapon
            print_with_delay("Ok, Now that you have bought a ColdWeapon, you can continue shopping and buy other Items that you want.\n");
            cout << "Please press any key to continue shopping...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            ShopMenu();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void goodbye(){
    if(player->getState()==PlayerState::DEFEATED){
        cout<<"GAME OVER!";
        exit(0);
    }
    else{
        cout<<"Really?\n";
        Sleep(1000);
        cout<<"It to soon!\n";
        Sleep(1000);
        cout<<"fine i let you go :(\nbye bye "<<player->getUsername()<<"hope to see you again:)";
        exit(0);
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void battleGround_humanEnemy(){
    int turn = 1;        //odd turn for player even turn for enemy
    HE_Controller Enemy(*humanEnemy);
    while(Enemy.getStatus()==HumanEnemyStatus::ALIVE and player->getState()==PlayerState::ALIVE){
        if(turn%2!=0){
        //player turn

            cout<<"Your Turn:\n";
            showPlayerInfo();
            Enemy.showInfo();
            BattleMenu();
        }
        else{
        //enemy turn

            cout<<"Enemy's Turn:\n";
            Enemy.updateState();
            Enemy.decision();
            Enemy.showInfo();
        }
        turn++;
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<"Congratulations !! , YOU HAVE WON THE MATCH \n The following item's will add to you backpack : \n";
        
        //show Enemy's item
        //method to add to player backpack;
        cout << "THE FOLLOWING ITEMS WILL BE ADDED TO YOUR BACKPACK : \n";
        Enemy.showBackpackItems();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getch();
        Enemy.transferItems();
    }

}

// *----------------------------------------------------------------*

void battleGround_basicZombie(){
    int turn = 1;        //odd turn for player even turn for enemy
    BZ_Controller Enemy(*basicZombie);
    while(Enemy.getState()==ZombieState::ALIVE and player->getState()==PlayerState::ALIVE){
        if(turn%2!=0){
        //player turn

            cout<<"Your Turn:\n";
            showPlayerInfo();
            Enemy.showInfo();
            BattleMenu();
        }
        else{
        //enemy turn
            cout<<"Enemy's Turn:\n";
            Enemy.bite();
            Enemy.showInfo();
        }
        turn ++;
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<"Congratulations !! , YOU HAVE WON THE MATCH \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getch();
    }
}

// *----------------------------------------------------------------*

void battleGround_advZombie(){
    int turn = 1;        //odd turn for player even turn for enemy
    AZ_Controller Enemy(*advZombie);
    while(Enemy.getState()==ZombieState::ALIVE and player->getState()==PlayerState::ALIVE){
        if(turn%2!=0){
        //player turn
        
            cout<<"Your Turn:\n";
            showPlayerInfo();
            Enemy.showInfo();
            BattleMenu();
        }
        else{
        //enemy turn
        cout<<"Enemy's Turn:\n";
        Enemy.Attack();
        Enemy.showInfo();
        }
        turn ++;
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<"Congratulations !! , YOU HAVE WON THE MATCH \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getch();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void BattleMenu() {
    cout << endl << endl; 
    int number;
    cout << "What do you want to do? (Attack ends your turn.)" << endl
    << "[1].Attack" << endl << "[2].BackPack" << endl << "[3].Player Info" << endl << "[4].Upgrade Weapon's Skill" << endl ;
    cout << "ENTER YOUR CHOICE OF ACTION : " ;
    cin >> number;
    WarmWeapon* wweapon;
    ColdWeapon* cweapon;
    Throwable* tweapon;
    Backpack *backpack = player->getBackpack();
    switch(number){
        case 1: 
            {auto weapon = backpack->useWeapons();
            if (static_cast<WarmWeapon*>(weapon)){
                wweapon = static_cast<WarmWeapon*>(weapon);
                wweapon->Attack(*player,*humanEnemy);
            }
            else if (static_cast<ColdWeapon*>(weapon)){
                cweapon = static_cast<ColdWeapon*>(weapon);
                cweapon->Attack(*player,*humanEnemy);
            }
            else{
                tweapon = static_cast<Throwable*>(weapon);
                tweapon->Throw(*player,*humanEnemy);
            }
            break;}
            
        case 2:
            {auto consumable = backpack->useConsumableItems();
            if (static_cast<Medicine*>(consumable)){
                Medicine* medicine = static_cast<Medicine*>(consumable);
                medicine->use(*player);
            }
            else{
                Food* food = static_cast<Food*>(consumable);
                food->use(*player);
            }
            BattleMenu();
            break; }

        case 3: 
            {showPlayerInfo();
            cout << "\nPress any key to go back...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getch();  // Wait for a key press
            break; }

        case 4:
            {BankAccount* creditcard = player->getBankAccount();
            auto chosenweapon = backpack->useWeapons();
            if (static_cast<WarmWeapon*>(chosenweapon)){
                WarmWeapon* wweapon = static_cast<WarmWeapon*>(chosenweapon);
                WarmWeaponAbility wwa = wweapon->getwwa();
                wwa.upgradeSkill(creditcard);
            }     
            else if (static_cast<ColdWeapon*>(chosenweapon)){
                ColdWeapon* cweapon = static_cast<ColdWeapon*>(chosenweapon);
                ColdWeaponAbility cwa = cweapon->getcwa();
                cwa.upgradeSkill(creditcard);
            }
            else{
                Throwable* tweapon = static_cast<Throwable*>(chosenweapon);
                ThrowableWeaponAbility twa= tweapon->gettwa();
                twa.upgradeSkill(creditcard);
            }
            BattleMenu();
            break; }

        default:
            {cout << "Invalid choice. Please choose a number between 1 and 4." << endl;
            BattleMenu(); }
    }
}



// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void createWarmWeapons() {
    // Creating warm weapons with their respective attributes
    WarmWeapon longclaw("Longclaw", 2800, 3, 20, 15);
    WarmWeapon wildfire("Wildfire Flask", 2500, 1, 25, 18);
    WarmWeapon dragonfire("Dragonfire Bow", 2200, 2, 15, 22);
    WarmWeapon valyrian("Valyrian Steel Dagger", 1900, 3, 30, 14);
    WarmWeapon dragonglass("Dragonglass Blade", 1600, 1, 12, 20);
    WarmWeapon ice("Ice Sword", 3000, 2, 18, 12);
    WarmWeapon crimson("Crimson Sword", 2700, 2, 22, 16);
    WarmWeapon shadow("Shadow Dagger", 2400, 3, 28, 19);
    WarmWeapon blackfyre("Blackfyre Sword", 2100, 1, 16, 23);
    WarmWeapon storm("Storm Bow", 1800, 2, 14, 17);
}

void createColdWeapons() {
    // Creating cold weapons with their respective attributes
    ColdWeapon icicle("Icicle Dagger", 2800, 3, 30, 13);
    ColdWeapon frostbite("Frostbite Axe", 2500, 2, 20, 18);
    ColdWeapon blizzard("Blizzard Mace", 2200, 1, 25, 21);
    ColdWeapon snowfall("Snowfall Sword", 1900, 2, 18, 16);
    ColdWeapon freeze("Freeze Bow", 1600, 1, 12, 11);
    ColdWeapon frost("Frost Dagger", 2700, 3, 16, 22);
    ColdWeapon coldsteel("Coldsteel Axe", 2400, 1, 22, 14);
    ColdWeapon winter("Winter Mace", 2100, 2, 28, 19);
    ColdWeapon icy("Icy Sword", 1800, 1, 14, 17);
    ColdWeapon chill("Chill Bow", 1500, 3, 20, 12);
}

void createThrowableItems() {
    // Creating throwable items with their respective attributes
    Throwable bomb("Firebomb", 2500, 2, 16, 15);
    Throwable grenade("Ice Grenade", 2200, 3, 12, 17);
    Throwable dart("Poison Dart", 1600, 1, 18, 20);
    Throwable rock("Rock", 1300, 3, 14, 23);
    Throwable knife("Throwing Knife", 1000, 2, 30, 10);
}

void createMedicines() {
    // Creating medicines with their respective attributes
    Medicine milkOfThePoppy("Milk of the Poppy", 1200, 20);
    Medicine essenceOfNightshade("Essence of Nightshade", 1100, 18);
    Medicine dreamwine("Dreamwine", 1000, 15);
    Medicine sourRed("Sour Red", 1300, 22);
    Medicine sweetsleep("Sweetsleep", 1500, 25);
    Medicine moonTea("Moon Tea", 1050, 15);
    Medicine weirwoodPaste("Weirwood Paste", 1250, 18);
    Medicine firemilk("Firemilk", 1400, 22);
    Medicine greyscaleCure("Greyscale Cure", 1800, 30);
    Medicine heartsbane("Heartsbane", 1700, 28);
}

void createFoods() {
    // Creating foods with their respective attributes
    Food roastPork("Roast Pork", 1200, 25);
    Food lemonCakes("Lemon Cakes", 1100, 23);
    Food honeyedChicken("Honeyed Chicken", 1000, 20);
    Food beefAndBarleyStew("Beef and Barley Stew", 1300, 26);
    Food blackBread("Black Bread", 1500, 28);
    Food fishPie("Fish Pie", 1050, 18);
    Food lemonWater("Lemon Water", 1250, 20);
    Food mulledWine("Mulled Wine", 1400, 23);
    Food venisonStew("Venison Stew", 1800, 30);
    Food honeycakes("Honeycakes", 1700, 28);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*