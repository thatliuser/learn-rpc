LDFLAGS=-lgrpc++ -lprotobuf -lgpr
PROTOC=protoc -I proto/

rpc-server: server/hello.pb.o server/hello.grpc.pb.o server/main.o server/users.o
	$(CXX) $^ -o $@ $(LDFLAGS)

rpc-client: client/hello.pb.o client/hello.grpc.pb.o client/main.o
	$(CXX) $^ -o $@ $(LDFLAGS)

proto: server/hello.grpc.pb.cc server/hello.pb.cc client/hello.grpc.pb.cc client/hello.pb.cc

clean:
	rm server/*.pb.* client/*.pb.*

server/%.pb.cc: proto/%.proto
	$(PROTOC) --cpp_out=server/ $^

server/%.grpc.pb.cc: proto/%.proto
	$(PROTOC) --grpc_out=server/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $^

client/%.pb.cc: proto/%.proto
	$(PROTOC) --cpp_out=client/ $^

client/%.grpc.pb.cc: proto/%.proto
	$(PROTOC) --grpc_out=client/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $^
