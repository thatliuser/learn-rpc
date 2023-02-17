rpc-server: server/hello.pb.o server/main.o
	$(CXX) $^ -o $@ -lgrpc++ -lprotobuf

proto: server/hello.grpc.pb.cc server/hello.pb.cc

PROTOC=protoc -I proto/
server/%.pb.cc: proto/%.proto
	$(PROTOC) --cpp_out=server/ $^

server/%.grpc.pb.cc: proto/%.proto
	$(PROTOC) --grpc_out=server/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $^

client/%.pb.cc: proto/%.proto
	$(PROTOC) --cpp_out=client/ $^

client/%.grpc.pb.cc: proto/%.proto
	$(PROTOC) --grpc_out=client/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $^
