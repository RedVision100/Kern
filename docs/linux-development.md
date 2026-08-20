# Kern Linux Development Guide

This guide documents a Linux development environment and workflow for
developing, building, testing, and maintaining the Kern Bitcoin signer
firmware and desktop simulator.

The goal is to provide a repeatable development environment, a safe Git
workflow, and practical procedures for testing Kern on Linux.

This guide distinguishes between:

- Requirements and workflows provided by the Kern project
- Optional development tools and editor configurations
- Development workflows that have been verified with the Kern simulator

---

# 1. Development Environment

Kern development on Linux can use the following tools and components:

- Linux
- Git
- GitHub
- SSH authentication
- CMake
- ESP-IDF
- `just`
- Kern Git submodules
- Kern desktop simulator
- `clangd` for C/C++ language tooling

The exact Linux distribution and editor are not requirements of the Kern
project.

The development machine should not contain sensitive credentials or private
keys when running untrusted development or simulator code.

---

# 2. GitHub Repository Model

Kern development can use two Git remotes.

## `origin`

`origin` normally refers to the developer's personal GitHub fork.

Example:

    git@github.com:YOUR-USERNAME/Kern.git

## `upstream`

`upstream` refers to the official Kern repository.

Example:

    git@github.com:odudex/Kern.git

Check the configured remotes:

    git remote -v

A typical configuration looks like:

    origin    git@github.com:YOUR-USERNAME/Kern.git (fetch)
    origin    git@github.com:YOUR-USERNAME/Kern.git (push)
    upstream  git@github.com:odudex/Kern.git (fetch)
    upstream  git@github.com:odudex/Kern.git (push)

## Recommended Workflow

Normally:

- Fetch updates from `upstream`
- Keep local `master` synchronized with `upstream/master`
- Develop on a feature or development branch
- Push development branches to `origin`
- Do not push directly to the official `upstream` repository unless
  authorized

This keeps the personal fork separate from the official repository while
allowing development to remain synchronized with upstream.

---

# 3. GitHub SSH Authentication

SSH allows Git to communicate with GitHub without entering a password for
every operation.

The recommended key type is Ed25519.

Generate a key if one does not already exist:

    ssh-keygen -t ed25519 -C "your-email@example.com"

Start the SSH agent:

    eval "$(ssh-agent -s)"

Add the private key:

    ssh-add ~/.ssh/id_ed25519

Verify that the key is loaded:

    ssh-add -l

The public key is:

    ~/.ssh/id_ed25519.pub

Add the public key to the appropriate GitHub account's SSH key settings.

Test authentication:

    ssh -T git@github.com

A successful authentication normally reports:

    Hi USERNAME! You've successfully authenticated, but GitHub does not
    provide shell access.

That message indicates successful SSH authentication.

## Security

Never share:

- `~/.ssh/id_ed25519`
- SSH private keys
- Bitcoin seed material
- passwords
- other private credentials

Only the `.pub` file is intended to be shared with GitHub.

---

# 4. Clone and Verify the Kern Repository

Clone the repository using the appropriate GitHub URL.

For example:

    git clone git@github.com:YOUR-USERNAME/Kern.git

Enter the repository:

    cd Kern

If working with both a personal fork and the official repository, add the
official repository as `upstream`:

    git remote add upstream git@github.com:odudex/Kern.git

Verify the repository:

    git status

Check recent commits:

    git log --oneline -10

Check configured remotes:

    git remote -v

Initialize and synchronize submodules:

    git submodule update --init --recursive

---

# 5. Git Branch Strategy

The local `master` branch should normally remain a clean synchronization
point.

Do not normally develop directly on `master`.

Create a development branch:

    git switch -c linux-dev-setup

For feature work, use a descriptive name:

    git switch -c feature/example-change

Other examples:

    git switch -c fix/qr-scanning
    git switch -c feature/ui-improvement
    git switch -c docs/linux-development

Verify the active branch:

    git status

A clean development branch should report:

    On branch linux-dev-setup
    nothing to commit, working tree clean

Keep the branch names descriptive enough to identify their purpose.

---

# 6. Keeping Kern Synchronized

Before beginning new development, fetch updates from the official Kern
repository:

    git fetch upstream

View available remote branches:

    git branch -r

The official master branch should appear as:

    upstream/master

Compare local `master` with upstream:

    git log --oneline master..upstream/master

This displays commits that exist upstream but not on local `master`.

Compare the GitHub fork with upstream:

    git log --oneline origin/master..upstream/master

---

# 7. Updating Local Master

Only update `master` when the working tree is clean.

Check:

    git status

Switch to master:

    git switch master

Update using a fast-forward-only merge:

    git merge --ff-only upstream/master

Using `--ff-only` prevents Git from creating an unexpected merge commit.

After updating:

    git status

Verify the current commit:

    git log --oneline -1

A typical synchronization sequence is:

    git fetch upstream
    git switch master
    git merge --ff-only upstream/master
    git submodule update --init --recursive

---

# 8. Kern Git Submodules

Kern uses Git submodules.

Important submodules include:

    components/cUR
    components/k_quirc
    components/libwally-core

The exact set of submodules may change as the project evolves.

After changing Kern revisions or cloning the repository, synchronize the
submodules:

    git submodule update --init --recursive

This checks out the exact revisions required by the current Kern commit.

Check submodule state:

    git submodule status

Then check the repository:

    git status

A properly synchronized repository should not show the submodules as
modified.

## Important

Do not assume that updating the main Kern repository automatically checks
out the correct submodule revisions.

Use:

    git submodule update --init --recursive

after changing Kern revisions or cloning the repository.

---

# 9. Updating the GitHub Fork

Once local `master` has been successfully updated from upstream and verified,
the GitHub fork can be synchronized:

    git push origin master

Verify:

    git status

The local master should no longer report that it is ahead of
`origin/master`.

## Safe Synchronization Model

    Official Kern
          |
          | git fetch upstream
          v
    Local master
          |
          | git push origin master
          v
    GitHub fork

Development branches should normally be pushed separately:

    git push -u origin <development-branch>

---

# 10. Optional Editor: Sublime Text

Sublime Text is the preferred editor used in this Linux development
environment.

**Sublime Text is not required by the Kern project.**

Developers may use any editor or IDE that supports the Kern source tree and
their preferred C/C++ development workflow.

The following sections document the Sublime Text configuration used for
this development environment.

---

# 11. Installing Sublime Text

On Debian/Ubuntu-based Linux distributions, Sublime Text can be installed
from the official Sublime Text repository.

Install prerequisites:

    sudo apt install curl

Import the repository signing key:

    curl -fsSL https://download.sublimetext.com/sublimehq-pub.gpg | \
    gpg --dearmor | \
    sudo tee /usr/share/keyrings/sublimehq-archive.gpg > /dev/null

Add the stable repository:

    echo "deb [signed-by=/usr/share/keyrings/sublimehq-archive.gpg] \
    https://download.sublimetext.com/ apt/stable/" | \
    sudo tee /etc/apt/sources.list.d/sublime-text.list

Update package information:

    sudo apt update

Install Sublime Text:

    sudo apt install sublime-text

Verify:

    subl --version

The exact Sublime Text build will depend on the current release.

---

# 12. Opening Kern in Sublime Text

From the Kern repository:

    cd Kern

Open the repository:

    subl .

The sidebar should contain directories such as:

    components/
    docs/
    main/
    simulator/
    tools/

and project files such as:

    CMakeLists.txt
    README.md
    .clangd

---

# 13. Sublime Text Settings

The following settings were used for this development environment:

    {
        "font_size": 24,
        "ignored_packages":
        [
            "Vintage",
        ],
        "tab_size": 4,
        "translate_tabs_to_spaces": true,
        "detect_indentation": false,
        "word_wrap": false,
        "show_minimap": false,
        "highlight_line": true,
        "draw_white_space": "selection"
    }

These settings are personal preferences rather than Kern requirements.

The font size can be adjusted according to display size and preference.

---

# 14. Dark Mode

Sublime Text includes built-in dark themes and color schemes.

A suitable starting configuration is:

- Theme: Adaptive
- Color Scheme: Mariana

No third-party theme is required.

---

# 15. Package Control

Package Control is Sublime Text's package manager.

It can be installed through:

    Ctrl+Shift+P

Search for:

    Install Package Control

Once installed, packages can be installed with:

    Ctrl+Shift+P

then:

    Package Control: Install Package

Package Control may install packages in the background.

If the interface does not appear to respond immediately, check the Sublime
console before repeating the installation.

Open the console with:

    Ctrl+`

---

# 16. clangd

`clangd` provides C/C++ language intelligence.

Install it on Debian/Ubuntu-based systems:

    sudo apt install clangd

Verify:

    clangd --version

The exact version depends on the Linux distribution.

Kern's `.clangd` file provides project-specific configuration when the
language server is operating correctly.

---

# 17. Sublime LSP

Sublime's LSP package provides the language-server framework used to connect
Sublime Text with `clangd`.

Install the package using Package Control.

Search for:

    LSP

The package should appear as:

    LSP

After installation, the command palette should contain commands beginning
with:

    LSP:

Do not create a competing project configuration unless there is a specific
reason to do so. Kern's existing `.clangd` configuration should be used.

---

# 18. LSP-clangd

Install the Sublime package:

    LSP-clangd

This connects Sublime's LSP framework to the system `clangd` executable.

After installation, open a Kern C source file such as:

    main/core/psbt.c

Allow the language server several seconds to initialize.

---

# 19. Verifying clangd

Open:

    main/core/psbt.c

Find a known function, for example:

    psbt_scriptpubkey_to_address

Place the cursor on the function name.

Use:

    F12

or the appropriate LSP "Go to Definition" command.

A successful setup should allow navigation to the symbol definition.

The LSP troubleshooting command can also be used:

    LSP: Troubleshoot Server

The clangd server test should complete successfully.

This verifies the chain:

    Sublime Text
         |
         v
       LSP
         |
         v
    LSP-clangd
         |
         v
      clangd
         |
         v
    Kern .clangd
         |
         v
    Kern source tree

---

# 20. Useful Sublime Text Navigation

Useful commands while working through Kern include:

### Find

    Ctrl+F

### Find in Files

    Ctrl+Shift+F

### Go to Symbol

    Ctrl+R

### Go to Definition

    F12

### Command Palette

    Ctrl+Shift+P

### Console

    Ctrl+`

These commands are particularly useful when tracing Kern's C code.

---

# 21. Kern Desktop Simulator

The Kern desktop simulator runs the Kern LVGL user interface in an SDL2
window and provides mouse-as-touch input.

The simulator is useful for:

- UI development
- Functional testing
- QR workflows
- Transaction workflow testing
- Development without physical hardware

## Security Warning

The simulator must be treated as non-trusted development software.

Do not run it on a machine containing:

- Bitcoin private keys
- Bitcoin seed phrases
- GPG private keys
- SSH private keys
- Password databases
- Other sensitive credentials

The simulator contains development-only implementations and does not provide
the same security properties as physical Kern hardware.

A dedicated development machine is strongly recommended.

---

# 22. Simulator Prerequisites

On Debian/Ubuntu-based systems:

    sudo apt install build-essential cmake libsdl2-dev libmbedtls-dev

The exact prerequisites may change as the simulator evolves. Refer to
`simulator/README.md` in the Kern repository for the current requirements.

---

# 23. Building the Simulator

From the Kern repository:

    cd simulator

Configure:

    cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

Build:

    cmake --build build -- -j$(nproc)

Or use the Kern `just` recipe from the repository root:

    just sim-build

---

# 24. Running the Simulator

From the repository root:

    ./simulator/build/kern_simulator

The `just` recipes can also be used.

With webcam support:

    just sim

Without camera support:

    just sim-no-cam

The simulator can also be run directly with additional options.

Display the available options:

    ./simulator/build/kern_simulator --help

---

# 25. Simulator QR Testing

A QR image can be supplied directly:

    ./simulator/build/kern_simulator --qr-image path/to/qr.png

A directory of QR images can also be used:

    ./simulator/build/kern_simulator --qr-dir path/to/qr-images/

The simulator can cycle through QR images in the supplied directory.

This is useful for testing QR encoding and decoding without a physical
camera.

---

# 26. Simulator Data

The simulator uses a local data directory for simulated storage.

The default location is:

    simulator/sim_data/

It contains simulated storage such as:

    nvs/
    spiffs/
    sdcard/

Settings persist between simulator runs.

To reset the simulator to its factory-like state, remove the simulator data
directory after verifying that no test data needs to be retained:

    rm -rf simulator/sim_data

Do not remove simulator data containing anything that needs to be preserved.

---

# 27. Simulator Webcam Support

The simulator can use a real webcam for QR scanning and entropy capture.

Build with webcam support:

    cd simulator
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DSIM_WEBCAM=ON
    cmake --build build -- -j$(nproc)

Run:

    ./build/kern_simulator --webcam

On Linux, the user may need access to the video device.

Add the current user to the video group:

    sudo usermod -aG video $USER

Log out and back in after changing group membership.

The exact webcam implementation may differ between operating systems.

---

# 28. Simulator Over SSH

When using X forwarding over SSH, SDL may display a white screen.

The following environment variables force software rendering:

    SDL_VIDEODRIVER=x11 SDL_RENDER_DRIVER=software \
    ./simulator/build/kern_simulator

The `just sim` recipes may already configure these variables depending on
the current Kern implementation.

---

# 29. Simulator Limitations

The simulator is not a replacement for physical Kern hardware.

Important differences include:

- The simulator uses the host system's mbedTLS rather than necessarily using
  the exact ESP-IDF environment used by physical hardware.
- Simulator cryptographic output must not be assumed to be bit-for-bit
  identical to hardware.
- Development-only eFuse and HMAC behavior is not equivalent to hardware.
- PPA behavior may differ from physical hardware.
- Webcam behavior differs between operating systems.
- Simulator storage is local host storage rather than physical device
  storage.

Do not use simulator output as evidence that a security-sensitive operation
is equivalent to operation on a physical Kern device.

---

# 30. Testnet Development with Coconut Wallet

Testnet should be used for wallet and transaction development whenever
possible.

Never use real Bitcoin mainnet funds for simulator testing.

The workflow described in this section is a development workflow that has
been successfully tested using the Kern simulator and Coconut Wallet.

Coconut Wallet is **not a Kern project requirement**.

It is an example wallet used to test Bitcoin transaction and PSBT
interoperability.

---

# 31. Configuring the Kern Simulator for Testnet

The Kern simulator starts with the network configured for Mainnet.

Before performing Testnet development:

1. Start the Kern simulator.
2. Open **Settings**.
3. Locate the **Network** setting.
4. Change the network from **Mainnet** to **Testnet**.
5. Verify that the simulator is operating on Testnet.

The network can be changed through the running simulator's user interface.
No source-code modification is required.

This distinction is important because changing the network in the simulator
is different from modifying the simulator source code.

---

# 32. Coconut Wallet Testnet Setup

Coconut Wallet was successfully used on an iPhone/iPad as the wallet side of
a Testnet PSBT workflow.

Before testing, verify that:

- Coconut Wallet is configured for Testnet.
- Kern is configured for Testnet.
- The transaction being tested is a Testnet transaction.
- No mainnet funds are being used.

The wallet and signer must be operating on the same Bitcoin network.

---

# 33. Testnet PSBT Workflow

A typical development workflow is:

    Coconut Wallet
          |
          | Testnet transaction / PSBT
          v
       QR Code
          |
          v
    Kern Simulator
          |
          | Review and sign
          v
     Signed PSBT
          |
          | QR Code
          v
    Coconut Wallet

This workflow allows wallet and signer interoperability to be tested without
using real Bitcoin.

The exact QR interaction may change as Kern or Coconut Wallet evolves.

---

# 34. Verified Simulator/Testnet Workflow

A typical Testnet development session is:

1. Start the Kern simulator.
2. Open Settings.
3. Open the Network setting.
4. Change Mainnet to Testnet.
5. Verify the simulator is on Testnet.
6. Open Coconut Wallet on an iPhone or iPad.
7. Verify Coconut Wallet is also configured for Testnet.
8. Create or prepare a Testnet transaction.
9. Export the transaction as a PSBT/QR.
10. Scan the PSBT with Kern.
11. Review the transaction in the simulator.
12. Sign the transaction in the simulator.
13. Return the signed PSBT to Coconut Wallet using the appropriate QR
    workflow.
14. Verify the resulting transaction in the wallet.

Always verify the network before beginning a new testing session.

Do not assume the simulator is still configured for Testnet because it was
configured that way during a previous session.

---

# 35. Testnet Safety

Testnet coins have no monetary value, but the signing workflow should still
be treated seriously.

Testnet can be used to verify:

- PSBT parsing
- Transaction review
- Fee handling
- Input and output handling
- QR encoding and decoding
- Signing workflow
- Wallet interoperability
- Simulator UI behavior

Testnet testing does not replace a security review or testing on actual
Kern hardware.

---

# 36. Understanding `less`

Many Git commands open the `less` pager when there is a large amount of
output.

For example:

    git log

To exit `less`:

    q

Useful navigation:

    Space       page down
    b           page up
    /text       search
    n           next search result
    q           quit

Do not use Ctrl+C to exit normal `less` viewing unless necessary.

---

# 37. Checking Repository State

Before making changes:

    git status

A clean repository should report:

    nothing to commit, working tree clean

After editing:

    git status

Review changed files carefully before staging anything.

For a detailed diff:

    git diff

For staged changes:

    git diff --cached

Check recent commits:

    git log --oneline -10

---

# 38. Accidental Files and Git Mistakes

If `git status` reports an unexpected untracked file, do not immediately
delete it.

Inspect it first.

Examples:

    ls -l

    file "filename"

    cat "filename"

If a filename contains spaces or unusual characters, quote it:

    file 'filename'

Only remove an accidental file after confirming that it is not needed.

If a tracked file was accidentally modified and the changes should be
discarded, inspect the diff first:

    git diff -- path/to/file

Then restore only after confirming that the changes are unwanted:

    git restore path/to/file

---

# 39. Before Starting Development

Enter the Kern repository:

    cd Kern

Check the current state:

    git status

Confirm:

- The correct branch is active
- The working tree is clean
- Upstream has been fetched recently
- Local `master` is current
- Submodules are synchronized

A typical synchronization sequence is:

    git fetch upstream
    git switch master
    git merge --ff-only upstream/master
    git submodule update --init --recursive
    git switch <development-branch>

Then verify:

    git status

---

# 40. Before Closing for the Day

Before shutting down the development machine:

    git status

Review:

    git diff

If work is complete and should be saved remotely:

    git add <files>
    git commit -m "description of change"
    git push origin <development-branch>

If work is unfinished, do not create meaningless commits simply to make the
working tree clean.

Instead, leave a clear note about the current state or use an appropriate
development commit if the work is logically checkpointed.

After committing, verify:

    git status

Confirm that the expected branch and remote are being used.

---

# 41. Recommended Daily Workflow

A typical development session is:

    cd Kern

    git fetch upstream

    git status

    git switch master
    git merge --ff-only upstream/master
    git submodule update --init --recursive

    git switch <development-branch>

Open the project in the preferred editor.

Make and test changes.

Review:

    git status
    git diff

Stage and commit:

    git add <files>
    git commit -m "description"

Push the development branch:

    git push -u origin <development-branch>

---

# 42. Important Security Practices

Kern is Bitcoin signing software.

Treat development and simulator code as untrusted unless independently
reviewed.

Do not run untrusted development or simulator code on a machine containing:

- Bitcoin private keys
- Bitcoin seed phrases
- GPG private keys
- SSH private keys
- Password databases
- Other sensitive credentials

Never place secrets in:

- Git commits
- GitHub repositories
- source files
- configuration files
- shell history
- issue reports
- documentation

Always inspect:

    git status

and:

    git diff

before committing.

Do not copy private keys, seed phrases, or other sensitive material into
development tools or issue trackers.

---

# 43. Current Verified Development Setup

The following components have been verified in this Linux development
environment:

- Linux Mint
- Git
- GitHub SSH authentication
- Personal GitHub fork configured as `origin`
- Official Kern repository configured as `upstream`
- Kern Git submodules
- Sublime Text 4200
- Package Control
- LSP 2.13.0
- LSP-clangd
- clangd 18.x
- Kern `.clangd`
- C syntax highlighting
- clangd Go-to-Definition
- Kern desktop simulator
- Simulator Testnet configuration
- Testnet PSBT workflow
- Coconut Wallet interoperability testing

This list describes a verified development environment and should not be
interpreted as a list of mandatory Kern project dependencies.

Versions should be updated when the development environment changes.

---

# 44. Troubleshooting

## GitHub Authentication Fails

Check loaded SSH keys:

    ssh-add -l

Test GitHub authentication:

    ssh -T git@github.com

---

## `upstream/master` Is Missing

Run:

    git fetch upstream

Then:

    git branch -r

The official branch should appear as:

    upstream/master

---

## Local Master Is Behind Upstream

Run:

    git fetch upstream
    git switch master
    git merge --ff-only upstream/master

Then synchronize submodules:

    git submodule update --init --recursive

---

## Submodules Show as Modified

Run:

    git submodule update --init --recursive

Then:

    git status

If a submodule still appears modified, inspect it before changing anything.

Check its current state:

    git submodule status

---

## Sublime Does Not Recognize C Files

Open a `.c` file and check the syntax indicator in the lower-right corner.

It should report:

    C

---

## clangd Is Missing

Install:

    sudo apt install clangd

Verify:

    clangd --version

---

## LSP Appears Installed but Does Not Work

Open the Sublime console:

    Ctrl+`

Check for Package Control and LSP messages.

Run:

    LSP: Troubleshoot Server

Verify that the clangd server test succeeds.

---

## Simulator Does Not Build

Start by checking the simulator documentation:

    simulator/README.md

Then verify the required Linux packages are installed.

Clean and rebuild if necessary:

    rm -rf simulator/build

    cmake -B simulator/build -S simulator \
      -DCMAKE_BUILD_TYPE=Debug

    cmake --build simulator/build -- -j$(nproc)

Only remove the build directory after confirming that no build artifacts
need to be retained.

---

## Simulator Starts on Mainnet

The simulator normally starts with Mainnet selected.

Open:

    Settings → Network

Change the network to:

    Testnet

Verify the setting before beginning Testnet transaction testing.

---

## Simulator QR Testing Fails

Verify that:

- The simulator is running
- The QR data is valid
- The correct QR image or directory was supplied
- The simulator was built with the required camera support when using a
  physical webcam
- The webcam device is accessible

For image-based testing:

    ./simulator/build/kern_simulator --qr-image path/to/qr.png

---

# 45. Core Principles

Keep the development environment simple.

Keep `master` clean.

Use feature and development branches.

Use `upstream` for the official Kern source.

Use `origin` for the personal GitHub fork.

Synchronize submodules after updating Kern.

Review changes before committing.

Never commit secrets.

Treat simulator code as untrusted unless independently reviewed.

Use Testnet for development transaction testing.

Verify the network before signing.

When something unexpected happens, stop and inspect it before changing or
deleting anything.
