# Documentation deployment

This guide shows you how to deploy your documentation online using different tools. Using GitHub pages is preferred, so it is shown first.

## GitHub Pages

MkDocs has an in-built command to deploy to GitHub Pages: `mkdocs gh-deploy`. You can use this command on a CI in the same way as this documentation gets deployed, by copying the [GitHub Action for MkDocs deployment][mkdocs_ci].

A new branch `gh-pages` will be created in your repository to host the documentation's webpage.

### Using a custom domain

By default, GitHub will assign the URL `https://<github_username>.github.io/<repo_name>/` to your page. To change it... TODO

## ReadTheDocs

Setting ReadTheDocs is quite easy, only two steps are needed:

1. Create a `.readthedocs.yaml` in the root of the repository like this one:

    ```yaml
    version: 2

    build:
    os: "ubuntu-24.04"
    tools:
        python: "3"
    jobs:
        pre_install:
        - pip install mkdocs-material

    mkdocs:
    configuration: mkdocs.yml
    ```

2. Create a ReadTheDocs account, log in, and create a new project pointing to the name of your repository.

However, having a separate account and using a different tool outside of GitHub or GitLab seems clunky, so I prefer to avoid this and leverage a full CI/CD pipeline with GitHub Pages.

For more info, check [the ReadTheDocs docs on MkDocs][readthedocs_mkdocs].

<!-- External links -->
[readthedocs_mkdocs]: https://docs.readthedocs.com/platform/stable/intro/mkdocs.html

<!-- Internal links -->
[mkdocs_ci]: /.github/workflows/update-docs.yml
