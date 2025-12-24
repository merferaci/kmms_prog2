#include "menu_function.hpp"

#include <iostream>
#include <cstdlib>

const PY::MenuItem* PY::show_menu(const MenuItem* current){
    if (current->title) {
        std::cout << current->title << std::endl;
    }
	std::cout << "Меню" << std::endl;
    
    for (int i = 1; i < current->children_count; i++){
        std::cout << current->children[i]->title << std::endl;
    }
    std::cout << current->children[0]->title << std::endl;
    std::cout << "Меню > ";
    
    int user_input;
    std::cin >> user_input;
    std::cout << std::endl;
    
    return current->children[user_input];
}

const PY::MenuItem* PY::exit(const MenuItem* current){
    std::exit(0);
}

const PY::MenuItem* PY::university_spbgu(const MenuItem* current){
	return PY::show_menu(current);
}

const PY::MenuItem* PY::university_itmo(const MenuItem* current){
    std::cout << "ИТМО" << std::endl << std::endl;
    return current->parent;
}

const PY::MenuItem* PY::university_politech(const MenuItem* current){
    std::cout << "Политех" << std::endl << std::endl;
    return current->parent;
}

const PY::MenuItem* PY::university_leti(const MenuItem* current){
    std::cout << "Лэти" << std::endl << std::endl;
    return current->parent;
}

const PY::MenuItem* PY::university_go_back(const MenuItem* current){
    std::cout << "Возвращаемся в главное меню" << std::endl << std::endl;
    return current->parent->parent;
}

const PY::MenuItem* PY::spbgu_pm_py(const MenuItem* current){
    std::cout << "Факультет ПМ-ПУ" << std::endl << std::endl;
    return current->parent;
}

const PY::MenuItem* PY::spbgu_mat_mech(const MenuItem* current){
    std::cout << "Факультет МатМех" << std::endl << std::endl;
    return current->parent;
}

const PY::MenuItem* PY::spbgu_ling(const MenuItem* current){
    std::cout << "Лингвистический факультет" << std::endl << std::endl;
    return current->parent;
}

const PY::MenuItem* PY::spbgu_go_back(const MenuItem* current){
    std::cout << "Возвращаемся к выбору университета..." << std::endl << std::endl;
    return current->parent->parent;
}
