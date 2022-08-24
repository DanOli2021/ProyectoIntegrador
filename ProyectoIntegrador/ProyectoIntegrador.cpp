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


// Creamos una funci�n para manejar los datos de la base de datos por medio solo de SQL
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



// Creamos una funci�n para manejar los datos de la base de datos por medio solo de SQL
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


// Estructura estudiante
struct estudiante
{
	string apellido_paterno = "";
	string apellido_materno = "";
	string nombre = "";
	string carrera = "";
	string matricula = "";
	int edad = 0;
	string correo = "";
	string telefono = "";
};


// Arreglo de estudiantes
estudiante estudiantes[1024];
int estudiante_index = 0;

// Esta funci�n inserta los datos del estudiante
// Usamos como parametro la estructura estudiante
string insertarEstudiante(estudiante e)
{
	string result = SQliteExecute("INSERT INTO estudiantes (apellido_paterno, apellido_materno, nombre, edad, matricula, carrera, correo, telefono) VALUES ('" + 
		e.apellido_paterno + "', '" + 
		e.apellido_materno + "', '" + 
		e.nombre + "', " + 
		to_string(e.edad) + ",'" + 
		e.matricula + "', '" + 
		e.carrera + "', '" + 
		e.correo + "', '" + 
		e.telefono + "')");

	if (result == "Ok.")
	{
		estudiantes[estudiante_index] = e;
		++estudiante_index;
	}
	
	return result;
	
}

// Esta funci�n actualiza los datos del estudiante
// Usamos como parametro la estructura estudiante
string actualizarEstudiante(estudiante e)
{
	return SQliteExecute("UPDATE estudiantes SET apellido_paterno = '" + e.apellido_paterno + "', "
		                                          + "apellido_materno = '" + e.apellido_materno + "', "
		                                          + "nombre = '" + e.nombre + "', " 
		                                          + "edad = " + to_string(e.edad) + ", " 
		                                          + "carrera = '" + e.carrera + "', " 
		                                          + "correo = '" + e.correo + "', " 
		                                          + "telefono = '" + e.telefono + "' "
		                                          + "WHERE matricula = '" + e.matricula + "'");

	//Buscamos la matricula a actualizar en el arreglo de estudiantes
	for (int i = 0; i < estudiante_index; i++)
	{
		if (estudiantes[i].matricula == e.matricula)
		{
			estudiantes[i] = e;
			break;
		}
	}
	
}



// Esta funci�n solicita los datos al usuario 
string CapturarEstudianteYLoInserta() 
{

	estudiante e = estudiante();	
	
	cout << "Ingrese el nombre del estudiante: ";
	e.nombre = getUserInput();
	cout << "Ingrese el apellido paterno: ";
	e.apellido_paterno = getUserInput();
	cout << "Ingrese el apellido materno: ";
	e.apellido_materno = getUserInput();
	cout << "Ingrese la edad: ";
	e.edad = stoi(getUserInput());
	cout << "Ingrese la matricula: ";
	e.matricula = getUserInput();
	cout << "Ingrese el correo electr�nico: ";
	e.correo = getUserInput();
	cout << "Ingrese el t�lefono: ";
	e.telefono = getUserInput();
	cout << "Ingrese la carrera: ";
	e.carrera = getUserInput();

	string result = insertarEstudiante(e);
	return result;
}


// Modificamos los datos del estudiante preguntando por su matricula
// Lo tenemos pendiente para la otra etapa
string ModificacionDeEstudiante()
{
	//La matr�cula buscada no existe\n
	estudiante e = estudiante();
	
	cout << "Ingrese la matr�cula a modificar:";
	e.matricula = getUserInput();
	string result = SQliteQuery("SELECT * FROM estudiantes WHERE matricula = '" + e.matricula + "'");

	if (result == "[]")
	{
		cout << "La matr�cula buscada no existe: " << e.matricula << endl;
		return "Ok.";
	}

	cout << "�Est�s seguro de que deseas modificar al estudiante? 1. Para si / 2. Para no: ";
	string user_selection = getUserInput();

	if(user_selection != "1")
	{
		return "Ok.";
	}
	
	cout << "El estudiante es: \n" << result << "\n";

	//nlohmann::json j = nlohmann::json::parse(result);
	//nombre = j[0]["nombre"];	

	cout << "Ingrese el nombre del estudiante:";
	e.nombre = getUserInput();
	cout << "Ingrese el apellido paterno:";
	e.apellido_paterno = getUserInput();
	cout << "Ingrese el apellido materno:";
	e.apellido_materno = getUserInput();
	cout << "Ingrese la edad:";
	e.edad = stoi(getUserInput());
	cout << "Ingrese el correo electr�nico:";
	e.correo = getUserInput();
	cout << "Ingrese el t�lefono:";
	e.telefono = getUserInput();
	cout << "Ingrese la carrera:";
	e.carrera = getUserInput();

	return actualizarEstudiante( e );
}


// Eliminamos los datos del estudiante preguntando por su matr�cula
string BajaDeEstudiante()
{
	string matricula;

	cout << "Ingrese la matr�cula del estudiante: ";
	matricula = getUserInput();

	string result = SQliteQuery("SELECT * FROM estudiantes WHERE matricula = '" + matricula + "'");	

	if (result == "[]")
	{
		cout << "La matr�cula buscada no existe\n";
		return "La matr�cula buscada no existe\n";
	}

	int opcion;

	system("cls");
	cout << "Seleccione la opci�n deseada\n";
	cout << "  1.- Confirmar Eliminaci�n\n";
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

		//Buscamos al estudiante y lo eliminamos 
		for (int i = 0; i < estudiante_index; i++)
		{
			if (estudiantes[i].matricula == matricula)
			{
				estudiantes[i] = estudiante();
				break;
			}
		}
		
		cout << "El estudiante ha sido eliminado:" << matricula << endl << "\n";
		return "Ok.";
	}
	
	return "Ok.";
}



//Buscamos los datos del estudiante preguntando por su matr�cula
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
	
	//Buscamos al estudiante y lo eliminamos	
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


// Buscamos dentro de lo que hemos capturado en la sessi�n
string busquedaEnLoCapturado(){
	// Buscamos los elementos dentro del arreglo es estudiantes

	cout << "Buscar (* --> todos): ";
	string busqueda = getUserInput();

	int encontrados = 0;
	int todos = 0;

	if (busqueda == "*") 
	{
		todos = 1;
	}

	for (int i = 0; i < estudiante_index; i++)
	{

		string estudiante_string = estudiantes[i].apellido_materno + " " + estudiantes[i].apellido_paterno + " " + estudiantes[i].nombre + " " + estudiantes[i].matricula + " " + to_string( estudiantes[i].edad ) + " " + estudiantes[i].correo + " " + estudiantes[i].telefono + " " + estudiantes[i].carrera;
		estudiante_string = toUpper(estudiante_string);
		
		busqueda = toUpper(busqueda);

		cout << "Busqueda: " << busqueda << endl;	
		
		if (estudiante_string.find(busqueda) > 0 || todos == 1)
		{
			++encontrados;
			cout << "__________________________________________________________________________________" << endl;
			cout << "Apellido paterno: " << estudiantes[i].apellido_paterno << endl;
			cout << "Apellido materno: " << estudiantes[i].apellido_materno << endl;
			cout << "Nombre:           " << estudiantes[i].nombre << endl;
		    cout << "Matricula:        " << estudiantes[i].matricula << endl;
			cout << "Carrera:          " << estudiantes[i].carrera << endl;
			cout << "Correo:           " << estudiantes[i].correo << endl;
			cout << "Telefono:         " << estudiantes[i].telefono << endl;
			cout << "__________________________________________________________________________________" << endl;
		}
	}

	cout << "Encontrados: " << encontrados << endl;

	return "Ok.";
}


void RecuperarRegistros() 
{
	
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
		cout << "Seleccione la operaci�n deseada\n";
		cout << "  1.- Alta: \n";
		cout << "  2.- Baja: \n";
		cout << "  3.- Modificacion: \n";
		cout << "  4.- Busqueda: \n";
		cout << "  5.- Busqueda en lo capturado: \n";
		cout << "  6.- Leer en memoria registros: \n";
		cout << "  7.- Salir del sistema: \n";
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

		if (opcion == 5)
		{
			system("cls");
			result = busquedaEnLoCapturado();

			if (result != "Ok.")
			{
				cout << "Error: " + result << endl;
			}

		}

		if( opcion == 7 )
		{
			return 0;
		}

		system("pause");
		
		
	}
}