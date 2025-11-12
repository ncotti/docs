# Docker example

This directory contains everything needed to build, run and push a Docker container. It is encouraged to use it as a template for future projects.

The Dockerfile is a multi-stage build where in the first stage [Cppcheck][cppcheck] is built and, in the second stage, only the resulting binaries are copied.

By default, the `entrypoint.sh` script will replicate the host's user inside the docker container. Also, in this file you will find two places to add any commands to be run during the container's initialization:

* `root_commands` runs as the root user, perfect for installing packages with `apt update && apt install`.
* `user_commands` runs as an user, perfect for setting environment variables.

```bash
function root_commands() {
    # <Your commands>
    true
}

function user_commands() {
    # <Your commands>
    true
}
```

Docker commands cheatsheet:

```bash
# Build image
docker compose build docs_example

# Run container
docker compose run --rm docs_example
```

<!-- Links -->

[cppcheck]: https://github.com/danmar/cppcheck.git
