{
  inputs.nixpkgs.url = "nixpkgs";

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };
    mkPackage = path: import path rec {
      inherit pkgs;
      inherit (pkgs) lib stdenv;
    };
  in
  {
    packages."${system}" = rec {
      server = mkPackage ./pkgs/server.nix;
      client = mkPackage ./pkgs/client.nix;
      default = pkgs.symlinkJoin {
        name = "learn-rpc";
        paths = [
          server
          client
        ];
      };
    };
  };
}
