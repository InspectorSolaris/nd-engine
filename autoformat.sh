#!/bin/bash

find src | grep -E 'cpp$|hpp$' | xargs clang-format -i
