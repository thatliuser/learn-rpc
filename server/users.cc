#include "users.h"

#include <string>

std::pair<std::string, UserInfo> MakeUserPair(std::string name, std::uint32_t age, User::Job job) {
    return {
        name,
        {
            name,
            age,
            job
        }
    };
}

const std::unordered_map<std::string, UserInfo>& GetUsers() {
    static const std::unordered_map<std::string, UserInfo> users = {
        MakeUserPair("Bob", 32, User::JOB_SWE),
        MakeUserPair("Jill", 26, User::JOB_DESIGNER),
        MakeUserPair("John", 45, User::JOB_CEO),
        MakeUserPair("Bill", 35, User::JOB_MANAGER),
    };
    return users;
}
