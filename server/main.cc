#include <iostream>
#include <memory>
#include <grpc++/grpc++.h>

#include "hello.grpc.pb.h"
#include "hello.pb.h"

using google::protobuf::Empty;
using grpc::Status;

class Service : public Hello::Service {
    virtual Status GetVersion(grpc::ServerContext* ctx, const Empty* req, ServerVersion* resp) override {
        resp->set_major(0);
        resp->set_minor(1);
        resp->set_patch(0);
        return Status::OK;
    };
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
