{
  libsForQt5,
  self,
  stdenv,
  xorg,
}:
stdenv.mkDerivation {
  pname = "smart-piano-ui";
  version = "0.1.0";
  src = self;
  nativeBuildInputs = [
    libsForQt5.qt5.qmake # Qt build tool
    libsForQt5.qt5.wrapQtAppsHook # Qt build tool
  ];
  buildInputs = [
    libsForQt5.qt5.qtbase # Qt
    libsForQt5.qt5.qtwayland # For Wayland support (linux)
    xorg.libxcb # For X11 support (linux)
  ];
  configurePhase = "qmake";
  buildPhase = "make";
  installPhase = "install -D SmartPianoUI $out/SmartPianoUI";
}
