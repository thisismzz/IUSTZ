#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <cstdlib>
#include <cctype>

#include "header.h"

using namespace std;

// define colors  
string Black="\033[0;30m";      
string Red="\033[0;31m";          
string Green="\033[0;32m";        
string Yellow="\033[0;33m";       
string Blue="\033[0;34m";         
string Purple="\033[0;35m";       
string Cyan="\033[0;36m";         
string White="\033[0;37m";
string Reset = "\033[0m";        

//*****************************
Player* player = nullptr;
HumanEnemy* humanEnemy=nullptr;
BasicZombie* basicZombie=nullptr;
AdvZombie* advZombie=nullptr;
Person *enemy=nullptr;
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

Health::Health(int l=1) : maxHealth(l*100) , currentHealth(l*100) {} // Constructor

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
    cout<<"HEALTH INCREASED FOR "<< amount << " HP!\n";
    if(currentHealth >= maxHealth)
        currentHealth = maxHealth; // Ensures health doesn't exceed maxHealth
}

void Health::updateMaxHealth(){
    maxHealth += 50 * player->getLevel();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Stamina::Stamina(int cs,int l=1):maximum((l)*100),currentStamina(cs){} // Constructor

void Stamina::decreaseStamina(int amount) {
    currentStamina -= amount; // Decreases stamina
    cout<<"STAMINA DECREASED FOR "<< amount <<" STM!\n";
    if (currentStamina < 0)
        currentStamina = 0; // Ensures stamina doesn't go below 0
}

void Stamina::increaseStamina(int amount) {
    currentStamina += amount; // Increases stamina
    cout<<"STAMINA INCREASED FOR "<< amount <<" STM!\n";
    if ( currentStamina > maximum)
        currentStamina = maximum; // Ensures stamina doesn't exceed maximum
}

void Stamina::updateMaximumStamina(){
    maximum+=50 * player->getLevel(); // Increases maximum stamina
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
    maximum+= 50 * player-> getLevel(); // Increases maximum experience
}

void Experience::setCurrentExp(int selfDamage,int enemyDamage,int usedStamina){
    currentExp+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina); // Updates current experience
    if(currentExp>=maximum){
        currentExp=0; // Resets current experience if it exceeds maximum
        updateMaximum(); // Updates maximum experience
        humanObj->updateLevel(); // Updates player's level
        humanObj->stamina.updateMaximumStamina(); // Updates player's maximum stamina
        //humanObj->hp.updateMaxHealth();
        player->hp.updateMaxHealth();
    }
}

void Experience::increaseExp(int amount){
    currentExp+=amount; // Increases current experience
    cout<<"EXPERIENCE INCREASED FOR " << amount << " EXP!\n";
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
            if(currentSkill<maximum){
                creditcard->withdraw(upgradePrice); // Withdraws the upgrade price from the credit card
                currentSkill++; // Increases the current skill level
                cout << "WEAPON UPGRADE SUCCESSFULLY AND " << upgradePrice << "$ WAS DEDUCTED FROM YOUR ACCOUNT!";
                getch(); 
                setUpgradePrice(); // Sets the new upgrade price
            }
            else{
                cout<<"WEAPON'S LEVEL IS MAXIMUM!\n";
                getch();  // Wait for a key press
            }
            
        }
        else{
            throw 0; // Throws an exception if there's not enough money
        }
    }
    catch(int temp){
        cout<<"NOT ENOUGH MONEY!\n"; // Prints an error message if there's not enough money
    } 
}

void Skills::setUpgradePrice(){
    upgradePrice+=10; // Increases the upgrade price by 10
}

int Skills::getCurrentSkill(){
    return currentSkill; // Returns the current skill level
}

int Skills::getUpgradePrice(){
    return upgradePrice;
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
        cout<<"\nCOLD WEAPONS: \n";
        for(auto i:ColdWeaponItems){
            cout <<'\t' << "[" << index << "]. " << i.getName() << '\n'; // Prints cold weapon items
            index++;
        }
    }

    if(!WarmWeaponItems.empty()){
        index=1;
        cout<<"\nWARM WEAPONS: \n";
        for(auto i:WarmWeaponItems){
            cout<<'\t'<< "[" << index << "]. " <<i.getName()<<'\n'; // Prints warm weapon items
            index++;
        }
    }

    if(!ThrowableItems.empty()){
        index=1;
        cout<<"\nTHROWABLE WEAPONS: \n";
        for(auto i:ThrowableItems){
            Throwable item=i.first;
            cout<<'\t'<<"[" << index << "]. " <<item.getName()<<"(stock : "<<i.second<< ")" << '\n'; // Prints throwable items
            index++;
        }

        auto p=ThrowableItems.begin();
    }

    if(!MedicineItems.empty()){
        index=1;
        cout<<"\nMEDICINES: \n";
        for(auto i:MedicineItems){
            Medicine item=i.first;
            cout<<'\t' << "[" << index << "]. " <<item.getName()<<"(stock : "<<i.second<< ")" << '\n'; // Prints medicine items
            index++;
        }
    }

    if(!FoodItems.empty()){
        index=1;
        cout<<"\nFOODS: \n";
        for(auto i:FoodItems){
            Food item=i.first;
            cout<<'\t'<< "[" << index << "]. " <<item.getName()<<"(stock : "<<i.second<< ")" << '\n'; // Prints food items
            index++;
        }
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::showWarmWeaponItems(){
    int index = 1;
    for(auto i: WarmWeaponItems){
        cout  << "[" << index << "]. " << i.getName() << "(-" <<i.getwwa()->getCurrentSkill()*10<<"STM): " << "lvl : " << i.getwwa()->getCurrentSkill() <<" , harm : "<<i.getHarm()<< endl;
        index++;
    }
}

void Backpack::showColdWeaponItems(){
    int index = 1;
    for(auto i: ColdWeaponItems){
        cout  << "[" << index << "]. " << i.getName() << "(-" <<i.getcwa()->getCurrentSkill()*10<<"STM): " << "lvl : " << i.getcwa()->getCurrentSkill() <<" , harm : "<<i.getHarm() << endl;
        index++;
    }
}

void Backpack::showThrowableItems(){
    int index = 1;
    for(auto pair : ThrowableItems){
    Throwable item = pair.first;
    cout  << "[" << index << "]. " << item.getName() << "(-" <<item.gettwa()->getCurrentSkill()*10<<"STM): " << "lvl : " << item.gettwa()->getCurrentSkill() <<" , harm : "<<item.getHarm() << " , stock : " << pair.second << endl;
    index++;
    }
}

void Backpack::showMedicineItems(){
    int index = 1;
    for(auto pair : MedicineItems){
        Medicine item = pair.first;
        cout << "[" << index << "]. " << item.getName() << "(+" << item.getHeal() << "HP)(stock : " << pair.second << ")"  << '\n';
        index++;
    }
}

void Backpack::showFoodItems(){
    int index = 1;
    for(auto pair : FoodItems){
        Food item = pair.first;
        cout << "[" << index << "]. " << item.getName() << "(+" << item.getStrength() << "STM)(stock : " << pair.second << ")" << '\n';
        index++;
    }
}

void Backpack::showUpgradeWarmWeapon(){
    int index = 1;
    for(auto i: WarmWeaponItems){
        cout << "[" << index << "]. " << i.getName() << "(lvl " << i.getwwa()->getCurrentSkill()  << ")" <<"(money need for upgrading : "<<i.getwwa()->getUpgradePrice() << "$)" << endl;
        index++;
    }
}

void Backpack::showUpgradeColdWeapon(){
    int index = 1;
    for(auto i: ColdWeaponItems){
        cout << "[" << index << "]. " << i.getName() << "(lvl " << i.getcwa()->getCurrentSkill()  << ")" <<"(money need for upgrading : "<<i.getcwa()->getUpgradePrice() << "$)" << endl;
        index++;
    }
}

void Backpack::showUpgradeThrowable(){
    int index = 1;
    for(auto pair : ThrowableItems){
    Throwable item = pair.first;
    cout << "[" << index << "]. " << item.getName() << "(lvl " << item.gettwa()->getCurrentSkill()  << ")" <<"(money need for upgrading : "<<item.gettwa()->getUpgradePrice() << "$)" << endl;
    index++;
    }
}



// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack::ConsumeMedForSurvival() {
    
    if(MedicineItems.empty()){
        cout << "NO MEDICINE ITEM LEFT IN BACKPACK! YOUR ONLY CHANCE OF SURVIVAL IS TO GO TO SHOP AND BUY ANY MEDICINE THAT YOU CAN." << endl << endl;
        cout << Red << "Press any key to continue ... " << Reset;
        getch();
        medicineMenu();
    }

    cout << "HERE ARE THE AVAILABLE MEDICINE ITEMS IN THE BACKPACK:" << endl << endl;
    int index = 1;
    showMedicineItems();

    int choice;
    cout << "\nEnter the number of the medicine item you want to use: ";
    cin >> choice;

    if (choice >= 1 && choice <= MedicineItems.size()){
        auto iter = next(MedicineItems.begin(), choice - 1);
        Medicine* medicine = new Medicine(iter->first);
        medicine->use(*player);
        player->newLife();   //change the state from DEFEATED to ALIVE
        playground();
    }
    else{
        cout << "INVALID CHOICE! PLEASE ENTER A VALID NUMBER." << endl;
        getch();
        ConsumeMedForSurvival();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
void Backpack::ConsumeFoodForSurvival() {
    
    system("cls");
    
    if(FoodItems.empty()){
        cout << "NO FOOD ITEM LEFT IN BACKPACK! YOUR ONLY CHANCE OF SURVIVAL IS TO GO TO SHOP AND BUY ANY FOOD THAT YOU CAN." << endl << endl;
        cout <<  Red << "Press any key to continue ... " << Reset;
        getch();
        foodMenu();
    }

    cout << "HERE ARE THE AVAILABLE FOOD ITEMS IN THE BACKPACK:" << endl << endl;
    int index = 1;
    showFoodItems();  

    int choice;
    cout << "\nEnter the number of the food item you want to use: ";
    cin >> choice;

    if (choice >= 1 && choice <= FoodItems.size()){
        auto iter = next(FoodItems.begin(), choice - 1);
        Food* food = new Food(iter->first);
        food->use(*player);
        player->newLife();   //change the state from DEFEATED to ALIVE
        BattleMenu();
    }
    else{
        cout << "INVALID CHOICE! PLEASE ENTER A VALID NUMBER." << endl;
        getch();
        ConsumeFoodForSurvival();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Items* Backpack::useWeapons() {
    int number;
    int index;
    int ChosenWeapon;
    system("cls"); 
    cout << Purple << "ENTER THE INDEX OF THE WEAPON YOU WANT TO ATTACK WITH:" << Reset << endl << endl
    << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl << "[3].THROWABLE WEAPONS" << endl << "[0].BACK" << endl;
    cin >> number;
    switch(number){
        case 1: 
            if(!WarmWeaponItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE WARMWEAPON YOU WANT TO ATTACK WITH:" << Reset<< endl << endl;
                    index = 1;
                    showWarmWeaponItems(); // Prints warm weapon items
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= WarmWeaponItems.size()) {
                        WarmWeapon *wweapon=new WarmWeapon(WarmWeaponItems.at(ChosenWeapon-1));
                        return wweapon;
                    }
                    else if(ChosenWeapon == 0){
                        useWeapons();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red << "\nPress any key to continue..." << Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO WARMWEAPON EXISTS!" << endl;
                cout << Red << "\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                useWeapons();
            }
            break;

        case 2:
            if(!ColdWeaponItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE COLDWEAPON YOU WANT TO ATTACK WITH:" << Reset<< endl << endl;
                    index = 1;
                    showColdWeaponItems();      // Prints cold weapon items
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= ColdWeaponItems.size()) {
                        ColdWeapon* cweapon=new ColdWeapon(ColdWeaponItems.at(ChosenWeapon-1));
                        //cout << *cweapon;
                        return cweapon;
                    }
                    else if(ChosenWeapon == 0){
                        useWeapons();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO COLDWEAPON EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                useWeapons();
            }
            break;

        case 3:
            if(!ThrowableItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE THROWABLE WEAPON YOU WANT TO ATTACK WITH:" << Reset<< endl << endl;
                    index = 1;
                    showThrowableItems();      // Prints throwable items
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= ThrowableItems.size()) {
                        auto iter = next(ThrowableItems.begin(), ChosenWeapon - 1);
                        Throwable* tweapon = new Throwable(iter->first);
                        return tweapon;
                    }
                    else if(ChosenWeapon == 0){
                        useWeapons();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO THROWABLE WEAPON EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                useWeapons();
            }
            break;

        case 0:
            BattleMenu();
            break;

        default:
            cout << "INVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 4." << endl; 
            cout << Red <<"\nPress any key to continue..."<< Reset;
            getch(); // Wait for a key press
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
    cout <<Purple << "ENTER THE INDEX OF THE CONSUMABLE ITEM YOU WANT TO USE:" << Reset<< endl << endl
    << "[1].MEDICINE" << endl << "[2].FOOD" << endl << "[0].BACK" << endl;
    cin >> number;
    switch(number){
        case 1: 
            if(!MedicineItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE MEDICINE YOU WANT TO USE:" << Reset<< endl << endl;
                    index = 1;
                    showMedicineItems();
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenConsumableItem;
                    if (ChosenConsumableItem >= 1 && ChosenConsumableItem <= MedicineItems.size()) {
                        auto iter = next(MedicineItems.begin(), ChosenConsumableItem - 1);
                        Medicine* medicine = new Medicine(iter->first);
                        return medicine;
                    }
                    else if(ChosenConsumableItem == 0){
                        useConsumableItems();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO MEDICINE EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                useConsumableItems();
            }
            break;

        case 2:
            if(!FoodItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE FOOD YOU WANT TO USE:" << Reset<< endl << endl;
                    index = 1;
                    showFoodItems();
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenConsumableItem;
                    if (ChosenConsumableItem >= 1 && ChosenConsumableItem <= FoodItems.size()) {
                        auto iter = next(FoodItems.begin(), ChosenConsumableItem - 1);
                        Food* food = new Food(iter->first);
                        return food;
                    }
                    else if(ChosenConsumableItem == 0){
                        useConsumableItems();
                    }
                    else{
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO FOOD EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                useConsumableItems();
            }
            break;

        case 0:
            BattleMenu();
            break;

        default:
            cout <<Purple << "INVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 3." << endl;
            cout << Red <<"\nPress any key to continue..."<< Reset;
            getch(); // Wait for a key press
            useConsumableItems();
    }
    return nullptr;
}

Items* Backpack::upgradeWeapons() {
    int number;
    int index;
    int ChosenWeapon;
    system("cls");
    cout << Purple << "ENTER THE INDEX OF THE WEAPON YOU WANT TO UPGRADE IT:" << Reset << endl << endl
    << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl << "[3].THROWABLE WEAPONS" << endl << "[0].BACK" << endl;
    cin >> number;
    switch(number){
        case 1: 
            if(!WarmWeaponItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE WARMWEAPON YOU WANT TO UPGRADE IT:(YOUR MONEY : " << player->getMoney() << "$)" << Reset<< endl << endl;
                    index = 1;
                    showUpgradeWarmWeapon(); // Prints warm weapon items
                    cout <<"[0]. Back" << endl; 
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= WarmWeaponItems.size()) {
                        WarmWeapon *wweapon=new WarmWeapon(WarmWeaponItems.at(ChosenWeapon-1));
                        return wweapon;
                    } 
                    else if(ChosenWeapon == 0){
                        upgradeWeapons();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO WARMWEAPON EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                upgradeWeapons();
            }
            break;

        case 2:
            if(!ColdWeaponItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE COLDWEAPON YOU WANT TO UPGRADE IT:(YOUR MONEY : " << player->getMoney() << "$)" << Reset<< endl << endl;
                    index = 1;
                    showUpgradeColdWeapon();      // Prints cold weapon items
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= ColdWeaponItems.size()) {
                        ColdWeapon* cweapon=new ColdWeapon(ColdWeaponItems.at(ChosenWeapon-1));
                        return cweapon;
                    }
                    else if(ChosenWeapon == 0){
                        upgradeWeapons();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO COLDWEAPON EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                upgradeWeapons();
            }
            break;

        case 3:
            if(!ThrowableItems.empty()){
                while(true){
                    system("cls");
                    cout <<Purple << "CHOOSE THE THROWABLEWEAPON YOU WANT TO UPGRADE IT:(YOUR MONEY : " << player->getMoney() << "$)" << Reset<< endl << endl;
                    index = 1;
                    showUpgradeThrowable();      // Prints throwable items
                    cout << "[0]. Back" << endl; 
                    cin >> ChosenWeapon;
                    if (ChosenWeapon >= 1 && ChosenWeapon <= ThrowableItems.size()) {
                        auto iter = next(ThrowableItems.begin(), ChosenWeapon - 1);
                        Throwable* tweapon = new Throwable(iter->first);
                        //cout << *tweapon;
                        return tweapon;
                    }
                    else if(ChosenWeapon == 0){
                        upgradeWeapons();
                    }
                    else {
                        cout << "INVALID INDEX! TRY AGAIN." << endl;
                        cout << Red <<"\nPress any key to continue..."<< Reset;
                        getch();  // Wait for a key press
                    }
                }
            }
            else{
                cout << "NO THROWABLE WEAPON EXISTS!" << endl;
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                upgradeWeapons();
            }
            break;

        case 0:
            BattleMenu();
            break;

        default:
            cout << "INVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 4." << endl;
            cout << Red <<"\nPress any key to continue..."<< Reset;
            getch(); // Wait for a key press
            upgradeWeapons();
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
    balance += amount; // Increases the balance by the deposit amount
}

bool BankAccount::withdraw(int amount) {
    if (balance >= amount){
        balance -= amount; // Decreases the balance by the withdrawal amount if there's enough balance
        return true; // Returns true if withdrawal was successful
    }    
    else
        return false; // Returns false if there's not enough balance for withdrawal
}

void BankAccount::prize(int amount){
    balance+=amount;
    cout<<"YOU TOOK A "<<amount<<"$ PRIZE!"<<endl;
}


// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Person::Person(string n,int l=1):name(n),level(l),hp(l){} // Constructor that initializes name and level

void Person::updateLevel() {
    level++; // Increases level by 1
    cout<<"[!] NEW LEVEL!\t(YOUR LEVEL : "<<level << ")" <<endl;
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

Human::Human(string n,int s,int l=1):Person(n,l),stamina(s,l){} // Constructor that initializes name and stamina

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
        cout << "YOU HAVE BEEN DEFEATED!" << endl; // Prints a message if the player has been defeated
        state=PlayerState::DEFEATED;
    } 

    else 
        cout << "YOU TOOK " << amount << " DAMAGE!" << endl; // Prints a message if the player takes damage
    
}

void Player::newLife(){
    state=PlayerState::ALIVE;
}

PlayerState Player::getState(){
    return state;
}

void Player::setState(PlayerState s){
    state=s;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

HumanEnemy::HumanEnemy(Human& human,int l=1): Human(human.getName(), human.getStamina()*l,l),state(HumanEnemyState::FIGHT),status(HumanEnemyStatus::ALIVE){} // Constructor that initializes name and stamina from a Human object

void HumanEnemy::setState(HumanEnemyState newState){
    state = newState;
}

HumanEnemyState HumanEnemy::getState(){
    return state;
}

HumanEnemyStatus HumanEnemy::getStatus(){
    return status;
}

void HumanEnemy::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout << name << " HAS BEEN DEFEATED!" << endl; // Prints a message if the human enemy has been defeated
        status = HumanEnemyStatus :: DEFEATED;
    } 
    else{ 
        cout << name << " TAKES " << amount << " DAMAGE!" << endl; // Prints a message if the human enemy takes damage
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

HE_Controller :: HE_Controller (HumanEnemy *HE) : model(HE) , backpack(HE->getBackpack()) , view(HE_View()) {}

void HE_Controller::updateState() {
    double healthRatio = model->getHealthPoints() * 1.0 / model->hp.getMaxHealth();
    double staminaRatio = model->stamina.getCurrentStamina() * 1.0 / model->stamina.getMaxStamina();

    // Check if health or stamina is below 0.4
    if (healthRatio <= 0.4)
        model->setState(HumanEnemyState :: LOW_HEALTH);

    else if (staminaRatio <= 0.4)
        model->setState(HumanEnemyState :: LOW_POWER);

    else
        model->setState(HumanEnemyState :: FIGHT);
}

HumanEnemyState HE_Controller::getState(){
    return model->getState();
}

HumanEnemyStatus HE_Controller::getStatus(){
    return model->getStatus();
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

void HE_Controller::Attack(Items* weapon) {
    WarmWeapon* wweapon;
    ColdWeapon* cweapon;
    Throwable* tweapon;
    if (dynamic_cast<WarmWeapon*>(weapon)) {
        // Successfully cast to WarmWeapon
        wweapon=dynamic_cast<WarmWeapon*>(weapon);
        wweapon->Attack(*model, *player);
        view.attackView(model->getName(), *wweapon);
    } else if (dynamic_cast<ColdWeapon*>(weapon)) {
        // Successfully cast to ColdWeapon
        cweapon = dynamic_cast<ColdWeapon*>(weapon);
        cweapon->Attack(*model, *player);
        view.attackView(model->getName(), *cweapon);
    } else if (dynamic_cast<Throwable*>(weapon)) {
        // Successfully cast to Throwable
        tweapon = dynamic_cast<Throwable*>(weapon);
        tweapon->Throw(*model, *player);
        view.attackView(model->getName(), *tweapon);
    }
}

void HE_Controller :: decision() {
    Medicine *mitem;
    Food *fitem;

    switch(model->getState()){
    case HumanEnemyState::LOW_HEALTH:
        mitem = chooseMedicine();
        if(mitem!=nullptr){
            mitem->use(*model);
            view.updateHealth(model->getName(),mitem->getHeal());
            break;
        }
        else{
            
        }

    case HumanEnemyState::LOW_POWER:
        fitem = chooseFood();
        if(fitem!=nullptr){
            fitem->use(*model);
            view.updateStamina(model->getName(),fitem->getStrength());
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
    Backpack *enemyBackpack = model->getBackpack();

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
    view.showInfo(*model);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

HE_View::HE_View(){}

void HE_View::showInfo(HumanEnemy& model) {
    cout << "HUMAN ENEMY'S INFO:" << endl << endl;
    cout << "   " << "Name: " << model.getName() << endl;
    cout << "   " << "Level: " << model.getLevel() << endl;
    cout << "   " << "Stamina: " << model.getStamina() << " / (" << model.getMaxStamina() << ")" << endl;
    cout << "   " << "Health: " << model.getHealthPoints() << " / (" << model.getMaxHealth() << ")" << endl;
}

void HE_View::updateHealth(string name,int amount) {
    cout << name << "'s health increased for "<< amount << " HP!\n";
}

void HE_View::updateStamina(string name,int amount) {
    cout << name << "'s stamina increased for "<< amount << " STM!\n";
}

void HE_View::attackView(string name,Items weapon) {
    cout << name << " is attacking you by "<< weapon.getName() << "\n";
}

void HE_View :: showBackpackItems() {
    Backpack *HE_Backpack = humanEnemy->getBackpack();
    HE_Backpack->showItems();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Zombie::Zombie(string n,int l=1) : Person(n,l) , state(ZombieState::ALIVE){} // Constructor that initializes name from a Person object

void Zombie::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout << name << " HAS BEEN DEFEATED!" << endl; // Prints a message if the zombie has been defeated
        state=ZombieState::DEFEATED;
    } 
    else {
        cout << name << " TAKES " << amount << " DAMAGE!" << endl; // Prints a message if the zombie takes damage
    }
}

ZombieState Zombie::getState(){
    return state;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BasicZombie::BasicZombie(Zombie& zombie) : Zombie(zombie.getName(),zombie.getLevel()){}
BasicZombie::BasicZombie(string n,int l) : Zombie(n,l){}

void BasicZombie :: bite() {
    player->hp.decreaseHealth(this->getLevel()*10);
    cout<<"YOU HAVE BEEN BITTEN BY "<<this->getName() <<"." << endl;
    player->takeDamage(this->getLevel()*5);
}

// *----------------------------------------------------------------*

BZ_Controller :: BZ_Controller (BasicZombie* BZ) : model(BZ) , view(BZ_View())  {}

ZombieState BZ_Controller::getState(){
    return model->getState();
}

void BZ_Controller :: bite() {
    model->bite();
}
 
void BZ_Controller :: showInfo() {
    view.showInfo(*model);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BZ_View::BZ_View(){}

void BZ_View :: showInfo(BasicZombie& model) {
    cout << "BASIC ZOMBIE'S INFO:" << endl << endl;
    cout  << "   " << "Name : " << model.getName() << endl;
    cout  << "   " << "Level : " << model.getLevel() << endl;
    cout  << "   " << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AdvZombie::AdvZombie(Zombie & zombie) : Zombie(zombie.getName(),zombie.getLevel()){}
AdvZombie::AdvZombie(string n,int l) : Zombie(n,l){}

void AdvZombie :: bite() {
    player->hp.decreaseHealth(this->getLevel()*15);
    cout<<"YOU HAVE BEEN BITTEN BY "<<this->getName()<<"." <<endl;
    player->takeDamage(this->getLevel()*7);
}

void AdvZombie :: scratch() {
    player->hp.decreaseHealth(this->getLevel()*18);
    cout<<"YOU HAVE BEEN SCRATCHED BY "<<this->getName() <<"." <<endl;
    player->takeDamage(this->getLevel()*8);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AZ_Controller :: AZ_Controller (AdvZombie* AZ) : model(AZ) , view(AZ_View())  {}

ZombieState AZ_Controller::getState(){
    return model->getState();
}
 
void AZ_Controller :: showInfo() {
    view.showInfo(*model);
}

void AZ_Controller::Attack(){
    srand(time(0));
    int randomNum = rand();

    if(randomNum % 2 == 0){
        model->bite();
    }

    else{
        model->scratch();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AZ_View::AZ_View(){}

void AZ_View :: showInfo(AdvZombie& model) {
    cout << "ADVANCED ZOMBIE'S INFO:" << endl << endl;
    cout  << "   " << "Name : " << model.getName() << endl;
    cout  << "   " << "Level : " << model.getLevel() << endl;
    cout  << "   " << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
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

int Permanent::getHarm(){
    return harm;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

WarmWeapon::WarmWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Warm Weapon",e,h),wwa(new WarmWeaponAbility(x)){
    WarmWeapon::addToVectors(); // Adds this warm weapon to vectors
}

void WarmWeapon::showItems(){
    int n=1;
    cout << setw(32) << left << "             NAME" << setw(20) << "   +EXP"  << setw(20) << "  LEVEL" << setw(20) << "   HARM" << "    PRICE" << endl;
    for(WarmWeapon i : shop_items_permanent_warmweapon){  // Prints each warm weapon item
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

void WarmWeapon::buy(){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(backpack->warmWeaponExistence(*this)){
        cout<<"THIS ITEM ALREADY EXISTS IN YOUR BACKPACK!\n"; // Prints a message if the item already exists in the backpack
    }

    else if(creditcard->withdraw(price)){
        cout << "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"; // Prints a message if the item was bought successfully
        backpack->addWarmWeaponItem(*this); // Adds the item to the backpack
        player->exp.increaseExp(exp); // Increases the player's experience
    }
    else
        cout<<"NOT ENOUGH MONEY!\n"; // Prints a message if there's not enough money to buy the item
}

void WarmWeapon::addToVectors(){
    shop_items_permanent_warmweapon.push_back(*this); // Adds this warm weapon to the permanent warm weapon vector
    shop_items_permanent.push_back(*this); // Adds this warm weapon to the permanent items vector
    shop_items.push_back(*this); // Adds this warm weapon to the items vector
}

void WarmWeapon::Attack(Human& attacker, Person& attacked) {
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) +
                 (wwa->getCurrentSkill() * weaponLevelFactor) +
                 (attacker.getLevel() * playerLevelFactor);

    attacker.stamina.decreaseStamina(10 * wwa->getCurrentSkill()); // Decrease attacker's stamina
    attacked.hp.decreaseHealth(damage); // Decrease attacked person's health

    Player* pAttacker = dynamic_cast<Player*>(&attacker);
    if (pAttacker) {
        // Attacker is a Player
        pAttacker->exp.setCurrentExp(0, damage, 10 * wwa->getCurrentSkill()); // Update attacker's experience

        if (HumanEnemy* he = dynamic_cast<HumanEnemy*>(&attacked)) {
            // Attacked is a HumanEnemy
            he->takeDamage(damage); // HumanEnemy takes damage
        } else if (Zombie* z = dynamic_cast<Zombie*>(&attacked)) {
            // Attacked is a Zombie
            z->takeDamage(damage); // Zombie takes damage
        }
    } else {
        // Attacked is a Player
        Player* pAttacked = dynamic_cast<Player*>(&attacked);
        if (pAttacked) {
            pAttacked->takeDamage(damage);
            pAttacked->exp.setCurrentExp(damage, 0, 0); // Update attacked player's experience
            
        }
    }
}


ostream& operator<<(ostream &os,WarmWeapon &obj){
    os << setw(30) << left << obj.name << setw(20) << obj.exp  << setw(20) << obj.wwa->getCurrentSkill() << setw(20) << obj.harm << obj.price << "$";
    return os; // Prints the warm weapon's details
}

WarmWeaponAbility* WarmWeapon::getwwa(){
    return wwa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ColdWeapon::ColdWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Cold Weapon",e,h),cwa(new ColdWeaponAbility(x)){
    ColdWeapon::addToVectors(); // Adds this cold weapon to vectors
}

void ColdWeapon::showItems(){
    int n=1;
    cout << setw(32) << left << "          NAME" << setw(20) << "   +EXP"  << setw(20) << "  LEVEL" << setw(20) << "   HARM" << "    PRICE" << endl;
    for (ColdWeapon i : shop_items_permanent_coldweapon){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

void ColdWeapon::buy(){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(backpack->coldWeaponExistence(*this)){
        cout<<"THIS ITEM ALREADY EXISTS IN YOUR BACKPACK!\n"; // Prints a message if the item already exists in the backpack
    }

    else if(creditcard->withdraw(price)){
        cout << "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"; // Prints a message if the item was bought successfully
        backpack->addColdWeaponItem(*this); // Adds the item to the backpack
        player->exp.increaseExp(exp); // Increases the player's experience
    }
    else
        cout<<"NOT ENOUGH MONEY!\n"; // Prints a message if there's not enough money to buy the item
}

void ColdWeapon::addToVectors(){
    shop_items_permanent_coldweapon.push_back(*this); // Adds this cold weapon to the permanent cold weapon vector
    shop_items_permanent.push_back(*this); // Adds this cold weapon to the permanent items vector
    shop_items.push_back(*this); // Adds this cold weapon to the items vector
}

void ColdWeapon::Attack(Human& attacker, Person& attacked) {
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) +
                 (cwa->getCurrentSkill() * weaponLevelFactor) +
                 (attacker.getLevel() * playerLevelFactor);

    attacker.stamina.decreaseStamina(10 * cwa->getCurrentSkill()); // Decrease attacker's stamina
    attacked.hp.decreaseHealth(damage); // Decrease attacked person's health

    Player* pAttacker = dynamic_cast<Player*>(&attacker);
    if (pAttacker) {
        // Attacker is a Player
        pAttacker->exp.setCurrentExp(0, damage, 10 * cwa->getCurrentSkill()); // Update attacker's experience

        if (HumanEnemy* he = dynamic_cast<HumanEnemy*>(&attacked)) {
            // Attacked is a HumanEnemy
            he->takeDamage(damage); // HumanEnemy takes damage
        } else if (Zombie* z = dynamic_cast<Zombie*>(&attacked)) {
            // Attacked is a Zombie
            z->takeDamage(damage); // Zombie takes damage
        }
    } else {
        // Attacked is a Player
        Player* pAttacked = dynamic_cast<Player*>(&attacked);
        if (pAttacked) {
            pAttacked->exp.setCurrentExp(damage, 0, 0); // Update attacked player's experience
            pAttacked->takeDamage(damage);
        } else {
            // Handle unsupported attacker type (optional)
            // You can add custom logic here, e.g., logging an error message.
            // For now, I'll leave this part empty.
        }
    }
}


ostream& operator<<(ostream& os,ColdWeapon& obj){
    os << setw(30) << left << obj.name << setw(20) << obj.exp  << setw(20) << obj.cwa->getCurrentSkill() << setw(20) << obj.harm << obj.price << "$";
    return os; // Prints the cold weapon's details
}

ColdWeaponAbility* ColdWeapon::getcwa(){
    return cwa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Throwable::Throwable(string n,int p,int x,int e,int h):Items(n,p,"Throwable Weapon"),twa(new ThrowableWeaponAbility(x)),exp(e),harm(h){
    Throwable::addToVectors(); // Adds this throwable weapon to vectors
}

void Throwable::showItems(){
    int n=1;
    cout << setw(32) << left << "          NAME" << setw(20) << "   +EXP"  << setw(20) << "  LEVEL" << setw(20) << "   HARM" << "  PRICE(each)" << endl;
    for (Throwable i : shop_items_throwable){
        cout << "[0" << n <<"]. " << i << endl; // Prints each throwable weapon item
        n++;
    }
}

void Throwable::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout << "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"; // Prints a message if the item was bought successfully
        backpack->addThrowableItem(*this,quantity); // Adds the item to the backpack
        player->exp.increaseExp(exp*quantity); // Increases the player's experience
    }
    else
        cout<<"NOT ENOUGH MONEY!\n"; // Prints a message if there's not enough money to buy the item
}

void Throwable::addToVectors(){
    shop_items_throwable.push_back(*this); // Adds this throwable weapon to the throwable weapon vector
    shop_items.push_back(*this); // Adds this throwable weapon to the items vector
}

void Throwable::Throw(Human& attacker, Person& attacked) {
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) +
                 (twa->getCurrentSkill() * weaponLevelFactor) +
                 (attacker.getLevel() * playerLevelFactor);

    attacker.stamina.decreaseStamina(10 * twa->getCurrentSkill()); // Decrease attacker's stamina
    attacked.hp.decreaseHealth(damage); // Decrease attacked person's health

    Player* pAttacker = dynamic_cast<Player*>(&attacker);
    if (pAttacker) {
        // Attacker is a Player
        pAttacker->exp.setCurrentExp(0, damage, 10 * twa->getCurrentSkill()); // Update attacker's experience
        Backpack* b = pAttacker->getBackpack();
        b->useThrowableItemCount(*this); // Use a throwable item from the attacker's backpack

        if (HumanEnemy* he = dynamic_cast<HumanEnemy*>(&attacked)) {
            // Attacked is a HumanEnemy
            he->takeDamage(damage); // HumanEnemy takes damage
        } else if (Zombie* z = dynamic_cast<Zombie*>(&attacked)) {
            // Attacked is a Zombie
            z->takeDamage(damage); // Zombie takes damage
        }
    } else {
        // Attacked is a Player
        Player* pAttacked = dynamic_cast<Player*>(&attacked);
        if (pAttacked) {
            pAttacked->exp.setCurrentExp(damage, 0, 0); // Update attacked player's experience
            Backpack* b = attacker.getBackpack();
            b->useThrowableItemCount(*this); // Use a throwable item from the attacked person's backpack
            pAttacked->takeDamage(damage);
        }
    }
}

ThrowableWeaponAbility* Throwable::gettwa(){
    return twa;
}

int Throwable::getHarm(){
    return harm;
}

ostream& operator<<(ostream& os,Throwable& obj) {
    os << setw(30) << left << obj.name << setw(20) << obj.exp  << setw(20) << obj.twa->getCurrentSkill() << setw(20) << obj.harm << obj.price << "$";
    return os; // Prints the throwable weapon's details
}



// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Medicine::Medicine(string n,int p,int h):Items(n,p,"Medicine"),heal(h){
    Medicine::addToVectors(); // Adds this medicine to vectors
}

void Medicine::showItems(){
    int n=1;
    cout << setw(33) << left << "              NAME" << setw(20) << "  +HP" << "PRICE(each)" << endl;
    for (Medicine i : shop_items_medicine){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

void Medicine::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout << "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"; // Prints a message if the item was bought successfully
        backpack->addMedicineItem(*this,quantity); // Adds the item to the backpack
    }
    else
        cout<<"NOT ENOUGH MONEY!\n"; // Prints a message if there's not enough money to buy the item
}

void Medicine::addToVectors(){
    shop_items_medicine.push_back(*this); // Adds this medicine to the medicine vector
    shop_items.push_back(*this); // Adds this medicine to the items vector
}

void Medicine::use(Human &human){
    if(human.hp.getCurrentHealth() < human.hp.getMaxHealth()){
        cout << "MEDICINE CONSUME SUCCESSFULLY." << endl;
        human.hp.increaseHealth(heal); // Increases the human's health
        Backpack *b=human.getBackpack();
        b->useMedicineItemCount(*this); // Uses a medicine item from the human's backpack
        getch();
    }
    else{
        cout << "YOUR HP IS FULL. NO NEED TO MEDICINE."<< endl;
        getch();
    }
}

int Medicine::getHeal() {
    return heal;
}

ostream& operator<<(ostream& os,Medicine& obj) {
    os << setw(30) << left << obj.name << setw(20) << obj.heal << obj.price << "$";
    return os; // Prints the medicine's details
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Food::Food(string n,int p,int s):Items(n,p,"Food"),strength(s){
    Food::addToVectors(); // Adds this food to vectors
}

void Food::showItems(){
    int n=1;
    cout << setw(33) << left << "              NAME" << setw(20) << "  +STM" << "PRICE(each)" << endl;
    for (Food i : shop_items_food){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

void Food::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout << "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"; // Prints a message if the item was bought successfully
        backpack->addFoodItem(*this,quantity); // Adds the item to the backpack
    }
    else
        cout<<"NOT ENOUGH MONEY!\n"; // Prints a message if there's not enough money to buy the item
}

void Food::addToVectors(){
    shop_items_food.push_back(*this); // Adds this food to the food vector
    shop_items.push_back(*this); // Adds this food to the items vector
}

void Food::use(Human& human){
    if(human.stamina.getCurrentStamina() < human.stamina.getMaxStamina()){
        cout << "FOOD CONSUME SUCCESSFULLY." << endl;
        human.stamina.increaseStamina(strength); // Increases the human's stamina
        Backpack *b=human.getBackpack();
        b->useFoodItemCount(*this); // Uses a food item from the human's backpack
        getch();
    }
    else{
        cout << "YOUR STAMINA IS FULL. NO NEED TO FOOD." << endl;
        getch();
    }
}

int Food::getStrength (){
    return strength;
}

ostream& operator<<(ostream& os,Food& obj) {
    os << setw(30) << left << obj.name << setw(20) << obj.strength << obj.price << "$";
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
    return new Human(type, /*stamina*/ 100); // Creates a Daenerys character with 110 stamina
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
    cout << Blue;
    for (char c : text) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << Reset;
}

void getUserInfo(int& age , string& gender , string& username) {
    string input;
    cout << "Hello kid! i'm your guide. " << "First of all, I need to know you:" << endl;
    cout << endl << "What they call you? (enter your name)" << endl;
    cin >> username;
    cout << endl << "And you are older than 15 right? If not fuck off!!! (enter your age)" << endl;
    cin >> age;
    if (age < 15){
        cout << endl << "Sorry kid, we don't have time to deal with your parent's complaint!" << endl << endl;
        endGame();
        exit(0);
    }
    cout << endl << "And the last question!" << endl << "What do you have down there?! this is the only thing matters. (enter your gender)"<<endl;
    cin >> gender;
    cout << endl << "Ok, you can go now. I hope you are still alive next time I see you, kid!" ;

    cout << "\n\nPlease press any key to continue...";

    getch();

    system("cls");
}

void showPlayerInfo() {
    cout << Purple << "PLAYER'S INFO:" << Reset << endl << endl;
    cout << "   Name : " << player->getName() << endl;
    cout << "   Level : " << player->getLevel() << endl;
    cout << "   Experience : " << player->getExperience() << " / ( " << player->getMaxExperience() << " )" << endl;
    cout << "   Stamina : " << player->getStamina() <<  " / ( " << player->getMaxStamina() << " )" << endl;
    cout << "   Health : " << player->getHealthPoints() << " / ( " << player->getMaxHealth() << " )" << endl;
    cout << "   Money : " << player->getMoney() << " $" << endl;
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

    if(player->getMoney() >= 1000) {
        int item,quantity;
        Medicine *drug;
        cout << "YOU GO TO TAKE A LOOK AT THE MEDICINES:" << "(YOUR MONEY : " << player->getMoney() << ")" << endl << endl;

        Medicine::showItems(); // Shows medicines
        cout << "\nWhich one do you want to buy?" << endl;
        cin >> item;
        while(item<0 && item>10){
            cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ." << endl << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        cout << "How many?" << endl;
        cin >> quantity;
        while(quantity <= 0){
            cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ." << endl << "How many?" << endl;
            cin >> quantity;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        drug=new Medicine(Medicine::shop_items_medicine.at(item-1));
        if(player->getMoney() >= drug->getPrice() * quantity) {
            drug->buy(quantity); // Buys a medicine
        }
        else {
            cout << "Not Enough Money To Purchase " << quantity << " Amounts Of " << drug->getName() << " Items. Buy Less Items ... \n";
            getch(); // Wait for a key press
            medicineMenu();
        }
    } 
    else {
        //the Player Looses.
        goodbye();
    }
}

void foodMenu() {
    system("cls");

    if(player->getMoney() >= 1000) {
        int item,quantity;
        Food *meal;
        cout << "YOU GO TO TAKE A LOOK AT THE FOODS:" << "(YOUR MONEY : " << player->getMoney() << ")" << endl << endl;

        Food::showItems(); // Shows foods
        cout << "\nWhich one do you want to buy?" << endl;
        cin >> item;
        while(item<0 && item>10){
            cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ." << endl << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        cout << "How many?" << endl;
        cin >> quantity;
        while(quantity <= 0){
            cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ." << endl << "How many?" << endl;
            cin >> quantity;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        meal=new Food(Food::shop_items_food.at(item-1));
        if(player->getMoney() >= meal->getPrice() * quantity) {
            meal->buy(quantity); // Buys a food
        }
        else {
            cout << "Not Enough Money To Purchase " << quantity << " Amounts Of " << meal->getName() << " Items. Buy Less Items ... \n";
            getch(); // Wait for a key press
            foodMenu();
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

    if (player->getState() == PlayerState::DEFEATED) {
        cout<<"YOU HAVE BEEN DEFEATED BY YOUR ENEMY , YOUR HP IS 0! " << endl << "TO CONTINUE THE GAME YOU NEED TO INCREASE YOUR HP..."<< endl; 
        cout<<"GO TO YOUR BACKPACK AND CONSUME MEDICINE..." << endl;
        getch();
        playerBackpack->ConsumeMedForSurvival();
    }

    int choice;
    if ((randomNum % 100) < 95) {
    //fight ground
        randomNum=rand();

        if ((randomNum % 100) < 50) {
        //fight with human enemy

            //create random humanEnemy from characters
            int index = rand() % characterTypes.size();
            Human *character=Factory::createCharacter(characterTypes[index]);
            humanEnemy = new HumanEnemy(*character,player->getLevel());

            // Add items to the humanEnemy's backpack
            enemyBackpack = humanEnemy->getBackpack();
            enemyBackpack->addWarmWeaponItem(WarmWeapon::shop_items_permanent_warmweapon.at(rand() % WarmWeapon::shop_items_permanent_warmweapon.size()));
            enemyBackpack->addColdWeaponItem(ColdWeapon::shop_items_permanent_coldweapon.at(rand() % ColdWeapon::shop_items_permanent_coldweapon.size()));
            for (int j = 0; j < player->getLevel()*3; j++) {
                enemyBackpack->addThrowableItem(Throwable::shop_items_throwable.at(rand() % Throwable::shop_items_throwable.size()), 1);
                enemyBackpack->addFoodItem(Food::shop_items_food.at(rand() % Food::shop_items_food.size()), 1);
                enemyBackpack->addMedicineItem(Medicine::shop_items_medicine.at(rand() % Medicine::shop_items_medicine.size()), 1);
            }

            //show enemy's info
            cout << Purple << "THE HUMAN ENEMY YOU ARE FACING IS : " << Reset << endl << endl;
            cout << "   " << "Name : " << humanEnemy->getName() << endl;
            cout << "   " << "Level : " << humanEnemy->getLevel() << endl;
            cout << "   " << "Stamina : " << humanEnemy->getStamina() <<  " / ( " << humanEnemy->getMaxStamina() << " )" << endl;
            cout << "   " << "Health : " << humanEnemy->getHealthPoints() << " / ( " << humanEnemy->getMaxHealth() << " )" << endl << endl;
            cout << Red << "Press any key to Enter to fightground..." << Reset;
            getch(); // Wait for a key press

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
                cout << Purple << "THE BASIC ZOMBIE YOU ARE FACING IS : " << Reset<< endl << endl;
                cout << "   " << "Name : " << basicZombie->getName() << endl;
                cout << "   " << "Level : " << basicZombie->getLevel() << endl;
                cout << "   " << "Health : " << basicZombie->getHealthPoints() << " / ( " << basicZombie->getMaxHealth() << " )" << endl << endl;
                cout << Red <<"Press any key to Enter to fightground..." << Reset;
                getch();  // Wait for a key press

                system("cls");

                battleGround_basicZombie();
            }

            else{
            //fight with advance zombie

                //create advance zombie
                advZombie = Factory::createAdvanceZombie(player->getLevel());

                //show enemy's info
                cout << Purple <<"THE ADVANCED ZOMBIE YOU ARE FACING IS : " << Reset<< endl << endl;
                cout << "   " << "Name : " << advZombie->getName() << endl;
                cout << "   " << "Level : " << advZombie->getLevel() << endl;
                cout << "   " << "Health : " << advZombie->getHealthPoints() << " / ( " << advZombie->getMaxHealth() << " )" << endl << endl;
                cout << Red <<"Press any key to Enter to fightground..." << Reset;
                getch();  // Wait for a key press
                
                system("cls");

                battleGround_advZombie();
            }
        }
    }

    else {
        if(player->getMoney() >= 1000) {
            //reach to shop menu
            ShopMenu();
        }
        else {
            system("cls");

            cout << Purple << "YOU ENTER THE SHOP!" << Reset << endl << endl;
            cout << "YOU DON'T HAVE ENOUGH MONEY TO BUY ANY ITEMS OF THE SHOP " << endl << endl;
            cout << "Your current balance is " << player->getMoney() << " $" << endl;
            cout << "Continue your journey by facing an enemy and defeating them in the battle so you may collect their money...";
            getch();
        }
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Menu() {
    intro();

    // just getting user info :)))
    int age;
    string gender,username;

    system("cls");

    print_with_delay("IN THE LAND OF WESTEROS, WAR AND TENSIONS AMONG POWERFUL FAMILIES HAVE ALWAYS EXISTED.\nBUT NOW, THE WRATH AND UNINVITED POWER HAVE CAST A HARSH SHADOW OVER THIS LAND.\nYOU, A HERO, ARE FACED WITH AN IMPORTANT MISSION.\nYOU MUST NAVIGATE THROUGH THE DANGERS AND IMMENSE OBSTACLES AHEAD AND CONFRONT THE LOOMING THREATS THAT MENACE THE LAND OF WESTEROS.\n\n");
    getUserInfo(age , gender , username);
    print_with_delay("IN THIS JOURNEY, YOU MUST CHOOSE YOUR CHARACTER.\nWILL JON SNOW, THE STRONG AND JUST COMMANDER, SEIZE THE FATE OF THE LAND?\nOR WILL YOU, INSTEAD OF HIM, TRAVEL WITH JAIME LANNISTER, THE INTELLIGENT KNIGHT AND SEASONED STRATEGIST, AND OVERCOME ALL OBSTACLES?\nOR PERHAPS WITH DAENERYS TARGARYEN, THE DANGEROUS AND POWERFUL QUEEN, YOU SEEK TO RULE OVER WESTEROS?\nYOUR DECISION CAN CHANGE THE FATE OF THE LAND. ARE YOU READY?\n");
    cout << Red <<"\nPress any key to continue..."<< Reset;
   getch();

    // Creating the Player's Character Choices
    int chosenIndex;
    int money[6]={10000, 18000, 12000, 13000, 20000, 11000};
    
    while (true){
        system("cls");

        // Show all the different options a user has for the characters
        Human *character;
        cout << Purple << "CHOOSE THE INDEX OF THE CHARACTER YOU WANTED: " << Reset << endl << endl;
        for (int i = 0; i < 6; i++) {
            character = Factory::createCharacter(characterTypes[i]);
            cout << "[" << i+1 << "]. " << characterTypes[i] << endl;
            cout << "     " << "Stamina : " << character->getStamina() << endl;
            cout << "     " << "Money : " << money[i] << " $" << endl << endl;
        }
        cout << "CHOOSE THE INDEX OF THE CHARACTER YOU WANTED: " << endl;
        // Get the user's choice
        cin >> chosenIndex;
        if (chosenIndex > 0 and chosenIndex < 7){
            chosenIndex--;
            // Create Player Character
            character = Factory::createCharacter(characterTypes[chosenIndex]);
            player = new Player(*character,gender,username,age,money[chosenIndex]);
            characterTypes.erase(characterTypes.begin()+chosenIndex);
            break;
        }
        else{
            cout << "INVALID INDEX! PLEASE TRY AGAIN."; 
            cout << endl << "\nPlease press any key to continue...";
            getch();
        }
    }

    system("cls");

    // Show the details of the Player
    cout << Purple <<"CHARACTER YOU HAVE CHOSEN IS: " << Reset << endl << endl;
    cout << "Name : " << player->getName() << endl;
    cout << "Level : " << player->getLevel() << endl;
    cout << "Experience : " << player->getExperience() << endl;
    cout << "Stamina : " << player->getStamina() << endl;
    cout << "Health : " << player->getHealthPoints() << endl;
    cout << "Money : " << money[chosenIndex] << " $" << endl << endl;

    cout << "Now that you have chosen your character, you will go to shop to buy weapons to fight with.\n\n";
    cout << Red <<"Press any key to continue..." << Reset;
    getch();  // Wait for a key press

    createItem();
    Shop_PermanentItems_Menu();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void ShopMenu() {
    string temp;
    while(true){
        system("cls");
        int number;
        cout << Purple <<"YOU ENTER THE SHOP!" << Reset << endl << endl << "What do you want to buy?" << endl 
        << "[1].PERMANENT ITEMS" << endl << "[2].THROWABLE ITEMS" << endl << "[3].CONSUMABLE ITEMS" << endl << "[4].SHOW BACKPACK" << endl << "[5].EXIT SHOP" << endl << "[6].QUIT" << endl;
        cin >> number;
        switch(number){
            case 1: Show_Permanent_Items(); // Shows permanent items
                break;
        
            case 2: Show_Throwable_Items(); // Shows throwable items
                break;
        
            case 3: Show_Consumable_Items(); // Shows consumable items
                break;
        
            case 4: 
                system("cls");
                cout << Purple <<"HERE ARE THE AVAILABLE ITEMS IN THE BACKPACK:" << Reset << endl;
                player->getBackpack()->showItems();
                cout << Red <<"\nPress any key to go back..."<< Reset;
                getch(); // Wait for a key press
                ShopMenu(); // Recursive call
                break;

            case 5:
                cout << "YOU LEFT THE SHOP." << endl; // Exits the shop
                cout << Red <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                playground();
        
            case 6:
                cout<<"[!] ARE YOU SURE? (Y/N)";
                cin>>temp;
                if(temp=="Y"){
                    goodbye();
                }
                else{
                    ShopMenu();
                }
                break;

            default: 
                cout << "WRONG NUMBER" << endl << Red <<"\nPress any key to continue..."<< Reset;
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
    cout << "PERMANENT ITEMS:" << endl << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl << "[0].BACK" << endl;
    cin>>number;
    system("cls");
    switch(number){
        case 1: 
            cout << Purple <<"YOU GO TO TAKE A LOOK AT THE WARMWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
            WarmWeapon::showItems(); // Shows warm weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Show_Permanent_Items();
            }
            wweapon=new WarmWeapon(WarmWeapon::shop_items_permanent_warmweapon.at(item-1));
            wweapon->buy(); // Buys a warm weapon
            cout << Red <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
        
        case 2:
            cout << Purple <<"YOU GO TO TAKE A LOOK AT THE COLDWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
            ColdWeapon::showItems(); // Shows cold weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Show_Permanent_Items();
            }
            cweapon=new ColdWeapon(ColdWeapon::shop_items_permanent_coldweapon.at(item-1));
            cweapon->buy(); // Buys a cold weapon
            cout << Red <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
        
        case 0: 
            ShopMenu(); // Returns to the previous menu
    }
}

void Show_Throwable_Items() {
    system("cls");
    int item,quantity;
    Throwable *tweapon;
    cout << Purple <<"YOU GO TO TAKE A LOOK AT THE THROWABLE WEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
    Throwable::showItems(); // Shows throwable items
    cout << "[00]. Back" << endl;
    cout<<"\nWhich one do you want to buy?" << endl;
    cin >> item;
    while(item<0 && item>10){
        cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 5 ." << endl << "\nWhich one do you want to buy?" << endl;
        cin >> item;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    if ( item == 0) {
        return;
    }
    cout << "How many?" << endl;
    cin >> quantity;
    while(quantity <= 0){
        cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ." << endl << "How many?" << endl;
        cin >> quantity;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    tweapon=new Throwable(Throwable::shop_items_throwable.at(item-1));
    tweapon->buy(quantity); // Buys a throwable item
    cout << Red <<"\nPress any key to continue shopping..."<< Reset;
    getch();  // Wait for a key press
}

void Show_Consumable_Items() {
    system("cls");
    int number,item,quantity;
    Medicine *drug;
    Food *food;
    cout << "CONSUMAABLE ITEMS:" << endl << "[1].MEDICINES" << endl << "[2].FOODS" << endl << "[0].BACK" << endl;
    cin>>number;
    system("cls");
    switch(number){
        case 1: 
            cout << Purple <<"YOU GO TO TAKE A LOOK AT THE MEDICINES:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
            Medicine::showItems(); // Shows medicines
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 && item>10){
                cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ." << endl << "\nWhich one do you want to buy?" << endl;
                cin >> item;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if ( item == 0) {
                Show_Consumable_Items();
            }
            cout << "How many?" << endl;
            cin >> quantity;
            while(quantity <= 0){
                cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ." << endl << "How many?" << endl;
                cin >> quantity;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            drug=new Medicine(Medicine::shop_items_medicine.at(item-1));
            drug->buy(quantity); // Buys a medicine
            cout << Red <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
            
        case 2:
            cout << Purple <<"YOU GO TO TAKE A LOOK AT THE FOODS:" << "(YOUR MONEY : " << player->getMoney() << "$)" << Reset << endl << endl;
            Food::showItems(); // Shows foods
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 && item>10){
                cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ." << endl << "\nWhich one do you want to buy?" << endl;
                cin >> item;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if ( item == 0) {
                Show_Consumable_Items();
            }
            cout << "How many?" << endl;
            cin >> quantity;
            while(quantity <= 0){
                cout << "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ." << endl << "How many?" << endl;
                cin >> quantity;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            food=new Food(Food::shop_items_food.at(item-1));
            food->buy(quantity); // Buys a food
            cout << Red <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
        case 0: 
            ShopMenu(); // Returns to the previous menu
    }
}

void Shop_PermanentItems_Menu() {
    system("cls");
    int number,item;
    WarmWeapon *wweapon;
    ColdWeapon *cweapon;
    cout << Purple << "YOU ENTER THE SHOP!" << Reset << "\n\nYou should atleast buy one Permanent Item for fighting to continue the game. \nSo... What do you want to buy?" << endl << endl 
    << "PERMANENT ITEMS:" << endl << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl;
    cin >> number;
    system("cls");
    switch(number){
        case 1: 
            cout << Purple <<"YOU GO TO TAKE A LOOK AT THE WARMWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" << Reset << endl << endl;
            WarmWeapon::showItems(); // Shows warm weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Shop_PermanentItems_Menu();
            }
            wweapon=new WarmWeapon(WarmWeapon::shop_items_permanent_warmweapon.at(item-1));
            wweapon->buy(); // Buys a warm weapon
            cout << "\nOk, Now that you have bought a WarmWeapon, you can continue shopping and buy other Items that you want.\n";
            cout << Red <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            ShopMenu();
        
        case 2:
            cout << Purple <<"YOU GO TO TAKE A LOOK AT THE COLDWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" << Reset << endl << endl;
            ColdWeapon::showItems(); // Shows cold weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if ( item == 0) {
                Shop_PermanentItems_Menu();
            }
            cweapon=new ColdWeapon(ColdWeapon::shop_items_permanent_coldweapon.at(item-1));
            cweapon->buy(); // Buys a cold weapon
            cout << "\nOk, Now that you have bought a ColdWeapon, you can continue shopping and buy other Items that you want.\n";
            cout << Red <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            ShopMenu();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void goodbye(){
    system("cls");
    if(player->getState()==PlayerState::DEFEATED){
        gameOver();
        exit(0);
    }
    else{
        cout<<"Really?\n";
        Sleep(1000);
        cout<<"It's to soon!\n";
        Sleep(1000);
        cout<<"Fine i let you go :(\n";
        Sleep(1000);
        cout<<"Bye Bye "<<player->getUsername() << "." <<" hope to see you again:)" << endl << endl;
        endGame();
        exit(0);
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void battleGround_humanEnemy(){
    int turn = 1;        //odd turn for player even turn for enemy
    HE_Controller Enemy(humanEnemy);
    enemy = humanEnemy;
    Backpack *playerBackpack = player->getBackpack();
    while(Enemy.getStatus()==HumanEnemyStatus::ALIVE and player->getState()==PlayerState::ALIVE){
        if(turn%2!=0){
        //player turn
            BattleMenu();
            cout << endl;
            showPlayerInfo();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
        }
        else{
        //enemy turn
            cout<<"ENEMY'S TURN:\n\n";
            Enemy.updateState();
            Enemy.decision();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
            showPlayerInfo();
            cout << endl;
        }
        turn++;

        getch();

        system("cls");
    }

    if (player->getState()==PlayerState::ALIVE){
        cout<<"CONGRATULATIONS!! \nYOU HAVE WON THE MATCH.\n"; 
        player->getBankAccount()->prize(player->getLevel()*1000);
        getch();

        system("cls");
        cout << "THE FOLLOWING ITEMS WILL ADD TO YOUR BACKPACK: \n";

        //show Enemy's item
        //method to add to player backpack;
        Enemy.showBackpackItems();
        Enemy.transferItems();  
        getch();
    }
    playground();
}

// *----------------------------------------------------------------*

void battleGround_basicZombie(){
    int turn = 1;        //odd turn for player even turn for enemy
    BZ_Controller Enemy(basicZombie);
    enemy = basicZombie;
    Backpack *playerBackpack = player->getBackpack();
    while(Enemy.getState()==ZombieState::ALIVE and player->getState()==PlayerState::ALIVE){
        if(turn%2!=0){
        //player turn
            BattleMenu();
            cout << endl;
            showPlayerInfo();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
        }
        else{
        //enemy turn
            cout<<"ENEMY'S TURN:\n\n";
            Enemy.bite();
            cout << endl;
			//Enemy.updateState();
            Enemy.showInfo();
            cout << endl;
            showPlayerInfo();
            cout << endl;
        }
        turn ++;

        getch();

        system("cls");
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<"CONGRATULATIONS!! \nYOU HAVE WON THE MATCH.\n";
        player->getBankAccount()->prize(player->getLevel()*500);
        getch();
    }
    else{
        cout<<"YOU HAVE LOST THIS MATCH!\n\n";
        getch();
    }
    playground();
}

// *----------------------------------------------------------------*

void battleGround_advZombie(){
    int turn = 1;        //odd turn for player even turn for enemy
    AZ_Controller Enemy(advZombie);
    enemy = advZombie;
    Backpack *playerBackpack = player->getBackpack();
    while(Enemy.getState()==ZombieState::ALIVE and player->getState()==PlayerState::ALIVE){
        if(turn%2!=0){
        //player turn
            BattleMenu();
            cout << endl;
            showPlayerInfo();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
        }
        else{
        //enemy turn
        cout<<"ENEMY'S TURN:\n\n";
			//Enemy.updateState();
            Enemy.Attack();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
            showPlayerInfo();
            cout << endl;
        }
        turn ++;

        getch();

        system("cls");
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<"CONGRATULATIONS!! \nYOU HAVE WON THE MATCH.\n";
        player->getBankAccount()->prize(player->getLevel()*800);
        getch();
    } 
    playground();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void BattleMenu() {
    system("cls");
    int number;
    cout << Purple << "YOUR TURN : " << Reset << endl << endl;
    cout << "What do you want to do? (Attack ends your turn.)" << endl
         << "[1].ATTACK" << endl << "[2].CONSUME FOOD OR MEDICINE" << endl << "[3].SHOW PLAYER'S INFO" << endl
         << "[4].SHOW BACKPACK" << endl << "[5].UPGRADE WEAPON'S SKILL"<< endl <<"[6].QUIT"<< endl;
    cin >> number;

    string temp;
    WarmWeapon* wweapon;
    ColdWeapon* cweapon;
    Throwable* tweapon;
    Medicine* medicine;
    Food* food;

    Backpack* backpack = player->getBackpack();
    
    switch (number) {
        case 1: {
            if (player->getStamina() <= 0) {
            cout<<"YOU HAVE TOO LITTLE STAMINA! " << endl << "TO CONTINUE YOU NEED TO INCREASE YOUR STAMINA..." << endl; 
            cout<<"GO TO YOUR BACKPACK AND CONSUME FOOD..." << endl;
            player->setState(PlayerState::DEFEATED);
            getch(); // Wait for a key press
            backpack->ConsumeFoodForSurvival();
            }

            auto weapon = backpack->useWeapons();
            if (dynamic_cast<WarmWeapon*>(weapon)) {
                wweapon = dynamic_cast<WarmWeapon*>(weapon);
                wweapon->Attack(*player, *enemy);
            } else if (dynamic_cast<ColdWeapon*>(weapon)) {
                cweapon = dynamic_cast<ColdWeapon*>(weapon);
                cweapon->Attack(*player, *enemy);
            } else if (dynamic_cast<Throwable*>(weapon)) {
                tweapon = dynamic_cast<Throwable*>(weapon);
                tweapon->Throw(*player, *enemy);
            }
            break;
        }

        case 2: {
            auto consumable = backpack->useConsumableItems();
            if (dynamic_cast<Medicine*>(consumable)) {
                medicine = dynamic_cast<Medicine*>(consumable);
                medicine->use(*player);
            } else if (dynamic_cast<Food*>(consumable)) {
                food = dynamic_cast<Food*>(consumable);
                food->use(*player);
            }
            BattleMenu(); // Recursive call
            break;
        }

        case 3: {
            system("cls");
            showPlayerInfo();
            cout << Red <<"\nPress any key to go back..."<< Reset;
            getch();// Wait for a key press
            BattleMenu(); // Recursive call
            break;
        }

        case 4 : {
            system("cls");
            cout << Purple << "HERE ARE THE AVAILABLE ITEMS IN THE BACKPACK:" << Reset <<endl;
            backpack->showItems();
            cout << Red << "\nPress any key to go back..."<< Reset;
            getch();// Wait for a key press
            BattleMenu(); // Recursive call
            break;
        }
        
        case 5: {
            BankAccount* creditcard = player->getBankAccount();
            auto chosenweapon = backpack->upgradeWeapons();
            if (dynamic_cast<WarmWeapon*>(chosenweapon)) {
                wweapon = dynamic_cast<WarmWeapon*>(chosenweapon);
                if(wweapon->getwwa()->getUpgradePrice() > player->getMoney()){
                    cout << "NOT ENOUGHT MONEY TO UPGRADE THE WEAPON!" << endl;
                    cout << Red <<"\nPress any key to go back..."<< Reset;
                    getch();
                    BattleMenu();
                }
                wweapon->getwwa()->upgradeSkill(creditcard);
            } 
			else if (dynamic_cast<ColdWeapon*>(chosenweapon)) {
                cweapon = dynamic_cast<ColdWeapon*>(chosenweapon);
                if(cweapon->getcwa()->getUpgradePrice() > player->getMoney()){
                    cout << "NOT ENOUGHT MONEY TO UPGRADE THE WEAPON!" << endl;
                    cout << Red <<"\nPress any key to go back..."<< Reset;
                    getch();
                    BattleMenu();
                }
                cweapon->getcwa()->upgradeSkill(creditcard);
            } 
			else if (dynamic_cast<Throwable*>(chosenweapon)) {
                tweapon = dynamic_cast<Throwable*>(chosenweapon);
                if(tweapon->gettwa()->getUpgradePrice() > player->getMoney()){
                    cout << "NOT ENOUGHT MONEY TO UPGRADE THE WEAPON!" << endl;
                    cout << Red <<"\nPress any key to go back..."<< Reset;
                    getch();
                    BattleMenu();
                }
                tweapon->gettwa()->upgradeSkill(creditcard);
            }
            BattleMenu(); // Recursive call
            break;
        }
        
        case 6: {
            cout<<"[!] ARE YOU SURE? (Y/N)" << endl;
                cin>>temp;
                if(temp=="Y"){
                    goodbye();
                }
                else{
                    ShopMenu();
                }
            goodbye();
            break;
        }

        default: {
            cout << "INVALID CHOICE! PLEASE CHOOSE A NUMBER BETWEEN 1 AND 4." << endl;
            getch();
            BattleMenu(); // Recursive call
            break;
        }
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

void intro(){
    system("cls");
    cout << Cyan;
    cout <<" __        _____ _   _ _____ _____ ____    ___ ____     ____ ___  __  __ ___ _   _  ____" << '\n';
    cout <<"\\ \\      / /_ _| \\ | |_   _| ____|  _ \\  |_ _/ ___|   / ___/ _ \\|  \\/  |_ _| \\ | |/ ___| "<< '\n' ;
    cout <<" \\ \\ /\\ / / | ||  \\| | | | |  _| | |_) |  | |\\___ \\  | |  | | | | |\\/| || ||  \\| | |  _ "<< '\n' ;
    cout <<"  \\ V  V /  | || |\\  | | | | |___|  _ <   | | ___) | | |__| |_| | |  | || || |\\  | |_| |" << '\n';
    cout <<"   \\_/\\_/  |___|_| \\_| |_| |_____|_| \\_\\ |___|____/   \\____\\___/|_|  |_|___|_| \\_|\\____|" << '\n';
    cout << Reset;
    getch();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void gameOver(){
    cout << Red;
    cout <<"  ____    _    __  __ _____    _____     _______ ____" << '\n' ; 
    cout <<" / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\ " <<'\n' ;
    cout <<"| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |"<< '\n';
    cout <<"| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <" << '\n' ;
    cout <<" \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\ " << '\n';
    cout << Reset;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*


void endGame(){
    cout << Cyan;
    cout <<" _____ _   _ ____     ____    _    __  __ _____ "<< '\n';
    cout <<"| ____| \\ | |  _ \\   / ___|  / \\  |  \\/  | ____|"<<'\n';
    cout <<"|  _| |  \\| | | | | | |  _  / _ \\ | |\\/| |  _|" <<'\n';  
    cout <<"| |___| |\\  | |_| | | |_| |/ ___ \\| |  | | |___"<<'\n'; 
    cout <<"|_____|_| \\_|____/   \\____/_/   \\_\\_|  |_|_____|"<<'\n';
    cout << Reset;
}