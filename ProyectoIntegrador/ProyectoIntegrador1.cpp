// La libreria para entrada y salida standard
#include <iostream>

// Incluimos el motor de la base de datos SQLite para guardar y recuperar registros
#include "sqlite3.h"
#include <string>

// Estos espacios de nombres me ayudan a no tener que escribir todo el nombre de cada una de las librerias que usamos
using namespace std; std::string;

// Creamos una función para manejar los datos de la base de datos por medio solo de SQL
string Prompt(string SQL) 
{
	char* error;
	sqlite3* db;

	int result = sqlite3_open("test.db", &db);
	sqlite3_exec(db, SQL.data(), NULL, 0, &error);
	sqlite3_close(db);	
	
	if (error == NULL) 
	{
		return "Ok.";
	}

	return error;

}


// Esta función inserta los datos del estudiante
string insertarEstudiante(string apellido_paterno, string apellido_materno, string nombre, int edad, string carrera, string correo, string teléfono)
{
	return Prompt("INSERT INTO estudiantes VALUES ('" + apellido_paterno + "', '" + apellido_materno + "', '" + nombre + "', " + to_string(edad) + ")");
}

// Esta función solicita los datos al usuario D
string CapturarEstudianteYLoInserta() 
{
	string apellido_paterno;
	string apellido_materno;
	string nombre;
	string carrera;
	string correo;
	string teléfono;
	int edad;

	cout << "Ingrese el nombre del estudiante: ";
	cin >> nombre;
	cout << "Ingrese el apellido paterno del estudiante: ";
	cin >> apellido_paterno;
	cout << "Ingrese el apellido materno del estudiante: ";
	cin >> apellido_materno;
	cout << "Ingrese la edad del estudiante: ";
	cin >> edad;
	cout << "Ingrese la carrera: ";
	cin >> carrera;
	cout << "Ingrese el correo electrónico: ";
	cin >> correo;
	cout << "Ingrese el teléfono: ";
	cin >> teléfono;

	string result = insertarEstudiante(apellido_paterno , apellido_materno, nombre, edad,);
	return result;
}



// Esto lo que se ejecuta primero en cualquier programa de C++
int main()
{
	// Si no mandamos a la consola el "hola mundo", puede ser que llamemos a la mala suerte :-) 
	//cout << "Hola Mundo!!!";

	char* error;
	sqlite3* db;

	string result = Prompt("CREATE TABLE IF NOT EXISTS estudiantes (apellido_paterno, apellido_materno, nombre, edad,");

	if (result != "Ok.") 
	{
		cout << result;
		return 0;
	}

	int opcion = 0;

	while (true)
	{
		system("cls");
		cout << "Seleccione la opcion deseada\n";
		cout << "  1.- Alta de estudiantes: \n";
		cout << "  2.- Baja de estudiantes: \n";
		cout << "  3.- Modificacion de estudiantes: \n";
		cout << "  4.- Busqueda de estudiantes: \n";
		cout << "  5.- Salir del sistema: \n";
		cin >> opcion;

		if (opcion == 1)
		{
			system("cls");
			result = CapturarEstudianteYLoInserta();
			
			if (result != "Ok.") 
			{
				cout << "Error: " + result;
				return 0;
			}

		}

		if( opcion == 5 )
		{
			return 0;
		}

		system("pause");
		opcion = 0;
		
	}
}