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
string Cyan="\033[0;36m";         
string White="\033[0;37m";
string Pink = "\033[0;35m"; 
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

// Health class constructor
Health::Health(int l=1) : maxHealth(l*100) , currentHealth(l*100) {}

// Method to get current health
int Health::getCurrentHealth() {
    return currentHealth;
}

// Method to get max health
int Health::getMaxHealth() {
    return maxHealth;
}

// Method to decrease health
void Health::decreaseHealth(int damage){
    currentHealth -= damage;
    if(currentHealth <= 0)
        currentHealth = 0;
}

// Method to increase health
void Health::increaseHealth (int amount){
    currentHealth += amount;
    cout<< Yellow <<"HEALTH INCREASED FOR "<< amount << " HP!\n" << Reset;
    if(currentHealth >= maxHealth)
        currentHealth = maxHealth;
}

// Method to update max health
void Health::updateMaxHealth(){
    maxHealth += 50 * player->getLevel();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Stamina class constructor
Stamina::Stamina(int cs,int l=1):maximum((l)*100),currentStamina(cs){}

// Method to decrease stamina
void Stamina::decreaseStamina(int amount) {
    currentStamina -= amount;
    cout << Yellow<<"STAMINA DECREASED FOR "<< amount <<" STM!\n"<< Reset;
    if (currentStamina < 0)
        currentStamina = 0;
}

// Method to increase stamina
void Stamina::increaseStamina(int amount) {
    currentStamina += amount;
    cout<< Yellow<<"STAMINA INCREASED FOR "<< amount <<" STM!\n"<< Reset;
    if ( currentStamina > maximum)
        currentStamina = maximum;
}

// Method to update maximum stamina
void Stamina::updateMaximumStamina(){
    maximum+=50 * player->getLevel();
}

// Method to get current stamina
int Stamina::getCurrentStamina(){
    return currentStamina;
}

// Method to get max stamina
int Stamina::getMaxStamina(){
    return maximum;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Experience class constructor
Experience::Experience(Player *h):humanObj(h),maximum(100),currentExp(0){}

// Method to update maximum experience
void Experience::updateMaximum(){
    maximum+= 50 * player-> getLevel();
}

// Method to set current experience
void Experience::setCurrentExp(int selfDamage,int enemyDamage,int usedStamina){
    currentExp+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
    if(currentExp>=maximum){
        currentExp=0;
        updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
        player->hp.updateMaxHealth();
    }
}

// Method to increase experience
void Experience::increaseExp(int amount){
    currentExp+=amount;
    cout<< Yellow <<"EXPERIENCE INCREASED FOR " << amount << " EXP!\n" <<Reset;
    if(currentExp>=maximum){
        currentExp=0;
        humanObj->exp.updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
    }
}

// Method to get current experience
int Experience::getCurrentExp(){
    return currentExp;
}

// Method to get max experience
int Experience :: getMaxExp() {
    return maximum;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Skills class constructor
Skills::Skills(int n,int m):maximum(5),currentSkill(n),upgradePrice(m){}

// Method to upgrade skill
void Skills::upgradeSkill(BankAccount *creditcard){
    try{
        if(creditcard->getBalance()>=upgradePrice){
            if(currentSkill<maximum){
                creditcard->withdraw(upgradePrice);
                currentSkill++;
                cout << Yellow<< "\nWEAPON UPGRADE SUCCESSFULLY AND " << upgradePrice << "$ WAS DEDUCTED FROM YOUR ACCOUNT!" <<Reset;
                getch(); 
                setUpgradePrice();
            }
            else{
                cout<< Yellow <<"\nWEAPON'S LEVEL IS MAXIMUM!\n"<<Reset;
                getch();
            }
            
        }
        else{
            throw 0;
        }
    }
    catch(int temp){
        cout<< Yellow<<"\nNOT ENOUGH MONEY!\n"<<Reset;
    } 
}

// Method to set upgrade price
void Skills::setUpgradePrice(){
    upgradePrice+=10;
}

// Method to get current skill
int Skills::getCurrentSkill(){
    return currentSkill;
}

// Method to get upgrade price
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

// Method to remove food item
void Backpack::removeFoodItem(const Food foodItem) {
    FoodItems.erase(foodItem);
}

// Method to remove medicine item
void Backpack::removeMedicineItem(const Medicine medicineItem) {
    MedicineItems.erase(medicineItem);
}

// Method to remove throwable item
void Backpack::removeThrowableItem(const Throwable throwableItem) {
    ThrowableItems.erase(throwableItem);
}

// *----------------------------------------------------------------*

// Method to add food item
void Backpack::addFoodItem(const Food foodItem, int quantity){
    if(FoodItems.find(foodItem)!=FoodItems.end()){
        FoodItems[foodItem]+=quantity;
    }
    else
        FoodItems.insert({foodItem,quantity});
}

// Method to add medicine item
void Backpack::addMedicineItem(const Medicine medicineItem, int quantity){
    if(MedicineItems.find(medicineItem)!=MedicineItems.end()){
        MedicineItems[medicineItem]+=quantity;
    }
    else
        MedicineItems.insert({medicineItem,quantity});
}

// Method to add throwable item
void Backpack::addThrowableItem(const Throwable throwableItem, int quantity){
    if(ThrowableItems.find(throwableItem)!=ThrowableItems.end()){
        ThrowableItems[throwableItem]+=quantity;
    }
    else
        ThrowableItems.insert({throwableItem,quantity});
}

// Method to add warm weapon item
void Backpack::addWarmWeaponItem(const WarmWeapon warmWeaponItem){
    WarmWeaponItems.push_back(warmWeaponItem);
}

// Method to add cold weapon item
void Backpack::addColdWeaponItem(const ColdWeapon coldWeaponItem){
    ColdWeaponItems.push_back(coldWeaponItem);
}

// *----------------------------------------------------------------*

// Method to check warm weapon existence
bool Backpack::warmWeaponExistence(const WarmWeapon warmWeaponItem){
    for (const auto &item : WarmWeaponItems) {
        if (item== warmWeaponItem) {
            return true;
        }
    }
    return false;
}

// Method to check cold weapon existence
bool Backpack::coldWeaponExistence(const ColdWeapon coldWeaponItem){
    for (const auto &item : ColdWeaponItems) {
        if (item== coldWeaponItem) {
            return true;
        }
    }
    return false;
}

// *----------------------------------------------------------------*

// Method to use food item count
void Backpack::useFoodItemCount(const Food foodItem) {
    FoodItems[foodItem]-=1;
    if(FoodItems[foodItem]==0)
        removeFoodItem(foodItem);
}

// Method to use medicine item count
void Backpack::useMedicineItemCount(const Medicine medicineItem) {
    MedicineItems[medicineItem]-=1;
    if(MedicineItems[medicineItem]==0)
        removeMedicineItem(medicineItem);
}

// Method to use throwable item count
void Backpack::useThrowableItemCount(const Throwable throwableItem) {
    ThrowableItems[throwableItem]-=1;
    if(ThrowableItems[throwableItem]==0)
        removeThrowableItem(throwableItem);
}

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

// Method to show warm weapon items
void Backpack::showWarmWeaponItems(){
    int index = 1;
    for(auto i: WarmWeaponItems){
        cout  << "[" << index << "]. " << i.getName() << "(-" <<i.getwwa()->getCurrentSkill()*10<<"STM): " << "lvl : " << i.getwwa()->getCurrentSkill() <<" , harm : "<<i.getHarm()<< endl;
        index++;
    }
}

// Method to show cold weapon items
void Backpack::showColdWeaponItems(){
    int index = 1;
    for(auto i: ColdWeaponItems){
        cout  << "[" << index << "]. " << i.getName() << "(-" <<i.getcwa()->getCurrentSkill()*10<<"STM): " << "lvl : " << i.getcwa()->getCurrentSkill() <<" , harm : "<<i.getHarm() << endl;
        index++;
    }
}

// Method to show throwable items
void Backpack::showThrowableItems(){
    int index = 1;
    for(auto pair : ThrowableItems){
    Throwable item = pair.first;
    cout  << "[" << index << "]. " << item.getName() << "(-" <<item.gettwa()->getCurrentSkill()*10<<"STM): " << "lvl : " << item.gettwa()->getCurrentSkill() <<" , harm : "<<item.getHarm() << " , stock : " << pair.second << endl;
    index++;
    }
}

// Method to show medicine items
void Backpack::showMedicineItems(){
    int index = 1;
    for(auto pair : MedicineItems){
        Medicine item = pair.first;
        cout << "[" << index << "]. " << item.getName() << "(+" << item.getHeal() << "HP)(stock : " << pair.second << ")"  << '\n';
        index++;
    }
}

// Method to show food items
void Backpack::showFoodItems(){
    int index = 1;
    for(auto pair : FoodItems){
        Food item = pair.first;
        cout << "[" << index << "]. " << item.getName() << "(+" << item.getStrength() << "STM)(stock : " << pair.second << ")" << '\n';
        index++;
    }
}

// Method to show upgrade options for warm weapons
void Backpack::showUpgradeWarmWeapon(){
    int index = 1;
    for(auto i: WarmWeaponItems){
        cout << "[" << index << "]. " << i.getName() << "(lvl " << i.getwwa()->getCurrentSkill()  << ")" <<"(money need for upgrading : "<<i.getwwa()->getUpgradePrice() << "$)" << endl;
        index++;
    }
}

// Method to show upgrade options for cold weapons
void Backpack::showUpgradeColdWeapon(){
    int index = 1;
    for(auto i: ColdWeaponItems){
        cout << "[" << index << "]. " << i.getName() << "(lvl " << i.getcwa()->getCurrentSkill()  << ")" <<"(money need for upgrading : "<<i.getcwa()->getUpgradePrice() << "$)" << endl;
        index++;
    }
}

// Method to show upgrade options for throwable items
void Backpack::showUpgradeThrowable(){
    int index = 1;
    for(auto pair : ThrowableItems){
    Throwable item = pair.first;
    cout << "[" << index << "]. " << item.getName() << "(lvl " << item.gettwa()->getCurrentSkill()  << ")" <<"(money need for upgrading : "<<item.gettwa()->getUpgradePrice() << "$)" << endl;
    index++;
    }
}

// *----------------------------------------------------------------*

// Method to consume medicine from backpack after health is 0
void Backpack::ConsumeMedForSurvival() {
    system("cls");

    if(MedicineItems.empty()){
        cout << Yellow << "NO MEDICINE ITEM LEFT IN BACKPACK! YOUR ONLY CHANCE OF SURVIVAL IS TO GO TO SHOP AND BUY ANY MEDICINE THAT YOU CAN." << endl << endl <<Reset;
        cout << Pink << "Press any key to continue ... " << Reset;
        getch();
        medicineMenu();
    }
    
    system("cls");
    cout << Cyan << "HERE ARE THE AVAILABLE MEDICINE ITEMS IN THE BACKPACK:" << Reset << endl << endl;
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
        cout << Green << "\nNow that the Health has increased, as you have been lost the last game, you will go back to the playground." << Reset;
        getch();
    }
    else{
        cout << Yellow<< "\nINVALID CHOICE! PLEASE ENTER A VALID NUMBER." << endl <<Reset;
        getch();
        ConsumeMedForSurvival();
    }
}

// *----------------------------------------------------------------*

// Method to consume food from backpack after stamina is 0
void Backpack::ConsumeFoodForSurvival() {   
    
    system("cls");
    
    if(FoodItems.empty()){
        cout  << Yellow<< "NO FOOD ITEM LEFT IN BACKPACK! YOUR ONLY CHANCE OF SURVIVAL IS TO GO TO SHOP AND BUY ANY FOOD THAT YOU CAN." << Reset << endl << endl;
        cout <<  Pink << "Press any key to continue ... " << Reset;
        getch();
        foodMenu();
    }
    
    system("cls");
    cout << Cyan << "HERE ARE THE AVAILABLE FOOD ITEMS IN THE BACKPACK:" << Reset<< endl << endl;
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
        cout << Green << "\nNow that the stamina has increased, you will go back to the battleground and continue the last game." << Reset;
        getch();
    }
    else{
        cout  << Yellow<< "\nINVALID CHOICE! PLEASE ENTER A VALID NUMBER." << endl<< Reset;
        getch();
        ConsumeFoodForSurvival();
    }
}

//Method when player won't be able ta attack and have no food
void Backpack::AbleToUseWeapon(){
    bool flag = false;

    if(!FoodItems.empty()){
        flag = true; 
    }

    if(!WarmWeaponItems.empty() and flag == false){
        for(auto item : WarmWeaponItems){
            if(item.getwwa()->getCurrentSkill()*10<=player->getStamina()){
                flag = true;
                break;
            }
        }
    }

    if(!ColdWeaponItems.empty() and flag == false){
        for(auto item : ColdWeaponItems){
            if(item.getcwa()->getCurrentSkill()*10<=player->getStamina()){
                flag = true;
                break;
            }
        }
    }

    if(!ThrowableItems.empty() and flag == false){
        for(auto item : ThrowableItems){
            Throwable *t = new Throwable(item.first);
            if(t->gettwa()->getCurrentSkill()*10<=player->getStamina()){
                flag = true;
                break;
            }
        }
    }

    if(flag == false){
        system("cls");
        cout<<Yellow<<"YOU DON'T HAVE ENOGHT STAMINA AND ALSO NO FOOD TO USE ANY OF YOUR WEAPONS!"<<endl<<Reset;
        cout<<Yellow<<"So..."<<endl<<endl<<Reset;
        player->setState(PlayerState::DEFEATED);
        goodbye();
    }
}

// *----------------------------------------------------------------*

// Method to use weapons such as WARM , COLD and THROWABLE WEAPONS
Items* Backpack::useWeapons() {
    int number;
    int index;
    int ChosenWeapon;

    while(true){
        system("cls"); 
        cout << Cyan << "ENTER THE INDEX OF THE WEAPON YOU WANT TO ATTACK WITH:" << Reset << endl << endl
        << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl << "[3].THROWABLE WEAPONS" << endl << "[0].BACK" << endl;
        cin >> number;
        switch(number){
            case 1: 
                if(!WarmWeaponItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE WARMWEAPON YOU WANT TO ATTACK WITH:" << Reset<< endl << endl;
                        index = 1;
                        showWarmWeaponItems();
                        cout << "[0]. Back" << endl; 
                        cin >> ChosenWeapon;
                        if (ChosenWeapon >= 1 && ChosenWeapon <= WarmWeaponItems.size()) {
                            WarmWeapon *wweapon=new WarmWeapon(WarmWeaponItems.at(ChosenWeapon-1));
                            return wweapon;
                        }
                        else if(ChosenWeapon == 0){
                            break;
                        }
                        else {
                            cout << Yellow<< "\nINVALID INDEX! TRY AGAIN." << endl << Reset;
                            cout << Pink << "\nPress any key to continue..." << Reset;
                            getch();
                        }
                    }
                }
                else{
                    cout << Yellow<< "\nNO WARMWEAPON EXISTS!" << endl << Reset;
                    cout << Pink << "\nPress any key to continue..."<< Reset;
                    getch();
                    break;
                }
                break;

            case 2:
                if(!ColdWeaponItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE COLDWEAPON YOU WANT TO ATTACK WITH:" << Reset<< endl << endl;
                        index = 1;
                        showColdWeaponItems();
                        cout << "[0]. Back" << endl; 
                        cin >> ChosenWeapon;
                        if (ChosenWeapon >= 1 && ChosenWeapon <= ColdWeaponItems.size()) {
                            ColdWeapon* cweapon=new ColdWeapon(ColdWeaponItems.at(ChosenWeapon-1));
                            return cweapon;
                        }
                        else if(ChosenWeapon == 0){
                            break;
                        }
                        else {
                            cout <<Yellow<< "\nINVALID INDEX! TRY AGAIN." << endl << Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();
                        }
                    }
                }
                else{
                    cout <<Yellow<< "\nNO COLDWEAPON EXISTS!" << endl << Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();
                    break;
                }
                break;

            case 3:
                if(!ThrowableItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE THROWABLE WEAPON YOU WANT TO ATTACK WITH:" << Reset<< endl << endl;
                        index = 1;
                        showThrowableItems();
                        cout << "[0]. Back" << endl; 
                        cin >> ChosenWeapon;
                        if (ChosenWeapon >= 1 && ChosenWeapon <= ThrowableItems.size()) {
                            auto iter = next(ThrowableItems.begin(), ChosenWeapon - 1);
                            Throwable* tweapon = new Throwable(iter->first);
                            return tweapon;
                        }
                        else if(ChosenWeapon == 0){
                            break;
                        }
                        else {
                            cout <<Yellow<< "\nINVALID INDEX! TRY AGAIN." << endl<< Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();
                        }
                    }
                }
                else{
                    cout <<Yellow<< "\nNO THROWABLE WEAPON EXISTS!" << endl<< Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();
                    break;
                }
                break;

            case 0:
                return nullptr;
                break;

            default:
                cout <<Yellow<< "\nINVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 4." << endl<< Reset; 
                cout << Pink <<"\nPress any key to continue..."<< Reset;
                getch();
                break;
        }
    }
    return nullptr;
}

// *----------------------------------------------------------------*

// Method to use consumable items like FOOD , MEDICINE
Items* Backpack::useConsumableItems() {
    int number;
    int index;
    int ChosenConsumableItem;

    while(true){
        system("cls");
        cout <<Cyan << "ENTER THE INDEX OF THE CONSUMABLE ITEM YOU WANT TO USE:" << Reset<< endl << endl
        << "[1].MEDICINE" << endl << "[2].FOOD" << endl << "[0].BACK" << endl;
        cin >> number;
        switch(number){
            case 1: 
                if(!MedicineItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE MEDICINE YOU WANT TO USE:" << Reset<< endl << endl;
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
                            break;
                        }
                        else {
                            cout <<Yellow<< "\nINVALID INDEX! TRY AGAIN." << endl<< Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();  // Wait for a key press
                        }
                    }
                }
                else{
                    cout <<Yellow<< "\nNO MEDICINE EXISTS!" << endl<< Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();  // Wait for a key press
                    break;
                }
                break;

            case 2:
                if(!FoodItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE FOOD YOU WANT TO USE:" << Reset<< endl << endl;
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
                            break;
                        }
                        else{
                            cout <<Yellow<< "\nINVALID INDEX! TRY AGAIN." << endl<< Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();  // Wait for a key press
                        }
                    }
                }
                else{
                    cout <<Yellow<< "\nNO FOOD EXISTS!" << endl<< Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();  // Wait for a key press
                    break;
                }
                break;

            case 0:
                return nullptr;
                break;

            default:
                cout <<Yellow<< "\nINVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 3." << endl<< Reset;
                cout << Pink <<"\nPress any key to continue..."<< Reset;
                getch(); // Wait for a key press
                break;
        }
    }
    return nullptr;
}

// *----------------------------------------------------------------*

// Method to upgrade specific weapon type
Items* Backpack::upgradeWeapons() {
    int number;
    int index;
    int ChosenWeapon;

    while(true){
        system("cls");
        cout << Cyan << "ENTER THE INDEX OF THE WEAPON YOU WANT TO UPGRADE IT:" << Reset << endl << endl
        << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl << "[3].THROWABLE WEAPONS" << endl << "[0].BACK" << endl;
        cin >> number;
        switch(number){
            case 1: 
                if(!WarmWeaponItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE WARMWEAPON YOU WANT TO UPGRADE IT:(YOUR MONEY : " << player->getMoney() << "$)" << Reset<< endl << endl;
                        index = 1;
                        showUpgradeWarmWeapon(); // Prints warm weapon items
                        cout <<"[0]. Back" << endl; 
                        cin >> ChosenWeapon;
                        if (ChosenWeapon >= 1 && ChosenWeapon <= WarmWeaponItems.size()) {
                            WarmWeapon *wweapon=new WarmWeapon(WarmWeaponItems.at(ChosenWeapon-1));
                            return wweapon;
                        } 
                        else if(ChosenWeapon == 0){
                            break;
                        }
                        else {
                            cout <<Yellow<< "INVALID INDEX! TRY AGAIN." << endl<< Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();  // Wait for a key press
                        }
                    }
                }
                else{
                    cout <<Yellow<< "NO WARMWEAPON EXISTS!" << endl<< Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();  // Wait for a key press
                    break;
                }
                break;
    
            case 2:
                if(!ColdWeaponItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE COLDWEAPON YOU WANT TO UPGRADE IT:(YOUR MONEY : " << player->getMoney() << "$)" << Reset<< endl << endl;
                        index = 1;
                        showUpgradeColdWeapon();      // Prints cold weapon items
                        cout << "[0]. Back" << endl; 
                        cin >> ChosenWeapon;
                        if (ChosenWeapon >= 1 && ChosenWeapon <= ColdWeaponItems.size()) {
                            ColdWeapon* cweapon=new ColdWeapon(ColdWeaponItems.at(ChosenWeapon-1));
                            return cweapon;
                        }
                        else if(ChosenWeapon == 0){
                            break;
                        }
                        else {
                            cout <<Yellow<< "INVALID INDEX! TRY AGAIN." << endl<< Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();  // Wait for a key press
                        }
                    }
                }
                else{
                    cout <<Yellow<< "NO COLDWEAPON EXISTS!" << endl<< Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();  // Wait for a key press
                    break;
                }
                break;
    
            case 3:
                if(!ThrowableItems.empty()){
                    while(true){
                        system("cls");
                        cout <<Cyan << "CHOOSE THE THROWABLEWEAPON YOU WANT TO UPGRADE IT:(YOUR MONEY : " << player->getMoney() << "$)" << Reset<< endl << endl;
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
                            break;
                        }
                        else {
                            cout <<Yellow<< "INVALID INDEX! TRY AGAIN." << endl<< Reset;
                            cout << Pink <<"\nPress any key to continue..."<< Reset;
                            getch();  // Wait for a key press
                        }
                    }
                }
                else{
                    cout <<Yellow<< "NO THROWABLE WEAPON EXISTS!" << endl<< Reset;
                    cout << Pink <<"\nPress any key to continue..."<< Reset;
                    getch();  // Wait for a key press
                    break;
                }
                break;
    
            case 0:
                return nullptr;
                break;
    
            default:
                cout <<Yellow<< "\nINVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 4." << endl<< Reset;
                cout << Pink <<"\nPress any key to continue..."<< Reset;
                getch(); // Wait for a key press
                break;
        }
    }
    return nullptr;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// BankAccount class constructor
BankAccount::BankAccount(int n){
    this->balance = n;
}

// Method to get balance
int BankAccount::getBalance() {
    return balance;
}

// Method to deposit amount
void BankAccount::deposit(int amount) { 
    balance += amount;
}

// Method to withdraw amount
bool BankAccount::withdraw(int amount) {
    if (balance >= amount){
        balance -= amount;
        return true;
    }    
    else
        return false;
}

// Method to add prize amount
void BankAccount::prize(int amount){
    balance+=amount;
    cout<<Green<<"YOU TOOK A "<<amount<<"$ PRIZE!"<<endl<< Reset;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Person class constructor
Person::Person(string n,int l=1):name(n),level(l),hp(l){}

// Method to update level
void Person::updateLevel() {
    level++;
    cout<<Green<<"[!] NEW LEVEL!\t(YOUR LEVEL : "<<level << ")" <<endl<< Reset;
}

// Method to get level
int Person::getLevel() {
    return level;
}

// Method to get name
string Person::getName() {
    return name;
}

// Method to get health points
int Person::getHealthPoints() {
    return hp.getCurrentHealth();
}

// Method to get max health
int Person::getMaxHealth() {
    return hp.getMaxHealth();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Human class constructor
Human::Human(string n,int s,int l=1):Person(n,l),stamina(s,l){}

// Method to get backpack
Backpack* Human::getBackpack() {
    Backpack *p=&backpack;
    return p;
}

// Method to get stamina
int Human::getStamina() {
    return stamina.getCurrentStamina();
}

// Method to get max stamina
int Human::getMaxStamina() {
    return stamina.getMaxStamina();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Method to get bank account
BankAccount* Player::getBankAccount(){
    BankAccount* p = &bankAccount;
    return p;
}

// Player class constructor
Player::Player(string n, string g,string un, int a,int m,int s) : Human(n,s), age(a), gender(g), bankAccount(m) , username(un), exp(this), state(PlayerState::ALIVE){}

// Player class constructor
Player::Player(Human & human,string g,string un,int a,int m) : Human(human.getName(), human.getStamina()), age(a), gender(g), bankAccount(m), username(un),exp(this), state(PlayerState::ALIVE) {}

// Method to get age
int Player::getAge() {
    return age;
}

// Method to get gender
string Player::getGender() {
    return gender;
}

// Method to get money
int Player::getMoney() {
    return bankAccount.getBalance();
}

// Method to get experience
int Player::getExperience() {
    return exp.getCurrentExp();
}

// Method to get max experience
int Player::getMaxExperience() {
    return exp.getMaxExp();
}

// Method to get username
string Player::getUsername(){
    return username;
}

// Method to take damage
void Player::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout <<Yellow<< "YOU HAVE BEEN DEFEATED!" << endl << Reset;
        state=PlayerState::DEFEATED;
    } 
    else 
        cout <<Yellow<< "YOU TOOK " << amount << " DAMAGE!" << endl<< Reset;
}

// Method to set new life
void Player::newLife(){
    state=PlayerState::ALIVE;
}

// Method to get state
PlayerState Player::getState(){
    return state;
}

// Method to set state
void Player::setState(PlayerState s){
    state=s;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// HumanEnemy class constructor
HumanEnemy::HumanEnemy(Human& human,int l=1): Human(human.getName(), human.getStamina()*l,l),state(HumanEnemyState::FIGHT),status(HumanEnemyStatus::ALIVE){}

// Method to set state
void HumanEnemy::setState(HumanEnemyState newState){
    state = newState;
}

// Method to get state
HumanEnemyState HumanEnemy::getState(){
    return state;
}

// Method to get status
HumanEnemyStatus HumanEnemy::getStatus(){
    return status;
}

// Method to take damage
void HumanEnemy::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout <<Yellow<< name << " HAS BEEN DEFEATED!" << endl<< Reset;
        status = HumanEnemyStatus :: DEFEATED;
    } 
    else{ 
        cout <<Yellow<< name << " TAKES " << amount << " DAMAGE!" << endl << Reset;
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// HE_Controller class constructor
HE_Controller :: HE_Controller (HumanEnemy *HE) : model(HE) , backpack(HE->getBackpack()) , view(HE_View()) {}

// Method to update state
void HE_Controller::updateState() {
    double healthRatio = model->getHealthPoints() * 1.0 / model->hp.getMaxHealth();
    double staminaRatio = model->stamina.getCurrentStamina() * 1.0 / model->stamina.getMaxStamina();

    if (healthRatio <= 0.4)
        model->setState(HumanEnemyState :: LOW_HEALTH);

    else if (staminaRatio <= 0.4)
        model->setState(HumanEnemyState :: LOW_POWER);

    else
        model->setState(HumanEnemyState :: FIGHT);
}

// Method to get state
HumanEnemyState HE_Controller::getState(){
    return model->getState();
}

// Method to get status
HumanEnemyStatus HE_Controller::getStatus(){
    return model->getStatus();
}

// Method to choose weapon
Items* HE_Controller :: chooseWeapon() {
    srand(time(0)); // use current time as seed for random generator
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

// Method to choose food
Food* HE_Controller :: chooseFood() {
    if (!backpack->FoodItems.empty()) {
        int index = rand() % backpack->FoodItems.size();
        auto it = next(backpack->FoodItems.begin(),index);;
        Food *item = new Food(it->first);
        return item;
    }

    return nullptr;
}

// Method to choose medicine
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

// Method to attack with a weapon
void HE_Controller::Attack(Items* weapon) {
    WarmWeapon* wweapon;
    ColdWeapon* cweapon;
    Throwable* tweapon;
    if (dynamic_cast<WarmWeapon*>(weapon)) {
        wweapon=dynamic_cast<WarmWeapon*>(weapon);
        wweapon->Attack(*model, *player);
        view.attackView(model->getName(), *wweapon);
    } else if (dynamic_cast<ColdWeapon*>(weapon)) {
        cweapon = dynamic_cast<ColdWeapon*>(weapon);
        cweapon->Attack(*model, *player);
        view.attackView(model->getName(), *cweapon);
    } else if (dynamic_cast<Throwable*>(weapon)) {
        tweapon = dynamic_cast<Throwable*>(weapon);
        tweapon->Throw(*model, *player);
        view.attackView(model->getName(), *tweapon);
    }
}

// Method to make a decision based on the state
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

// Method to transfer items from enemy to player
void HE_Controller :: transferItems() {
    Backpack *playerBackpack = player->getBackpack();
    Backpack *enemyBackpack = model->getBackpack();

    if(!enemyBackpack->FoodItems.empty()) {
        for(auto& item : enemyBackpack->FoodItems) {
            playerBackpack->addFoodItem(item.first, item.second);
        }
    }

    if(!enemyBackpack->MedicineItems.empty()) {
        for(auto& item : enemyBackpack->MedicineItems) {
            playerBackpack->addMedicineItem(item.first, item.second);
        }
    }

    if(!enemyBackpack->ThrowableItems.empty()) {
        for(auto& item : enemyBackpack->ThrowableItems) {
            playerBackpack->addThrowableItem(item.first, item.second);
        }
    }

    if(!enemyBackpack->WarmWeaponItems.empty()) {
        for(auto& item : enemyBackpack->WarmWeaponItems) {
            if(!playerBackpack->warmWeaponExistence(item)) {
                playerBackpack->addWarmWeaponItem(item);
            }
        }
    }

    if(!enemyBackpack->ColdWeaponItems.empty()) {
        for(auto& item : enemyBackpack->ColdWeaponItems) {
            if(!playerBackpack->coldWeaponExistence(item)) {
                playerBackpack->addColdWeaponItem(item);
            }
        }
    }
}

// Method to show backpack items
void HE_Controller :: showBackpackItems() {
    view.showBackpackItems();
}

// Method to show info
void HE_Controller :: showInfo(){
    view.showInfo(*model);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// HE_View class constructor
HE_View::HE_View(){}

// Method to show info
void HE_View::showInfo(HumanEnemy& model) {
    cout  << Pink << "HUMAN ENEMY'S INFO:"  << Reset<< endl << endl;
    cout << "    " << "Name: " << model.getName() << endl;
    cout << "    " << "Level: " << model.getLevel() << endl;
    cout << "    " << "Stamina: " << model.getStamina() << " / (" << model.getMaxStamina() << ")" << endl;
    cout << "    " << "Health: " << model.getHealthPoints() << " / (" << model.getMaxHealth() << ")" << endl;
}

// Method to update health
void HE_View::updateHealth(string name,int amount) {
    cout << endl << name << "'s health increased for "<< amount << " HP!\n";
}

// Method to update stamina
void HE_View::updateStamina(string name,int amount) {
    cout << endl << name << "'s stamina increased for "<< amount << " STM!\n";
}

// Method to show attack view
void HE_View::attackView(string name,Items weapon) {
    cout << endl << name << " is attacking you by "<< weapon.getName() << "!" << "\n";
}

// Method to show backpack items
void HE_View :: showBackpackItems() {
    Backpack *HE_Backpack = humanEnemy->getBackpack();
    HE_Backpack->showItems();
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Zombie class constructor
Zombie::Zombie(string n,int l=1) : Person(n,l) , state(ZombieState::ALIVE){}

// Method to take damage
void Zombie::takeDamage(int amount) {
    if (hp.getCurrentHealth() <= 0) {
        cout <<Yellow<< name << " HAS BEEN DEFEATED!" << endl <<Reset;
        state=ZombieState::DEFEATED;
    } 
    else {
        cout <<Yellow<< name << " TAKES " << amount << " DAMAGE!" << endl<<Reset;
    }
}

// Method to get state
ZombieState Zombie::getState(){
    return state;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// BasicZombie class constructor
BasicZombie::BasicZombie(Zombie& zombie) : Zombie(zombie.getName(),zombie.getLevel()){}
BasicZombie::BasicZombie(string n,int l) : Zombie(n,l){}

// Method to bite
void BasicZombie :: bite() {
    player->hp.decreaseHealth(this->getLevel()*10);
    cout<<Yellow<<"YOU HAVE BEEN BITTEN BY "<<this->getName() <<"." << endl <<Reset;
    player->takeDamage(this->getLevel()*5);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// BZ_Controller class constructor
BZ_Controller :: BZ_Controller (BasicZombie* BZ) : model(BZ) , view(BZ_View())  {}

// Method to get state
ZombieState BZ_Controller::getState(){
    return model->getState();
}

// Method to bite
void BZ_Controller :: bite() {
    model->bite();
}
 
// Method to show info
void BZ_Controller :: showInfo() {
    view.showInfo(*model);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// BZ_View class constructor
BZ_View::BZ_View(){}

// Method to show info
void BZ_View :: showInfo(BasicZombie& model) {
    cout <<Pink<< "BASIC ZOMBIE'S INFO:" << Reset<< endl << endl;
    cout  << "    " << "Name : " << model.getName() << endl;
    cout  << "    " << "Level : " << model.getLevel() << endl;
    cout  << "    " << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// AdvZombie class constructor
AdvZombie::AdvZombie(Zombie & zombie) : Zombie(zombie.getName(),zombie.getLevel()){}
AdvZombie::AdvZombie(string n,int l) : Zombie(n,l){}

// Method to bite
void AdvZombie :: bite() {
    player->hp.decreaseHealth(this->getLevel()*15);
    cout<<Yellow<<"YOU HAVE BEEN BITTEN BY "<<this->getName()<<"." <<endl<< Reset;
    player->takeDamage(this->getLevel()*7);
}

// Method to scratch
void AdvZombie :: scratch() {
    player->hp.decreaseHealth(this->getLevel()*18);
    cout<<Yellow<<"YOU HAVE BEEN SCRATCHED BY "<<this->getName() <<"." <<endl<< Reset;
    player->takeDamage(this->getLevel()*8);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

AZ_Controller :: AZ_Controller (AdvZombie* AZ) : model(AZ) , view(AZ_View())  {}

// Method to get state
ZombieState AZ_Controller::getState(){
    return model->getState();
}
 
// Method to show info
void AZ_Controller :: showInfo() {
    view.showInfo(*model);
}

// Method to attack
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

// AZ_View class constructor
AZ_View::AZ_View(){}

// Method to show info
void AZ_View :: showInfo(AdvZombie& model) {
    cout << Pink << "ADVANCED ZOMBIE'S INFO:"  << Reset << endl << endl;
    cout  << "    " << "Name : " << model.getName() << endl;
    cout  << "    " << "Level : " << model.getLevel() << endl;
    cout  << "    " << "Health : " << model.getHealthPoints() << " / ( " << model.getMaxHealth() << " )" << endl;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Items class constructor
Items::Items(string n,int p,string t):name(n),price(p),type(t){}

// Method to check if two items are equal
bool Items::operator==(const Items& other) const {
    return (name == other.name);
}

// Method to check if one item is less than another
bool Items::operator<(const Items& other) const {
    return name < other.name;
}

// Method to get name
string Items::getName(){
    return name;
}

// Method to get type
string Items::getType(){
    return type;
}

// Method to get price
int Items::getPrice(){
    return price;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Permanent class constructor
Permanent::Permanent(string n,int p,string t,int e,int h):Items(n,p,t),exp(e),harm(h){}

// Method to get harm
int Permanent::getHarm(){
    return harm;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// WarmWeapon class constructor
WarmWeapon::WarmWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Warm Weapon",e,h),wwa(new WarmWeaponAbility(x)){
    WarmWeapon::addToVectors();
}

// Method to show items
void WarmWeapon::showItems(){
    int n=1;
    cout << Green << setw(32) << left << "             NAME" << setw(20) << "   +EXP"  << setw(20) << "  LEVEL" << setw(20) << "   HARM" << "    PRICE" << endl << Reset;
    for(WarmWeapon i : shop_items_permanent_warmweapon){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

// Method to buy item
void WarmWeapon::buy(){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(backpack->warmWeaponExistence(*this)){
        cout << endl <<Yellow<<"THIS ITEM ALREADY EXISTS IN YOUR BACKPACK!\n" << Reset;
    }

    else if(creditcard->withdraw(price)){
        cout <<Yellow<< "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"<< Reset;
        backpack->addWarmWeaponItem(*this);
        player->exp.increaseExp(exp);
    }
    else
        cout<<Yellow<<"\nNOT ENOUGH MONEY!\n"<< Reset;
}

// Method to add to vectors
void WarmWeapon::addToVectors(){
    shop_items_permanent_warmweapon.push_back(*this);
    shop_items_permanent.push_back(*this);
    shop_items.push_back(*this);
}

// Method to attack
void WarmWeapon::Attack(Human& attacker, Person& attacked) {
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) +
                 (wwa->getCurrentSkill() * weaponLevelFactor) +
                 (attacker.getLevel() * playerLevelFactor);

    attacker.stamina.decreaseStamina(10 * wwa->getCurrentSkill());
    attacked.hp.decreaseHealth(damage);

    Player* pAttacker = dynamic_cast<Player*>(&attacker);
    if (pAttacker) {
        pAttacker->exp.setCurrentExp(0, damage, 10 * wwa->getCurrentSkill());

        if (HumanEnemy* he = dynamic_cast<HumanEnemy*>(&attacked)) {
            he->takeDamage(damage);
        } else if (Zombie* z = dynamic_cast<Zombie*>(&attacked)) {
            z->takeDamage(damage);
        }
    } else {
        Player* pAttacked = dynamic_cast<Player*>(&attacked);
        if (pAttacked) {
            pAttacked->exp.setCurrentExp(damage, 0, 0);
            pAttacked->takeDamage(damage);
        }
    }
}

// Method to print warm weapon's details
ostream& operator<<(ostream &os,WarmWeapon &obj){
    os << setw(30) << left << obj.name << setw(20) << obj.exp  << setw(20) << obj.wwa->getCurrentSkill() << setw(20) << obj.harm << obj.price << "$";
    return os;
}

// Method to get warm weapon ability
WarmWeaponAbility* WarmWeapon::getwwa(){
    return wwa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// ColdWeapon class constructor
ColdWeapon::ColdWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Cold Weapon",e,h),cwa(new ColdWeaponAbility(x)){
    ColdWeapon::addToVectors();
}

// Method to show items
void ColdWeapon::showItems(){
    int n=1;
    cout <<Green<< setw(32) << left << "          NAME" << setw(20) << "   +EXP"  << setw(20) << "  LEVEL" << setw(20) << "   HARM" << "    PRICE" << endl<< Reset;
    for (ColdWeapon i : shop_items_permanent_coldweapon){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

// Method to buy item
void ColdWeapon::buy(){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(backpack->coldWeaponExistence(*this)){
        cout<<Yellow<<"THIS ITEM ALREADY EXISTS IN YOUR BACKPACK!\n"<< Reset;
    }

    else if(creditcard->withdraw(price)){
        cout <<Yellow<< "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"<< Reset;
        backpack->addColdWeaponItem(*this);
        player->exp.increaseExp(exp);
    }
    else
        cout<<Yellow<<"\nNOT ENOUGH MONEY!\n"<< Reset;
}

// Method to add to vectors
void ColdWeapon::addToVectors(){
    shop_items_permanent_coldweapon.push_back(*this);
    shop_items_permanent.push_back(*this);
    shop_items.push_back(*this);
}

// Method to attack
void ColdWeapon::Attack(Human& attacker, Person& attacked) {
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) +
                 (cwa->getCurrentSkill() * weaponLevelFactor) +
                 (attacker.getLevel() * playerLevelFactor);

    attacker.stamina.decreaseStamina(10 * cwa->getCurrentSkill());
    attacked.hp.decreaseHealth(damage);

    Player* pAttacker = dynamic_cast<Player*>(&attacker);
    if (pAttacker) {
        pAttacker->exp.setCurrentExp(0, damage, 10 * cwa->getCurrentSkill());

        if (HumanEnemy* he = dynamic_cast<HumanEnemy*>(&attacked)) {
            he->takeDamage(damage);
        } else if (Zombie* z = dynamic_cast<Zombie*>(&attacked)) {
            z->takeDamage(damage);
        }
    } else {
        Player* pAttacked = dynamic_cast<Player*>(&attacked);
        if (pAttacked) {
            pAttacked->exp.setCurrentExp(damage, 0, 0);
            pAttacked->takeDamage(damage);
        }
    }
}

// Method to print cold weapon's details
ostream& operator<<(ostream& os,ColdWeapon& obj){
    os << setw(30) << left << obj.name << setw(20) << obj.exp  << setw(20) << obj.cwa->getCurrentSkill() << setw(20) << obj.harm << obj.price << "$";
    return os;
}

// Method to get warm weapon ability
ColdWeaponAbility* ColdWeapon::getcwa(){
    return cwa;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Throwable class constructor
Throwable::Throwable(string n,int p,int x,int e,int h):Items(n,p,"Throwable Weapon"),twa(new ThrowableWeaponAbility(x)),exp(e),harm(h){
    Throwable::addToVectors();
}

// Method to show items
void Throwable::showItems(){
    int n=1;
    cout <<Green<< setw(32) << left << "          NAME" << setw(20) << "   +EXP"  << setw(20) << "  LEVEL" << setw(20) << "   HARM" << "  PRICE(each)" << endl<< Reset;
    for (Throwable i : shop_items_throwable){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

// Method to buy item
void Throwable::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout <<Yellow<< "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n" << Reset;
        backpack->addThrowableItem(*this,quantity);
        player->exp.increaseExp(exp*quantity);
    }
    else
        cout<<Yellow<<"\nNOT ENOUGH MONEY!\n"<< Reset;
}

// Method to add to vectors
void Throwable::addToVectors(){
    shop_items_throwable.push_back(*this);
    shop_items.push_back(*this);
}

// Method to throw item
void Throwable::Throw(Human& attacker, Person& attacked) {
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) +
                 (twa->getCurrentSkill() * weaponLevelFactor) +
                 (attacker.getLevel() * playerLevelFactor);

    attacker.stamina.decreaseStamina(10 * twa->getCurrentSkill());
    attacked.hp.decreaseHealth(damage);

    Player* pAttacker = dynamic_cast<Player*>(&attacker);
    if (pAttacker) {
        pAttacker->exp.setCurrentExp(0, damage, 10 * twa->getCurrentSkill());
        Backpack* b = pAttacker->getBackpack();
        b->useThrowableItemCount(*this);

        if (HumanEnemy* he = dynamic_cast<HumanEnemy*>(&attacked)) {
            he->takeDamage(damage);
        } else if (Zombie* z = dynamic_cast<Zombie*>(&attacked)) {
            z->takeDamage(damage);
        }
    }
    else {
        Player* pAttacked = dynamic_cast<Player*>(&attacked);
        if (pAttacked) {
            pAttacked->exp.setCurrentExp(damage, 0, 0);
            Backpack* b = attacker.getBackpack();
            b->useThrowableItemCount(*this);
            pAttacked->takeDamage(damage);
        }
    }
}

// Method to get throwable weapon ability
ThrowableWeaponAbility* Throwable::gettwa(){
    return twa;
}

// Method to get harm
int Throwable::getHarm(){
    return harm;
}

// Method to print throwable weapon's details
ostream& operator<<(ostream& os,Throwable& obj) {
    os << setw(30) << left << obj.name << setw(20) << obj.exp  << setw(20) << obj.twa->getCurrentSkill() << setw(20) << obj.harm << obj.price << "$";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Medicine class constructor
Medicine::Medicine(string n,int p,int h):Items(n,p,"Medicine"),heal(h){
    Medicine::addToVectors();
}

// Method to show items
void Medicine::showItems(){
    int n=1;
    cout <<Green<< setw(33) << left << "              NAME" << setw(20) << "  +HP" << "PRICE(each)" << endl<< Reset;
    for (Medicine i : shop_items_medicine){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

// Method to buy item
void Medicine::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout <<Yellow<< "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n" <<Reset;
        backpack->addMedicineItem(*this,quantity);
    }
    else
        cout<<Yellow<<"\nNOT ENOUGH MONEY!\n"<<Reset;
}

// Method to add to vectors
void Medicine::addToVectors(){
    shop_items_medicine.push_back(*this);
    shop_items.push_back(*this);
}

// Method to use item
void Medicine::use(Human &human){
    if(human.hp.getCurrentHealth() < human.hp.getMaxHealth()){
        cout <<Yellow<< "\nMEDICINE CONSUME SUCCESSFULLY." << endl<<Reset;
        human.hp.increaseHealth(heal);
        Backpack *b=human.getBackpack();
        b->useMedicineItemCount(*this);
        getch();
    }
    else{
        cout <<Yellow<< "\nYOUR HP IS FULL! NO NEED TO MEDICINE."<< endl<<Reset;
        getch();
    }
}

// Method to get heal
int Medicine::getHeal() {
    return heal;
}

// Method to print medicine's details
ostream& operator<<(ostream& os,Medicine& obj) {
    os << setw(30) << left << obj.name << setw(20) << obj.heal << obj.price << "$";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Food class constructor
Food::Food(string n,int p,int s):Items(n,p,"Food"),strength(s){
    Food::addToVectors();
}

// Method to show items
void Food::showItems(){
    int n=1;
    cout <<Green<< setw(33) << left << "              NAME" << setw(20) << "  +STM" << "PRICE(each)" << endl << Reset;
    for (Food i : shop_items_food){
        if (n < 10){
        cout << "[0" << n <<"]. " << i << endl;} 
        else{
        cout << "[" << n <<"]. " << i << endl;
        }
        n++;
    }
}

// Method to buy item
void Food::buy(int quantity){
    BankAccount *creditcard=player->getBankAccount();
    Backpack *backpack=player->getBackpack();

    if(creditcard->withdraw(price*quantity)){
        cout <<Yellow<< "\nITEM BOUGHT SUCCESSFULLY AND ADDED TO YOU BACKPACK!\n"<<Reset;
        backpack->addFoodItem(*this,quantity);
    }
    else
        cout<<Yellow<<"\nNOT ENOUGH MONEY!\n"<<Reset;
}

// Method to add to vectors
void Food::addToVectors(){
    shop_items_food.push_back(*this);
    shop_items.push_back(*this);
}

// Method to use item
void Food::use(Human& human){
    if(human.stamina.getCurrentStamina() < human.stamina.getMaxStamina()){
        cout <<Yellow<< "\nFOOD CONSUME SUCCESSFULLY." << endl<<Reset;
        human.stamina.increaseStamina(strength);
        Backpack *b=human.getBackpack();
        b->useFoodItemCount(*this);
        getch();
    }
    else{
        cout <<Yellow<< "\nYOUR STAMINA IS FULL! NO NEED TO FOOD." << endl<<Reset;
        getch();
    }
}

// Method to get strength
int Food::getStrength (){
    return strength;
}

// Method to print food's details
ostream& operator<<(ostream& os,Food& obj) {
    os << setw(30) << left << obj.name << setw(20) << obj.strength << obj.price << "$";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Factory class method to create character
Human* Factory::createCharacter(const string& type) {
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

// Factory class method to create Jon Snow character
Human* Factory::createJonSnow(const string& type) {
    return new Human(type, /*stamina*/ 100);
}

// Factory class method to create Jaime Lannister character
Human* Factory::createJaimeLannister(const string& type) {
    return new Human(type, /*stamina*/ 70);
}

// Factory class method to create Daenerys character
Human* Factory::createDaenerys(const string& type) {
    return new Human(type, /*stamina*/ 100);
}

// Factory class method to create Stannis character
Human* Factory::createStannis(const string& type) {
    return new Human(type, /*stamina*/ 90);
}

// Factory class method to create Joffrey character
Human* Factory::createJoffrey(const string& type) {
    return new Human(type, /*stamina*/ 75);
}

// Factory class method to create Theon Greyjoy character
Human* Factory::createTheonGreyjoy(const string& type) {
    return new Human(type, /*stamina*/ 85);
}

// Factory class method to create basic zombie
BasicZombie* Factory :: createBasicZombie(const int level) {
    return new BasicZombie("BasicZombie",level);
}

// Factory class method to create advanced zombie
AdvZombie* Factory :: createAdvanceZombie(const int level) {
    return new AdvZombie("AdvanceZombie",level);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

// Function to print text with delay
void print_with_delay(string text, int delay=4) {
    cout << Cyan;
    for (char c : text) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << Reset;
}

// Function to get user info
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

    cout << Pink << "\n\nPress any key to continue..." <<Reset;

    getch();

    system("cls");
}

// Function to show player info
void showPlayerInfo() {
    cout << "    Name: " << player->getName() << endl;
    cout << "    Level: " << player->getLevel() << endl;
    cout << "    Experience: " << player->getExperience() << " / ( " << player->getMaxExperience() << " )" << endl;
    cout << "    Stamina: " << player->getStamina() <<  " / ( " << player->getMaxStamina() << " )" << endl;
    cout << "    Health: " << player->getHealthPoints() << " / ( " << player->getMaxHealth() << " )" << endl;
    cout << "    Money: " << player->getMoney() << " $" << endl;
}

// Function to say goodbye
void goodbye(){

    if(player->getState()==PlayerState::DEFEATED){
        gameOver();
        exit(0);
    }
    else{
        system("cls");
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

// Function to show medicine menu
void medicineMenu() {
    system("cls");

    if(player->getMoney() >= 1000) {
        int item,quantity;
        Medicine *drug;
        cout  << Cyan << "YOU GO TO TAKE A LOOK AT THE MEDICINES:" << "(YOUR MONEY : " << player->getMoney() << ")" << Reset << endl << endl;

        Medicine::showItems();
        cout << "\nWhich one do you want to buy?" << endl;
        cin >> item;
        while(item<0 || item>10){
            cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ."<< Reset << endl << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        cout << "How many?" << endl;
        cin >> quantity;
        while(quantity <= 0){
            cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ."<< Reset << endl << "How many?" << endl;
            cin >> quantity;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        drug=new Medicine(Medicine::shop_items_medicine.at(item-1));
        if(player->getMoney() >= drug->getPrice() * quantity) {
            drug->buy(quantity);
            getch();
        }
        else {
            cout <<Yellow<< "NOT ENOUGH MONEY TO PURCHASE " << quantity << " AMOUNTS OF " << drug->getName() << " ITEMS. BUY LESS ITEMS ... \n"<< Reset;
            getch();
            medicineMenu();
        }
    } 
    else {
        cout << Yellow <<"NOT ENOUGH MONEY TO PURCHASE ANY MEDICINE FROM THE SHOP." << endl << Reset;
        cout << Yellow << "SO..." << endl  << endl<< Reset;
        goodbye();
    }
}

// Function to show food menu
void foodMenu() {
    system("cls");

    if(player->getMoney() >= 1000) {
        int item,quantity;
        Food *meal;
        cout << Cyan<< "YOU GO TO TAKE A LOOK AT THE FOODS:" << "(YOUR MONEY : " << player->getMoney() << ")"<< Reset<< endl << endl;

        Food::showItems();
        cout << "\nWhich one do you want to buy?" << endl;
        cin >> item;
        while(item<0 || item>10){
            cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ." << Reset<< endl << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        cout << "How many?" << endl;
        cin >> quantity;
        while(quantity <= 0){
            cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER GREATER THAN ZERO ."<< Reset<< endl << "How many?" << endl;
            cin >> quantity;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        meal=new Food(Food::shop_items_food.at(item-1));
        if(player->getMoney() >= meal->getPrice() * quantity) {
            meal->buy(quantity);
            getch();
        }
        else {
            cout <<Yellow<< "NOT ENOUGH MONEY TO PURCHASE  " << quantity << " AMOUNTS OF " << meal->getName() << " ITEMS. BUY LESS ITEMS ... \n"<< Reset;
            getch();
            foodMenu();
        }
    } 
    else {
        cout << Yellow <<"NOT ENOUGH MONEY TO PURCHASE ANY FOOD FROM THE SHOP." << endl << Reset;
        cout << Yellow << "SO..." << endl <<endl<< Reset;
        player->setState(PlayerState::DEFEATED);
        goodbye();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void playground() {
    system("cls");
    srand(time(0));
    Backpack *playerBackpack = player->getBackpack();
    Backpack *enemyBackpack;
    int randomNum = rand();

    if (player->getState() == PlayerState::DEFEATED) {
        cout<<Yellow<<"YOU HAVE BEEN DEFEATED BY YOUR ENEMY , YOUR HP IS 0! " << endl << "TO CONTINUE THE GAME YOU NEED TO INCREASE YOUR HP..."<< endl <<Reset; 
        cout<<Green<<"\nYou will go to your backpack and consume medicine." << endl << Reset;
        getch();
        playerBackpack->ConsumeMedForSurvival();
    }

    int choice;
    if ((randomNum % 100) < 70) {
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

            if(humanEnemy->getName() == "JaimeLannister"){
                JaimeLannister();
            }
            else if (humanEnemy->getName() == "JonSnow")
            {
                JonSnow();
            }
            else if (humanEnemy->getName() == "Daenerys")
            {
                Daenerys();
            }
            else if (humanEnemy->getName() == "Stannis")
            {
                Stannis();
            }
            else if (humanEnemy->getName() == "Joffrey")
            {
                Joffrey();
            }
            else if (humanEnemy->getName() == "TheonGreyjoy")
            {
                TheonGreyjoy();
            }
        
            //show enemy's info
            cout << Cyan << "THE HUMAN ENEMY YOU ARE FACING IS : " << Reset << endl << endl;
            cout << "    " << "Name : " << humanEnemy->getName() << endl;
            cout << "    " << "Level : " << humanEnemy->getLevel() << endl;
            cout << "    " << "Stamina : " << humanEnemy->getStamina() <<  " / ( " << humanEnemy->getMaxStamina() << " )" << endl;
            cout << "    " << "Health : " << humanEnemy->getHealthPoints() << " / ( " << humanEnemy->getMaxHealth() << " )" << endl << endl;
            cout << Pink << "Press any key to Enter to fightground..." << Reset;
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
                show_basicZombie();

                //show enemy's info
                cout << Cyan << "THE BASIC ZOMBIE YOU ARE FACING IS : " << Reset<< endl << endl;
                cout << "    " << "Name : " << basicZombie->getName() << endl;
                cout << "    " << "Level : " << basicZombie->getLevel() << endl;
                cout << "    " << "Health : " << basicZombie->getHealthPoints() << " / ( " << basicZombie->getMaxHealth() << " )" << endl << endl;
                cout << Pink <<"Press any key to Enter to fightground..." << Reset;
                getch();  // Wait for a key press

                system("cls");

                battleGround_basicZombie();
            }

            else{
            //fight with advance zombie

                //create advance zombie
                advZombie = Factory::createAdvanceZombie(player->getLevel());
                show_advZombie();

                //show enemy's info
                cout << Cyan <<"THE ADVANCED ZOMBIE YOU ARE FACING IS : " << Reset<< endl << endl;
                cout << "    " << "Name : " << advZombie->getName() << endl;
                cout << "    " << "Level : " << advZombie->getLevel() << endl;
                cout << "    " << "Health : " << advZombie->getHealthPoints() << " / ( " << advZombie->getMaxHealth() << " )" << endl << endl;
                cout << Pink <<"Press any key to Enter to fightground..." << Reset;
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

            cout << Cyan << "YOU ENTER THE SHOP!" << Reset << endl << endl;
            cout <<Yellow << "YOU DON'T HAVE ENOUGH MONEY TO BUY ANY ITEMS OF THE SHOP! " << endl;
            cout << "YOUR CURRENT BALANCE IS " << player->getMoney() << "$." << endl  << endl << Reset;
            cout << Green << "Continue your journey by facing an enemy and defeating them in the battle so you may collect their money..." <<Reset;
            getch();
            playground();
        }
    }
}

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
            cout << Pink << "PLAYER'S INFO:" << Reset << endl << endl;
            showPlayerInfo();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
        }
        else{
        //enemy turn
            cout<<Cyan <<"ENEMY'S TURN:\n" << Reset;
            Enemy.updateState();
            Enemy.decision();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
            cout << Pink << "PLAYER'S INFO:" << Reset << endl << endl;
            showPlayerInfo();
            cout << endl;
        }
        turn++;

        getch();

        system("cls");
    }

    if (player->getState()==PlayerState::ALIVE){
        cout <<Yellow <<"CONGRATULATIONS!! \nYOU HAVE WON THE MATCH.\n" << Reset; 
        player->getBankAccount()->prize(player->getLevel()*1000);
        getch();

        system("cls");
        cout << Cyan << "THE FOLLOWING ITEMS WILL ADD TO YOUR BACKPACK: \n" << Reset;

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
            cout << Pink << "PLAYER'S INFO:" << Reset << endl << endl;
            showPlayerInfo();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
        }
        else{
        //enemy turn
            cout << Cyan << "ENEMY'S TURN:\n\n" <<Reset;
            Enemy.bite();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
            cout << Pink << "PLAYER'S INFO:" << Reset << endl << endl;
            showPlayerInfo();
            cout << endl;
        }
        turn ++;

        getch();

        system("cls");
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<Yellow<<"CONGRATULATIONS!! \nYOU HAVE WON THE MATCH.\n" << Reset;
        player->getBankAccount()->prize(player->getLevel()*500);
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
            cout << Pink << "PLAYER'S INFO:" << Reset << endl << endl;
            showPlayerInfo();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
        }
        else{
        //enemy turn
        cout<<Cyan << "ENEMY'S TURN:\n\n" << Reset;
            Enemy.Attack();
            cout << endl;
            Enemy.showInfo();
            cout << endl;
            cout << Pink << "PLAYER'S INFO:" << Reset << endl << endl;
            showPlayerInfo();
            cout << endl;
        }
        turn ++;

        getch();

        system("cls");
    }

    if(player->getState() == PlayerState::ALIVE){
        cout<<Yellow<<"CONGRATULATIONS!! \nYOU HAVE WON THE MATCH.\n" << Reset;
        player->getBankAccount()->prize(player->getLevel()*800);
        getch();
    } 
    playground();
}

void BattleMenu() {

    int number;
    string temp;
    WarmWeapon* wweapon;
    ColdWeapon* cweapon;
    Throwable* tweapon;
    Medicine* medicine;
    Food* food;
    bool flag = true;

    Backpack* backpack = player->getBackpack();
    
    while(flag==true){
        system("cls");
        cout << Cyan << "YOUR TURN : " << Reset << endl << endl;
        cout << "What do you want to do? (Attack ends your turn.)" << endl
         << "[1].ATTACK" << endl << "[2].CONSUME FOOD OR MEDICINE" << endl << "[3].SHOW PLAYER'S INFO" << endl
         << "[4].SHOW BACKPACK" << endl << "[5].UPGRADE WEAPON'S SKILL"<< endl <<"[6].QUIT"<< endl;
        cin >> number;

        switch (number) {
            case 1: {
                if (player->getStamina() <= 0) {
                system("cls");
                cout<<Yellow<<"YOU HAVE TOO LITTLE STAMINA! " << endl << "TO CONTINUE YOU NEED TO INCREASE YOUR STAMINA..." << endl << Reset;
                cout << Green <<"\nYou will go to your backpack and consume food." << endl << Reset;
                player->setState(PlayerState::DEFEATED);
                getch(); // Wait for a key press
                backpack->ConsumeFoodForSurvival();
                }

                auto weapon = backpack->useWeapons();
                if(weapon!=nullptr){
                    if (dynamic_cast<WarmWeapon*>(weapon)) {
                        wweapon = dynamic_cast<WarmWeapon*>(weapon);
                        if(wweapon->getwwa()->getCurrentSkill()*10>player->getStamina()){
                            cout<<Yellow<<"YOU DON'T HAVE ENOGHT STAMINA TO USE THIS WEAPON! " << endl << "TO USE YOU NEED TO INCREASE YOUR STAMINA..." << endl << Reset;
                            cout << Green <<"\nYou should go to your backpack and consume food or use another weapon to attack."<< endl << Reset;
                            getch();
                            backpack->AbleToUseWeapon();
                            break;
                        }
                        else
                            wweapon->Attack(*player, *enemy);
                    } else if (dynamic_cast<ColdWeapon*>(weapon)) {
                        cweapon = dynamic_cast<ColdWeapon*>(weapon);
                        if(cweapon->getcwa()->getCurrentSkill()*10>player->getStamina()){
                            cout<<Yellow<<"YOU DON'T HAVE ENOGHT STAMINA TO USE THIS WEAPON! " << endl << "TO USE YOU NEED TO INCREASE YOUR STAMINA..." << endl << Reset;
                            cout << Green <<"\nYou should go to your backpack and consume food or use another weapon to attack." << endl << Reset;
                            getch();
                            backpack->AbleToUseWeapon();
                            break;
                        }
                        else
                            cweapon->Attack(*player, *enemy);
                    } else if (dynamic_cast<Throwable*>(weapon)) {
                        tweapon = dynamic_cast<Throwable*>(weapon);
                        if(tweapon->gettwa()->getCurrentSkill()*10>player->getStamina()){
                            cout<<Yellow<<"YOU DON'T HAVE ENOGHT STAMINA TO USE THIS WEAPON! " << endl << "TO USE YOU NEED TO INCREASE YOUR STAMINA..." << endl << Reset;
                            cout << Green <<"\nYou should go to your backpack and consume food or use another weapon to attack." << endl << Reset;
                            getch();
                            backpack->AbleToUseWeapon();
                            break;
                        }
                        else
                            tweapon->Throw(*player, *enemy);
                    }
                    flag = false;
                }
                break;
            }

            case 2: {
                auto consumable = backpack->useConsumableItems();
                if(consumable!=nullptr){
                    if (dynamic_cast<Medicine*>(consumable)) {
                        medicine = dynamic_cast<Medicine*>(consumable);
                        medicine->use(*player);
                    } else if (dynamic_cast<Food*>(consumable)) {
                        food = dynamic_cast<Food*>(consumable);
                        food->use(*player);
                    }
                }
                break;
            }

            case 3: {
                system("cls");
                cout << Cyan << "PLAYER'S INFO:" << Reset << endl << endl;
                showPlayerInfo();
                cout << Pink <<"\nPress any key to go back..."<< Reset;
                getch();// Wait for a key press
                break;
            }

            case 4 : {
                system("cls");
                cout << Cyan << "HERE ARE THE AVAILABLE ITEMS IN THE BACKPACK:" << Reset <<endl;
                backpack->showItems();
                cout << Pink << "\nPress any key to go back..."<< Reset;
                getch();// Wait for a key press
                break;
            }

            case 5: {
                BankAccount* creditcard = player->getBankAccount();
                auto chosenweapon = backpack->upgradeWeapons();
                if(chosenweapon!=nullptr){
                    if (dynamic_cast<WarmWeapon*>(chosenweapon)) {
                        wweapon = dynamic_cast<WarmWeapon*>(chosenweapon);
                        if(wweapon->getwwa()->getUpgradePrice() > player->getMoney()){
                            cout <<Yellow<< "NOT ENOUGHT MONEY TO UPGRADE THE WEAPON!" << endl << Reset;
                            cout << Pink <<"\nPress any key to go back..."<< Reset;
                            getch();
                            break;
                        }
                        wweapon->getwwa()->upgradeSkill(creditcard);
                    } 
	    		    else if (dynamic_cast<ColdWeapon*>(chosenweapon)) {
                        cweapon = dynamic_cast<ColdWeapon*>(chosenweapon);
                        if(cweapon->getcwa()->getUpgradePrice() > player->getMoney()){
                            cout <<Yellow<< "NOT ENOUGHT MONEY TO UPGRADE THE WEAPON!" << endl << Reset;
                            cout << Pink <<"\nPress any key to go back..."<< Reset;
                            getch();
                            break;
                        }
                        cweapon->getcwa()->upgradeSkill(creditcard);
                    } 
	    		    else if (dynamic_cast<Throwable*>(chosenweapon)) {
                        tweapon = dynamic_cast<Throwable*>(chosenweapon);
                        if(tweapon->gettwa()->getUpgradePrice() > player->getMoney()){
                            cout <<Yellow<< "NOT ENOUGHT MONEY TO UPGRADE THE WEAPON!" << endl << Reset;
                            cout << Pink <<"\nPress any key to go back..."<< Reset;
                            getch();
                            break;
                        }
                        tweapon->gettwa()->upgradeSkill(creditcard);
                    }
                }
                break;
            }

            case 6: {
                cout<<"[!] ARE YOU SURE? (Y/N)" << endl;
                    cin>>temp;
                    if(temp=="Y" || temp=="y"){
                        goodbye();
                    }
                    else{
                        break;
                    }
                goodbye();
                break;
            }

            default: {
                cout <<Yellow<< "\nINVALID CHOICE! PLEASE CHOOSE A NUMBER BETWEEN 1 AND 6." << endl << Reset;
                getch();
                break;
            }
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
    cout << Pink <<"\nPress any key to continue..."<< Reset;
    getch();

    // Creating the Player's Character Choices
    int chosenIndex;
    int money[6]={10000, 18000, 12000, 13000, 20000, 11000};
    
    while (true){
        system("cls");

        // Show all the different options a user has for the characters
        Human *character;
        cout << Cyan << "CHOOSE THE INDEX OF THE CHARACTER YOU WANTED: " << Reset << endl << endl;
        for (int i = 0; i < 6; i++) {
            character = Factory::createCharacter(characterTypes[i]);
            cout << "[" << i+1 << "]. " << characterTypes[i] << endl;
            cout << "     " << "Stamina : " << character->getStamina() << endl;
            cout << "     " << "Money : " << money[i] << " $" << endl << endl;
        }
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
            cout <<Yellow << "INVALID INDEX! PLEASE TRY AGAIN." <<Reset; 
            cout << Pink << "\nPress any key to continue..." << Reset;
            getch();
        }
    }

    system("cls");

    // Show the details of the Player
    cout << Cyan <<"CHARACTER YOU HAVE CHOSEN IS: " << Reset << endl << endl;
    cout << "     Name : " << player->getName() << endl;
    cout << "     Level : " << player->getLevel() << endl;
    cout << "     Experience : " << player->getExperience() << endl;
    cout << "     Stamina : " << player->getStamina() << endl;
    cout << "     Health : " << player->getHealthPoints() << endl;
    cout << "     Money : " << money[chosenIndex] << " $" << endl << endl;

    cout <<Green<< "Now that you have chosen your character, you will go to shop to buy weapons to fight with.\n\n" << Reset;
    cout << Pink <<"Press any key to continue..." << Reset;
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
        cout << Cyan <<"YOU ENTER THE SHOP!" << Reset << endl << endl << "What do you want to buy?" << endl 
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
                cout << Cyan <<"HERE ARE THE AVAILABLE ITEMS IN THE BACKPACK:" << Reset << endl;
                player->getBackpack()->showItems();
                cout << Pink <<"\nPress any key to go back..."<< Reset;
                getch(); // Wait for a key press
                ShopMenu(); // Recursive call
                break;

            case 5:
                cout <<Yellow<< "\nYOU LEFT THE SHOP." << endl << Reset; // Exits the shop
                cout << Pink <<"\nPress any key to continue..."<< Reset;
                getch();  // Wait for a key press
                playground();
        
            case 6:
                cout<<"[!] ARE YOU SURE? (Y/N)";
                cin>>temp;
                if(temp=="Y" || temp=="y"){
                    goodbye();
                }
                else{
                    ShopMenu();
                }
                break;

            default: 
                cout <<Yellow << "WRONG NUMBER!" << Reset << endl << Pink <<"\nPress any key to continue..."<< Reset;
                getch(); // Handles invalid input
                break;
        }
    }
}

// *----------------------------------------------------------------*

void Show_Permanent_Items() {
    system("cls");
    int number,item;
    WarmWeapon *wweapon;
    ColdWeapon *cweapon;
    cout << "PERMANENT ITEMS:" << endl << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl << "[0].BACK" << endl;
    cin>>number;
    switch(number){
        case 1: 
            system("cls");
            cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE WARMWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
            WarmWeapon::showItems(); // Shows warm weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 || item>10){
                cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ."<< Reset << endl << "\nWhich one do you want to buy?" << endl;
                cin >> item;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if ( item == 0) {
                Show_Permanent_Items();
            }
            wweapon=new WarmWeapon(WarmWeapon::shop_items_permanent_warmweapon.at(item-1));
            wweapon->buy(); // Buys a warm weapon
            cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
        
        case 2:
            system("cls");
            cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE COLDWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
            ColdWeapon::showItems(); // Shows cold weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 || item>10){
                cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ."<< Reset << endl << "\nWhich one do you want to buy?" << endl;
                cin >> item;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if ( item == 0) {
                Show_Permanent_Items();
            }
            cweapon=new ColdWeapon(ColdWeapon::shop_items_permanent_coldweapon.at(item-1));
            cweapon->buy(); // Buys a cold weapon
            cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
        
        case 0: 
            ShopMenu(); // Returns to the previous menu
        default :
            cout <<Yellow<< "\nINVALID CHOICE! CHOOSE A NUMBER BETWEEN 0 AND 2." << endl<< Reset;
            cout << Pink <<"\nPress any key to continue..."<< Reset;
            getch();
            Show_Permanent_Items();
    }
}

// *----------------------------------------------------------------*

void Show_Throwable_Items() {
    system("cls");
    int item,quantity;
    Throwable *tweapon;
    cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE THROWABLE WEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
    Throwable::showItems(); // Shows throwable items
    cout << "[00]. Back" << endl;
    cout<<"\nWhich one do you want to buy?" << endl;
    cin >> item;
    while(item<0 || item>5){
        cout <<Yellow << "YOU HAVE ENTERED THE WRONG NUMBER, ENTER A NUMBER BETWEEN 0 AND 5."  << Reset << endl << "\nWhich one do you want to buy?" << endl;
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
        cout <<Yellow << "YOU HAVE ENTERED THE WRONG NUMBER, ENTER A NUMBER GREATER THAN ZERO." << Reset << endl << "How many?" << endl;
        cin >> quantity;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    tweapon=new Throwable(Throwable::shop_items_throwable.at(item-1));
    tweapon->buy(quantity); // Buys a throwable item
    cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
    getch();  // Wait for a key press
}

// *----------------------------------------------------------------*

void Show_Consumable_Items() {
    system("cls");
    int number,item,quantity;
    Medicine *drug;
    Food *food;
    cout << "CONSUMAABLE ITEMS:" << endl << "[1].MEDICINES" << endl << "[2].FOODS" << endl << "[0].BACK" << endl;
    cin>>number;
    switch(number){
        case 1: 
            system("cls");
            cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE MEDICINES:" << "(YOUR MONEY : " << player->getMoney() << "$)" <<Reset << endl << endl;
            Medicine::showItems(); // Shows medicines
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 || item>10){
                cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER, ENTER A NUMBER BETWEEN 0 AND 10."  <<Reset << endl << "\nWhich one do you want to buy?" << endl;
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
                cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER, ENTER A NUMBER GREATER THAN ZERO."  << Reset<< endl << "How many?" << endl;
                cin >> quantity;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            drug=new Medicine(Medicine::shop_items_medicine.at(item-1));
            drug->buy(quantity); // Buys a medicine
            cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
            
        case 2:
            system("cls");
            cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE FOODS:" << "(YOUR MONEY : " << player->getMoney() << "$)" << Reset << endl << endl;
            Food::showItems(); // Shows foods
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 || item>10){
                cout <<Yellow << "YOU HAVE ENTERED THE WRONG NUMBER, ENTER A NUMBER BETWEEN 0 AND 10."  <<Reset<< endl << "\nWhich one do you want to buy?" << endl;
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
                cout <<Yellow << "YOU HAVE ENTERED THE WRONG NUMBER, ENTER A NUMBER GREATER THAN ZERO."  << Reset<< endl << "How many?" << endl;
                cin >> quantity;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            food=new Food(Food::shop_items_food.at(item-1));
            food->buy(quantity); // Buys a food
            cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            break;
        case 0: 
            ShopMenu(); // Returns to the previous menu
        default :
            cout <<Yellow<< "\nINVALID CHOICE! CHOOSE A NUMBER BETWEEN 0 AND 2." << endl<< Reset;
            cout << Pink <<"\nPress any key to continue..."<< Reset;
            getch();
            Show_Consumable_Items();
    }
}

// *----------------------------------------------------------------*

void Shop_PermanentItems_Menu() {
    system("cls");
    int number,item;
    WarmWeapon *wweapon;
    ColdWeapon *cweapon;
    cout << Cyan << "YOU ENTER THE SHOP!" << Reset << Green <<"\n\nYou should atleast buy one Permanent Item for fighting to continue the game."  << Reset << "\n\nSo... What do you want to buy?" << endl 
    << "PERMANENT ITEMS:" << endl << "[1].WARMWEAPONS" << endl << "[2].COLDWEAPONS" << endl;
    cin >> number;
    switch(number){
        case 1: 
            system("cls");
            cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE WARMWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" << Reset << endl << endl;
            WarmWeapon::showItems(); // Shows warm weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 || item>10){
                cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ."<< Reset << endl << "\nWhich one do you want to buy?" << endl;
                cin >> item;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if ( item == 0) {
                Shop_PermanentItems_Menu();
            }
            wweapon=new WarmWeapon(WarmWeapon::shop_items_permanent_warmweapon.at(item-1));
            wweapon->buy(); // Buys a warm weapon
            cout <<Green << "\nOk, Now that you have bought a WarmWeapon, you can continue shopping and buy other Items that you want.\n" << Reset;
            cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            ShopMenu();
        
        case 2:
            system("cls");
            cout << Cyan <<"YOU GO TO TAKE A LOOK AT THE COLDWEAPONS:" << "(YOUR MONEY : " << player->getMoney() << "$)" << Reset << endl << endl;
            ColdWeapon::showItems(); // Shows cold weapons
            cout << "[00]. Back" << endl;
            cout << "\nWhich one do you want to buy?" << endl;
            cin >> item;
            while(item<0 || item>10){
                cout <<Yellow<< "YOU HAVE ENTERED THE WRONG NUMBER , ENTER A NUMBER BETWEEN 0 AND 10 ."<< Reset << endl << "\nWhich one do you want to buy?" << endl;
                cin >> item;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if ( item == 0) {
                Shop_PermanentItems_Menu();
            }
            cweapon=new ColdWeapon(ColdWeapon::shop_items_permanent_coldweapon.at(item-1));
            cweapon->buy(); // Buys a cold weapon
            cout <<Green<< "\nOk, Now that you have bought a ColdWeapon, you can continue shopping and buy other Items that you want.\n"<< Reset;
            cout << Pink <<"\nPress any key to continue shopping..."<< Reset;
            getch(); // Wait for a key press
            ShopMenu();
        
        default:
            cout <<Yellow<< "\nINVALID CHOICE! CHOOSE A NUMBER BETWEEN 1 AND 2." << endl<< Reset;
            cout << Pink <<"\nPress any key to continue..."<< Reset;
            getch();
            Shop_PermanentItems_Menu();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void createItem() {
    createWarmWeapons();
    createColdWeapons();
    createThrowableItems();
    createMedicines();
    createFoods();
}

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

void gameOver(){
    cout << Red;
    cout <<"  ____    _    __  __ _____    _____     _______ ____" << '\n' ; 
    cout <<" / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\ " <<'\n' ;
    cout <<"| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |"<< '\n';
    cout <<"| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <" << '\n' ;
    cout <<" \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\ " << '\n';
    cout << Reset;
    Sleep(5000);
}

// *----------------------------------------------------------------*

void endGame(){
    cout << Pink;
    cout <<" _____ _   _ ____     ____    _    __  __ _____ "<< '\n';
    cout <<"| ____| \\ | |  _ \\   / ___|  / \\  |  \\/  | ____|"<<'\n';
    cout <<"|  _| |  \\| | | | | | |  _  / _ \\ | |\\/| |  _|" <<'\n';  
    cout <<"| |___| |\\  | |_| | | |_| |/ ___ \\| |  | | |___"<<'\n'; 
    cout <<"|_____|_| \\_|____/   \\____/_/   \\_\\_|  |_|_____|"<<'\n';
    cout << Reset;
    Sleep(5000);
}

// *----------------------------------------------------------------*

void JaimeLannister(){

    cout <<" _  _____ _   _  ____ ____  _        _ __    _______ ____" << endl;  
    cout <<"| |/ /_ _| \\ | |/ ___/ ___|| |      / \\ \\  / / ____|  _  \\ " << endl; 
    cout <<"| ' / | ||  \\| | |  _\\___ \\| |     / _ \\  V /|  _| | |_) |" << endl;
    cout <<"| . \\ | || |\\  | |_| |___) | |___ / ___ \\| | | |___|  _ < " << endl;
    cout <<"|_|\\_\\___|_| \\_|\\____|____/|_____/_/   \\_\\_| |_____|_| \\_\\ " <<endl << endl << endl;

}

// *----------------------------------------------------------------*

void JonSnow(){
    cout <<" _  _____ _   _  ____   ___ _   _   _____ _   _ _____   _   _  ___  ____ _____ _   _" << endl; 
    cout <<"| |/ /_ _| \\ | |/ ___| |_ _| \\ | | |_   _| | | | ____| | \\ | |/ _ \\|  _ \\_   _| | | |"<< endl;
    cout <<"| ' / | ||  \\| | |  _   | ||  \\| |   | | | |_| |  _|   |  \\| | | | | |_) || | | |_| |"<< endl;
    cout <<"| . \\ | || |\\  | |_| |  | || |\\  |   | | |  _  | |___  | |\\  | |_| |  _ < | | |  _  |"<< endl;
    cout <<"|_|\\_\\___|_| \\_|\\____| |___|_| \\_|   |_| |_| |_|_____| |_| \\_|\\___/|_| \\_\\|_| |_| |_|"<< endl <<endl << endl;
}

// *----------------------------------------------------------------*

void Daenerys(){

    cout <<" __  __  ___ _____ _   _ _____ ____     ___  _____   ____  ____      _    ____  ___  _   _ ____" << endl;  
    cout <<"|  \\/  |/ _ \\_   _| | | | ____|  _ \\   / _ \\|  ___| |  _ \\|  _ \\    / \\  / ___|/ _ \\| \\ | / ___|"<< endl; 
    cout <<"| |\\/| | | | || | | |_| |  _| | |_) | | | | | |_    | | | | |_) |  / _ \\| |  _| | | |  \\| \\___ \\ "<<endl; 
    cout <<"| |  | | |_| || | |  _  | |___|  _ <  | |_| |  _|   | |_| |  _ <  / ___ \\ |_| | |_| | |\\  |___) |"<< endl;
    cout <<"|_|  |_|\\___/ |_| |_| |_|_____|_| \\_\\  \\___/|_|     |____/|_| \\_\\/_/   \\_\\____|\\___/|_| \\_|____/ " << endl << endl << endl;
}

// *----------------------------------------------------------------*

void Stannis(){

    cout <<" _____ _   _ _____   _   _ ___ ____ _   _ _____   ___ ____    ____    _    ____  _  __" << endl;
    cout <<"|_   _| | | | ____| | \\ | |_ _/ ___| | | |_   _| |_ _/ ___|  |  _ \\  / \\  |  _ \\| |/ / " << endl;
    cout <<"  | | | |_| |  _|   |  \\| || | |  _| |_| | | |    | |\\___ \\  | | | |/ _ \\ | |_) | ' /  "<< endl;
    cout <<"  | | |  _  | |___  | |\\  || | |_| |  _  | | |    | | ___  | | |_| / ___ \\|  _ <| . \\  " << endl;
    cout <<"  |_| |_| |_|_____| |_| \\_|___\\____|_| |_| |_|   |___|____/  |____/_/   \\_\\_| \\_\\_|\\_\\ " << endl << endl << endl;
}

// *----------------------------------------------------------------*

void Joffrey(){

    cout <<" ___      _    __  __      _      _  _____ _   _  ____" << endl; 
    cout <<"|_ _|    / \\  |  \\/  |    / \\    | |/ /_ _| \\ | |/ ___|"<< endl;
    cout <<" | |    / _ \\ | |\\/| |   / _ \\   | ' / | ||  \\| | |  _ "<< endl;
    cout <<" | |   / ___ \\| |  | |  / ___ \\  | . \\ | || |\\  | |_| |"<< endl;
    cout <<"|___| /_/   \\_\\_|  |_| /_/   \\_\\ |_|\\_\\___|_| \\_|\\____|"<< endl << endl << endl;
}

// *----------------------------------------------------------------*

void TheonGreyjoy(){

    cout<<"__        _______   ____   ___  _   _ _ _____   ____   _____        __" << endl;
    cout<<"\\ \\      / / ____| |  _ \\ / _ \\| \\ | ( )_   _| / ___| / _ \\ \\      / /" << endl;
    cout<<" \\ \\ /\\ / /|  _|   | | | | | | |  \\| |\\| | |   \\___ \\| | | \\ \\ /\\ / / "<< endl;
    cout<<"  \\ V  V / | |___  | |_| | |_| | |\\  |   | |    ___) | |_| |\\ V  V /  " << endl;
    cout<<"   \\_/\\_/  |_____| |____/ \\___/|_| \\_|   |_|   |____/ \\___/  \\_/\\_/   " << endl << endl << endl;
}

// *----------------------------------------------------------------*

void show_advZombie(){
    cout <<" _   _ ___ ____ _   _ _____   _  _____ _   _  ____" << endl; 
    cout <<"| \\ | |_ _/ ___| | | |_   _| | |/ /_ _| \\ | |/ ___|" << endl;
    cout <<"|  \\| || | |  _| |_| | | |   | ' / | ||  \\| | |  _" << endl;
    cout <<"| |\\  || | |_| |  _  | | |   | . \\ | || |\\  | |_| |" << endl;
    cout <<"|_| \\_|___\\____|_| |_| |_|   |_|\\_\\___|_| \\_|\\____|"<< endl <<endl << endl;
}

// *----------------------------------------------------------------*

void show_basicZombie(){

    cout <<"__        ___   _ ___ _____ _____  __        ___    _     _  _______ ____" << endl;  
    cout <<"\\ \\      / / | | |_ _|_   _| ____| \\ \\      / / \\  | |   | |/ / ____|  _ \\ " << endl; 
    cout <<" \\ \\ /\\ / /| |_| || |  | | |  _|    \\ \\ /\\ / / _ \\ | |   | ' /|  _| | |_) |" << endl;
    cout <<"  \\ V  V / |  _  || |  | | | |___    \\ V  V / ___ \\| |___| . \\| |___|  _ < " << endl;
    cout <<"   \\_/\\_/  |_| |_|___| |_| |_____|    \\_/\\_/_/   \\_\\_____|_|\\_\\_____|_| \\_\\ " << endl << endl << endl;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*