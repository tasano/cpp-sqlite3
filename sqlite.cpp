#include "sqlite.h"

SQLiteStmt::SQLiteStmt(sqlite3_stmt* stmt)
    : m_stmt(stmt)
{
}

SQLiteStmt::~SQLiteStmt()
{
}

bool SQLiteStmt::step(bool& is_done)
{
    int ret = sqlite3_step(m_stmt);
    if ((ret != SQLITE_ROW) && (ret != SQLITE_DONE)) {
        std::cerr << "Failed to step: " << ret << std::endl;
        return false;
    }

    is_done = (ret == SQLITE_DONE);

    return true;
}

int SQLiteStmt::to_int(int col)
{
    return sqlite3_column_int(m_stmt, col);
}

std::string SQLiteStmt::to_text(int col)
{
    return std::string((const char*)sqlite3_column_text(m_stmt, col));
}

bool SQLiteStmt::finalize(void)
{
    int ret = sqlite3_finalize(m_stmt);
    if (ret != SQLITE_OK) {
        std::cerr << "Failed to finalize: " << ret << std::endl;
        return false;
    }

    return true;
}

SQLite::SQLite()
    : m_db(nullptr)
{
}

SQLite::~SQLite()
{
    close();
}

bool SQLite::open(std::string path)
{
    int ret = sqlite3_open(path.c_str(), &m_db);
    if (ret != SQLITE_OK) {
        std::cerr << "Failed to open: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    return true;
}

bool SQLite::close(void)
{
    if (m_db == nullptr) {
        return true;
    }

    int ret = sqlite3_close(m_db);
    if (ret != SQLITE_OK) {
        std::cerr << "Failed to close: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    m_db = nullptr;

    return true;
}

bool SQLite::exec(std::string sql)
{
    char* err_msg = nullptr;

    int ret = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &err_msg);
    if (ret != SQLITE_OK) {
        std::cerr << "Failed to exec: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    return true;
}

bool SQLite::prepare(std::string sql, std::unique_ptr<SQLiteStmt>& stmt)
{
    sqlite3_stmt* prep_stmt = nullptr;

    int ret = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &prep_stmt, nullptr);
    if (ret != SQLITE_OK) {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    stmt = std::make_unique<SQLiteStmt>(prep_stmt);

    return true;
}
