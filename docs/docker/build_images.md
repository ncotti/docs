# Build Docker images with Dockerfile

A [Dockerfile][dockerfile_reference] is a set of shell instructions used to install packages and tools. The versions installed are frozen in time, giving as a result an immutable and reproducible environment.

There are plenty of tutorials on Docker, so in this document I just want to remark the most essential concepts and some of the [best practices][best_practices].

This repository's sources includes a [complete example][docker_example] of a Dockerfile and a Docker Compose file.

## How to properly install packages

This is the recommended way to install packages in Debian-based images:

```docker
RUN apt-get update && apt-get install -y --no-install-recommends \
    <packages to install> \
    && rm -rf /var/lib/apt/lists/*
```

## Multi-stage builds make your image smaller

The concept of a [multi-stage build][multi-stage_build] is similar at how you would build an embedded software image:

1. First, you compile all the tools you need in the **builder** image.
2. Then, you copy only the run-time required files into the **target** image.

By doing this, you avoid clogging your **target** with unneeded build-time dependencies, reducing the final image size.

```docker
FROM ubuntu:24.04 AS builder
    # Install whatever you need

FROM ubuntu:24.04 AS target
    # Copy all built binaries from build stages
    COPY --from=builder <source_path> <dest_path>
```

<!-- External Links -->
[best_practices]: https://docs.docker.com/build/building/best-practices/
[dockerfile_reference]: <https://docs.docker.com/reference/dockerfile/>
[multi-stage_build]: <https://docs.docker.com/build/building/multi-stage/>

<!-- Internal links -->
[docker_example]: /docker/
