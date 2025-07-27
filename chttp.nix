{ pkgs, ... }:
pkgs.stdenv.mkDerivation (finalAttrs: {
  pname = "chttp";
  version = "0.0.1";
  src = ./.;

  nativeBuildInputs = with pkgs; [ cmake ninja gcc ];

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin
    cp chttp $out/bin

    runHook postInstall
  '';
})
