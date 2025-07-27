{
  description = "Flake for building the package and development environment.";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" ] (system:

      let pkgs = import nixpkgs { inherit system; };
      in {

        packages.default = pkgs.callPackage ./chttp.nix { };

        devShells.default =
          pkgs.mkShell { packages = [ self.packages.${system}.default ]; };

      });
}
