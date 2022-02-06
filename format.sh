#!/bin/bash

find src | grep -E 'cpp$|hpp$' | grep -v 'src/libs/' | xargs clang-format -i
