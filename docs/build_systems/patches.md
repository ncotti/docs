# Patches

If you work with an open-source repository and your project requires doing "small modifications" to it, then patches are the solution.

They can be thought as the usage of `git stash push` and `git stash pop`, in the sense that you store the changes and later apply them, but the only difference being that the changes themselves are stored in a `.patch` file.

The steps for creating a patch are as follows:

1. Clone the Git repository:

    ```bash
    git clone <url>
    cd <repo_name>
    ```

2. Create a branch and move to it:

    ```bash
    git branch <branch_name>
    git checkout <branch_name>
    ```

3. Do your changes and commit them:

    ```bash
    git add .
    git commit -m "patch"
    ```

4. Generate the patches. For each commit from your active branch that differs from main/master, a file called `<number>-<commit_message>.patch` will be created.

    ```bash
    git format-patch <master|main>
    ```

    If you want a single patch for several commits, you should rebase and squash the commits into one.

Finally, if you want to apply a patch use:

```bash
    git am *.patch
```

By using `*.patch`, all patches will be applied in alphabetical order.
