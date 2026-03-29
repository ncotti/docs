#!/bin/bash
echo "Executing pre-build script"

DT_PATH="${BR2_EXTERNAL_COTTI_PATH}/board/cotti/cotti-board/dts"
DTS="${DT_PATH}/am335x-boneblack.dts"
DTB="${DT_PATH}/am335x-boneblack.dtb"

cpp -nostdinc -undef -x assembler-with-cpp -Idts \
    "${DTS}" > "${DTS}.preprocessed"

dtc -I dts -o "${DTB}" "${DTS}.preprocessed"

rm -f "${DTS}.preprocessed"

echo "Ended pre-build script"
