#pragma once
#include <string>
#include <mysql/jdbc.h>

struct Column
{
	std::string name;
	std::string type;
};

class TableInfo
{
public:
	TableInfo() = default;
	TableInfo(sql::Connection* connection, const std::string& table);

	const std::vector<Column>& GetColumns() const { return m_Columns; };
	const std::string& GetName() const { return m_Name; };

private:
	std::string m_Name;
	std::vector<Column> m_Columns;
};
