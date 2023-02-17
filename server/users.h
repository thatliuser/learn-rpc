#ifndef USERS_H
#define USERS_H

#include "hello.pb.h"

#include <unordered_map>

struct UserInfo {
    std::string name;
    std::uint32_t age;
    User::Job job;
};

const std::unordered_map<std::string, UserInfo>& GetUsers();

#endif // USERS_H
