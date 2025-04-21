# Minishell Project

This document outlines a simple Git workflow designed for a two-person team to collaborate effectively on the Minishell project, minimizing merge conflicts and keeping the `main` branch stable.

## Core Philosophy

1.  **`main` is the Stable Source of Truth:** The `main` branch should always contain code that compiles and is relatively stable. Do not push broken or incomplete work directly to `main`.
2.  **Work on Feature Branches:** All new development happens on dedicated branches (`parsing`, `execution`). Never commit directly to `main`.
3.  **Synchronize Frequently (Daily!):** Keep your feature branch updated with the latest changes from `main` to catch integration issues early and keep conflicts small.
4.  **Communicate:** Talk to your teammate! Let each other know what you're working on, especially if your tasks might affect the same files.

## Branches

*   `main`: The primary stable branch. Represents the latest "good" version of the project.
*   `parsing`: Feature branch dedicated to parsing logic. (Managed by Person A)
*   `execution`: Feature branch dedicated to command execution logic. (Managed by Person B)

## Daily Workflow Routine

Follow these steps *at the start of your workday* and *before starting significant new work* on your branch.

**1. Update Your Local `main` Branch:**

Ensure your local `main` branch has the absolute latest code from the remote repository.

```bash
# Switch to the main branch
git checkout main

# Pull the latest changes from the remote main branch
git pull origin main
```

**2. Update Your Feature Branch:**

Integrate the latest stable changes from `main` into your working branch (`parsing` or `execution`).

```bash
# Switch to your feature branch (e.g., parsing)
git checkout parsing
# Or: git checkout execution

# Pull the latest changes from main into your current branch
git pull origin main
```

*   **Conflict Handling:** If `git pull origin main` results in merge conflicts:
    *   Git will mark the conflicting files.
    *   Open these files in your editor.
    *   Look for the conflict markers (`<<<<<<<`, `=======`, `>>>>>>>`).
    *   Edit the files to resolve the differences, keeping the code you need and removing the markers.
    *   Save the resolved files.
    *   Stage the resolved files: `git add <conflicted-file-1> <conflicted-file-2> ...`
    *   Commit the merge: `git commit -m "Merge main into <your-branch>"` (Git often pre-fills this message).

**3. Do Your Work:**

Make your code changes on your feature branch.

**4. Commit and Push Your Changes:**

Regularly save your work and push it to the remote repository *on your feature branch*.

```bash
# Stage the files you've changed
git add .

# Commit your changes with a clear message
git commit -m "feat: Implement <specific feature> in parsing"
# Or: git commit -m "fix: Correct execution logic for <command>"

# Push your commits to your remote feature branch
git push origin parsing
# Or: git push origin execution
```

## Merging Features into `main` (Using Pull Requests - Recommended)

When a feature on `parsing` or `execution` is complete and ready to be integrated:

1.  **Ensure Branch is Up-to-Date:** Repeat **Step 2** (Update Your Feature Branch) one last time to ensure you have the very latest `main` merged in. Resolve any conflicts. Push the merge commit if one was created.
2.  **Create a Pull Request (PR) on GitHub:**
    *   Go to your repository on GitHub.
    *   Navigate to the "Pull requests" tab.
    *   Click "New pull request".
    *   Set the **base** branch to `main`.
    *   Set the **compare** branch to your feature branch (`parsing` or `execution`).
    *   Review the changes and click "Create pull request". Add a title and description.
3.  **Review the PR:**
    *   The **other** teammate should review the PR.
    *   Check the code for correctness, style, and potential issues.
    *   Discuss any necessary changes in the PR comments. The branch owner makes the requested changes, commits, and pushes them to their feature branch (this automatically updates the PR).
4.  **Merge the PR:**
    *   Once the PR is approved, the **reviewer** (or the owner if agreed upon) clicks the "Merge pull request" button on GitHub.
    *   Use the default "Create a merge commit" option unless you have a specific reason not to.
5.  **Delete the Feature Branch (Optional but Recommended):**
    *   GitHub usually offers a button to delete the source branch after merging. This keeps the repository clean. The branch can always be restored if needed.

By following this workflow, you integrate changes frequently, keep `main` clean, and use Pull Requests for code review and controlled merging, significantly reducing the chances of painful merge conflicts.
