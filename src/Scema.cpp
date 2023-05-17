#include "Scema.h"

#include "Exception.h"

Scema::Scema(const std::string& scemaName, Console::InputStream* pLogstream)
    :
    p_LogStream(pLogstream)
{
    // Connecting to MySQL database
    m_Driver.reset(sql::mysql::get_mysql_driver_instance());
    auto connectionRoute = "tcp://127.0.0.1:3306/" + scemaName;
    m_Connection.reset(m_Driver->connect(connectionRoute.c_str(), "root", "2718"));
}

const std::vector<std::string>& Scema::EnumerateTables()
{
    if (!m_CachedTableNames.empty())
    {
        return m_CachedTableNames;
    }
    sql::Statement* stmt = m_Connection->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SHOW TABLES");

    // Loop through the result set and store table names in the vector
    while (res->next())
    {
        std::string tableName = res->getString(1);
        m_CachedTableNames.push_back(tableName);
    }

    return m_CachedTableNames;
}

const TableInfo& Scema::GetTableInfo(const std::string& table)
{
    auto pTableInfo = m_TableInfos.find("table");

    if (pTableInfo != m_TableInfos.end())
    {
        return pTableInfo->second;
    }

    m_TableInfos[table] = std::move(TableInfo(m_Connection.get(), table)); // TODO: implement move constructor explicitly

    return m_TableInfos[table];
}

void Scema::Insert(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& values)
{
    std::string query = "INSERT INTO `" + tableName + "` (";
    size_t keyCounter = 0;
    for (size_t i = 0; i < values.size(); ++i) 
    {
        if (keyCounter >  0)
        {
            query += ", ";
        }
        keyCounter++;
        query += values.at(i).first;
    }
    query += ") VALUES (";
    keyCounter = 0;
    for (size_t i = 0; i < values.size(); ++i) 
    {
        if (keyCounter > 0) 
        {
            query += ", ";
        }
        keyCounter++;
        query += "'" + values.at(i).second + "'";
    }
    query += ")";

    sql::Statement* statement = m_Connection->createStatement();
    std::string timeStamp = FORMAT("[%s]", TIME_STAMP());
    p_LogStream->CommandLog(FORMAT("%20s: %s", timeStamp.c_str(), query.c_str()));
    try
    {
        statement->execute(query);

        for (auto& [inputLabel, data] : values)
        {
            p_LogStream->Log(FORMAT("%20s: %20s", inputLabel.c_str(), data.c_str()));
        }
    }
    catch (const sql::SQLException& e)
    {
        p_LogStream->ErrorLog(FORMAT("%20s: %s", "SQL error", e.what()));
    }
}

void Scema::Update(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& oldValues, 
    const std::vector<std::pair<std::string, std::string>>& newValues)
{
    std::string query = "UPDATE `" + tableName + "` SET ";
    size_t keyCounter = 0;
    for (size_t i = 1; i < newValues.size(); ++i) 
    {
        if (keyCounter > 0)
        {
            query += ", ";
        }
        keyCounter++;
        query += oldValues.at(i).first + "='" + newValues.at(i).second + "'";
    }
    query += " WHERE ";
    keyCounter = 0;
    for (size_t i = 0; i < oldValues.size(); ++i) 
    {
        if (oldValues.at(i).second == "") continue;
        if (keyCounter > 0)
        {
            query += " AND ";
        }
        keyCounter++;
        query += oldValues.at(i).first + "='" + oldValues.at(i).second + "'";
    }

    sql::Statement* statement = m_Connection->createStatement();
    std::string timeStamp = FORMAT("[%s]", TIME_STAMP());
   p_LogStream->CommandLog(FORMAT("%20s: %s", timeStamp.c_str(), query.c_str()));
    try
    {
        statement->execute(query);
        for (auto& [inputLabel, data] : newValues)
        {
            p_LogStream->Log(FORMAT("%20s: %20s", inputLabel.c_str(), data.c_str()));
        }
    }
    catch (const sql::SQLException& e)
    {
        p_LogStream->ErrorLog(FORMAT("%20s: %s", "SQL error", e.what()));
    }
}

void Scema::Delete(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& values)
{
    std::string query = "DELETE FROM " + tableName + " WHERE ";
    size_t keyCounter = 0;
    for (size_t i = 0; i < values.size(); ++i) 
    {
        if (keyCounter >  0)
        {
            query += " AND ";
        }
        keyCounter++;
        query += values[i].first + "='" + values[i].second + "'";
    }

    sql::Statement* statement = m_Connection->createStatement();
    std::string timeStamp = FORMAT("[%s]", TIME_STAMP());
   p_LogStream->CommandLog(FORMAT("%20s: %s", timeStamp.c_str(), query.c_str()));
    try
    {
        statement->execute(query);
        for (auto& [inputLabel, data] : values)
        {
            std::string markedData = (data.empty() ? "***" : data);
            p_LogStream->Log(FORMAT("%20s: %20s", inputLabel.c_str(), markedData.c_str()));
        }
    }
    catch (const sql::SQLException& e)
    {
        p_LogStream->ErrorLog(FORMAT("%20s: %s", "SQL error", e.what()));
    }
}

void Scema::Find(const std::string& tableName, const std::vector<std::pair<std::string, 
    std::string>>& sourceValues, std::vector<std::pair<std::string, std::string>>& destValues)
{
    size_t keyCounter = 0;
    std::string query = "SELECT * FROM `" + tableName + "` WHERE ";
    for (size_t i = 0; i < sourceValues.size(); ++i) 
    {
        if (sourceValues[i].second == "") continue;
        if (keyCounter > 0)
        {
            query += " AND ";
        }
        keyCounter++;
        query += sourceValues[i].first + "='" + sourceValues[i].second + "'";
    }

    // Execute the SELECT query
    sql::Statement* statement = m_Connection->createStatement();
    sql::ResultSet* resultSet;
    std::string timeStamp = FORMAT("[%s]", TIME_STAMP());
    p_LogStream->CommandLog(FORMAT("%20s: %s", timeStamp.c_str(), query.c_str()));
    try
    {
        resultSet = statement->executeQuery(query);
        // Retrieve the row data
        if (resultSet->next()) 
        {
            int columnCount = resultSet->getMetaData()->getColumnCount();
            p_LogStream->Log(FORMAT("%20s: Found a row with %i columns.", "", columnCount));
            for (int i = 1; i <= columnCount; ++i) 
            {
                destValues[i-1].second = resultSet->getString(i);
            }
        }
        else
        {
            p_LogStream->ErrorLog(FORMAT("%20s: Row is not found.", ""));
        }
    }
    catch (const sql::SQLException& e)
    {
        p_LogStream->ErrorLog(FORMAT("%20s: %s", "SQL error", e.what()));
    }
}
