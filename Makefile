rpc-server: server/hello.pb.o server/main.o
	$(CXX) $^ -o $@ -lgrpc++ -lprotobuf

server/%.pb.cc: proto/%.proto
	protoc -I proto/ --cpp_out=server/ $^
