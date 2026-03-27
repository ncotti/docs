#!/bin/bash
echo "Executing post build script"
echo "Hello world from post_build script" >> "${TARGET_DIR}/script_text.txt"