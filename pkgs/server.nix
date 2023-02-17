{ pkgs, stdenv, ... }:
stdenv.mkDerivation rec {
  pname = "rpc-server";
  version = "0.1.0";

  nativeBuildInputs = with pkgs; [
    grpc
    protobuf
    which
  ];

  src = ../.;

  buildPhase = "make ${pname}";

  installPhase = "install -D ${pname} $out/bin/${pname}";
}
