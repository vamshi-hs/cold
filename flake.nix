{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {inherit system;};
      # let
      c-src-buildPhase = ''
        gcc -Wall ./src/main.c ./src/erow/erow.c ./src/find/find.c ./src/input/input.c ./src/display/display.c ./src/editor/editor.c  ./src/keypress/keypress.c `pkg-config --cflags ncurses` -o cold -lncurses
      '';
      # ''
      #   gcc -Wall -Wextra -std=c11 -o clox ./src/main.c ./src/scanner.c ./src/token.c ./src/tokentype.c ./src/literaltype.c
      # '';
      test-buildPhase = ''
        gcc -Wall -Wextra -std=c11 -o test ./tests/main.c ./src/scanner.c ./src/token.c ./src/tokentype.c ./src/literaltype.c
      '';
    in {
      packages.test = pkgs.stdenv.mkDerivation {
        name = "test";
        src = self;
        buildInputs = with pkgs; [gcc indent];
        buildPhase = test-buildPhase;
        installPhase = ''
          mkdir -p $out/bin
           install -t $out/bin test
        '';
      };
      packages.main = pkgs.stdenv.mkDerivation {
        name = "cold";
        src = self;
        buildInputs = with pkgs; [gcc indent gnumake ncurses pkg-config];
        buildPhase = c-src-buildPhase;
        installPhase = ''
          mkdir -p $out/bin
           install -t $out/bin cold
        '';
      };
      devShells.default = pkgs.mkShell {
        buildInputs = [pkgs.gcc pkgs.gnumake];
      };
    });
}
# defaultPackage.aarch64-linux =
# with import nixpkgs { system = "aarch64-linux"; };
# {
#   inputs = { nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable"; };
#   outputs = { self, nixpkgs }:
#     let
#       c-buildPhase = ''
#                   	gcc -Wall -Wextra -std=c99  -o clox ./src/main.c ./src/scanner.c ./src/token.c ./src/tokentype.c ./src/literaltype.c
#         	          '';
#     in {
#       defaultPackage.aarch64-linux =
#         with import nixpkgs { system = "aarch64-linux"; };
#     flake-utils.lib.eachDefaultSystem (system:
#         packages.main = stdenv.mkDerivation {
#           name = "clox";
#           src = self;
#           buildInputs = [gcc indent clang-tools valgrind];
#           buildPhase = c-buildPhase;
#           # nativeBuildInputs = [ pkgs.pkg-config pkgs.raylib ];
#           nativeBuildInputs = [ pkgs.pkg-config pkgs.raylib ];
#           installPhase = ''
#             	             mkdir -p $out/bin
#                            install -t $out/bin clox
#           '';
#         };
#     )};
# }

