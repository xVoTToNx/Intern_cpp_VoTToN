/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

using namespace std;

void d(int i) { cout << i << endl; }
int main(void)
{
	try 
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		/* Connect to the MySQL test database */

		con->setSchema("test");

		stmt = con->createStatement();


		stmt->execute("CREATE TABLE list ( id INT, name VARCHAR(255), surname VARCHAR(255) )");
		delete stmt;

		pstmt = con->prepareStatement("INSERT INTO list VALUES (1, \"one\", \"subone\")");
		pstmt->executeUpdate();
		d(2);
		pstmt = con->prepareStatement("INSERT INTO list VALUES (2, \"two\", \"subtwo\")");
		pstmt->executeUpdate();
		d(3);
		pstmt = con->prepareStatement("INSERT INTO list VALUES (3, \"three\", \"subthree\")");
		pstmt->executeUpdate();
		delete pstmt;

		pstmt = con->prepareStatement("SELECT * FROM list WHERE id = 1");
		res = pstmt->executeQuery();

		res->beforeFirst();
		while (res->next())
		{
			cout << "Name: " << res->getString("name").c_str() << endl;
			cout << "Surname: " << res->getString("surname").c_str() << endl;
		}
		delete res;

		delete pstmt;
		delete con;


		cin.get();
	}
	catch (sql::SQLException &e) 
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}