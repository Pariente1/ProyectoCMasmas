#include <windows.h>
#include <sqlext.h>
#include <sql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Función para eliminar acentos
wstring eliminarAcentos(const wstring& input) {
    wstring output = input;
    for (size_t i = 0; i < output.length(); ++i) {
        switch (output[i]) {
        case L'á': case L'Á': output[i] = L'a'; break;
        case L'é': case L'É': output[i] = L'e'; break;
        case L'í': case L'Í': output[i] = L'i'; break;
        case L'ó': case L'Ó': output[i] = L'o'; break;
        case L'ú': case L'Ú': output[i] = L'u'; break;
        case L'ñ': case L'Ñ': output[i] = L'n'; break;
        default: break;
        }
    }
    return output;
}

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"SQL17", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        wcout << L"La conexión a la base de datos fue exitosa." << endl;

        // Asignar un gestor de sentencias
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Cambiar a la base de datos "Escuela"
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"USE Escuela", SQL_NTS);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            // Ejecutar la consulta en la tabla "empleados"
            ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT num_empleado, nombre, apellido_materno FROM dbo.empleados", SQL_NTS);

            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
                const int columnWidth = 25;

                // Imprimir línea superior de la tabla
                wcout << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << endl;

                // Imprimir encabezados
                wcout << left << L"|"
                    << setw(columnWidth) << L"num_empleado"
                    << L"|" << setw(columnWidth) << L"nombre"
                    << L"|" << setw(columnWidth) << L"apellido_materno" << L"|" << endl;

                // Imprimir línea separadora
                wcout << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << endl;

                // Obtener y mostrar los resultados
                int num_empleado;
                SQLWCHAR name[50];
                SQLWCHAR last_name[50];

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_WCHAR, name, sizeof(name), NULL);
                    SQLGetData(hStmt, 3, SQL_C_WCHAR, last_name, sizeof(last_name), NULL);

                    // Convertir SQLWCHAR a wstring y eliminar acentos
                    wstring nombre = eliminarAcentos(name);
                    wstring apellido = eliminarAcentos(last_name);

                    // Imprimir los datos en formato de tabla
                    wcout << left << L"|"
                        << setw(columnWidth) << num_empleado
                        << L"|" << setw(columnWidth) << nombre
                        << L"|" << setw(columnWidth) << apellido << L"|" << endl;
                }

                // Imprimir línea inferior de la tabla
                wcout << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << endl;
            }
            else {
                wcout << L"Error al ejecutar la consulta." << endl;
            }

            // Liberar el gestor de sentencias
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            wcout << L"Error al seleccionar la base de datos." << endl;
        }
    }
    else {
        wcout << L"Fallo al conectar a la base de datos." << endl;
    }

    // Desconectar y liberar manejadores
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
