# CI/CD: Continuous Integration / Continuos Delivery

What is CI/CD? The short answer is: CI = `make all`, CD = `make install`. The long answer is that CI is a set of instructions (like compiling or running tests) executed on specific events (like creating pull request or pushing new code), while CD is a set of instructions that deploys the built application into a server or a production environment.

This section is going to be a quick tutorial to get some basic workflows running with [GitHub Actions][gh_actions]. To get started quickly, go check a list of [workflow templates][templates].

## GitHub Actions

You can configure a **workflow**, a set of instructions, to be triggered when an event occurs in your repository, such as pull request being opened or an issue being created.

All workflows are declared inside the `.github/workflows` directory, and are perfectly defined by the following components:

* The [**event**](events.md) that initiates the workflow.
* The [**runner**](runners.md), where the workflow is executed.
* The [**jobs**](jobs.md), sets of instructions to be executed.

These three elements are highlighted in the following example workflow, and further explained in the following pages.

```yaml
on: [push]                  # The event: pushing to the repo
jobs:
  job-name:                 # The job: commands to be executed
    runs-on: ubuntu-latest  # The runner: VM with ubuntu-latest
    steps:
      - uses: actions/checkout@v5
      - run: echo "hello workflows
```

### GitHub expressions

The `${{ <variable | expression> }}` operator, with double braces makes the GitHub workflow runner execute the expression inside and return its result, or substitute the variable's value. All `${{ }}` are resolved before the workflow is executed[^1].

[^1]: A common confusion arises with the `${variable}` operator, which is just Bash's variable substitution that gets replaced at run-time.

[gh_actions]:   https://docs.github.com/en/actions
[templates]:    https://github.com/actions/starter-workflows
