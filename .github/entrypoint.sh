#!/bin/bash
set -e
# This script will be run by the docker image just after being spawned

if [[ -z "${GITHUB_WORKSPACE}" ]]; then
  FILEPATH="../"
else
  FILEPATH="${GITHUB_WORKSPACE}"
fi

cd "$FILEPATH"/"$1"
chmod +x $2
./$2
