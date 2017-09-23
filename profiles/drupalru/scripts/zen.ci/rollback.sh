#!/bin/sh

print_status "Возврат предыдущей версии"
ln -sfn "$VERSIONS_DIR/$PREVIOUS_VERSION" "$SITE_DIR"
rm -rf $VERSION_DIR