#!/bin/bash

set -e

[ "$(head -n +1 README.md  | sed -E 's/^.*- *(.*)$/\1/')" == "v$1" ]
