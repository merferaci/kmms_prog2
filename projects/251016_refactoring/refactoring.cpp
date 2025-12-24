#include <iostream>
#include <cmath>

double f1(double x);
double f2(double x);

int main(){
	double x;
	std::cout << "Введите угол" << std::endl;
	std::cin >> x;
	
	if (1 - std::cos(2*x) != 0){
		std::cout << "F(x1) = " << f1(x) << std::endl;
	}
	else{
		std::cout << "Неправильный ввод для f(1)" << std::endl;
	}
	
	if (1 - std::tan(x) != 0){
		std::cout << "F(X2) = " << f2(x) << std::endl;
	}
	else{
		std::cout << "Неправильный ввод для F(x2)" << std::endl;
	}
	
	return 0;
}

double f1(double x){
    double x1 = 1 + std::sin(2*x), x2 = 1 - std::cos(2*x);
    double rez = x1 / x2;
    return rez;
}

double f2(double x){
    double x1 = 1 + std::tan(x)*std::tan(x), x2 = 1 - std::tan(x)*std::tan(x);
    double rez = x1 / x2;
    return rez;
}