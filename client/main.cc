#include <iostream>
#include <memory>
#include <grpc++/grpc++.h>

#include "hello.grpc.pb.h"
#include "hello.pb.h"

using grpc::ClientContext;

class Client {
private:
    std::unique_ptr<Hello::Stub> stub;

public:
    Client(std::shared_ptr<grpc::ChannelInterface> channel)
    : stub(Hello::NewStub(channel)) {}

    ServerVersion GetVersion() {
        ClientContext ctx;
        ServerVersion ver;
        this->stub->GetVersion(&ctx, {}, &ver);
        return ver;
    }
};

int main() {
    Client client(grpc::CreateChannel("localhost:1111", grpc::InsecureChannelCredentials()));

    ServerVersion ver = client.GetVersion();
    std::cout << "Server version: " << ver.major() << "." << ver.minor() << "." << ver.patch() << "\n";
}
