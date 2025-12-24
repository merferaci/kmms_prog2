#include "menu_items.hpp"

#include <cstddef>

#include "menu_functions.hpp"

const PY::MenuItem PY::ALGEBRA_SUMM = { 
    "1 - Хочу научиться складывать!", PY::algebra_summ, &PY::STUDY_ALGEBRA
};

const PY::MenuItem PY::ALGEBRA_SUBSTRACT = { 
    "2 - Хочу научиться вычитать!", PY::algebra_substract, &PY::STUDY_ALGEBRA
};

const PY::MenuItem PY::ALGEBRA_MULTIPLY = { 
    "3 - Хочу научиться умножать!", PY::algebra_multiply, &PY::STUDY_ALGEBRA
};

const PY::MenuItem PY::ALGEBRA_DIVIDE = { 
    "4 - Хочу научиться делить!", PY::algebra_divide, &PY::STUDY_ALGEBRA
};

const PY::MenuItem PY::ALGEBRA_GO_BACK = { 
    "0 - Вернуться к выбору предмета.", PY::algebra_go_back, &PY::STUDY_ALGEBRA
};

const PY::MenuItem PY::MAT_ANALUSIS_DIF = { 
    "1 - Хочу изучить дифференциальное исчисление!", PY::mat_analusis_dif, &PY::STUDY_MAT_ANALUSIS
};

const PY::MenuItem PY::MAT_ANALUSIS_INT = { 
    "2 - Хочу изучить интегральное исчисление!", PY::mat_analusis_int, &PY::STUDY_MAT_ANALUSIS
};

const PY::MenuItem PY::MAT_ANALYSIS_GO_BACK = { 
    "0 - Вернуться к выбору предмета.", PY::mat_analysis_go_back, &PY::STUDY_MAT_ANALUSIS
};

namespace {
    const PY::MenuItem* const algebra_children[] = {
        &PY::ALGEBRA_GO_BACK,
        &PY::ALGEBRA_SUMM,
        &PY::ALGEBRA_SUBSTRACT,
        &PY::ALGEBRA_MULTIPLY,
        &PY::ALGEBRA_DIVIDE
    };
    const int algebra_size = sizeof(algebra_children) / sizeof(algebra_children[0]);
}

const PY::MenuItem PY::STUDY_ALGEBRA = { 
    "1 - Хочу изучать алгебру!", PY::show_menu, &PY::STUDY, algebra_children, algebra_size
};

namespace {
    const PY::MenuItem* const mat_analusis_children[] = {
        &PY::MAT_ANALYSIS_GO_BACK,
        &PY::MAT_ANALUSIS_DIF,
        &PY::MAT_ANALUSIS_INT
    };
    const int mat_analusis_size = sizeof(mat_analusis_children) / sizeof(mat_analusis_children[0]);
}

const PY::MenuItem PY::STUDY_MAT_ANALUSIS = { 
    "2 - Хочу изучать математический анализ!", PY::show_menu, &PY::STUDY, mat_analusis_children, mat_analusis_size
};

const PY::MenuItem PY::STUDY_GO_BACK = { 
    "0 - Выйти в главное меню.", PY::study_go_back, &PY::STUDY
};

namespace {
    const PY::MenuItem* const study_children[] = {
        &PY::STUDY_GO_BACK,
        &PY::STUDY_ALGEBRA,
        &PY::STUDY_MAT_ANALUSIS
    };
    const int study_size = sizeof(study_children) / sizeof(study_children[0]);
}

const PY::MenuItem PY::STUDY = { 
    "1 - Хочу учиться математике!", PY::show_menu, &PY::MAIN, study_children, study_size
};

const PY::MenuItem PY::EXIT = { 
    "0 - Я лучше пойду полежу ...", PY::exit, &PY::MAIN
};

namespace {
    const PY::MenuItem* const main_children[] = {
        &PY::EXIT, 
        &PY::STUDY
    };
    const int main_size = sizeof(main_children) / sizeof(main_children[0]);
}

const PY::MenuItem PY::MAIN = { 
    nullptr, PY::show_menu, nullptr, main_children, main_size 
};