set(PROJECTS_LIST
    251016_refactoring
    251023_ccpp_compilation
    251029_refactoring_2
    251125_menu
    251126_menu_2
    260302_long_number
    260314_vector_list
)

foreach(PROJECT ${PROJECTS_LIST})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}/CMakeLists.txt)
        add_subdirectory(${PROJECT})
        message(STATUS "Added project: ${PROJECT}")
    else()
        message(WARNING "Project ${PROJECT} has no CMakeLists.txt, skipping")
    endif()
endforeach()