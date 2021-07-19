#!/bin/sh
set -e
echo "Prepare build environments"
package/install-duild-deps
echo "Run build"
cd package
sh -x ./build.package
cd ..
echo "End build"
