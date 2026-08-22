# Kern Simulator Development

The Kern desktop simulator runs the Kern LVGL user interface in an SDL2 window and provides mouse-as-touch input.

Use the simulator for development and functional testing. It is not a replacement for physical Kern hardware.

> **Security warning:** The simulator is non-trusted development software. Do not run it on a machine containing Bitcoin private keys, seed phrases, GPG private keys, SSH private keys, password databases, or other sensitive credentials. A dedicated development machine is strongly recommended.

## Requirements

The simulator requires the Linux build toolchain and the SDL2/mbedTLS development libraries described below. The exact requirements may change; check `simulator/README.md`.

## 1. Simulator Prerequisites

On Debian/Ubuntu-based Linux systems:

```bash
sudo apt install build-essential cmake libsdl2-dev libmbedtls-dev
```

The exact requirements may change. Check:

```text
simulator/README.md
```

for the current simulator requirements.

## 2. Build the Simulator

From the repository root:

```bash
cd Kern/simulator
```

Configure a Debug build:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
```

Build:

```bash
cmake --build build -- -j$(nproc)
```

Kern also provides `just` recipes from the repository root when available:

```bash
just sim-build
```

## 3. Run the Simulator

From the repository root:

```bash
./simulator/build/kern_simulator
```

The simulator should open an SDL2 window containing the Kern LVGL interface.

Mouse input acts as simulated touch input.

To see available command-line options:

```bash
./simulator/build/kern_simulator --help
```

## 4. Development Use

Keep simulator changes on a development branch rather than directly on `master`.

The simulator is useful for:

- UI development
- Layout testing
- Navigation testing
- QR workflow testing
- Transaction workflow testing
- Functional testing without physical hardware

A useful development loop is:

```text
change code
   ↓
build
   ↓
run simulator
   ↓
test interaction
   ↓
inspect result
   ↓
repeat
```

## 5. Simulator Data

The simulator uses local simulated storage:

```text
simulator/sim_data/
```

The documented structure includes:

```text
simulator/sim_data/
├── nvs/
├── spiffs/
└── sdcard/
```

Settings and simulated state can persist between runs.

To reset the simulator to a factory-like state:

```bash
rm -rf simulator/sim_data
```

Only do this after confirming that no test data needs to be retained.

## 6. QR Image Testing

A QR image can be supplied directly:

```bash
./simulator/build/kern_simulator --qr-image path/to/qr.png
```

A directory of QR images can also be supplied:

```bash
./simulator/build/kern_simulator --qr-dir path/to/qr-images/
```

This is useful for testing QR workflows without a physical camera.

## 7. Webcam Support

The simulator can use a real webcam for QR scanning and entropy capture.

Build with webcam support:

```bash
cd simulator

cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSIM_WEBCAM=ON

cmake --build build -- -j$(nproc)
```

Run:

```bash
./build/kern_simulator --webcam
```

On Linux, webcam access may require membership in the `video` group:

```bash
sudo usermod -aG video $USER
```

Log out and back in after changing group membership.

## 8. `just` Simulator Recipes

From the repository root, the documented recipes include:

```bash
just sim-build
```

Run with webcam support:

```bash
just sim
```

Run without camera support:

```bash
just sim-no-cam
```

Use:

```bash
just --list
```

or inspect the repository's `justfile` if you need to verify current recipe names.

## 9. Running Over SSH

When using X forwarding over SSH, SDL may display a white screen.

Try software rendering:

```bash
SDL_VIDEODRIVER=x11 SDL_RENDER_DRIVER=software \
  ./simulator/build/kern_simulator
```

The current `just` recipes may configure these variables depending on the Kern implementation.

## 10. Simulator Limitations

Simulator results must not be treated as proof that physical hardware behaves identically.

Known differences include:

- The simulator uses the host system's mbedTLS rather than necessarily using the exact ESP-IDF environment used by physical hardware.
- Simulator cryptographic output must not be assumed to be bit-for-bit identical to hardware.
- Development-only eFuse and HMAC behavior is not equivalent to hardware.
- PPA behavior may differ from physical hardware.
- Webcam behavior differs between operating systems.
- Simulator storage is local host storage rather than physical device storage.

The simulator demonstrates behavior in the simulated environment. Security-sensitive behavior still requires appropriate testing and review on physical hardware.

## 11. Clean Rebuild

If the simulator build becomes inconsistent, remove only its build directory:

```bash
rm -rf simulator/build
```

Then rebuild:

```bash
cmake -B simulator/build -S simulator \
  -DCMAKE_BUILD_TYPE=Debug

cmake --build simulator/build -- -j$(nproc)
```

Do not remove `simulator/sim_data/` unless you intentionally want to reset simulator state.

## 12. Troubleshooting

### Build fails

Check:

```text
simulator/README.md
```

Then verify the required packages are installed.

Try a clean simulator build:

```bash
rm -rf simulator/build

cmake -B simulator/build -S simulator \
  -DCMAKE_BUILD_TYPE=Debug

cmake --build simulator/build -- -j$(nproc)
```

### QR image testing fails

Verify:

- The simulator is running.
- The QR data is valid.
- The correct image path was supplied.
- The image contains the expected QR data.

Example:

```bash
./simulator/build/kern_simulator --qr-image path/to/qr.png
```

### Webcam testing fails

Verify:

- The simulator was built with `SIM_WEBCAM=ON`.
- The webcam is connected.
- The current user has access to the video device.
- The required OS permissions are available.

### Simulator starts but appears blank over SSH

Try:

```bash
SDL_VIDEODRIVER=x11 SDL_RENDER_DRIVER=software \
  ./simulator/build/kern_simulator
```

## 13. Simulator vs. Hardware

Use the simulator for:

- UI development
- Layout and navigation
- Functional workflows
- Reproducible QR testing

Use physical hardware for appropriate:

- Signing behavior
- Key handling
- Hardware-specific behavior
- Security validation

The rule is:

```text
Simulator = development environment
Hardware   = hardware validation
```

## 14. Testnet

For transaction development, use Testnet rather than real Bitcoin funds.

See [Testnet Testing](testnet-testing.md) for the complete wallet/PSBT workflow.
