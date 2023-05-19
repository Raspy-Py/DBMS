#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <mysql/jdbc.h>

#include "TableInfo.h"
#include "Console.h"

class Scema
{
public:
	Scema(const std::string& scemaName, Console::InputStream* pLogstream);

	const std::vector<std::string>& EnumerateTables();
	const TableInfo& GetTableInfo(const std::string& table);

	void Insert(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& values);
	void Update(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& oldValues, 
				const std::vector<std::pair<std::string, std::string>>& newValues);
	void Delete(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& values);
	void Find(	const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& sourceValues, 
				std::vector<std::pair<std::string, std::string>>& destValues);
	void Select(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& values);

private:
	std::unique_ptr<sql::mysql::MySQL_Driver> m_Driver;
	std::unique_ptr<sql::Connection> m_Connection;
	std::vector<std::string> m_CachedTableNames;
	std::unordered_map<std::string, TableInfo> m_TableInfos;

	Console::InputStream* p_LogStream;
};

