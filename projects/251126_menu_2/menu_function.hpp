#pragma once

#include "menu.hpp"

namespace PY{
    const MenuItem* show_menu(const MenuItem* current);
    
    const MenuItem* exit(const MenuItem* current);
    
    const MenuItem* university_spbgu(const MenuItem* current);
    const MenuItem* university_itmo(const MenuItem* current);
    const MenuItem* university_politech(const MenuItem* current);
    const MenuItem* university_leti(const MenuItem* current);
    const MenuItem* university_go_back(const MenuItem* current);
    
    const MenuItem* spbgu_pm_py(const MenuItem* current);
    const MenuItem* spbgu_mat_mech(const MenuItem* current);
    const MenuItem* spbgu_ling(const MenuItem* current);
    const MenuItem* spbgu_go_back(const MenuItem* current);
}
