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