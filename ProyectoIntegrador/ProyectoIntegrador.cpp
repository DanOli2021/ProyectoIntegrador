// La libreria para entrada y salida standard
#include <iostream>

// Incluimos el motor de la base de datos SQLite para guardar y recuperar registros
#include "sqlite3.h"
#include <string>
#include "nlohmann/json.hpp"
#include <windows.h>
#include <fstream>

#pragma execution_character_set( "utf-8" )

// Estos espacios de nombres me ayudan a no tener que escribir todo el nombre de cada una de las librerias que usamos
using namespace std; std::string;


//Write string to file
void WriteFile(string fileName, string text)
{
	ofstream myfile;
	myfile.open(fileName);
	myfile << text << endl;
	myfile.close();
}

// Capture the user input and return it as a string
string getUserInput()
{
	string userInput;
	getline(cin >> ws, userInput);
	return userInput;
}


//Convert string to upper case
string toUpper(string str)
{
	for (auto& c : str) c = toupper(c);
	return str;
}


// Convert char* to string
string toString(char* str)
{
	string s(str);
	return s;
}


// dbResult to json string
string dbResultToJson(char** dbResult, int nrows, int ncols)
{

	if (nrows == 0) 
	{
		return "[]";
	}
	
	string json = "[\n";
	int j = 0;
	
	for (int n = 0; n < nrows; n++)
	{
		json += " {\n";

		for (int i = 0; i < ncols; i++)
		{
			json += "  \"" + toString(dbResult[i]) + "\":\"" + toString(dbResult[(ncols * n) + i + ncols]) + "\"";

			if (i < ncols - 1)
				json += ",\n";
			else
				json += "\n";
		}

		if( n < nrows - 1)
			json += " },\n";
		else
			json += " }\n";
	}
	
	json += "]\n";
	return json;
}

// Creamos una función para manejar los datos de la base de datos por medio solo de SQL
// Solo permite ejecutar sentencias INSERT, UPDATE, DELETE
string SQliteExecute(string SQL) 
{
	char* error;
	sqlite3* db;
	string string_sql_result = "";
	int nrows;
	int ncols;
	int query_result;
	char** dbResult;

	int result = sqlite3_open("test.db", &db);
	sqlite3_exec(db, SQL.data(), NULL, 0, &error);
	sqlite3_close(db);	
	
	if (error == NULL) 
	{
		return "Ok.";
	}

	return error;

}



// Creamos una función para manejar los datos de la base de datos por medio solo de SQL
// Permite ejecutar sentencias SELECT
// Regresa un string en formato json con los datos de la base de datos
string SQliteQuery(string SQL)
{
	char* error;
	sqlite3* db;
	string string_sql_result = "";
	int nrows;
	int ncols;
	int query_result;
	char** dbResult;

	int result = sqlite3_open("test.db", &db);
	query_result = sqlite3_get_table(db, SQL.c_str(), &dbResult, &nrows, &ncols, &error);
    sqlite3_close(db);

	
	if (error != NULL)
	{
		return error;
	}

	return dbResultToJson(dbResult, nrows, ncols);
}



// Esta función inserta los datos del estudiante
string insertarEstudiante(string apellido_paterno, string apellido_materno, string nombre, int edad, string matricula, string carrera, string correo, string telefono)
{
	return SQliteExecute("INSERT INTO estudiantes (apellido_paterno, apellido_materno, nombre, edad, matricula, carrera, correo, telefono) VALUES ('" + apellido_paterno + "', '" + apellido_materno + "', '" + nombre + "', " + to_string(edad) + ",'" + matricula + "', '" + carrera + "', '" + correo + "', '" + telefono + "')");
}

// Esta función actualiza los datos del estudiante
string actualizarEstudiante(string apellido_paterno, string apellido_materno, string nombre, int edad, string matricula, string carrera, string correo, string telefono)
{
	return SQliteExecute("UPDATE estudiantes SET apellido_paterno = '" + apellido_paterno + "', "
		                                          + "apellido_materno = '" + apellido_materno + "', "
		                                          + "nombre = '" + nombre + "', " 
		                                          + "edad = " + to_string(edad) + ", " 
		                                          + "carrera = '" + carrera + "', " 
		                                          + "correo = '" + correo + "', " 
		                                          + "telefono = '" + telefono + "' "
		                                          + "WHERE matricula = '" + matricula + "'");
	
}



// Esta función solicita los datos al usuario 
string CapturarEstudianteYLoInserta() 
{
	string apellido_paterno;
	string apellido_materno;
	string nombre;
	int edad;
	string matricula;
	string correo;
	string telefono;
	string carrera;
	
	cout << "Ingrese el nombre del estudiante: ";
	nombre = getUserInput();
	cout << "Ingrese el apellido paterno: ";
	apellido_paterno = getUserInput();
	cout << "Ingrese el apellido materno: ";
	apellido_materno = getUserInput();
	cout << "Ingrese la edad: ";
	edad = stoi(getUserInput());
	cout << "Ingrese la matricula: ";
	matricula = getUserInput();
	cout << "Ingrese el correo electrónico: ";
	correo = getUserInput();
	cout << "Ingrese el télefono: ";
	telefono = getUserInput();
	cout << "Ingrese la carrera: ";
	carrera = getUserInput();

	string result = insertarEstudiante(apellido_paterno , apellido_materno, nombre, edad, matricula, carrera, correo, telefono);
	return result;
}


// Modificamos los datos del estudiante preguntando por su matricula
// Lo tenemos pendiente para la otra etapa
string ModificacionDeEstudiante()
{
	//La matrícula buscada no existe\n

	cout << "Ingrese la matrícula a modificar:";
	string matricula = getUserInput();

	string apellido_paterno;
	string apellido_materno;
	string nombre;
	int edad;
	string carrera;
	string correo;
	string telefono;

	string result = SQliteQuery("SELECT * FROM estudiantes WHERE matricula = '" + matricula + "'");

	if (result == "[]")
	{
		cout << "La matrícula buscada no existe: " << matricula << endl;
		return "Ok.";
	}

	cout << "¿Estás seguro de que deseas modificar al estudiante? 1. Para si / 2. Para no: ";
	string user_selection = getUserInput();

	if(user_selection != "1")
	{
		return "Ok.";
	}
	
	cout << "El estudiante es: \n" << result << "\n";

	//nlohmann::json j = nlohmann::json::parse(result);
	//nombre = j[0]["nombre"];	

	cout << "Ingrese el nombre del estudiante:";
	nombre = getUserInput();
	cout << "Ingrese el apellido paterno:";
	apellido_paterno = getUserInput();
	cout << "Ingrese el apellido materno:";
	apellido_materno = getUserInput();
	cout << "Ingrese la edad:";
	edad = stoi(getUserInput());
	cout << "Ingrese el correo electrónico:";
	correo = getUserInput();
	cout << "Ingrese el télefono:";
	telefono = getUserInput();
	cout << "Ingrese la carrera:";
	carrera = getUserInput();

	return actualizarEstudiante( apellido_materno, apellido_paterno, nombre, edad, matricula, carrera, correo, telefono);
}


// Eliminamos los datos del estudiante preguntando por su matrícula
string BajaDeEstudiante()
{
	string matricula;

	cout << "Ingrese la matrícula del estudiante: ";
	matricula = getUserInput();

	string result = SQliteQuery("SELECT * FROM estudiantes WHERE matricula = '" + matricula + "'");	

	if (result == "[]")
	{
		cout << "La matrícula buscada no existe\n";
		return "La matrícula buscada no existe\n";
	}

	int opcion;

	system("cls");
	cout << "Seleccione la opción deseada\n";
	cout << "  1.- Confirmar Eliminación\n";
	cout << "  2.- Regresar \n";
	cin >> opcion;

	if( opcion == 1)
	{
		result = SQliteExecute("DELETE FROM estudiantes WHERE matricula = '" + matricula + "'");

		if (result != "Ok.")
		{
			cout << "Error: " + result;
			return "Error: " + result;
		}
		
		cout << "El estudiante ha sido eliminado:" << matricula << endl << "\n";
		return "Ok.";
	}
	
	return "Ok.";
}



//Buscamos los datos del estudiante preguntando por su matrícula
string BusquedaDeEstudiante()
{
	string matricula;

	cout << "Buscar (* --> todos): ";
	matricula = getUserInput();

	string result;
	
	if( matricula == "*")
	{
		result = SQliteQuery("SELECT * FROM estudiantes");		
	}
	else 
	{
		result = SQliteQuery("SELECT * FROM estudiantes WHERE estudiantes MATCH '" + matricula + "'");
	}	

	if (result == "[]") 
	{
		cout << "No se encontraron datos: " << matricula << endl;
		return "Ok.";
	}
	
	cout << "Resultado: \n" << result << "\n";

	cout << "Salvar resultados 1 = Si, 2 = No: ";
	string salvar = getUserInput();

	if (salvar != "1")
	{
		return "Ok.";		
	}

	cout << "Nombre del archivo: ";
	string file = getUserInput();

	WriteFile(file, result);

	cout << "Archivo salvado: " + file << endl;

	return "Ok.";
}




// Esto lo que se ejecuta primero en cualquier programa de C++
int main()
{
	// Si no mandamos a la consola el "hola mundo", puede ser que llamemos a la mala suerte :-) 
	//cout << "Hola Mundo!!!";

	SetConsoleOutputCP(65001);
	string result = SQliteExecute("CREATE VIRTUAL TABLE IF NOT EXISTS estudiantes USING fts5(matricula, apellido_paterno, apellido_materno, nombre, edad, carrera, correo, telefono)");
		
	//string result = SQliteExecute("CREATE TABLE IF NOT EXISTS estudiantes (matricula PRIMARY KEY, apellido_paterno, apellido_materno, nombre, edad, carrera, correo, telefono)");

	if (result != "Ok.") 
	{
		cout << result;
		return 0;
	}
	
	int opcion = 0;

	while (true)
	{

		opcion = 0;

		system("cls");
		cout << "Control de Estudiantes\n";
		cout << "Seleccione la operación deseada\n";
		cout << "  1.- Alta: \n";
		cout << "  2.- Baja: \n";
		cout << "  3.- Modificacion: \n";
		cout << "  4.- Busqueda: \n";
		cout << "  5.- Salir del sistema: \n";
		cin >> opcion;

		if (opcion == 1)
		{
			system("cls");
			result = CapturarEstudianteYLoInserta();
			
			if (result != "Ok.") 
			{
				cout << "Error: " + result;
			}
			else 
			{
				cout << "Estudiante agregado\n";
			}			

		}

		if (opcion == 2)
		{
			system("cls");
			result = BajaDeEstudiante();

			if (result != "Ok.")
			{
				cout << "Error: " + result << endl;
			}
		}

		if (opcion == 3)
		{
			system("cls");
			result = ModificacionDeEstudiante();		

			if (result != "Ok.")
			{
				cout << "Error: " + result << endl;
			}
			else 
			{
				//cout << "Datos de Estudiante modificados\n";
			}
		}

		if (opcion == 4)
		{
			system("cls");
			result = BusquedaDeEstudiante();

			if (result != "Ok.")
			{
				cout << "Error: " + result << endl;
			}

		}

		if( opcion == 5 )
		{
			return 0;
		}

		system("pause");
		
		
	}
}