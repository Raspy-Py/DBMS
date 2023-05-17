#include "TableInfo.h"
#include "Exception.h"

TableInfo::TableInfo(sql::Connection* connection, const std::string& table)
	:
	m_Name(table)
{
	if (connection == nullptr)
	{
		throw EXCEPTION("Connection* must not be a nullptr.");
	}
	if (table == "")
	{
		throw EXCEPTION("Empty table name.");
	}

	sql::Statement* stmt = connection->createStatement();;
	sql::ResultSet* res = stmt->executeQuery("DESCRIBE `" + table + "`");

	while (res->next())
	{
		std::string name = res->getString("Field");
		std::string type = res->getString("Type");
		m_Columns.emplace_back(name, type);
	}

	delete res;
	delete stmt;
}
