# Runners: Where workflows run

Basically, there are three options: run in a predefined GitHub-hosted runner, a custom GitHub-hosted runner or a self-hosted runner. The runner is chosen by the `jobs.<job_id>.runs-on` key:

```yaml
jobs:
  <job_id>:
    runs-on: ubuntu-latest
```

[Reference][reference].

## Predefined GitHub-hosted runners

This type of runners are freshly started, predefined virtual machines that execute the jobs. They are filled with the latest version of development to accommodate for all kinds of environments. A simple example is shown below:

```yaml
name: Run hello world
on: push
jobs:
  hello_world:
    runs-on: ubuntu-latest
    steps:
      - run: echo "Hello world"
```

Check the full list of [GitHub-hosted runners][runners].

## Custom GitHub-hosted runners

This type of runners execute inside a user created Docker container. You should have created a Docker image and uploaded it to [DockerHub][dockerhub].

For example, if your Docker image is called `image_repo/image_name`, then you can execute the CI workflow in that container by including the `jobs.<job_id>.containers.image` property. Below is a full workflow example, that uses the `alpine:latest` image, with the environmental `CUSTOM_VARIABLE = "author"`

```yaml
name: Run hello world on a Docker container
on: push
jobs:
  hello_world:
    runs-on: ubuntu-latest
    container:
      image: alpine:latest
      env:
        CUSTOM_VARIABLE: "author"
    steps:
      - run: echo "Hello ${CUSTOM_VARIABLE}"
```

!!! note
    Both `runs-on` and `container` should be specified. The container image will run on top of the `runs-on` operating system by calling the docker CLI.

## Self-hosted runners

Self hosted runners imply having a local machine hooked to the internet, constantly listening for job request from GitHub. This methodology won't be discussed further since it is not needed by the author.

<!-- Links -->
[reference]: https://docs.github.com/en/actions/how-tos/write-workflows/choose-where-workflows-run

[runners]: https://docs.github.com/en/actions/how-tos/write-workflows/choose-where-workflows-run/choose-the-runner-for-a-job#standard-github-hosted-runners-for-public-repositories

[dockerhub]: https://hub.docker.com/
