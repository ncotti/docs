# Docker: build local, run everywhere

[Docker][docker] solves the long-standing problem of *"but it runs on my machine"*.

In practical terms, a Docker image is like a virtual machine (VM)[^1]: it allows you to run any program you want inside an operating system (OS) of your choice, with any set of packages and libraries installed.

[^1]: The main difference with VMs is that Docker images don't actually run their own kernel and bootloader. Instead, each system call executed by the "simulated" OS inside the Docker is translated to the OS currently running in your computer.

This guide is structured in two parts:

* [How to build Docker images](build_images.md): bundle all your tools inside a reusable and easily shareable Docker image, using a `Dockerfile`.

* [How to run Docker containers](run_containers.md): build your image, execute it, push it to [DockerHub][dockerhub], pull it. Everything that you need for using Docker with `Docker Compose` files.

To install Docker, consult the [Docker Engine installation guide][install_guide].

<!-- Links -->
[docker]: https://www.docker.com/
[install_guide]: https://docs.docker.com/engine/install/
[dockerhub]: https://hub.docker.com/
