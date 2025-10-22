# lineageos-hal

This repository contains minimal placeholder HAL implementations and Android.bp build files for two common LineageOS/AOSP paths used on Qualcomm msm8953-based devices. It's intended to be used as an overlay into a full Android build tree so you can compile only the HAL shared objects (so) with `mmm`.

Included paths:

- `hardware/qcom-caf/msm8953/audio` — minimal audio HAL for msm8953
- `hardware/qcom/audio` — minimal fallback audio HAL

Usage
1. Clone or copy these folders into the root of your LineageOS/AOSP source tree so their paths match (for example: `<android_root>/hardware/qcom-caf/msm8953/audio`).
2. In your Android build environment (source the build/envsetup.sh and select a lunch), run one of the following to build the audio HAL only:

```
mmm hardware/qcom-caf/msm8953/audio -j1 || mmm hardware/qcom/audio -j1
```

Notes and caveats
- These are minimal stub implementations intended only to produce a .so for testing. They do not implement Android audio HAL APIs.
- Build requires a full AOSP/LineageOS build environment with matching toolchain and platform headers.
- If you need a HAL that actually implements VOIP-GSM bridging, we'll need to implement the Android HAL interfaces (and possibly add vendor-specific glue). I can help iterate from this stub to a working HAL if you provide the API requirements.

If you want, I can:
- Expand the stub into a real audio HAL skeleton that compiles against a specific Android/LineageOS branch.
- Add additional modules (e.g., audio_policy) commonly required by audio stacks.

Progress: created minimal Android.bp and source files for both paths so they can be built with `mmm` as requested.

CI: GitHub Actions build

This repo includes a GitHub Actions workflow to cross-compile the minimal HAL into a shared object on GitHub runners.

Workflow: `.github/workflows/build-hal.yml`

How to run
- Push to GitHub and trigger the workflow from the Actions tab, or run `workflow_dispatch` via API. You can also run it manually using the `Run workflow` button and set `abi` (default `aarch64`).
- The workflow downloads Android NDK r25b and runs the build script in `.github/scripts/build-hal.sh`.
- On success the workflow uploads an artifact named `audio-hal-so` containing `libaudio.primary.default.so`.

Lightweight build (no full LineageOS source)

To avoid pulling the full LineageOS source, this repo includes a minimal set of header stubs under `include/` so the HAL skeleton can be built on GitHub runners using only the Android NDK. The workflow will now produce an artifact named `audio-hal-so` containing `libaudio.primary.msm8953.so` suitable for quick testing on device.

Caveats
- This builds a minimal stub .so intended for testing. It does not implement Android audio HAL v1/v2 interfaces required in a real device.
- Cross-compiling with only the NDK is significantly simpler than building full LineageOS 22.1 (which requires the entire source tree and Soong build). If you need full Soong/AOSP compatibility you must build in an environment with the full source tree (not feasible on GitHub-hosted runners due to size/time limits).
- The produced .so may still need correct SONAME/symbols and vendor-specific headers if you intend to load it on device. Expect to iterate.

Next steps
- If you want, I can adapt the build script to produce other naming conventions (e.g. `libaudio.primary.default.so` vs `libaudio.primary.<variant>.so`) or build additional ABIs.
- If you provide the exact LineageOS 22.1 audio HAL headers / function signatures you need, I can implement a proper audio primary HAL skeleton that compiles cleanly against that version.

Build inside LineageOS 22.1 tree

The sources in this repo now contain a minimal legacy audio primary HAL skeleton and Android.bp modules named `audio.primary.msm8953`. To build the module inside a LineageOS 22.1 (Android 13) source tree and produce the shared object named like `libaudio.primary.msm8953.so`:

1. Copy/overlay the `hardware/qcom-caf/msm8953/audio` (or `hardware/qcom/audio`) folder into your LineageOS 22.1 source root so the path matches.
2. In the LineageOS build env:

```bash
source build/envsetup.sh
lunch <your_red7a_lunch>
mmm hardware/qcom-caf/msm8953/audio -j1 || mmm hardware/qcom/audio -j1
```

3. On success, the produced library will be in the output folder (e.g., `out/target/product/<device>/system/lib64/libaudio.primary.msm8953.so` or similar depending on your build configuration). You can also search the out tree for the produced file.

Replacing on device

- Push the built .so to the device's vendor or system partition where the platform expects the HAL (paths vary by device/vendor). For testing, you can push to `/vendor/lib64/` or `/system/lib64/` depending on your device and mount state:

```bash
adb root
adb remount
adb push out/.../libaudio.primary.msm8953.so /vendor/lib64/
adb shell stop
adb shell start
```

- Please be careful: 替换 HAL 可能导致音频服务启动失败或系统崩溃，建议先做备份并在可恢复的测试设备上操作。

