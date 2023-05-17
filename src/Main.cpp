#include <iostream>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

int main()
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306/soccer_cup", "root", "2718");

    // Execute SQL queries and process results

    delete con;

    return 0;
}