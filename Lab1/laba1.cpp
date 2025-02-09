#include<iostream>
#include<cmath>


void get_coefficients(double &a, double &b, double &c)
{
	std::cout << "Введите коэффициенты a, b и с: ";
	std::cin >> a >> b >> c;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	double a, b, c, root1, root2;
	bool HasRealRoots = true;

	get_coefficients(a, b, c);

	return 0;
}
