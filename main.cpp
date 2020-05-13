#include <iostream>
#include <cmath>
#include "Matrix.h"
#include "NewtonRaphson.h"

void Portada();
void clearScreen();

void Menu();
void Newton();

int main() {
	int opc;
	char decision;

	Portada();
	getchar();
	clearScreen();

	do {
        Menu();
		std::cout<<"Selecciona una opcion: ";
		std::cin>>opc;

		switch (opc) {
		case 1:
            Newton();
			break;
		case 2:
			// other methods
			break;
		default:
			std::cout<<"Opcion incorrecta"<<std::endl;
		}
		std::cout<<"¿Te gustaria resolver otro sistema? (y/n)";
		std::cin>>decision;
	} while(decision == 'y');
	return 0;
}

void Portada(){
    std::cout<<"Metodos Numericos II"<<std::endl;
    std::cout<<"Paquete de programas"<<std::endl;
    std::cout<<"Profesora: Carrillo Ramirez Teresa"<<std::endl;
    std::cout<<"Integrantes:"<<std::endl;
    std::cout<<"Diaz Lopez Alan Fernando"<<std::endl;
    std::cout<<"Mejia Espinosa Ruben Alan"<<std::endl;
    std::cout<<"Grupo: 2401\n"<<std::endl;
}

void clearScreen(){
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
#else
#error "OS not supported."
#endif
}

void Menu(){
    std::cout<<"Metodos Numericos\n"
        <<"1) Newton Raphson (Sistemas de ecuaciones no lineales)\n"
        <<"2) Metodos de interpolacion\n"
        <<"3) Metodos de integracion y diferenciacion"<<std::endl;
}

void Newton() {
    int opc;
	clearScreen();
	NewtonRaphson::Menu();
	std::cout<<"Selecciona una opcion: ";
	std::cin>>opc;

	switch(opc) {
	case 1:
		NewtonRaphson::SistemaEcuaciones1();
		break;
	case 2:
		NewtonRaphson::SistemaEcuaciones2();
		break;
	case 3:
		NewtonRaphson::SistemaEcuaciones3();
		break;
	case 4:
		NewtonRaphson::SistemaEcuaciones4();
		break;
	default:
		std::cout<<"Opcion incorrecta"<<std::endl;
		break;
	}
}
