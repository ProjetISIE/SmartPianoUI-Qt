{
  description = "Nix-flake-based Qt/C++ development environment";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05"; # NixOS Stable
  outputs =
    { self, nixpkgs }:
    let
      eachSystem = f: {
        x86_64-linux = f (import nixpkgs { system = "x86_64-linux"; });
        aarch64-darwin = f (import nixpkgs { system = "aarch64-darwin"; });
        aarch64-linux = f (import nixpkgs { system = "aarch64-linux"; });
      };
      crossSystem = "aarch64-linux";
    in
    {
      packages = eachSystem (pkgs: {
        smart-piano-ui = pkgs.stdenv.mkDerivation {
          pname = "smart-piano-ui";
          version = "0.1.0";
          src = self;
          nativeBuildInputs = with pkgs; [
            libsForQt5.qt5.qmake # Qt build tool
            libsForQt5.qt5.wrapQtAppsHook # Qt build tool
          ];
          buildInputs =
            let
              inputs =
                _pkgs: with _pkgs; [
                  libsForQt5.qt5.qtbase # Qt
                  libsForQt5.qt5.qtwayland # For Wayland support (linux)
                  xorg.libxcb # For X11 support (linux)
                ];
            in
            (inputs pkgs)
            ++ (nixpkgs.lib.lists.optional (pkgs.system != crossSystem) [ ]
              # (inputs (
              #   import nixpkgs {
              #     localSystem = pkgs.system;
              #     crossSystem = crossSystem;
              #   }))
            );
          installPhase = "install -D SmartPianoUI $out/SmartPianoUI";
        };
        default = self.packages.${pkgs.system}.smart-piano-ui;
      });
      devShells = eachSystem (pkgs: {
        default = pkgs.mkShell {
          nativeBuildInputs = self.packages.${pkgs.system}.default.nativeBuildInputs;
          buildInputs = self.packages.${pkgs.system}.default.buildInputs;
          # env = {
          #   CXX = "c++";
          #   CC = "clang";
          #   CXXFLAGS = ''
          #     -std=c++23 -Wall -Wextra -Wpedantic -Wshadow -Wconversion
          #   '';
          # };
        };
      });
    };
}
