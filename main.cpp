#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include <thread>
#include <chrono>
#include <map>
#include <conio.h>

#include "header.h"
#include "header.cpp"

using namespace std;

int main(){
    createWarmWeapons();
    createColdWeapons();
    createThrowableItems();
    createMedicines();
    createFoods();
    Menu();
    ShopMenu();

    cout << "Your backpack:" << endl;
    Backpack *bp=player->getBackpack();
    bp->showItems();
}