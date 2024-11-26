#include <windows.h>
#include <sqlext.h>
#include <sql.h>
#include <iostream>
#include <iomanip>

using namespace std;

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

        // Seleccionar la base de datos "Escuela"
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"USE Escuela", SQL_NTS);

        // Ejecutar la consulta
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT num_empleado, nombre, apellido_materno FROM dbo.empleados", SQL_NTS);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            SQLSMALLINT numColumns;
            SQLNumResultCols(hStmt, &numColumns);

            const int columnWidth = 30;  // Ancho de las columnas

            // Imprimir línea superior de la tabla
            wcout << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << endl;

            // Imprimir encabezados de columna
            wcout << left << L"|"
                << setw(columnWidth) << L"num_empleado"
                << L"|" << setw(columnWidth) << L"nombre"
                << L"|" << setw(columnWidth) << L"apellido_materno" << L"|" << endl;

            // Imprimir línea separadora
            wcout << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << wstring(columnWidth, '-') << L"+" << endl;

        }
        else {
            wcout << L"Error al ejecutar la consulta." << endl;
        }

        // Liberar el gestor de sentencias
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
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
