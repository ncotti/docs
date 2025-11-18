# Python linters

[Ruff][ruff] is a Python linter and formatter written in Rust.

## VS Code integration

In [the Ruff VS Code extension][ruff_extension] you can find the recommended configuration for the `.vscode/settings.json` file to enable automatic linting and formatting on file save:

```json
{
    "[python]": {
        "editor.formatOnSave": true,
        "editor.formatOnPaste": true,
        "editor.formatOnSaveMode": "file",
        "editor.codeActionsOnSave": {
            "source.fixAll": "always",
            "source.organizeImports": "always"
        },
        "editor.defaultFormatter": "charliermarsh.ruff"
    },
}
```

![Ruff VS Code extension](images/ruff_vs_extension.png)

![Python VS Code extension](images/python_vs_extension.png)

## Modifying linting and formatting rules

By default, Ruff doesn't have too much linting rules enabled. For example, it doesn't enforce arguments types and return values in functions.

The recommended way of [configuring Ruff][ruff_config] is to enable all rules, and ignore the ones you don't really need. Create a `ruff.toml` file in the root directory of your repository, and fill it with something like this:

```toml
line-length = 80
indent-width = 4

[lint]

select = ["ALL"]
ignore = [
    "D203", # Space for first line of docstring
    "D213"  # First line of docstring on new line
]

[format]
quote-style = "double"
indent-style = "space"
```

You can ignore rules in a file-basis by adding a `# noqa: <rule>` comment in the violating line as such:

```python
# D103 requires to have a docstring in the function.
def my_function() -> None:  # noqa: D103
    pass
```

## CI integration

In the [Integrations][ruff_integration] section of the Ruff's documentation there are examples of CI workflows.

Specifically, you can use the GitHub Action [ruff-action][ruff_action] as follows:

```yaml
name: Python linter
on: [ push, pull_request ]
jobs:
  ruff:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v5
      - uses: astral-sh/ruff-action@v3
```

## Command line usage

Since Ruff is a Python PIP package, it can be installed with:

```bash
pip install ruff
```

Then, files can be linted and formatted with the following commands:

```bash
ruff check [OPTIONS] [FILES]
ruff format [OPTIONS] [FILES]
```

If no files are given, these commands will check every Python file recursively from the current directory.

<!-- External links -->
[ruff]: https://docs.astral.sh/ruff/
[ruff_extension]: https://marketplace.visualstudio.com/items?itemName=charliermarsh.ruff
[ruff_config]: https://docs.astral.sh/ruff/configuration/
[ruff_integration]: https://docs.astral.sh/ruff/integrations/
[ruff_action]: https://github.com/astral-sh/ruff-action
