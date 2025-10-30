{
  description = "Nix-flake-based Qt/C++ development environment";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
  outputs =
    { self, nixpkgs }:
    let
      eachSystem =
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
      packages = eachSystem (
        pkgs: crossPkgs: {
          smart-piano-ui = pkgs.libsForQt5.callPackage ./smartPianoUi.nix { inherit self; };
          cross-smart-piano-ui = crossPkgs.libsForQt5.callPackage ./smartPianoUi.nix { inherit self; };
          default = self.packages.${pkgs.system}.smart-piano-ui;
          cross = self.packages.${pkgs.system}.cross-smart-piano-ui;
        }
      );
      devShells = eachSystem (
        pkgs: crossPkgs: {
          default = pkgs.mkShell {
            packages = with pkgs; [
              clang-tools # Clang CLIs, including LSP
              cmake-language-server # Cmake LSP
              cppcheck # C++ Static analysis
              doctest # Testing framework
              doxygen # Documentation generator
              gtest # Testing framework
              lcov # Code coverage
              lldb # Clang debug adapter
              valgrind # Debugging and profiling
            ];
            nativeBuildInputs = self.packages.${pkgs.system}.smart-piano-ui.nativeBuildInputs;
            buildInputs = self.packages.${pkgs.system}.smart-piano-ui.buildInputs;
          };
        }
      );
    };
}
