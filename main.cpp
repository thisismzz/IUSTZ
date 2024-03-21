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
}