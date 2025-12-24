#include <clocale>
#include <iostream>

#include "menu.hpp"
#include "menu_function.hpp"
#include "menu_items.hpp"

int main(){   
    const PY::MenuItem* current = &PY::MAIN;
    do{
        current = current->func(current);
    }while (true);
   
    return 0;
}