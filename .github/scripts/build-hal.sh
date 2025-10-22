#!/usr/bin/env bash
set -euo pipefail

ABI=${1:-aarch64}
ROOT=$(pwd)
NDK_ROOT=${NDK_ROOT:-$RUNNER_TEMP/android-ndk/android-ndk-r25b}

OUTDIR="$ROOT/out"
mkdir -p "$OUTDIR"

case "$ABI" in
  aarch64)
    TARGET_HOST=arm-linux-androideabi
    # Use aarch64 clang triple
    CLANG_TRIPLE=aarch64-linux-android
    API=31
    ;;
  arm)
    TARGET_HOST=armv7a-linux-androideabi
    CLANG_TRIPLE=armv7a-linux-androideabi
    API=24
    ;;
  *)
    echo "Unsupported ABI: $ABI" >&2
    exit 2
    ;;
esac

CLANG="$NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin/${CLANG_TRIPLE}${API}-clang++"
AR="$NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar"

if [ ! -x "$CLANG" ]; then
  echo "Cannot find clang at $CLANG" >&2
  echo "NDK_ROOT=$NDK_ROOT" >&2
  exit 3
fi

SRC_DIR="$ROOT/hardware/qcom-caf/msm8953/audio"
if [ ! -f "$SRC_DIR/audio_hal.cpp" ]; then
  SRC_DIR="$ROOT/hardware/qcom/audio"
fi

echo "Building from $SRC_DIR using $CLANG"


OUT_SO="$OUTDIR/libaudio.primary.msm8953.so"

"$CLANG" -fPIC -shared -O2 \
  -I"$NDK_ROOT/sysroot/usr/include" \
  -I"$NDK_ROOT/sysroot/usr/include/${CLANG_TRIPLE}" \
  -I"$ROOT/include" \
  -DANDROID -std=gnu++17 \
  "$SRC_DIR/audio_hal.cpp" -o "$OUT_SO" -llog

file "$OUT_SO" || true
ls -l "$OUT_SO"

echo "Built $OUT_SO"
