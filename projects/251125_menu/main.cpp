#include <clocale>
#include <iostream>

#include "menu.hpp"
#include "menu_functions.hpp"
#include "menu_items.hpp"

int main(){
    std::setlocale(LC_ALL, ""); 
   
    const PY::MenuItem* current = &PY::MAIN;
    do{
        current = current->func(current);
    }while (true);
   
    return 0;
}