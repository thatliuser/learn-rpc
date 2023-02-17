#include <iostream>
#include <memory>
#include <thread>
#include <grpc++/grpc++.h>

#include "hello.grpc.pb.h"
#include "hello.pb.h"

using grpc::ClientContext;

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

        grpc::Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "Couldn't check if users exist!\n";
            std::exit(1);
        }

        return statuses;
    }
};

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
}
