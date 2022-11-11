#include "sqlite.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

typedef struct {
    std::string email;
    std::string first_name;
    std::string last_name;
} user;

int main(int argc, char* argv[])
{
    std::unique_ptr<SQLite> sqlite = std::make_unique<SQLite>();

    // open
    if (!sqlite->open("sample.db")) {
        exit(-1);
    }

    // All delete
    if (!sqlite->exec("DELETE FROM users")) {
        exit(-1);
    }

    // insert
    std::vector<user> users {
        { "fuga@xyz.com", "Stephen", "Campbell" },
        { "hoge@xyz.com", "Marco", "Piccolo" },
        { "piyo@xyz.com", "James", "Hunter" },
    };
    for (int i = 0; i < users.size(); i++) {
        std::string insert = "INSERT INTO users(email, first_name, last_name) ";
        insert += "VALUES('" + users[i].email + "',";
        insert += "'" + users[i].first_name + "',";
        insert += "'" + users[i].last_name + "')";

        if (!sqlite->exec(insert)) {
            exit(-1);
        }
    }

    // select
    std::unique_ptr<SQLiteStmt> stmt;
    std::string sql_select = "SELECT * FROM users";
    if (!sqlite->prepare("SELECT * FROM users", stmt)) {
        exit(-1);
    }

    bool is_done = false;
    while (true) {
        if (!stmt->step(is_done)) {
            exit(-1);
        }

        if (is_done) {
            break;
        }

        int id = stmt->to_int(0);
        std::string email = stmt->to_text(1);
        std::string first_name = stmt->to_text(2);
        std::string last_name = stmt->to_text(3);
        std::cout << "id:" << id << ", ";
        std::cout << "email:" << email << ", ";
        std::cout << "first_name:" << first_name << ", ";
        std::cout << "last_name:" << last_name << std::endl;
    }

    if (!stmt->finalize()) {
        exit(-1);
    }

    // close
    if (!sqlite->close()) {
        exit(-1);
    }

    return 0;
}
