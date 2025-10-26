{
  description = "Nix-flake-based Qt/C++ development environment";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05"; # unstable"; #
  outputs =
    { self, nixpkgs }:
    let
      eachSystem = f: {
        aarch64-linux = f (import nixpkgs { system = "aarch64-linux"; }) (
          import nixpkgs { system = "aarch64-linux"; }
        );
        x86_64-linux = f (import nixpkgs { system = "x86_64-linux"; }) (
          import nixpkgs {
            localSystem = "x86_64-linux";
            crossSystem = "aarch64-linux";
          }
        );
        aarch64-darwin = f (import nixpkgs { system = "aarch64-darwin"; }) (
          import nixpkgs {
            localSystem = "aarch64-darwin";
            crossSystem = "aarch64-linux";
          }
        );
      };
    in
    {
      packages = eachSystem (
        pkgs: crossPkgs: {
          smart-piano-ui = pkgs.callPackage ./smartPianoUi.nix { inherit self; };
          cross-smart-piano-ui = crossPkgs.callPackage ./smartPianoUi.nix { inherit self; };
          default = self.packages.${pkgs.system}.smart-piano-ui;
          cross = self.packages.${pkgs.system}.cross-smart-piano-ui;
        }
      );
      devShells = eachSystem (
        pkgs: crossPkgs: {
          default = pkgs.mkShell {
            packages = with pkgs; [
              libsForQt5.qt5.qmake # Qt build tool
              libsForQt5.qt5.wrapQtAppsHook # Qt build tool
              libsForQt5.qt5.qtbase # Qt
              libsForQt5.qt5.qtwayland # For Wayland support (linux)
              xorg.libxcb # For X11 support (linux)
              quickemu # Run VM easily by executing vm.conf
            ];
          };
        }
      );
    };
}
