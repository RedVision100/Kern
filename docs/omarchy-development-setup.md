# Kern — New Omarchy Development Environment Setup

**Date:** August 30, 2026  
**System:** Omarchy (Arch Linux)  
**Repository:** `~/Projects/Kern`

This document records the complete setup and verification process used to establish the Kern development environment on a new Omarchy system.

The goal was to reproduce the existing Kern development environment without disturbing the existing GitHub work, then verify both the firmware build and desktop simulator.

---

## 1. Git and GitHub SSH Setup

### Check for existing SSH configuration

```bash
ls -la ~/.ssh
```

On the new system, `~/.ssh` did not exist.

### Start the SSH agent

```bash
eval "$(ssh-agent -s)"
```

### Add the GitHub SSH key

A new Ed25519 key was created for GitHub and added to the SSH agent:

```bash
ssh-add ~/.ssh/id_ed25519
```

The key was associated with the GitHub account `RedVision100`.

### Test GitHub authentication

```bash
ssh -T git@github.com
```

The first connection asks whether to trust GitHub's host key. Answer:

```text
yes
```

Successful authentication produced:

```text
Hi RedVision100! You've successfully authenticated, but GitHub does not provide shell access.
```

This confirmed that GitHub SSH authentication was working.

---

## 2. Clone Kern

The repository was placed in:

```text
~/Projects/Kern
```

The repository's Git configuration was verified:

```bash
git remote -v
```

Expected result:

```text
origin  git@github.com:RedVision100/Kern.git (fetch)
origin  git@github.com:RedVision100/Kern.git (push)
```

The working tree initially contained the `master` branch and was clean:

```bash
git status
```

Expected:

```text
On branch master
Your branch is up to date with 'origin/master'.
nothing to commit, working tree clean
```

---

## 3. Configure the Upstream Repository

The upstream Kern repository is:

```text
git@github.com:odudex/Kern.git
```

Fetch upstream:

```bash
git fetch upstream
```

This populated the upstream branches and tags, including:

```text
upstream/master
upstream/chore/idf-6.1
upstream/hw-pbkdf2
upstream/misc
upstream/quantum-benchmark
```

The upstream repository should remain separate from the personal fork:

```text
origin   = RedVision100/Kern
upstream = odudex/Kern
```

---

## 4. Initialize Git Submodules

From the Kern repository:

```bash
git submodule update --init --recursive
```

This initialized:

```text
components/cUR
components/k_quirc
components/libwally-core/upstream
components/libwally-core/upstream/src/secp256k1
```

Verify:

```bash
git submodule status
```

The submodules should show checked-out commits rather than missing entries.

---

## 5. Verify the Base Development Tools

The initial environment check showed:

```text
OS       = Omarchy
Git      = 2.55.0
CMake    = installed
Just     = not initially installed
Python   = 3.14.7
clangd   = 22.1.8
ESP-IDF  = not initially configured
```

Install the required host build tools with Arch's package manager:

```bash
sudo pacman -S --needed cmake ninja base-devel just
```

Verify:

```bash
cmake --version
just --version
```

The resulting versions were:

```text
CMake 4.4.2
just 1.58.0
```

Do **not** use the Ubuntu/Debian `apt` commands on Omarchy.

---

## 6. Install ESP-IDF

Kern targets **ESP-IDF 6.0.2** and the **ESP32-P4**.

ESP-IDF was installed under:

```text
~/esp/esp-idf
```

After installation, load the ESP-IDF environment with:

```bash
source ~/esp/esp-idf/export.sh
```

Verify:

```bash
idf.py --version
```

Expected:

```text
ESP-IDF v6.0.2
```

Verify the environment path:

```bash
echo $IDF_PATH
```

Expected:

```text
/home/rca/esp/esp-idf
```

---

## 7. Automatically Load ESP-IDF in New Terminals

The Omarchy shell uses Bash.

The `.bashrc` file was edited to load ESP-IDF automatically:

```bash
source "$HOME/esp/esp-idf/export.sh"
```

The existing Omarchy environment/bootstrap configuration and Yazi function were preserved.

The final structure included the Yazi function followed by:

```bash
source "$HOME/esp/esp-idf/export.sh"
```

After editing, validate Bash syntax:

```bash
bash -n ~/.bashrc
```

No output means the syntax check passed.

Opening a new terminal should now automatically initialize ESP-IDF and display its activation messages.

This means:

```bash
idf.py --version
```

is available immediately in a new terminal.

---

## 8. Configure Kern for ESP32-P4

From:

```text
~/Projects/Kern
```

run:

```bash
idf.py set-target esp32p4
```

This also performs the required initial clean/configuration.

The configuration completed successfully and generated:

```text
build/
sdkconfig
```

The output confirmed:

```text
ESP-IDF 6.0.2
Target: esp32p4
```

It also generated the development signing key:

```text
dev_signing_key.pem
```

### Important dependency note

ESP-IDF reported that several managed dependencies have newer versions available.

Do **not** automatically run:

```bash
idf.py update-dependencies
```

during a clean workstation setup.

Kern contains:

```text
dependencies.lock
```

The locked versions should be preserved unless dependency updates are intentionally part of the development work.

---

## 9. Install Simulator Host Dependency

The first simulator build failed because the host system did not have mbedTLS development files.

Kern's CMake error suggested:

```text
sudo apt install libmbedtls-dev
```

That command is for Debian/Ubuntu and is **not appropriate for Omarchy**.

On Omarchy/Arch, install:

```bash
sudo pacman -S --needed mbedtls
```

Then rebuild the simulator.

---

## 10. Inspect Available Kern `just` Commands

Run:

```bash
just --list
```

Available recipes included:

```text
build
clean
dep
flash
format
monitor
sim
sim-build
sim-build-webcam
sim-clean
sim-no-cam
sim-qr
sim-reset
site
test
```

The default board for these recipes is:

```text
wave_4b
```

For example:

```bash
just build
```

uses:

```text
board = "wave_4b"
```

Do not assume the board name itself is a `just` recipe. `wave_4b` is the default value of the `board` parameter for the build-related recipes.

---

## 11. Verify the Firmware Build on `master`

The initial firmware build was performed from the clean `master` branch.

```bash
idf.py fullclean
```

Then:

```bash
just build
```

The build completed successfully.

The generated firmware included:

```text
build_wave_4b/kern.bin
```

This verified that the ESP32-P4 compiler, ESP-IDF, CMake, Ninja, managed components, and Kern source tree were all functioning together.

---

## 12. Build the Desktop Simulator

Build the simulator:

```bash
just sim-build
```

After installing the Arch `mbedtls` package, the simulator built successfully:

```text
[100%] Built target kern_simulator
```

This verified the host-side simulator toolchain.

---

## 13. Run the Simulator

Launch the simulator:

```bash
just sim
```

The simulator opened successfully with camera support.

This confirmed that the desktop simulator and its camera path were functioning on the new Omarchy system.

---

## 14. Restore the Previous Accessibility Development Branch

The previous development work had already been pushed to the GitHub fork.

Check branches:

```bash
git branch -a
```

Relevant remote branches included:

```text
origin/feat/accessibility-text-size
origin/notes
origin/linux-dev-setup
```

Fetch the latest origin state:

```bash
git fetch origin
```

The existing accessibility branch was then checked out locally:

```bash
git switch --track -c feat/accessibility-text-size origin/feat/accessibility-text-size
```

Verify tracking:

```bash
git branch -vv
```

The result showed:

```text
* feat/accessibility-text-size
  c99d1c7 [origin/feat/accessibility-text-size] feat(ui): add accessibility text size support
```

---

## 15. Verify the Previous Accessibility Commits

The branch history was:

```text
c99d1c7 feat(ui): add accessibility text size support
f392f3d feat(ui): add text size setting
56e98be docs: add development workflow guides
68be355 feat(crypto): mix an auxiliary entropy pool into crypto_random_bytes
6bf9ad2 fix(crypto): make crypto_random_bytes failures detectable
```

The branch was confirmed clean:

```bash
git status
```

Result:

```text
On branch feat/accessibility-text-size
Your branch is up to date with 'origin/feat/accessibility-text-size'.
nothing to commit, working tree clean
```

No previous development work needed to be copied manually.

---

## 16. Build the Accessibility Branch

With:

```text
feat/accessibility-text-size
```

checked out, run:

```bash
just build
```

The firmware build completed successfully.

The resulting Kern binary was:

```text
kern.bin
```

with:

```text
0x1d1000 bytes
```

The smallest app partition was:

```text
0x600000 bytes
```

with approximately:

```text
70% free
```

This verified that the previous accessibility changes compile correctly under the new Omarchy development environment.

---

## 17. Rebuild and Test the Accessibility Simulator

Build the simulator on the accessibility branch:

```bash
just sim-build
```

Then run:

```bash
just sim
```

The simulator launched successfully.

The previously implemented **font-size/accessibility changes were present and working**.

This was the final verification that the previous Kern development state was successfully restored on the new machine.

---

# Final Verified Environment

At the end of the setup, the following components were verified:

| Component | Status |
|---|---|
| Omarchy | Working |
| Bash | Working |
| Git 2.55.0 | Working |
| GitHub SSH authentication | Working |
| `origin` fork | Working |
| `upstream` remote | Working |
| Git submodules | Initialized |
| CMake 4.4.2 | Working |
| Ninja 1.13.2 | Working |
| Just 1.58.0 | Working |
| Python 3.14.7 | Working |
| clangd 22.1.8 | Installed |
| ESP-IDF 6.0.2 | Working |
| ESP32-P4 toolchain | Working |
| Kern firmware build | Successful |
| Desktop simulator | Successful |
| Simulator camera | Working |
| Accessibility branch | Restored |
| Accessibility firmware build | Successful |
| Accessibility simulator test | Successful |
| Git working tree | Clean |

---

# Normal Kern Development Workflow

Open a new terminal. ESP-IDF should load automatically.

Go to Kern:

```bash
cd ~/Projects/Kern
```

Check the current branch:

```bash
git status
```

Fetch remote changes when needed:

```bash
git fetch origin
git fetch upstream
```

Build firmware:

```bash
just build
```

Build simulator:

```bash
just sim-build
```

Run simulator:

```bash
just sim
```

Run tests:

```bash
just test
```

See all available commands:

```bash
just --list
```

---

# Git Branch Safety

Before beginning new work:

```bash
git status
git branch -vv
```

Keep the repository clean before switching branches.

For new development, create a branch from the intended base:

```bash
git switch master
git pull --ff-only origin master
git switch -c <new-branch-name>
```

Do not develop directly on `master`.

The personal fork remains:

```text
origin
git@github.com:RedVision100/Kern.git
```

The project repository remains:

```text
upstream
git@github.com:odudex/Kern.git
```

---

# Important Setup Notes

1. **Kern currently targets ESP-IDF 6.0.2.** Do not upgrade ESP-IDF casually.
2. **Do not update managed dependencies without a deliberate reason.** Kern uses `dependencies.lock`.
3. **Omarchy is Arch-based.** Use `pacman`, not Ubuntu/Debian `apt` instructions.
4. **The simulator is host software.** Its dependencies are separate from the ESP32 firmware toolchain.
5. **Do not flash physical hardware merely to verify the development environment.** The firmware and simulator builds provide a safer first verification.
6. **The accessibility branch already exists on GitHub.** It should be checked out from `origin/feat/accessibility-text-size`, not recreated.
7. **The working tree was clean at the end of setup.** This is the baseline for future development.

---

# Baseline

**Repository:** `~/Projects/Kern`

**Active development branch at completion:**

```text
feat/accessibility-text-size
```

**HEAD:**

```text
c99d1c7 feat(ui): add accessibility text size support
```

**ESP-IDF:**

```text
v6.0.2
```

**Target:**

```text
esp32p4
```

**Default board:**

```text
wave_4b
```

**Firmware:** builds successfully.

**Simulator:** builds and runs successfully.

**Accessibility/font-size changes:** confirmed working in the simulator.

