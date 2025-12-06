{
  description = "Nix flake Qt/C++ development environment";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  outputs =
    { self, nixpkgs }:
    let
      systems =
        f:
        let
          localSystems = [
            "aarch64-linux"
            "x86_64-linux"
            "aarch64-darwin"
          ];
          crossSystem = "aarch64-linux";
        in
        nixpkgs.lib.genAttrs localSystems (
          localSystem:
          f (import nixpkgs { inherit localSystem; }) (
            import nixpkgs {
              inherit localSystem;
              inherit crossSystem;
            }
          )
        );
    in
    {
      packages = systems (
        pkgs: crossPkgs: {
          smart-piano-ui = pkgs.qt5.callPackage ./ui.nix { inherit self; };
          cross-smart-piano-ui = crossPkgs.qt5.callPackage ./ui.nix { inherit self; };
          default = self.packages.${pkgs.stdenv.hostPlatform.system}.smart-piano-ui;
          cross = self.packages.${pkgs.stdenv.hostPlatform.system}.cross-smart-piano-ui;
        }
      );
      devShells = systems (
        pkgs: crossPkgs: {
          default =
            pkgs.mkShell.override
              {
                stdenv = pkgs.clangStdenv; # Clang instead of GCC
              }
              {
                packages = with pkgs; [
                  alsa-utils
                  bear # Build EAR
                  # clang-tools # Clang CLIs, including LSP
                  clang-uml # Clang UML diagram generator
                  # cmake-language-server # Cmake LSP
                  # lldb # Clang debug adapter
                ];
                nativeBuildInputs =
                  self.packages.${pkgs.stdenv.hostPlatform.system}.smart-piano-ui.nativeBuildInputs;
                buildInputs = self.packages.${pkgs.stdenv.hostPlatform.system}.smart-piano-ui.buildInputs;
              };
        }
      );
    };
}
