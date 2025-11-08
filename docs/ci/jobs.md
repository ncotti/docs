# Jobs: What workflows do

Each job runs a series of steps. Each step can have a `name`, and can be either an action (`uses: <action>`) or a shell command (`run: <command>`).

Commands are any valid Bash instruction. Actions are pre-made sets of commands that can be used in your workflow, the most common ones being [checkout][checkout_action], to retrieve files from a repository, and [setup-python][python_action], to install a certain Python version.

The syntax for actions is: `uses: actions/<action_name>@v<version_number>`. The following example checkouts this repository, installs Python and then runs a simple Python script located in the repository's root path[^1]:

[^1]: By default, the runner's default path is the empty directory `/home/runner/work/<repo_name>/<repo_name>`. After the  `actions/checkout`, the repository is cloned inside this folder and execution proceeds.

```yaml
name: "Using actions"
on: push
jobs:
  job_id:
    runs-on: ubuntu-latest
    steps:
      - name: "Checkout repo"
        uses: actions/checkout@v5

      - name: "Install Python"
        uses: actions/setup-python@v6
        with:
          python-version: "3.12"

      - name: "Run Python script"
        run: python3 sample_script.py
```

Check the [GitHub Actions Marketplace][actions_market] to see the full list of available actions.

!!! tip
    There are A LOT of actions to choose from. Always try to use actions from verified creators and check if what you want to do hasn't been implemented by someone else already.

## Jobs' execution order

By default, all jobs run on parallel. You can request sequential execution by using the `jobs.<job_id>.needs` property. Jobs that depend on others will only execute if the previous ones were successful.

```yaml
name: "Lots of sequential jobs"
on: push
jobs:
  job1:
    runs-on: ubuntu-latest
    steps:
      - run: echo "Hello from job1"

  job2:
    runs-on: ubuntu-latest
      needs: [job1]
      steps:
        - run: echo "Hello from job2"

  job3:
    runs-on: ubuntu-latest
    needs: [job1, job2]
      steps:
        - run: echo "Hello from job3"
```

## Sharing values between steps and jobs

Each job runs on a separate runner (virtual machine), indicated by the `runs-on` property. Also, each step of a job runs on a separate shell. Therefore, it is not possible to share environmental variables using solely Bash syntax, as shown in the below example.

```yaml
name: "Failed attempt at sharing env variables"
on: push
env: 
  ENV_VARIABLE: "0"
jobs:
  job1:
    runs-on: ubuntu-latest
    steps:
      - run: |
          echo "Original env variable value: $ENV_VARIABLE"     # Prints 0
          ENV_VARIABLE="1"

      - run: |
          echo "Env variable in new step: $ENV_VARIABLE"        # Prints 0

  job2:
    runs-on: ubuntu-latest
    needs: [job1]
    steps:
      - run: |
          echo "Env variable value in new job: $ENV_VARIABLE"   # Prints 0
```

To share results between steps in the same job, you can use use the `$GITHUB_ENV` environment file. This file is sourced by each step's new shell before executing its commands. Therefore, the previous example can be rewritten to save the environment variable in this file instead:

```yaml
name: "Sharing env variables through GITHUB_ENV"
on: push
env: 
  ENV_VARIABLE: "0"
jobs:
  job1:
    runs-on: ubuntu-latest
    steps:
      - run: |
          echo "Original env variable value: $ENV_VARIABLE"     # Prints 0
          echo "ENV_VARIABLE=1" >> "$GITHUB_ENV"

      - run: |
          echo "Env variable in new step: $ENV_VARIABLE"        # Prints 1

  job2:
    runs-on: ubuntu-latest
    needs: [job1]
    steps:
      - run: |
          echo "Env variable value in new job: $ENV_VARIABLE"   # Prints 0
```

However, the second job can't see the changes made in the first job, because it runs in a different virtual machine, therefore a completely different environment. In consequence, it must be GitHub who handles the passage of variables.

To share results between jobs, first define an `id` for the desired step. The variable to be shared should be written to the file `$GITHUB_OUTPUT` as one of the commands in that step. Then, define the `outputs` of the job as a list of variables like this: `<output_variable_name>: ${{ steps.<step_id>.outputs.<variable_name> }}`. Finally, the next job must have a dependency on the previous job (using the `needs` key), and retrieve the value using `${{ needs.<needed_job_name>.outputs.<output_variable_name> }}`.

The following example demonstrates both sharing an environment variable between steps for *job1*, and sharing the output of *job1* with *job2*.

```yaml
name: "Sharing values between steps and jobs"
on: push
jobs:
  job1:
    runs-on: ubuntu-latest
    env: 
      ENV_VARIABLE: "0"
    outputs:
      job1_output: ${{ steps.step2.outputs.job1_output }}
    steps:
      - id: step1
        run: |
          echo "Original env variable value: $ENV_VARIABLE" # Prints 0
          echo "ENV_VARIABLE=1" >> "$GITHUB_ENV"

      - id: step2
        run: |
          echo "Env variable in new step: $ENV_VARIABLE"    # Prints 1
          echo "job1_output=55" >> "$GITHUB_OUTPUT"

  job2:
    runs-on: ubuntu-latest
    needs: [job1]
    env:
      ENV_VARIABLE: ${{ needs.job1.outputs.job1_output }}
    steps:
        - run: |
            echo "Env variable value: $ENV_VARIABLE"        # Prints 55
```

[actions_market]:   https://github.com/marketplace?type=actions
[checkout_action]:  https://github.com/marketplace/actions/checkout
[python_action]:    https://github.com/marketplace/actions/setup-python