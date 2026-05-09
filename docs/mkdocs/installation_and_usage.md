# MkDocs installation and usage

## Installation

Create a Python virtual environment and install the `mkdocs` and `mkdocs-material` packages. Then create a new mkdocs project in the current directory:

```bash
python3 -m venv .venv
source venv/bin/activate
pip install mkdocs mkdocs-material
mkdocs new .
```

That will create the `docs` folder and the `mkdocs.yml` file. After that, you can serve the documentation page in your local host with:

```bash
mkdocs serve --livereload
```

Finally, utilize the material theme by adding the following lines to the `mkdocs.yml` file:

```yaml
theme:
  name: material
```

More details can be found in the [MkDocs getting started][mkdocs_getting_started].

## Using MkDocs

The best to use MkDocs is a demand-based approach: when you want to do something specific, consult either the [MkDocs docs][mkdocs_docs] or the [Material docs][material_docs].

<!-- External links -->
[mkdocs_getting_started]:   https://www.mkdocs.org/getting-started/
[mkdocs_docs]:              https://www.mkdocs.org/user-guide/
[material_docs]:            https://squidfunk.github.io/mkdocs-material/reference/
