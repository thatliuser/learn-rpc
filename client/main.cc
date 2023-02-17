#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <grpc++/grpc++.h>

#include "hello.grpc.pb.h"
#include "hello.pb.h"

using grpc::ClientContext;
using grpc::Status;

class Client {
private:
    std::unique_ptr<Hello::Stub> stub;

    static UserName MakeName(std::string name) {
        UserName userName;
        userName.set_name(name);
        return userName;
    }

public:
    Client(std::shared_ptr<grpc::ChannelInterface> channel)
    : stub(Hello::NewStub(channel)) {}

    ServerVersion GetVersion() {
        ClientContext ctx;
        ServerVersion ver;
        this->stub->GetVersion(&ctx, {}, &ver);
        return ver;
    }

    std::vector<bool> UsersExist(std::vector<std::string> names) {
        ClientContext ctx;
        std::shared_ptr<grpc::ClientReaderWriter<UserName, UserStatus>> stream(
            this->stub->UsersExist(&ctx)
        );

        std::thread writer([&]() {
            for (const auto& name : names) {
                stream->Write(MakeName(name));
            }
            stream->WritesDone();
        });

        std::vector<bool> statuses;
        UserStatus exists;
        while (stream->Read(&exists)) {
            statuses.push_back(exists.exists());
        }

        writer.join();

        Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "Couldn't check if users exist!\n";
            std::exit(1);
        }

        return statuses;
    }

    User GetUser(std::string name) {
        ClientContext ctx;

        UserName userName;
        userName.set_name(name);

        User user;

        Status status = this->stub->GetUser(&ctx, userName, &user);
        if (!status.ok()) {
            std::cerr << "Couldn't get user info for " << name << "!\n";
            throw std::invalid_argument("Invalid name");
        }

        return user;
    }
};

void PrintUser(const User& user) {
    std::clog << user.name() << " (age " << user.age() << ") is a ";
    const char* job;
    switch (user.job()) {
        case User::JOB_SWE:
            job = "Software Engineer";
            break;
        case User::JOB_DESIGNER:
            job = "Designer";
            break;
        case User::JOB_MANAGER:
            job = "Manager";
            break;
        case User::JOB_CEO:
            job = "CEO";
            break;
        case User::JOB_OTHER:
            job = "(unknown!)";
            break;
        default:
            std::cerr << "Couldn't determine string for job";
            std::exit(1);
    }
    std::clog << job << "\n";
}

int main() {
    Client client(grpc::CreateChannel("localhost:1111", grpc::InsecureChannelCredentials()));

    ServerVersion ver = client.GetVersion();
    std::clog << "Server version: " << ver.major() << "." << ver.minor() << "." << ver.patch() << "\n";
    if (!(ver.major() == 0 && ver.minor() == 1 && ver.patch() == 0)) {
        std::cerr << "Mismatched server and client versions!\n"
            "Please upgrade this client or downgrade the server.";
        std::exit(1);
    }

    std::vector<std::string> names = { "Bob", "Joe", "Sally", "Walter", "John", "Dylan" };
    std::vector<bool> exists = client.UsersExist(names);
    for (std::uint32_t i = 0; i < names.size(); ++i) {
        std::clog << names[i] << " exists? " << std::boolalpha << exists[i] << "\n";
    }

    for (const auto& name : names) {
        try {
            PrintUser(client.GetUser(name));
        }
        catch (const std::exception& except) {
            std::cerr << except.what() << "\n";
        }
    }
}
