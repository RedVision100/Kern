# Git Workflow for Kern

A practical Git reference for the Kern project.

## 1. Check the Current Directory

```bash
pwd
```

Shows where you are.

Kern is located at:

```text
~/Projects/Kern
```

Enter the repository:

```bash
cd ~/Projects/Kern
```

## 2. Check Repository Status

```bash
git status
```

This should be the first Git command you use.

It tells you:
- Current branch
- Remote tracking status
- Modified files
- Untracked files
- Staged changes
- Whether the working tree is clean

A clean repository looks like:

```text
On branch notes
Your branch is up to date with 'origin/notes'.

nothing to commit, working tree clean
```

## 3. See the Current Branch

```bash
git branch --show-current
```

Or:

```bash
git branch
```

The `*` marks the current branch.

## 4. See All Branches

```bash
git branch -a
```

This shows local branches and remote branches.

For Kern:
- `origin` = your GitHub fork
- `upstream` = the original Kern repository

See the remote URLs with:

```bash
git remote -v
```

## 5. Check Branch Tracking

```bash
git branch -vv
```

This shows which remote branch each local branch tracks.

For example:

```text
* notes abc1234 [origin/notes] docs: update notes
```

This means the local `notes` branch tracks `origin/notes`.

## 6. Switch Branches

Switch to an existing branch:

```bash
git switch notes
```

Accessibility branch:

```bash
git switch feat/accessibility-text-size
```

Master:

```bash
git switch master
```

Always check `git status` if you are unsure which branch you are on.

## 7. Create a New Branch

Create and immediately switch to a new branch:

```bash
git switch -c feat/my-feature
```

For example:

```bash
git switch -c feat/new-camera-support
```

Feature development should normally be done on a feature branch rather than directly on `master`.

## 8. Fetch Remote Changes

Fetch your fork:

```bash
git fetch origin
```

Fetch the original Kern repository:

```bash
git fetch upstream
```

Fetching updates Git's knowledge of remote branches and commits without changing your working files.

## 9. View Recent Commits

```bash
git log --oneline --decorate -5
```

Shows the five most recent commits in compact form.

## 10. See Your Changes

Unstaged changes:

```bash
git diff
```

Staged changes:

```bash
git diff --cached
```

Use these before committing to review exactly what changed.

## 11. Stage a File

For example:

```bash
git add docs/omarchy-development-setup.md
```

`git add` places the current version of the file into Git's staging area.

It does not create a commit.

The basic flow is:

```text
Working directory
       ↓
    git add
       ↓
Staging area
       ↓
   git commit
       ↓
Git history
```

You can stage multiple files:

```bash
git add file1 file2
```

Or all changed files:

```bash
git add .
```

Use `git add .` carefully and check `git status` afterward.

## 12. Check Staged Changes

```bash
git status
```

Look under:

```text
Changes to be committed
```

Then review them:

```bash
git diff --cached
```

## 13. Commit Changes

Example:

```bash
git commit -m "docs: update Omarchy development notes"
```

A commit creates a permanent entry in your local Git history.

Good commit messages describe what changed.

Examples:

```bash
git commit -m "docs: add Omarchy development notes"
git commit -m "docs: update Git workflow"
git commit -m "feat(ui): add text size setting"
git commit -m "fix(ui): correct accessibility text scaling"
```

## 14. Push to GitHub

For the notes branch:

```bash
git push origin notes
```

If the branch already tracks its remote branch, you can normally use:

```bash
git push
```

`git push` sends your local commits to the remote repository.

The normal workflow is:

```text
Edit
 ↓
git add
 ↓
git commit
 ↓
git push
 ↓
GitHub
```

## 15. Verify the Push

```bash
git status
```

You want:

```text
On branch notes
Your branch is up to date with 'origin/notes'.

nothing to commit, working tree clean
```

You can also check:

```bash
git log --oneline --decorate -3
```

## 16. Undo Staging

If you accidentally staged a file:

```bash
git restore --staged docs/omarchy-development-setup.md
```

This removes it from the staging area but keeps your edits.

## 17. Discard Uncommitted Changes

To discard changes to a specific file:

```bash
git restore docs/omarchy-development-setup.md
```

**Warning:** this permanently discards that file's uncommitted changes.

Do not use it unless you are certain you want to lose the changes.

## 18. Compare a Branch With Master

Commits on the current branch that are not in `master`:

```bash
git log --oneline master..HEAD
```

Commits in `master` that are not on the current branch:

```bash
git log --oneline HEAD..master
```

## 19. Compare Master With Upstream

```bash
git log --oneline master..upstream/master
```

Then:

```bash
git log --oneline upstream/master..master
```

If both commands produce no output, neither branch has commits that the other lacks.

## 20. Kern Documentation Workflow

For documentation work:

```bash
cd ~/Projects/Kern
git status
git switch notes
```

Edit the documentation.

Review changes:

```bash
git diff
```

Stage the file:

```bash
git add docs/omarchy-development-setup.md
```

Review the staged changes:

```bash
git diff --cached
```

Commit:

```bash
git commit -m "docs: update Omarchy development notes"
```

Push:

```bash
git push origin notes
```

Verify:

```bash
git status
```

Expected:

```text
nothing to commit, working tree clean
```

## 21. Kern Feature Development Workflow

For a new feature:

```bash
cd ~/Projects/Kern
git status
git switch master
git fetch upstream
```

Create a feature branch:

```bash
git switch -c feat/my-feature
```

Make your changes.

Review:

```bash
git status
git diff
```

Stage:

```bash
git add <files>
```

Review staged changes:

```bash
git diff --cached
```

Commit:

```bash
git commit -m "feat: describe the feature"
```

Push the new branch:

```bash
git push -u origin feat/my-feature
```

The `-u` establishes tracking between the local and remote branch.

Future pushes can then use:

```bash
git push
```

## 22. Quick Reference

For normal work:

```bash
git status
```

**Check repository state.**

```bash
git diff
```

**Review changes.**

```bash
git add <file>
```

**Stage changes.**

```bash
git diff --cached
```

**Review staged changes.**

```bash
git commit -m "description of change"
```

**Create a commit.**

```bash
git push
```

**Push the commit to GitHub.**

```bash
git status
```

**Confirm the working tree is clean.**

### Core Workflow

```text
1. git status
2. Edit files
3. git diff
4. git add <file>
5. git diff --cached
6. git commit -m "..."
7. git push
8. git status
```

This is the core Git workflow used for Kern documentation and feature development.
