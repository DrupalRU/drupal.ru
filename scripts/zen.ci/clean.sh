#!/usr/bin/env bash

while [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ]
do
    DEPRECATED_VERSION=$(ls -r | tail -n 1)
    echo "Удаление устаревшей версии $DEPRECATED_VERSION"
    rm -rf ./$DEPRECATED_VERSION
done