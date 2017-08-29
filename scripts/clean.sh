#!/usr/bin/env bash

cd $VERSION_DIR
if [ $(ls -l | grep -c ^d) > 5 ]; then
  mkdir $VERSION_DIR
fi