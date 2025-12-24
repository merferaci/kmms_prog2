#pragma once

#include "menu.hpp"

namespace PY{
    const MenuItem* show_menu(const MenuItem* current);
    
    const MenuItem* exit(const MenuItem* current);
    
    const MenuItem* study_algebra(const MenuItem* current);
    const MenuItem* study_mat_analusis(const MenuItem* current);
    const MenuItem* study_go_back(const MenuItem* current);
    
    const MenuItem* algebra_summ(const MenuItem* current);
    const MenuItem* algebra_substract(const MenuItem* current);
    const MenuItem* algebra_multiply(const MenuItem* current);
    const MenuItem* algebra_divide(const MenuItem* current);
    const MenuItem* algebra_go_back(const MenuItem* current);
    
    const MenuItem* mat_analusis_dif(const MenuItem* current);
    const MenuItem* mat_analusis_int(const MenuItem* current);
    const MenuItem* mat_analysis_go_back(const MenuItem* current);
}