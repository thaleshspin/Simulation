#!/bin/bash
echo -n "Toolchain name: "
read toolchain

qibuild configure -c $toolchain
qibuild make -c $toolchain
qibuild package NaoLib -c $toolchain
qitoolchain add-package ../package/NaoLib-0.1-$toolchain.zip  -c $toolchain