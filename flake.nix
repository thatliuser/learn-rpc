{
  inputs.nixpkgs.url = "nixpkgs";

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    mkPackage = path: import path rec {
      pkgs = import nixpkgs {
        inherit system;
      };
      inherit (pkgs) lib stdenv;
    };
  in
  {
    packages."${system}" = {
      server = mkPackage ./server.nix;
      # client = mkPackge ./client.nix;
    };
  };
}
