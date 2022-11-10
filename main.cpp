#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

typedef struct {
    std::string email;
    std::string first_name;
    std::string last_name;
} user;

int main(int argc, char* argv[])
{
    int ret = 0;
    sqlite3* db = nullptr;
    char* err_msg = nullptr;

    // open
    ret = sqlite3_open("sample.db", &db);
    if (ret != SQLITE_OK)
    {
        std::cerr << "Failed to open db:" << sqlite3_errmsg(db) << std::endl;
        exit(-1);
    }

    // All delete
    std::string sql_delete = "DELETE FROM users";
    ret = sqlite3_exec(db, sql_delete.c_str(), nullptr, nullptr, &err_msg);
    if (ret != SQLITE_OK)
    {
        std::cerr << "Failed to delete:" << err_msg << std::endl;
        exit(-1);
    }

    // insert
    std::vector<user> users {
        { "fuga@xyz.com", "Stephen", "Campbell" },
        { "hoge@xyz.com", "Marco",   "Piccolo"  },
        { "piyo@xyz.com", "James",   "Hunter"   },
    };
    for (int i = 0; i < users.size(); i++)
    {
        std::string sql_insert = "INSERT INTO users(email, first_name, last_name) ";
        sql_insert += "VALUES('" + users[i].email + "',";
        sql_insert += "'" + users[i].first_name + "',";
        sql_insert += "'" + users[i].last_name + "')";

        ret = sqlite3_exec(db, sql_insert.c_str(), nullptr, nullptr, &err_msg);
        if (ret != SQLITE_OK)
        {
            std::cerr << "Failed to insert:" << err_msg << std::endl;
            exit(-1);
        }
    }

    // select
    sqlite3_stmt* stmt = nullptr;
    std::string sql_select = "SELECT * FROM users";
    ret = sqlite3_prepare_v2(db, sql_select.c_str(), -1, &stmt, nullptr);
    if (ret == SQLITE_OK)
    {
        while ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* email = sqlite3_column_text(stmt, 1);
            const unsigned char* first_name = sqlite3_column_text(stmt, 2);
            const unsigned char* last_name = sqlite3_column_text(stmt, 3);
            printf("id:%d, email:%s, first_name:%s, last_name:%s\n",
                    id, email, first_name, last_name);
        }

        if (ret != SQLITE_DONE)
        {
            std::cerr << "Failed to step:" << sqlite3_errmsg(db) << std::endl;
            exit(-1);
        }

        ret = sqlite3_finalize(stmt);
        if (ret != SQLITE_OK)
        {
            std::cerr << "Failed to finalize:" << sqlite3_errmsg(db) << std::endl;
            exit(-1);
        }
    }
    else
    {
        std::cerr << "Failed to prepare:" << sqlite3_errmsg(db) << std::endl;
        exit(-1);
    }

    // close
    ret = sqlite3_close(db);
    if (ret != SQLITE_OK)
    {
        exit(-1);
    }

    return 0;
}
