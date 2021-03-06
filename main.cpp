#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <deque>
#include <algorithm>
#include "Matrix.h"
#include "NewtonRaphson.h"

#define EPSILON 0.0001

void Portada();
void clearScreen();

void Menu();
void MenuInterpolacion();
void MenuDiferenciacionIntegracion();

bool OrdernarPorValor(const std::pair<float, float> &a, const std::pair<float, float> &b) {
	return (a.first < b.first);
}

Matrix Spline(std::vector<float>& anchuraH, std::vector<float>& fx);
void SetupTable();
void LeerTabla();
void ModificarPosicion(int position);
void ImprimirTabla();
void SortData();
bool EstaEnRango(float numInterpolar);
void InterpolacionDiferenciasDivididas(float numInterpolar, int grado);

void SetupTableDiferenciacionIntegracion();
void LeerTablaDiferenciacionIntegracion();
void ModificarPosicionDiferenciacionIntegracion(int position);
int posicionIntervalo(float intervalo);

void Newton();
void MetodosInterpolacion();
void MetodosDiferenciacionIntegracion();
void Diferencias();
void SplineCubico();
void Diferenciacion();
void Integracion();
float SimpsonOneThird(const std::vector<float> subPosition, float hStep);
float SimpsonThreeEighths(const std::vector<float> subPosition, float hStep);
std::vector<float> copyVector(int initialPosition, int finalPosition);
int Subintervals(int dataSize);
bool DataIsEquidistant();


std::vector<std::pair<float, float>> data;

int main() {
	int opc;
	char decision;

	Portada();
	getchar();
	clearScreen();
	std::cout << std::setprecision(6);
	do {
		clearScreen();
		Menu();
		std::cout<<"Selecciona una opcion: ";
		std::cin>>opc;

		switch (opc) {
		case 1:
			Newton();
			break;
		case 2:
			MetodosInterpolacion();
			break;
		case 3:
			MetodosDiferenciacionIntegracion();
			break;
		default:
			std::cout<<"Opcion incorrecta"<<std::endl;
		}
		std::cout<<"Te gustaria ver el menu principal? (s/n): ";
		std::cin>>decision;
	} while(decision == 's');
	return 0;
}

void Portada() {
	std::cout << "\n\n\t\tMetodos Numericos II \n"
	          << "\tPaquete de programas\n"
	          << "\tProfesora: Carrillo Ramirez Teresa\n"
	          << "\tIntegrantes:\n"
	          << "\t  Diaz Lopez Alan Fernando\n"
	          << "\t  Mejia Espinosa Ruben Alan\n"
	          << "\tGrupo: 2401" << std::endl;
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
	system("clear");
#else
#error "OS not supported."
#endif
}

void Menu() {
	std::cout<<"Metodos Numericos\n"
	         <<"1) Newton Raphson (Sistemas de ecuaciones no lineales)\n"
	         <<"2) Metodos de interpolacion\n"
	         <<"3) Metodos de integracion y diferenciacion"<<std::endl;
}

void Newton() {
	int opc;
	char respuesta;
	do {
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
		std::cout<<"Te gustaria resolver otro sistema de de ecuaciones? (s/n): ";
		std::cin>>respuesta;
	} while(respuesta == 's');
}

void MetodosInterpolacion() {
	int opc;
	char respuesta;
	do {
		clearScreen();
		MenuInterpolacion();
		std::cout<<"Selecciona una opcion: ";
		std::cin>>opc;

		switch(opc) {
		case 1:
			Diferencias();
			break;
		case 2:
			SplineCubico();
			break;
		default:
			std::cout<<"Opcion incorrecta"<<std::endl;
			break;
		}
		std::cout<<"Te gustaria regresar al menu de interpolacion? (s/n): ";
		std::cin>>respuesta;
	} while(respuesta == 's');

}

void Diferencias() {
	char opc;
	float numInterpolar;
	int grado, elementoPosicion;

	if(!data.empty()) {
		ImprimirTabla();
		std::cout<<"Te gustaria usar la ultima tabla ingresada? (s/n): ";
		std::cin>>opc;

		if(opc != 's') {
			data.clear();
			clearScreen();
			SetupTable();
		}
	}

	do {
		clearScreen();
		if(data.empty())
			SetupTable();
		do {
			clearScreen();
			SortData();
			ImprimirTabla();
			std::cout << "Ingresa el punto a interpolar: ";
			std::cin >> numInterpolar;
			if (EstaEnRango(numInterpolar)) {
				std::cout << "Ingresa grado del polinomio que se interpolara: ";
				std::cin >> grado;

				for (int i = 0; i < data.size() - 1; i++) {
					if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
						elementoPosicion = i;
				}

				if ((grado + 1) > data.size()) {
					std::cout << "Puntos insuficientes" << std::endl;
				} else {
					InterpolacionDiferenciasDivididas(numInterpolar, grado);
				}
			} else {
				std::cout << "El punto no esta en el rango de la tabla" << std::endl;
			}
			std::cout << "Deseas interpolar otro punto con la misma tabla? (s/n): ";
			std::cin >> opc;
		} while (opc == 's');
		//data.clear();
		std::cout << "Te gustaria cambiar la tabla? (s/n): ";
		std::cin >> opc;
		if(opc == 's') {
			clearScreen();
			data.clear();
			SetupTable();
		}
	} while (opc == 's');
}

void SplineCubico() {
	std::vector<float> anchuraH;
	std::vector<float> fx;
	char ajusteOpc;

	clearScreen();

	if(!data.empty()) {
		ImprimirTabla();
		std::cout<<"Te gustaria usar la ultima tabla ingresada? (s/n): ";
		std::cin>>ajusteOpc;

		if(ajusteOpc != 's') {
			data.clear();
			anchuraH.clear();
			fx.clear();

			clearScreen();
			SetupTable();
		}
	}

	do {

		if(data.empty()) {
			clearScreen();
			SetupTable();
		}
		SortData();

		Matrix matCoeficientes = Spline(anchuraH, fx);
		float **matCoeficientesPtr = matCoeficientes.GetMatrixPtr();

		for (int i = 0; i < data.size() - 1; i++) {
			std::cout << "g" << i << "(x)=" << std::showpos << matCoeficientesPtr[i][0] << "(x-xi)^3 " << matCoeficientesPtr[i][1] << "(x-xi)^2 " << matCoeficientesPtr[i][2] << "(x-xi) " << matCoeficientesPtr[i][3] << std::endl;
			std::cout << data[i].first << " <= x <= " << data[i + 1].first << "\n"<< std::endl;
			std::cout << std::noshowpos;
		}

//        data.clear();
//		anchuraH.clear();
//		fx.clear();
		std::cout << "¿Desea realizar otro ajuste con otra tabla? (s/n): ";
		std::cin >> ajusteOpc;

		if(ajusteOpc == 's') {
			clearScreen();
			data.clear();
			anchuraH.clear();
			fx.clear();
			SetupTable();
		}

	} while (ajusteOpc == 's');
}

void SetupTable() {
	char opc;

	LeerTabla();
	clearScreen();
	ImprimirTabla();
	std::cout << "Los datos de la tabla son correctos: (s/n): ";
	std::cin >> opc;

	if (opc == 's')
		return;

	while (opc != 's') {
		int position;
		std::cout << "Indica la posicion que se modificara: ";
		std::cin >> position;
		ModificarPosicion(position);
		ImprimirTabla();
		std::cout << "Los datos de la tabla son correctos: (s/n): ";
		std::cin >> opc;
	}
}

void LeerTabla() {
	int numPuntos;
	float x, fx;
	std::cout << "Ingresa el numero de puntos: ";
	std::cin >> numPuntos;

	for (int i = 0; i < numPuntos; ++i) {
		std::cout << "Ingresa el valor de x" << i << ": ";
		std::cin >> x;
		std::cout << "Ingresa el valor de f" << i << ": ";
		std::cin >> fx;
		data.push_back(std::make_pair(x, fx));
	}
}

void ModificarPosicion(int position) {
	float x, fx;
	std::cout << "Ingresa el valor de x" << position << ": ";
	std::cin >> x;
	std::cout << "Ingresa el valor de f" << position << ": ";
	std::cin >> fx;
	data.at(position) = std::make_pair(x, fx);
}

void ImprimirTabla() {
	std::cout << "i \tx \tf(x) " << std::endl;
	for (int i = 0; i < data.size(); i++)
		std::cout << i << "\t" << data[i].first << "    " << data[i].second << std::endl;
}

void SortData() {
	sort(data.begin(), data.end(), OrdernarPorValor);
}

bool EstaEnRango(float numInterpolar) {
	for (int i = 0; i < data.size() - 1; i++) {
		if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
			return true;
	}

	return false;
}

void InterpolacionDiferenciasDivididas(float numInterpolar, int gradoPolinomio) {
	std::vector<std::vector<float>> interpolacion(gradoPolinomio);
	std::vector<std::pair<float, float>> dataTable = data;
	int numElementos = gradoPolinomio;
	int grado = 1;

	for (int i = 0; i < gradoPolinomio; i++) {
		std::vector<std::pair<float, float>> dat;
		for (int j = 0; j < numElementos; j++) {
			float fx = dataTable[j + 1].second - dataTable[j].second;
			float x = data[j + grado].first - data[j].first;
			float result = fx / x;
			interpolacion[i].push_back(result);
			dat.push_back(std::make_pair(x, result));
		}
		grado++;
		numElementos--;
		dataTable = dat;
	}

	float valorInterpolacion = data[0].second;
	float mult = 1;
	for (int i = 0; i < gradoPolinomio; i++) {
		for (int j = 0; j <= i; j++) {
			mult *= (numInterpolar - data[j].first);
		}
		valorInterpolacion += interpolacion[i][0] * mult;
		mult = 1;
	}

	std::cout << "Valor de interpolacion para " << numInterpolar << " es: " << valorInterpolacion << std::endl;
}

Matrix Spline(std::vector<float>& anchuraH, std::vector<float>& fx) {
	for (int i = 0; i < data.size() - 1; i++) {
		float deltaH = data[i + 1].first - data[i].first;
		float func = (data[i + 1].second - data[i].second) / deltaH;
		anchuraH.push_back(deltaH);
		fx.push_back(func);
	}

	Matrix matA(data.size() - 2, data.size() - 2);
	float **matAPtr = matA.GetMatrixPtr();

	for (int i = 0; i < matA.GetRows(); ++i) {
		for (int j = 0; j < matA.GetColumns(); ++j) {
			if (j == i - 1)
				matAPtr[i][j] = anchuraH[i];
			else if (i == j)
				matAPtr[i][j] = 2 * (anchuraH[i] + anchuraH[i + 1]);
			else if (j == i + 1)
				matAPtr[i][j] = anchuraH[i + 1];
			else
				matAPtr[i][j] = 0;
		}
	}

	Matrix matB(data.size() - 2, 1);
	float **matBPtr = matB.GetMatrixPtr();

	for (int i = 0; i < matB.GetRows(); i++)
		matBPtr[i][0] = 6 * (fx[i + 1] - fx[i]);

	Matrix matInv = Matrix::Inverse(matA);
	Matrix matResult = Matrix::Mult(matInv, matB);
	float **matResultPtr = matResult.GetMatrixPtr();

	std::deque<float> sCoeficientes;
	for (int i = 0; i < matResult.GetRows(); i++) {
		sCoeficientes.push_back(matResultPtr[i][0]);
	}
	sCoeficientes.push_front(0);
	sCoeficientes.push_back(0);

	Matrix matCoeficientes(anchuraH.size(), 4);
	float **matCoeficientesPtr = matCoeficientes.GetMatrixPtr();

	for (int i = 0; i < matCoeficientes.GetRows(); i++) {
		for (int j = 0; j < matCoeficientes.GetColumns(); j++) {
			if (j == 0)
				matCoeficientesPtr[i][j] = (sCoeficientes[i + 1] - sCoeficientes[i]) / (6 * anchuraH[i]);
			else if (j == 1)
				matCoeficientesPtr[i][j] = sCoeficientes[i] / 2.0;
			else if (j == 2)
				matCoeficientesPtr[i][j] = ((data[i + 1].second - data[i].second) / anchuraH[i]) - anchuraH[i] * ((sCoeficientes[i + 1] + 2 * sCoeficientes[i]) / 6.0);
			else if (j == 3)
				matCoeficientesPtr[i][j] = data[i].second;
		}
	}
	return matCoeficientes;
}

void MenuInterpolacion() {
	std::cout<<"Metodos de interpolacion\n"
	         << "1) Interpolacion por diferencias divididas\n"
	         << "2) Ajuste de curvas (Spline cubico)\n";
}

void MenuDiferenciacionIntegracion() {
	std::cout<<"Metodos de diferenciacion e integracion\n"
	         << "1) Metodo de diferenciacion (Diferencias centradas)\n"
	         << "2) Metodo de integracion (Reglas de simpson)\n";
}

void MetodosDiferenciacionIntegracion() {
    char opc;
//	int opc;
//	clearScreen();
//	MenuDiferenciacionIntegracion();
//	std::cout<<"Selecciona una opcion: ";
//	std::cin>>opc;

//	switch(opc) {
//	case 1:
//		Diferenciacion();
//		break;
//	case 2:
//		Integracion();
//		break;
//	default:
//		std::cout<<"Opcion Incorrecta"<<std::endl;
//		break;
//	}
    do{
    clearScreen();
    std::cout<<"Metodo de diferenciacion (Diferencias centradas)"<<std::endl;
    Diferenciacion();
    std::cin.ignore();
	std::cout<<"Presiona enter para continuar con el metodo de integracion"<<std::endl;
	getchar();
    Integracion();
    std::cout<<"Deseas realizar otro calculo? (s/n): ";
    std::cin>>opc;
    }while(opc == 's');
}

void Diferenciacion() {
	char opc;
	float hStep;
	float intervaloInf, intervaloSup;
	std::vector<float> derivadas;

	clearScreen();

	if(!data.empty()) {
        if(DataIsEquidistant()){
		ImprimirTabla();
		std::cout<<"Te gustaria usar la ultima tabla ingresada? (s/n): ";
		std::cin>>opc;

		if(opc != 's') {
			data.clear();
			clearScreen();
			SetupTableDiferenciacionIntegracion();
		}
		}else{
            data.clear();
			clearScreen();
			std::cout<<"Datos de la ultima tabla no equidistantes"<<std::endl;
			SetupTableDiferenciacionIntegracion();
		}
	}

	// do statement
	clearScreen();
	std::cout<<"Metodo de diferenciacion (Diferencias centradas)"<<std::endl;
	if(data.empty()) {
		SetupTableDiferenciacionIntegracion();
	}

	clearScreen();
	std::cout<<"Metodo de diferenciacion (Diferencias centradas)"<<std::endl;
	ImprimirTabla();

	std::cout<<"Ingresa el valor del intervalo inferior: ";
	std::cin>>intervaloInf;
	std::cout<<"Ingresa el valor del intervalo superior: ";
	std::cin>>intervaloSup;
	//ImprimirTabla();
	int indexInf = posicionIntervalo(intervaloInf);
	int indexSup = posicionIntervalo(intervaloSup);

//	if((indexInf < 0 || indexSup < 0) || (intervaloInf > intervaloSup) || (abs(indexSup-indexInf) < 1)) {
//		std::cout<<"intervalo fuera de rango"<<std::endl;
//		return;
//	}

	while((indexInf < 0 || indexSup < 0) || (intervaloInf > intervaloSup) || (abs(indexSup-indexInf) < 1)) {
		std::cout<<"intervalo fuera de rango"<<std::endl;
		std::cout<<"Ingresa el valor del intervalo inferior: ";
		std::cin>>intervaloInf;
		std::cout<<"Ingresa el valor del intervalo superior: ";
		std::cin>>intervaloSup;

        indexInf = posicionIntervalo(intervaloInf);
        indexSup = posicionIntervalo(intervaloSup);
	}

	hStep = data[1].first - data[0].first;
	for(int i=indexInf+1; i<indexSup; i++) {
		float derivada = ((1/(2*hStep)) * (data[i+1].second - data[i-1].second));
		derivadas.push_back(derivada);
	}

	std::cout<<"x \t f'(x)"<<std::endl;
	for(int i=0; i<derivadas.size(); i++) {
		std::cout<<data[indexInf + i +1].first<<"\t "<<derivadas[i]<<std::endl;
	}
}

void Integracion() {
	char opc;
	float hStep;
	clearScreen();
	std::cout<<"Metodo de integracion (Reglas de Simpson)"<<std::endl;

	if(!data.empty()) {
		ImprimirTabla();
		std::cout<<"Te gustaria modificar la ultima tabla ingresada? (s/n): ";
		std::cin>>opc;

		while (opc == 's') {
		int position;
		std::cout << "Indica la posicion que se modificara: ";
		std::cin >> position;
		ModificarPosicionDiferenciacionIntegracion(position);
		ImprimirTabla();
		std::cout << "Te gustaria modificar otra posicion de la tabla?: (s/n): ";
		std::cin >> opc;
	}

//		if(opc != 's') {
//			data.clear();
//			clearScreen();
//			SetupTableDiferenciacionIntegracion();
//		}
	}

	// do statement
	clearScreen();
	std::cout<<"Metodo de integracion (Reglas de Simpson)"<<std::endl;
	if(data.empty()) {
		SetupTableDiferenciacionIntegracion();
	}

	clearScreen();
	std::cout<<"Metodo de integracion (Reglas de Simpson)"<<std::endl;
	ImprimirTabla();

	hStep = data[1].first - data[0].first;
	if((data.size() -1) % 2 == 0) {
        std::cout<<"Regla de Simpson 1/3"<<std::endl;
		float integral = 0.0;
		int intervals = Subintervals(data.size()-1);

		for(int i=0; i < intervals; i++) {
			int initalSubinterval = (i == 0) ? 0 : (i % 2 == 0) ? i+2 : i+1;
			std::vector<float> subPosition = copyVector(initalSubinterval, initalSubinterval+2);
			integral += SimpsonOneThird(subPosition, hStep);
		}

		std::cout<<"El valor de la integral es: "<<integral<<std::endl;

	} else if((data.size() - 1) % 2 != 0) {
		std::cout<<"simpson 1/3 + 3/8"<<std::endl;

		float integral = 0.0;
		int intervals = Subintervals(data.size()-1) - 2;

		for(int i=0; i < intervals; i++) {
			int initalSubinterval = (i == 0) ? 0 : (i % 2 == 0) ? i+2 : i+1;
			std::vector<float> subPosition = copyVector(initalSubinterval, initalSubinterval+2);
			integral += SimpsonOneThird(subPosition, hStep);
		}
		std::vector<float> subPosition = copyVector(data.size()-4, data.size()-1);
		integral += SimpsonThreeEighths(subPosition, hStep);

		std::cout<<"El valor de la integral es: "<<integral<<std::endl;
	}
}

void LeerTablaDiferenciacionIntegracion() {
	float fx, x0, hStep;
	int numDatos;

	std::cout<<"Ingresa el numero de datos: ";
	std::cin>>numDatos;
	std::cout<<"Ingresa el valor incial x0: ";
	std::cin>>x0;
	std::cout<<"Ingresa el valor de paso h: ";
	std::cin>>hStep;

	for (int i = 0; i < numDatos; ++i) {
		std::cout<<"valor para x"<<i<<": "<<x0<<std::endl;
		std::cout<<"Ingresa el valor para f"<<i<<": ";
		std::cin>>fx;
		data.push_back(std::make_pair(x0, fx));
		x0 += hStep;
	}
}

int posicionIntervalo(float intervalo) {
	for(int i=0; i<data.size(); i++)
		if(fabs(intervalo - data[i].first) <= EPSILON)
			return i;

	return -1;
}

float SimpsonOneThird(const std::vector<float> subPosition, float hStep) {
	float sum = 0;
	sum += subPosition.at(0) + subPosition.at(2);
	sum += 4 * subPosition.at(1);

	return (hStep/3.0) * (sum);
}

float SimpsonThreeEighths(const std::vector<float> subPosition, float hStep) {
    float sum = 0;
	sum += subPosition.at(0) + subPosition.at(3);
	sum += 3 * (subPosition.at(1) + subPosition.at(2));

	return ((hStep * 3.0)/8.0) * (sum);
}

int Subintervals(int dataSize) {
	int numIntervals = 0;
	int suma = 0;
	for(int i=0; suma < dataSize; i++) {
		suma += 2;
		numIntervals++;
	}

	return numIntervals;
}

std::vector<float> copyVector(int initialPosition,int finalPosition) {
	std::vector<float> subPosition;
	for(int i=initialPosition; i <= finalPosition; i++) {
		subPosition.push_back(data[i].second);
	}

	return subPosition;
}

void ModificarPosicionDiferenciacionIntegracion(int position){
    float fx;
	std::cout << "Ingresa el valor de f" << position << ": ";
	std::cin >> fx;
	data.at(position).second = fx;
}

void SetupTableDiferenciacionIntegracion(){
    char opc;

	LeerTablaDiferenciacionIntegracion();
	clearScreen();
	ImprimirTabla();
	std::cout << "Los datos de la tabla son correctos: (s/n): ";
	std::cin >> opc;

	if (opc == 's')
		return;

	while (opc != 's') {
		int position;
		std::cout << "Indica la posicion que se modificara: ";
		std::cin >> position;
		ModificarPosicionDiferenciacionIntegracion(position);
		ImprimirTabla();
		std::cout << "Los datos de la tabla son correctos: (s/n): ";
		std::cin >> opc;
	}
}

bool DataIsEquidistant(){
    float hStep = data[1].first - data[0].first;

    for(int i=0; i<data.size()-1; i++){
        if(fabs(fabs(data[i].first-data[i+1].first) - hStep) >= EPSILON)
            return false;
    }
    return true;
}
