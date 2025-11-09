# MarkDown Linters

[Markdownlint] has a list of `MDxxx` rules that should be followed. By default, it shows a wabbly line underneath every mistake, with an explanation of the rule being broken.

![Markdownlint Visual Studio Code Extension](images/markdownlint_vs_extension.png)

The following is the suggested configuration for this extension:

```json
// .vscode/settings.json
{
    "files.autoSave": "onFocusChange",

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
on: push
jobs:
  markdown_linter:
    runs-on: ubuntu-latest
    steps:
      - name: "Checkout repo"
        uses: actions/checkout@v5

      - name: "Markdown linter"
        uses: DavidAnson/markdownlint-cli2-action@v20
        with:
          globs: '**/*.md'
```

<!-- Link -->
[markdownlint]: https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint

[markdownlint_action]: https://github.com/marketplace/actions/markdownlint-cli2-action
