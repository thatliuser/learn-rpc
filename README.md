# learn-rpc

Simple gRPC example for my own educational purposes.

# Running
For the easiest time, use Nix.
```sh
nix build
result/bin/rpc-server &
result/bin/rpc-client
```
You can also just use the Makefile, but build artifacts aren't put in a different directory.
```sh
make rpc-server
make rpc-client
./rpc-server &
./rpc-client
```
You'll need to install gRPC and Protobuf for the Makefile to work.
