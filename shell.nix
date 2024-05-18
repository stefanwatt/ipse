{ pkgs ? import <nixpkgs> { } }:

with pkgs;

mkShell {
  buildInputs = [
    arduino
    clang-tools
    #gnumake # upload with Makefile
    #screen # serial console
  ];
}
