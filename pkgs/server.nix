{ pkgs, stdenv, ... }:
stdenv.mkDerivation {
  pname = "server";
  version = "0.1.0";

  nativeBuildInputs = with pkgs; [
    grpc
    protobuf
  ];

  src = ./.;

  installPhase = "install -D rpc-server $out/bin/rpc-server";
}
