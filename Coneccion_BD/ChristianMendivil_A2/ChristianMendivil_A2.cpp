#include <windows.h>
#include <sqlext.h>
#include <sql.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;  // Declaración del gestor de sentencias
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

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            // Captura errores al cambiar la base de datos
            SQLWCHAR sqlState[6], message[256];
            SQLINTEGER nativeError;
            SQLSMALLINT textLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
            wcout << L"Error al seleccionar la base de datos." << endl;
            wcout << L"SQL State: " << sqlState << endl;
            wcout << L"Native Error: " << nativeError << endl;
            wcout << L"Message: " << message << endl;
            return 1; // Salir si no se puede seleccionar la base de datos
        }

        // Ejecutar la consulta en la tabla "empleados"
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT num_empleado, nombre, apellido_materno FROM dbo.empleados", SQL_NTS);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            // Captura errores en la consulta
            SQLWCHAR sqlState[6], message[256];
            SQLINTEGER nativeError;
            SQLSMALLINT textLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
            wcout << L"Error al ejecutar la consulta." << endl;
            wcout << L"SQL State: " << sqlState << endl;
            wcout << L"Native Error: " << nativeError << endl;
            wcout << L"Message: " << message << endl;
        }
        else {
            // Obtener y mostrar los resultados
            int num_empleado;
            SQLWCHAR name[50];
            SQLWCHAR last_name[50];

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_WCHAR, name, sizeof(name), NULL);
                SQLGetData(hStmt, 3, SQL_C_WCHAR, last_name, sizeof(last_name), NULL);

                // Imprimir los datos
                wcout << L"Empleado #" << num_empleado << L": " << name << L" " << last_name << endl;
            }
        }

        // Liberar el gestor de sentencias
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    else {
        wcout << L"Fallo al conectar la base de datos." << endl;
    }

    // Desconectar y liberar manejadores
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
