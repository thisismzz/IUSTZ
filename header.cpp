
#include<iostream>
#include<string>
#include <algorithm>
#include<vector>
#include "header.h"

Stamina ::Stamina(int val , int max) : value(val) , maximum(max) {}
int Stamina ::getValue() const {
    return value;
}
void Stamina ::setValue(int val) {
    value = val;
}
void Stamina ::decrease(int amount) {
    value -= amount;
    if (value < 0)
        value = 0;
}
void Stamina ::increase(int amount) {
    value += amount;
    if ( value > maximum)
        value = maximum;
}
int Stamina ::getMaximum() const {
    return maximum;
}