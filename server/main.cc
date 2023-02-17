#include <iostream>
#include <memory>
#include <grpc++/grpc++.h>

#include "hello.grpc.pb.h"
#include "hello.pb.h"
#include "users.h"

using google::protobuf::Empty;
using grpc::Status;
using grpc::ServerContext;

class Service : public Hello::Service {
    std::mutex mtx;

    virtual Status GetVersion(ServerContext* ctx, const Empty* req, ServerVersion* resp) override {
        resp->set_major(0);
        resp->set_minor(1);
        resp->set_patch(0);
        return Status::OK;
    };

    virtual Status UsersExist(ServerContext* ctx, grpc::ServerReaderWriter<UserStatus, UserName>* stream) override {
        const auto& users = GetUsers();

        UserName name;
        while (stream->Read(&name)) {
            std::unique_lock<std::mutex> lock(this->mtx);
            UserStatus status;
            status.set_exists(users.find(name.name()) != users.end());
            stream->Write(status);
        }

        return Status::OK;
    }
};

int main() {
    Service service;

    grpc::ServerBuilder builder;
    builder
        .AddListeningPort("localhost:1111", grpc::InsecureServerCredentials())
        .RegisterService(&service);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    std::cout << "Server up and waiting...\n";
    server->Wait();
}
