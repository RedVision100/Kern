# Kern Development Setup

This guide takes a Linux development machine from a basic installation to a working Kern development environment.

It is written for learning and practice with a personal fork. It separates Kern project requirements from tools and configurations used in the documented development environment.

This guide takes a Linux development machine from a fresh setup to a working Kern repository.

Kern is a research and development project focused on Bitcoin self-custody and hardware signing. This guide is written for development and practice, including work on a personal fork.

> **Security warning:** Kern development and simulator code must be treated as untrusted unless independently reviewed. Do not run it on a machine containing Bitcoin private keys, seed phrases, GPG private keys, SSH private keys, password databases, or other sensitive credentials.

## 1. Install the Required Linux Packages

On Debian/Ubuntu-based systems:

```bash
sudo apt update
sudo apt install build-essential cmake git libsdl2-dev libmbedtls-dev
```

The exact dependency set may change as Kern evolves. Check `simulator/README.md` for simulator-specific requirements.

Kern does not require a particular Linux distribution or editor.

## 2. Install ESP-IDF

Kern firmware development uses Espressif's ESP-IDF environment.

The exact ESP-IDF version and installation procedure should follow the version currently required by the Kern repository. Do not substitute a different version simply because it is newer; firmware dependencies can be version-sensitive.

## 3. Install `just`

Kern uses `just` for repository development recipes.

Verify that it is installed:

```bash
just --version
```

If it is not installed, install it using the method appropriate for your Linux environment.

## 4. Set Up GitHub SSH Authentication

SSH lets Git communicate with GitHub without entering a password for every operation.

If you do not already have an Ed25519 key:

```bash
ssh-keygen -t ed25519 -C "your-email@example.com"
```

Start the SSH agent:

```bash
eval "$(ssh-agent -s)"
```

Add the key:

```bash
ssh-add ~/.ssh/id_ed25519
```

Verify that it is loaded:

```bash
ssh-add -l
```

Your public key is:

```text
~/.ssh/id_ed25519.pub
```

Add the **public** key to your GitHub account's SSH key settings.

Test the connection:

```bash
ssh -T git@github.com
```

A successful authentication normally reports:

```text
Hi USERNAME! You've successfully authenticated, but GitHub does not
provide shell access.
```

Never share:

```text
~/.ssh/id_ed25519
```

Only the `.pub` file is intended to be shared with GitHub.

## 5. Fork Kern on GitHub

For practice, work from your own GitHub fork.

Your fork will be `origin`.

The official Kern repository will be `upstream`.

The repository model is:

```text
Official Kern repository
        |
        | fetch
        v
   Local repository
        |
        | push
        v
   Your GitHub fork
```

## 6. Clone Your Fork

Clone your fork using SSH:

```bash
git clone git@github.com:YOUR-USERNAME/Kern.git
```

Enter the repository:

```bash
cd Kern
```

Add the official repository as `upstream`:

```bash
git remote add upstream git@github.com:odudex/Kern.git
```

Verify the remotes:

```bash
git remote -v
```

You should have `origin` pointing to your fork and `upstream` pointing to the official Kern repository.

## 7. Initialize Submodules

Kern uses Git submodules.

Initialize and synchronize them:

```bash
git submodule update --init --recursive
```

Check their state:

```bash
git submodule status
```

Important: updating the main Kern repository does not automatically mean that every submodule is checked out at the required revision. Run the command above after cloning and after changing Kern revisions.

## 8. Verify the Repository

Check the working tree:

```bash
git status
```

Check recent commits:

```bash
git log --oneline -10
```

Check the configured remotes:

```bash
git remote -v
```

At this point, the repository should be ready for the Git workflow described in [Git Workflow](git-workflow.md).

## 7. Optional Development Tools

The following tools were used in the documented Linux development environment. They are **not Kern project requirements**.

- Sublime Text
- Package Control
- LSP
- LSP-clangd
- clangd

Any editor or IDE capable of working with the Kern source tree may be used.

### clangd

Install on Debian/Ubuntu-based systems:

```bash
sudo apt install clangd
```

Verify:

```bash
clangd --version
```

Kern's `.clangd` file provides project-specific language-server configuration.

### Sublime Text

Sublime Text can be installed from its official repository. If you use it, the documented setup uses:

- Package Control
- LSP
- LSP-clangd
- clangd

Open the Kern repository:

```bash
cd Kern
subl .
```

Open a C source file such as:

```text
main/core/psbt.c
```

After LSP initializes, `F12` or the appropriate LSP command should provide Go-to-Definition.

For troubleshooting, use:

```text
LSP: Troubleshoot Server
```

The important tool chain is:

```text
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
```

## 8. First Setup Check

Before moving on, verify:

```bash
git status
git remote -v
git submodule status
```

You should have:

- A working GitHub SSH connection
- Your fork configured as `origin`
- Official Kern configured as `upstream`
- Kern submodules initialized
- A clean working tree

Next:

- [Git Workflow](git-workflow.md)
- [Simulator Development](simulator-development.md)
- [Testnet Testing](testnet-testing.md)
