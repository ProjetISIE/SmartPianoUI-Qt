{
  cmake,
  mkDerivation,
  self,
}:
mkDerivation {
  pname = "smart-piano-ui";
  version = "0.0.0";
  src = self;
  nativeBuildInputs = [
    cmake # CMake build tool
    # qmake # Qt build tool, included by qt5.mkDerivation
    # qttools # Qt tooling
    # wrapQtAppsHook # Qt build tool, included by qt5.mkDerivation
  ];
  # buildInputs = [
  #   qtbase # Qt itself, included by qt5.mkDerivation
  #   qtsvg # Qt SVG lib, included by qt5.mkDerivation
  #   libxcb # X11 support (old Linux)
  #   qtwayland # Wayland support (Linux)
  # ];
  configurePhase = ''
    qmake
  '';
  installPhase = ''
    install -D SmartPianoUI $out/bin/ui
  '';
}
