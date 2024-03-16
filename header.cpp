#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>


#include "header.h"

using namespace std;

// Function And Class Method Decleration

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Person::Person(string n) : name(n), level(1){}

void Person :: updateLevel(){
    level++;
}

int Person::getLevel(){
    return level;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Backpack* Human::getBackpack(){
    Backpack *p=&backpack;
    return p;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BankAccount* Player :: getBankAccount(){
    BankAccount* p = &bankAccount;
    return p;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Health :: Health() : maxHealth(100),currentHealth(100){}

void Health :: decreaseHealth(int damage){
    currentHealth -= damage;
    if(currentHealth <= 0)
        currentHealth = 0;
}

void Health :: increaseHealth (int amount){
    currentHealth += amount;
    if(currentHealth >= maxHealth)
        currentHealth = maxHealth;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Stamina :: Stamina():maximum(50),currentStamina(100){}

void Stamina :: decreaseStamina(int amount) {
    currentStamina -= amount;
    if (currentStamina < 0)
        currentStamina = 0;
}

void Stamina :: increaseStamina(int amount) {
    currentStamina += amount;
    if ( currentStamina > maximum)
        currentStamina = maximum;
}

void Stamina :: updateMaximumStamina(){
    maximum+=50;
}

int Stamina :: getCurrentStamina(){
    return currentStamina;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Experience :: Experience(Human *h):humanObj(h),maximum(100),currentExp(0){}

void Experience :: updateMaximum(){
    maximum+=100;
}

void Experience :: setCurrentExp(int selfDamage,int enemyDamage,int usedStamina){
    currentExp+=(0.5*selfDamage)+(0.2*enemyDamage)+(0.3*usedStamina);
    if(currentExp>=maximum){
        currentExp=0;
        updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
    }
}

void Experience :: increaseExp(int amount){
    currentExp+=amount;
    if(currentExp>=maximum){
        currentExp=0;
        humanObj->exp.updateMaximum();
        humanObj->updateLevel();
        humanObj->stamina.updateMaximumStamina();
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Skills::Skills(int n,int m):maximum(5),currentSkill(n),upgradePrice(m){}

void Skills::upgradeSkill(BankAccount *creditcard){
    if(currentSkill == 5){
        cout << "Maximum level!\n";
    }
    try{
        if(creditcard->getBalance()>=upgradePrice){
            creditcard->withdraw(upgradePrice);
            currentSkill++;
            setUpgradePrice();
        }
        else{
            throw 0;
        }
    }
    catch(int temp){
        cout << "not enough money!\n";
    } 
}

void Skills::setUpgradePrice(){
    upgradePrice+=10;
}

int Skills::getCurrentSkill(){
    return currentSkill;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

WarmWeaponAbility::WarmWeaponAbility(int n):Skills(n,n*10){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ColdWeaponAbility::ColdWeaponAbility(int n):Skills(n,n*15){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ThrowableWeaponAbility::ThrowableWeaponAbility(int n):Skills(n,n*20){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void Backpack :: addFoodItem(const Food& foodItem, int quantity){
    for (auto &item : FoodItems) {
        if (item.first == foodItem) {
            item.second += quantity;
            return;
        }
    }
    FoodItems.push_back(make_pair(foodItem, quantity));
}

void Backpack :: addMedicineItem(const Medicine& medicineItem, int quantity){
    for (auto &item : MedicineItems) {
        if (item.first == medicineItem) {
            item.second += quantity;
            return;
        }
    }
    MedicineItems.push_back(make_pair(medicineItem, quantity));
}

void Backpack :: addThrowableItem(const Throwable& throwableItem, int quantity){
    for (auto &item : ThrowableItems) {
        if (item.first == throwableItem) {
            item.second += quantity;
            return;
        }
    }
    ThrowableItems.push_back(make_pair(throwableItem, quantity));
}

void Backpack :: addWarmWeaponItem(const WarmWeapon& WarmWeaponItem){
    for (const auto &item : WarmWeaponItems) {
        if (item.first == WarmWeaponItem) {
            return;
        }
    }
    WarmWeaponItems.push_back(make_pair(WarmWeaponItem, 1));
}

void Backpack :: addColdWeaponItem(const ColdWeapon& ColdWeaponItem){
    for (const auto &item : ColdWeaponItems) {
        if (item.first == ColdWeaponItem) {
            return;
        }
    }
    ColdWeaponItems.push_back(make_pair(ColdWeaponItem, 1));
}

// *----------------------------------------------------------------*

void Backpack :: removeFoodItem(const Food& foodItem) {
    for (auto it = FoodItems.begin(); it != FoodItems.end(); ) {
        if (it->first == foodItem) {
            it = FoodItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeMedicineItem(const Medicine& medicineItem) {
    for (auto it = MedicineItems.begin(); it != MedicineItems.end(); ) {
        if (it->first == medicineItem) {
            it = MedicineItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeThrowableItem(const Throwable& throwableItem) {
    for (auto it = ThrowableItems.begin(); it != ThrowableItems.end(); ) {
        if (it->first == throwableItem) {
            it = ThrowableItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeWarmWeaponItem(const WarmWeapon& WarmWeaponItem) {
    for (auto it = WarmWeaponItems.begin(); it != WarmWeaponItems.end(); ) {
        if (it->first == WarmWeaponItem) {
            it = WarmWeaponItems.erase(it);
        } else {
            ++it;
        }
    }
}

void Backpack :: removeColdWeaponItem(const ColdWeapon& ColdWeaponItem) {
    for (auto it = ColdWeaponItems.begin(); it != ColdWeaponItems.end(); ) {
        if (it->first == ColdWeaponItem) {
            it = ColdWeaponItems.erase(it);
        } else {
            ++it;
        }
    }
}

// *----------------------------------------------------------------*

int Backpack :: getFoodItemsCount() {
    int total = 0;
    for (const auto& item : FoodItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getMedicineItemsCount() {
    int total = 0;
    for (const auto& item : MedicineItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getThrowableItemsCount() {
    int total = 0;
    for (const auto& item : ThrowableItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getWarmWeaponItemsCount() {
    int total = 0;
    for (const auto& item : WarmWeaponItems) {
        total += item.second;
    }
    return total;
}

int Backpack :: getColdWeaponItemsCount() {
    int total = 0;
    for (const auto& item : ColdWeaponItems) {
        total += item.second;
    }
    return total;
}

// *----------------------------------------------------------------*

int Backpack :: getSpecificFoodItemCount(const Food& specificItem) const {
    int count = 0;                                           
    for (const auto& item : FoodItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificMedicineItemCount(const Medicine& specificItem) const {
    int count = 0;
    for (const auto& item : MedicineItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificThrowableItemCount(const Throwable& specificItem) const {
    int count = 0;
    for (const auto& item : ThrowableItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificWarmWeaponItemCount(const WarmWeapon& specificItem) const {
    int count = 0;
    for (const auto& item : WarmWeaponItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

int Backpack :: getSpecificColdWeaponItemCount(const ColdWeapon& specificItem) const {
    int count = 0;
    for (const auto& item : ColdWeaponItems) {
        if (item.first == specificItem) {
            count += item.second;
        }
    }
    return count;
}

// *----------------------------------------------------------------*

void Backpack :: clearFoodItems() {
    FoodItems.clear();
}

void Backpack :: clearMedicineItems() {
    MedicineItems.clear();
}

void Backpack :: clearThrowableItems() {
    ThrowableItems.clear();
}

void Backpack :: clearWarmWeaponItems() {
    WarmWeaponItems.clear();
}

void Backpack :: clearColdWeaponItems() {
    ColdWeaponItems.clear();
}

// *----------------------------------------------------------------*

void Backpack :: useFoodItemCount(const Food& specificItem, int quantity) {
    for (auto& item : FoodItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

void Backpack :: useMedicineItemCount(const Medicine& specificItem, int quantity) {
    for (auto& item : MedicineItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

void Backpack :: useThrowableItemCount(const Throwable& specificItem, int quantity) {
    for (auto& item : ThrowableItems) {
        if (item.first == specificItem) {
            item.second -= quantity;
            if (item.second < 0) item.second = 0;
            break;
        }
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

BankAccount :: BankAccount(int n){
    this->balance = n;
}
int BankAccount :: getBalance() {
    return balance;
}
void BankAccount :: deposit(int amount) { 
    this->balance += amount;
}
bool BankAccount::withdraw(int amount) {
    if (balance >= amount){
        balance -= amount;
        return true;
    }    
    else
        return false;
        
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Items::Items(string n,int p,string t):name(n),price(p),type(t){}

bool Items::operator==(const Items& other) const {
    return (name == other.name);
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Permanent::Permanent(string n,int p,string t,int e,int h):Items(n,p,t),exp(e),harm(h){}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

WarmWeapon::WarmWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Warm Weapon",e,h),wwa(x){
    WarmWeapon::addToVectors();
}

void WarmWeapon::showItems(){
    for(auto& i : shop_items_permanent_warmweapon){
        cout << i << endl;
    }
}

void WarmWeapon::buy(Player& player){
    BankAccount *creditcard=player.getBankAccount();

    if(creditcard->withdraw(price)){
        cout << "Item bought successfully!\n";
        add_to_backpack;
        cout<<"Item added to your backpack!\n";
        player.exp.increaseExp(exp);
        cout<<"EXP increased!\n";
    }
    else
        cout<<"Not enough money!\n";
}

void WarmWeapon::addToVectors(){
    shop_items_permanent_warmweapon.push_back(this);
    shop_items_permanent.push_back(this);
    shop_items.push_back(this);
}

void WarmWeapon::Attack(Human attacker, Human attacked){
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) + (wwa.getCurrentSkill() * weaponLevelFactor) + (attacker.getLevel() * playerLevelFactor);
    attacker.stamina.decreaseStamina(10*wwa.getCurrentSkill());
    attacked.hp.decreaseHealth(damage);

    if(static_cast<Player*>(&attacker)){
        Player *p=static_cast<Player*>(&attacker);
        p->exp.setCurrentExp(0,damage,10*wwa.getCurrentSkill());
    }
    else{
        Player *p=static_cast<Player*>(&attacked);
        p->exp.setCurrentExp(damage,0,0);
    }
}

ostream& operator<<(ostream& os,WarmWeapon& obj){
    os<<"name : "<<obj.name<<"\ttype : "<<obj.type<<"\tlvl "<<obj.wwa.getCurrentSkill()<<"\t(+"<<obj.exp<<"EXP)\tprice : "<<obj.price<<"$";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

ColdWeapon::ColdWeapon(string n,int p,int x,int e,int h):Permanent(n,p,"Cold Weapon",e,h),cwa(x){
    ColdWeapon::addToVectors();
}

void ColdWeapon::showItems(){
    for (auto& i : shop_items_permanent_coldweapon){
        cout << i << endl;
    }
}

void ColdWeapon::buy(Player& player){
    BankAccount *creditcard=player.getBankAccount();

    if(creditcard->withdraw(price)){
        cout << "Item bought successfully!\n";
        add_to_backpack;
        cout<<"Item added to your backpack!\n";
        player.exp.increaseExp(exp);
        cout<<"EXP increased!\n";
    }
    else
        cout<<"Not enough money!\n";
}

void ColdWeapon::addToVectors(){
    shop_items_permanent_coldweapon.push_back(this);
    shop_items_permanent.push_back(this);
    shop_items.push_back(this);
}

void ColdWeapon::Attack(Human attacker, Human attacked){
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) + (cwa.getCurrentSkill() * weaponLevelFactor) + (attacker.getLevel() * playerLevelFactor);
    attacker.stamina.decreaseStamina(10*cwa.getCurrentSkill());
    attacked.hp.decreaseHealth(damage);

    if(static_cast<Player*>(&attacker)){
        Player *p=static_cast<Player*>(&attacker);
        p->exp.setCurrentExp(0,damage,10*cwa.getCurrentSkill());
    }
    else{
        Player *p=static_cast<Player*>(&attacked);
        p->exp.setCurrentExp(damage,0,0);
    }
}

ostream& operator<<(ostream& os,ColdWeapon& obj){
    os<<"name : "<<obj.name<<"\ttype : "<<obj.type<<"\tlvl "<<obj.cwa.getCurrentSkill()<<"(+"<<obj.exp<<"EXP)\tprice : "<<obj.price<<"$";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Throwable::Throwable(string n,int p,int x,int e,int h):Items(n,p,"Throwable Weapon"),twa(x),exp(e),harm(h){
    Throwable::addToVectors();
}

void Throwable::showItems(){
    for (auto& i : shop_items_throwable){
        cout << i << endl;
    }
}

void Throwable::buy(Player& player,int quantity){
    BankAccount *creditcard=player.getBankAccount();

    if(creditcard->withdraw(price*quantity)){
        cout << "Item bought successfully!\n";
        add_to_backpack;
        cout<<"Item added to your backpack!\n";
        player.exp.increaseExp(exp*quantity);
        cout<<"EXP increased!\n";
    }
    else
        cout<<"Not enough money!\n";
}

void Throwable::addToVectors(){
    shop_items_throwable.push_back(this);
    shop_items.push_back(this);
}

void Throwable::Throw(Human attacker, Human attacked){
    double staminaFactor = 0.3;
    double weaponLevelFactor = 1.0;
    double playerLevelFactor = 0.8;

    int damage = harm + (attacker.stamina.getCurrentStamina() * staminaFactor) + (twa.getCurrentSkill() * weaponLevelFactor) + (attacker.getLevel() * playerLevelFactor);
    attacker.stamina.decreaseStamina(10*twa.getCurrentSkill());
    attacked.hp.decreaseHealth(damage);

    if(static_cast<Player*>(&attacker)){
        Player *p=static_cast<Player*>(&attacker);
        p->exp.setCurrentExp(0,damage,10*twa.getCurrentSkill());
    }
    else{
        Player *p=static_cast<Player*>(&attacked);
        p->exp.setCurrentExp(damage,0,0);
    }

    decrease_a_throwable_from_backpack;
}

ostream& operator<<(ostream& os,Throwable& obj) {
    os<<"name : "<<obj.name<<"\ttype : "<<obj.type<<"\tlvl "<<obj.twa.getCurrentSkill()<<"(+"<<obj.exp<<"EXP)\tprice : "<<obj.price<<"$ (each)";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Medicine::Medicine(string n,int p,int h):Items(n,p,"Medicine"),heal(h){
    Medicine::addToVectors();
}

void Medicine::showItems(){
    for (auto& i : shop_items_medicine){
        cout << i << endl;
    }
}

void Medicine::buy(Player& player,int quantity){
    BankAccount *creditcard=player.getBankAccount();

    if(creditcard->withdraw(price*quantity)){
        cout << "Item bought successfully!\n";
        add_to_backpack;
        cout<<"Item added to your backpack!\n";
    }
    else
        cout<<"Not enough money!\n";
}

void Medicine::addToVectors(){
    shop_items_medicine.push_back(this);
    shop_items.push_back(this);
}

void Medicine::use(Player& player){
    player.hp.increaseHealth(heal);
    decrease_a_medicine_from_backpack;
}

ostream& operator<<(ostream& os,Medicine& obj) {
    os<<"name : "<<obj.name<<"\ttype : "<<obj.type<<"\theal : +"<<obj.heal<<"HP"<<"\tprice : "<<obj.price<<"$ (each)";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

Food::Food(string n,int p,int s):Items(n,p,"Food"),strength(s){
    Food::addToVectors();
}

void Food::showItems(){
    for (auto& i : shop_items_food){
        cout << i << endl;
    }
}

void Food::buy(Player& player,int quantity){
    BankAccount *creditcard=player.getBankAccount();

    if(creditcard->withdraw(price*quantity)){
        cout << "Item bought successfully!\n";
        add_to_backpack;
        cout<<"Item added to your backpack!\n";
    }
    else
        cout<<"Not enough money!\n";
}

void Food::addToVectors(){
    shop_items_food.push_back(this);
    shop_items.push_back(this);
}

void Food::use(Player& player){
    player.stamina.increaseStamina(strength);
    decrease_a_food_from_backpack;
}

ostream& operator<<(ostream& os,Food& obj) {
    os<<"name : "<<obj.name<<"\ttype : "<<obj.type<<"\theal : +"<<obj.strength<<"STM"<<"\tprice : "<<obj.price<<"$ (each)";
    return os;
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void printWithDelay(){
    int delay_ms = 20;
    string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tincidunt tristique maximus. Sed eget massa nec sem accumsan sagittis in a nisl. Cras pellentesque, est in feugiat consequat, orci magna tempus lorem, id pretium nibh nisl in tellus.";
    for (char c : text) {
        cout << c;
        cout.flush();  // Ensure the character is immediately printed
        this_thread :: sleep_for(chrono::milliseconds(delay_ms));
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void ShopMenu(){
    while(true){
        system("cls");
        int number;
        cout << "You enter the shop." << endl << "What do you want to buy?" << endl 
        << "[1].Permanent Items" << endl << "[2].Throwable Items" << endl << "[3].Consumable Items" << endl << "[4].Exit" << endl;
        cin >> number;
        switch(number){
            case 1: Show_Permanent_Items();
                break;
        
            case 2: Show_Throwable_Items();
                break;
        
            case 3: Show_Consumable_Items();
                break;
        
            case 4: 
            cout << "You left the shop." << endl;
                return;

            default: 
            cout << "Wrong number!" << endl << "Press enter to continue";
            getch();
                break;
        }
    }
}

void Show_Permanent_Items(){
    system("cls");
    int number;
    cout << "[1].WarmWeapons" << endl << "[2].ColdWeapons" << endl << "[3].back" << endl;
    switch(number){
        case 1: WarmWeapon::showItems();
            break;
        
        case 2: ColdWeapon::showItems();
            break;
        
        case 3: 
            return;
    }
}

void Show_Throwable_Items(){
    system("cls");
    Throwable::showItems();
}

void Show_Consumable_Items(){
    system("cls");
    int number;
    cout << "[1].Medicines" << endl << "[2].Foods" << endl << "[3].back" << endl;
    switch(number){
        case 1: Medicine::showItems();
            break;
        
        case 2: Food::showItems();
            break;
        
        case 3: 
            return;
    }
}

// *----------------------------------------------------------------*
// *----------------------------------------------------------------*
// *----------------------------------------------------------------*

void createWarmWeapons() {
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
    Throwable bomb("Firebomb", 2500, 2, 16, 15);
    Throwable grenade("Ice Grenade", 2200, 3, 12, 17);
    Throwable dart("Poison Dart", 1600, 1, 18, 20);
    Throwable rock("Rock", 1300, 3, 14, 23);
    Throwable knife("Throwing Knife", 1000, 2, 30, 10);
}

void createMedicines() {
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