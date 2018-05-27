#!/usr/bin/env sh

GREEN='\033[1;32m'
NC='\033[0m' # No Color
sm() {
  echo "";
  echo "${GREEN}$@${NC}";
}

exe() {
  sm "$@";
  $@;
}