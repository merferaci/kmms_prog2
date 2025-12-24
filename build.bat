@echo off

::Вид сборки
set BUILD_TYPE=Ninja
::Суффикс в название каталога сборки
set BUILD_SUFFIX=ninja 

::Кодировка
chcp 65001

::Название для папки build
set BUILD_FOLDER=build_%BUILD_SUFFIX%
set SOURCE_FOLDER=projects

if not exist %BUILD_FOLDER% mkdir %BUILD_FOLDER%

cd %BUILD_FOLDER%

cmake -G %BUILD_TYPE% ..\%SOURCE_FOLDER%
cmake --build .

copy ..\%SOURCE_FOLDER%\251023_cpp_compilation\run_sort.bat .\sort 
copy ..\%SOURCE_FOLDER%\251016_refactoring\run_refactoring.bat .\refactoring
copy ..\%SOURCE_FOLDER%\251029_refactoring_2\run_refactoring2.bat .\refactoring2
copy ..\%SOURCE_FOLDER%\251125_menu\run_menu.bat .\menu
copy ..\%SOURCE_FOLDER%\251126_menu_2\run_menu2.bat .\menu2