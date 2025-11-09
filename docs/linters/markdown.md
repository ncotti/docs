# MarkDown Linters

[Markdownlint] has a list of `MDxxx` rules that should be followed. By default, it shows a wabbly line underneath every mistake, with an explanation of the rule being broken.

![Markdownlint Visual Studio Code Extension](images/markdownlint_vs_extension.png)

The following is the suggested configuration for this extension:

* Wraps words up to 80 columns.
* Auto-formats files on save (either with `Ctrl + s` or on focus change).
* Allows to define a configuration file at `.vscode/.markdownlint.json` to personalize the linting rules.

```json
{
    "files.autoSave": "onFocusChange",

    "markdownlint.configFile": null, // ".vscode/.markdownlint.json",
    "[markdown]": {
        "editor.wordWrap": "wordWrapColumn",
        "editor.wordWrapColumn": 80,
        "editor.formatOnSave": true,
        "editor.formatOnPaste": true,
        "editor.formatOnSaveMode": "file",
        "editor.defaultFormatter": "DavidAnson.vscode-markdownlint"
    },
}
```

## MarkDown CI

The [markdownlint-cli-action][markdownlint_action] is a GitHub action developed by the creator of [markdownlint][markdownlint].

A basic CI that does MarkDown linting on the whole repository looks like this:

```yaml
name: Markdown Linter
on: [push, pull_request]
jobs:
  markdown_linter:
    runs-on: ubuntu-latest
    env:
      CONFIG_FILE: '.vscode/.markdownlint.json'
    steps:
      - name: "Checkout repo"
        uses: actions/checkout@v5

      - name: Create config file if it does not exist # (1)!
        if: ${{ !hashFiles(env.CONFIG_FILE) }}
        run: |
          echo "{\
            \"default\": true,\
            \"MD013\": false\
            }" > "$CONFIG_FILE"

      - name: "Markdown linter"
        uses: DavidAnson/markdownlint-cli2-action@v20
        with:
          globs: "**/*.md"
          config: ${{ env.CONFIG_FILE }}
```

1. By default MD013, the line length rule, is enabled in the CI runner. Create a default configuration file to disable it.

<!-- Link -->
[markdownlint]: https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint

[markdownlint_action]: https://github.com/marketplace/actions/markdownlint-cli2-action
