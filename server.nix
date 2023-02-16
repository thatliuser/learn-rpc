{ pkgs, stdenv, ... }:
stdenv.mkDerivation {
  pname = "server";
  version = "0.1.0";

  nativeBuildInputs = [ pkgs.grpc ];

  src = ./src;
}
