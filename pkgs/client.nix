{ pkgs, stdenv, ... }:
stdenv.mkDerivation {
  pname = "rpc-client";
  version = "0.1.0";

  nativeBuildInputs = with pkgs; [
    grpc
    protobuf
    gtkmm4
  ];

  src = ./.;

  installPhase = "install -D rpc-client $out/bin/rpc-client";
}
