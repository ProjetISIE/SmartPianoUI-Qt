{
  description = "A Nix-flake-based C/C++ development environment";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05"; # NixOS Stable
  # inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f: nixpkgs.lib.genAttrs supportedSystems (system: f { pkgs = import nixpkgs { inherit system; }; });
    in
    {
      packages = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "smart-piano-ui";
            version = "0.1.0";
            src = self;
            nativeBuildInputs = [
              pkgs.libsForQt5.qt5.qmake

            ];
            buildInputs = with pkgs; [
              libsForQt5.qt5.qmake
              libsForQt5.qt5.qtbase
              libsForQt5.qt5.qtwayland
              libsForQt5.qwt
              xorg.libxcb
              xorg.xcbutilwm
              xorg.xcbutilimage
              xorg.xcbutilkeysyms
              xorg.xcbutilrenderutil
              xcb-util-cursor
            ];
            configurePhase = ''
              qmake SmartPianoUI.pro
            '';
            # buildPhase = ''
            #   make -j$NIX_BUILD_CORES
            # '';
            installPhase = ''
              install -Dm755 SmartPianoUI $out/bin/SmartPianoUI
            '';
          };
        }
      );
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              libsForQt5.qt5.qmake
              libsForQt5.qt5.qtbase
              libsForQt5.qt5.qtwayland
              libsForQt5.qwt
              xorg.libxcb
              xorg.xcbutilwm
              xorg.xcbutilimage
              xorg.xcbutilkeysyms
              xorg.xcbutilrenderutil
              xcb-util-cursor
            ];
            env = {
              CXX = "c++";
              CC = "clang";
              CXXFLAGS = ''
                -std=c++23 -Wall -Wextra -Wpedantic -Wshadow -Wconversion
              '';
            };
          };
        }
      );
    };
}
