#include <iostream>
#include <memory>
#include <sqlite3.h>

class SQLiteStmt {
public:
    SQLiteStmt(sqlite3_stmt* stmt);
    ~SQLiteStmt();
    bool step(bool& is_done);
    int to_int(int col);
    std::string to_text(int col);
    bool finalize(void);

private:
    sqlite3_stmt* m_stmt;
};

class SQLite {
public:
    SQLite();
    ~SQLite();
    bool open(std::string path);
    bool close(void);
    bool exec(std::string sql);
    bool prepare(std::string sql, std::unique_ptr<SQLiteStmt>& stmt);

private:
    sqlite3* m_db;
};
