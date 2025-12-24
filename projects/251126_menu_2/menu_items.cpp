#include "menu_function.hpp"
#include "menu_items.hpp"

#include <cstddef>

const PY::MenuItem PY::SPBGU_PM_PY = { 
    "1 - Факультет ПМ-ПУ", PY::spbgu_pm_py, &PY::UNIVERSITY_SPBGU
};

const PY::MenuItem PY::SPBGU_MAT_MECH = { 
    "2 - Факультет МатМех", PY::spbgu_mat_mech, &PY::UNIVERSITY_SPBGU
};

const PY::MenuItem PY::SPBGU_LING = { 
    "3 - Лингвистический факультет", PY::spbgu_ling, &PY::UNIVERSITY_SPBGU
};

const PY::MenuItem PY::SPBGU_GO_BACK = { 
    "0 - Выйти в предыдущее меню ", PY::spbgu_go_back, &PY::UNIVERSITY_SPBGU
};

namespace {
    const PY::MenuItem* const spbgu_children[] = {
		&PY::SPBGU_GO_BACK,
		&PY::SPBGU_PM_PY,
		&PY::SPBGU_MAT_MECH,
		&PY::SPBGU_LING
    };
    const int spbgu_size = sizeof(spbgu_children) / sizeof(spbgu_children[0]);
}

const PY::MenuItem PY::UNIVERSITY_SPBGU = { 
    "1 - СПБГУ", PY::university_spbgu, &PY::UNIVERSITY, spbgu_children, spbgu_size
};

const PY::MenuItem PY::UNIVERSITY_ITMO = { 
    "2 - ИТМО", PY::university_itmo, &PY::UNIVERSITY
};

const PY::MenuItem PY::UNIVERSITY_POLITECH = { 
    "3 - Политех", PY::university_politech, &PY::UNIVERSITY
};

const PY::MenuItem PY::UNIVERSITY_LETI = { 
    "4 - Лэти", PY::university_leti, &PY::UNIVERSITY
};

const PY::MenuItem PY::UNIVERSITY_GO_BACK = { 
    "0 - Выйти в главное меню ", PY::university_go_back, &PY::UNIVERSITY
};

namespace {
    const PY::MenuItem* const university_children[] = {
		&PY::UNIVERSITY_GO_BACK,
        &PY::UNIVERSITY_SPBGU,
        &PY::UNIVERSITY_ITMO,
        &PY::UNIVERSITY_POLITECH,
        &PY::UNIVERSITY_LETI
    };
    const int university_size = sizeof(university_children) / sizeof(university_children[0]);
}

const PY::MenuItem PY::UNIVERSITY = { 
    "1 - Посмотреть университеты Санкт-Петербурга", PY::show_menu, &PY::MAIN, university_children, university_size
};

const PY::MenuItem PY::EXIT = { 
    "0 - Я уже студент", PY::exit, &PY::MAIN
};

namespace {
    const PY::MenuItem* const main_children[] = {
		&PY::EXIT,
        &PY::UNIVERSITY
    };
    const int main_size = sizeof(main_children) / sizeof(main_children[0]);
}

const PY::MenuItem PY::MAIN = { 
    nullptr, PY::show_menu, nullptr, main_children, main_size 
};