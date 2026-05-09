# MkDocs: The best tool for static site documentation

When deciding which tool to use to create this website, there were three main contenders:

1. [MkDocs][mkdocs], specifically with [Material theme][material].

2. [Sphinx][sphinx].

3. [Docusaurus][docusaurus].

The following sections are quick reviews of the pros and cons of each one, so that it's clear why MkDocs is the best of them all.

See [how to install and use MkDocs][usage]. See [how to deploy the page to the Internet][deploy].

## Sphinx: Pythonic with reStructuredText

[Sphinx][sphinx] was made to be used with reStructuredText (rst). You can use Markdown as well, but *rst* is preferred. Also, it is mainly used for Python documentation.

Personally, I don't really like the style of Sphinx documentation, since 95% of the time is used together with [ReadTheDocs][readthedocs], being great examples the [ReadTheDocs documentation itself][readthedocs_docs] or [MicroPython docs][micropython_docs]. Also, there is the drawback of actually learning and using yet another language (rst).

## Docusaurus: better for web developers

[Docusaurus][docusaurus] is really good looking, that can't be denied. The problem is its complexity: it is built upon Javascript and React. You can use Markdown to write your documentation, but you won't be able to utilize its full potential without having a background in React.

## MkDocs: Markdown and good-looking

[MkDocs][mkdocs] is a simple and effective tool. Fully focused on Markdown, made with Python, and with the ability to host a local server to see real-time changes.

The default theme for MkDocs is not great, but the [Material theme][material] is really good-looking.

<!-- External links -->
[mkdocs]:           https://www.mkdocs.org/
[sphinx]:           https://www.sphinx-doc.org/en/master/
[material]:         https://squidfunk.github.io/mkdocs-material/
[docusaurus]:       https://docusaurus.io/
[readthedocs]:      https://about.readthedocs.com/

[readthedocs_docs]: https://docs.readthedocs.com/platform/stable/index.html#
[micropython_docs]: https://docs.micropython.org/en/latest/

<!-- Internal links -->
[usage]: /docs/mkdocs/installation_and_usage
[deploy]: /docs/mkdocs/deployment
