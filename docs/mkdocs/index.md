# MkDocs: The winner of static site documentation

As the time of writing this article, there were three main contenders:

1. [MkDocs][mkdocs], specifically with [Material theme][material].

2. [Sphinx][sphinx].

3. [Docusaurus][docusaurus].

First, let's start by reviewing the pros and cons of each one, so that it's clear why MkDocs is the best of them all.

## A review of documentation frameworks

### Sphinx: pythonic with reStructuredText

[Sphinx][sphinx] was made to be used with reStructuredText (rst). You can use Markdown as well, but *rst* is preferred. Also, it is mainly used for Python documentation.

Personally, I don't really like the style of Sphinx documentation, since 95% of the time is used together with [ReadTheDocs][readthedocs], being great examples the [ReadTheDocs documentation itself][readthedocs_docs] or [MicroPython docs][micropython_docs].

### Docusaurus: better for web developers

[Docusaurus][docusaurus] is really good looking, that can't be denied. The problem is its complexity: it is built upon Javascript and React. You can use Markdown to write your documentation, but you won't be able to utilize its full potential without having a background in React.

### MkDocs: Markdown and good-looking

[MkDocs][mkdocs] is a simple and effective tool. Fully focused on Markdown, made with Python, and with the ability to locally host a server to see changes in the documentation in real time.

The default theme for MkDocs is not great, but the [Material theme][material] is really good-looking.

## Setting up MkDocs

First, install the `mkdocs` package in a virtual environment, and create a new mkdocs project in the current directory:

```bash
python3 -m venv venv
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

<!-- Links -->
[mkdocs]:           https://www.mkdocs.org/
[sphinx]:           https://www.sphinx-doc.org/en/master/
[material]:         https://squidfunk.github.io/mkdocs-material/
[docusaurus]:       https://docusaurus.io/
[readthedocs]:      https://about.readthedocs.com/

[readthedocs_docs]: https://docs.readthedocs.com/platform/stable/index.html#
[micropython_docs]: https://docs.micropython.org/en/latest/

[mkdocs_getting_started]:   https://www.mkdocs.org/getting-started/
[mkdocs_docs]:              https://www.mkdocs.org/user-guide/
[material_docs]:            https://squidfunk.github.io/mkdocs-material/reference/
