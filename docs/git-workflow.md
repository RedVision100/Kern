# Kern Git Workflow

This guide describes the Git workflow for working from a personal Kern fork.

For practice, the goal is simple:

```text
upstream → local master → origin
                 |
                 v
          development branch
```

`master` is the synchronization point. Development happens on separate branches.

## 1. Understand the Remotes

Check them at any time:

```bash
git remote -v
```

### `origin`

Your personal GitHub fork.

Example:

```text
git@github.com:YOUR-USERNAME/Kern.git
```

### `upstream`

The official Kern repository.

Example:

```text
git@github.com:odudex/Kern.git
```

Normally:

- Fetch from `upstream`
- Keep local `master` synchronized with `upstream/master`
- Develop on a separate branch
- Push development branches to `origin`
- Do not push to the official repository unless authorized

## 2. Start From a Clean Repository

Before starting work:

```bash
git status
```

Do not begin a new piece of work with unexplained local changes.

## 3. Update Local Master

Fetch the official repository:

```bash
git fetch upstream
```

Switch to `master`:

```bash
git switch master
```

Update it with a fast-forward-only merge:

```bash
git merge --ff-only upstream/master
```

Synchronize submodules:

```bash
git submodule update --init --recursive
```

Verify:

```bash
git status
git log --oneline -1
```

Using `--ff-only` prevents Git from creating an unexpected merge commit.

## 4. Update Your Fork

Once local `master` is synchronized and verified:

```bash
git push origin master
```

The resulting flow is:

```text
Official Kern
      |
      | git fetch upstream
      v
Local master
      |
      | git push origin master
      v
Your fork
```

## 5. Create a Development Branch

Do not normally develop directly on `master`.

Create a branch from the current `master`:

```bash
git switch -c linux-dev-setup
```

Use descriptive names for other work:

```text
feature/qr-scanning
feature/ui-improvement
fix/simulator-crash
docs/linux-development
```

Verify:

```bash
git status
```

## 6. Make Changes

Work on the development branch.

Regularly inspect the repository:

```bash
git status
```

Review unstaged changes:

```bash
git diff
```

Review staged changes:

```bash
git diff --cached
```

Do not blindly stage everything. Review what is actually being committed.

## 7. Commit Changes

Stage the files you intend to commit:

```bash
git add <files>
```

Commit with a clear message:

```bash
git commit -m "Add simulator development documentation"
```

Good commit messages describe the change.

Avoid messages such as:

```text
fix
changes
update
stuff
```

Keep commits logically focused. A commit should explain one coherent change rather than combining unrelated work.

## 8. Push Your Development Branch

The first push:

```bash
git push -u origin <development-branch>
```

Later pushes can normally use:

```bash
git push
```

Your development branch belongs on `origin`, not on the official repository.

## 9. Pull Requests

When working toward a contribution to Kern, open a Pull Request from your fork into the official repository according to the project's contribution guidelines.

For the current practice phase, it is also reasonable to use your fork simply to practice the workflow without submitting anything upstream.

The important discipline is the same:

```text
master = clean synchronization point
branch = development work
origin = personal fork
upstream = official project
```

## 10. Keep a Development Branch Current

If `master` has moved forward, update it first:

```bash
git fetch upstream
git switch master
git merge --ff-only upstream/master
git submodule update --init --recursive
git push origin master
```

Then return to your development branch:

```bash
git switch <development-branch>
```

Do not blindly merge or rebase. Inspect the state of the branch and follow the project's contribution workflow for integrating upstream changes.

## 11. Compare Branches

See commits that upstream has and local `master` does not:

```bash
git log --oneline master..upstream/master
```

Compare your fork with upstream:

```bash
git log --oneline origin/master..upstream/master
```

See the remote branches:

```bash
git branch -r
```

## 12. Submodules

After changing Kern revisions:

```bash
git submodule update --init --recursive
```

Check:

```bash
git submodule status
git status
```

Important submodules in the documented setup include:

```text
components/cUR
components/k_quirc
components/libwally-core
```

The exact set may change as Kern evolves.

## 13. Accidental Files or Changes

If `git status` shows an unexpected file, inspect it before deleting it.

Useful commands:

```bash
ls -l
file "filename"
cat "filename"
```

If a tracked file was accidentally modified, inspect it:

```bash
git diff -- path/to/file
```

Only discard changes after confirming they are unwanted:

```bash
git restore path/to/file
```

## 14. Before You Commit

Always check:

```bash
git status
git diff
```

Look specifically for:

- Secrets
- Private keys
- Seed material
- Credentials
- Unintended files
- Debug artifacts
- Generated files
- Changes unrelated to the commit

Never put secrets into source files, Git commits, GitHub issues, or documentation.

## 15. End-of-Day Check

If the work is complete:

```bash
git status
git diff
git add <files>
git commit -m "description of change"
git push
git status
```

If work is unfinished, do not create meaningless commits just to make the working tree clean. Use a logical checkpoint commit when appropriate and leave the development branch in a state you can understand when you return.

## 16. Git Recovery Principle

When something unexpected happens, stop before changing or deleting anything.

First inspect:

```bash
git status
git diff
git log --oneline -10
git branch --show-current
git remote -v
```

The safest Git habit is to understand the current state before attempting to fix it.

## Daily Workflow

A normal development session looks like:

```bash
cd Kern

git fetch upstream

git status

git switch master
git merge --ff-only upstream/master
git submodule update --init --recursive

git switch <development-branch>
```

Work and test.

Then:

```bash
git status
git diff

git add <files>
git commit -m "description of change"
git push
```

Keep `master` clean and use development branches for actual work.
