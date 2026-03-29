#!/bin/bash
echo "Executing post-image script"
echo "Hello world from post_image script" >> "${BINARIES_DIR}/post_image_text.txt"

cp "${BR2_EXTERNAL_COTTI_PATH}/board/cotti/cotti-board/dts/am335x-boneblack.dtb" \
    "${BINARIES_DIR}/am335x-boneblack.dtb"

echo "Ended post-image script"
