#!/bin/bash

docker run --rm -it \
  -v $(pwd):/app -w /app \
  --user 1000:1000 \
  -e PLATFORM=PLATFORM_WEB emscripten/emsdk \
  make clean all

