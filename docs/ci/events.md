# Events: When workflows run

Use the `on` key to specify what events trigger your workflow. Check the full [list of events][event_list].

```yaml
on: push            # Trigger on a push
on: [push, fork]    # Trigger on a push or a fork
```

You can filter each event to be more specific, for example, to trigger the workflow only on push to a certain branch, or when certain files are modified:

```yaml
# Event triggered when pushed code to branch "main",
# and a file inside the folder "sources" was modified
on:     
  push:
    branches:
      - main
    paths:
      - "sources/**"
```

## Manually triggered workflows

The special event `workflow_dispatch` allows the workflow to be triggered manually. In the webpage of your GitHub repository go to the "Actions" tab. A message mentioning the "workflow_dispatch event trigger" should appear, together with a button to run the workflow, as shown in the picture below.

![Manual workflow GitHub GUI](images/manual_workflow.png)

## Conditionally triggered workflows

You can add and `if` conditional expression on a job, using the `${{}}` operator: if the condition is true, the job will run; if it's false, then the job won't run and will be marked as skipped. The following example only runs the job if the string "FORCE_CI" is found in the commit message.

```yaml
name: Run workflow if FORCE_CI is present in commit message
on: push
jobs:
  force_ci:
    runs-on: ubuntu-latest
    if: ${{ contains(github.event.head_commit.message, 'FORCE_CI') }}
    steps:
      - run: echo "Hello world"
```

[event_list]: https://docs.github.com/en/actions/reference/workflows-and-actions/events-that-trigger-workflows
