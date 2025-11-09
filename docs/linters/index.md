# Linters and Formatters

In this section we will review the author's choice of the best linters and formatters for each language, and how to set up them both in a local environment and as a CI pipeline.

The preferred workflow goes like this:

1. Work comfortably in your local environment, committing files as normal. The linting tool should be configured in your IDE to either highlight errors or autocorrect them on save.
2. Before pushing code to the remote repository, manually run your linting and formatting tools, if desired.
3. Always set a CI workflow up in the repository with linting and code-style analysis.

Please review the guide for setting up the linter and formatter for each language in the following table. Then, the next sections discuss some of the author's personal opinions on this topic.

## General VS Code settings

To be able to use auto-formatters in VS Code, you need to configure your file saving strategy to anything but `"files.autoSave": "afterDelay"`. Auto-formatters can still run after pressing manually `Ctrl + s`, but they won't run on autosaves.

Therefore, the recommended setting is to use `"files.autoSave": "onFocusChange"`.

### VS Code extension list

| Extension | Author |
| --------- | ---------------- |
| Code Spell Checker | streetsidesoftware.com |
| GitHub Actions | github.com |
| Trailing Spaces | Shardul Mahadik |

## Why pre-commit or pre-push hooks aren't recommended

Pre-commit hooks discourage making small incremental commits, because the linter and formatter need to run on every commit. Besides, if your local environment has any issues, they block you from being able to commit. Also, they can take quite some time to execute, and it's normally better to leave this job to the CI pipeline.

## Why Github's super-linter is a bad idea

GitHub's [super-linter][super-linter] is a great tool, but it does too many things and you lose control on what it is doing. It bundles a whole bunch of linters for different languages and runs them all on your repository.

The problem with this approach is that you ideally want to have the linter and formatter installed in your local environment, most likely integrated with your IDE, and this tool can only be executed from a Docker container.

[super-linter]: https://github.com/marketplace/actions/super-linter
