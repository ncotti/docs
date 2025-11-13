# Run containers with Docker Compose

I personally believe that people who learn Docker never bother themselves to learn [Docker Compose][compose], which is a real shame. In most Docker tutorials, you will often see some long command-line incantations to build or execute a Docker container with lots of options.

[Docker Compose][compose] is a way to gather all those options in a `docker-compose.yaml` file, and then it allows you to use Docker with this simple commands:

```bash
# Single service or "image"
docker compose build <service>
docker compose run --rm <service>
docker compose pull <service>
docker compose push <service>

# Execute multiple services at the same time
docker compose up
docker compose down
```

There are lots of tutorials on this topic, and the [Compose file reference][compose] is very complete, so in the following sections I will only highlight some useful tips.

A [complete example of a Docker environment][docker_folder] can be found in this repository's sources.

## Environmental variables

By default, upon execution of any `docker compose <command>` command, the environmental variables in the `.env` file will be read. These variables can be used for replacement in the `docker-compose.yaml` file, and can be seen inside the container as well.

## Volumes

Volumes are the way to share information between containers or with the host computer. They are simply shared folders, and you only need to define the host's path and the container's path.

## Entrypoint

The container's entrypoint is a bash script that contains all instructions to execute inside the container on boot-up.

By default, containers execute with the `root` user and group. When sharing volumes with the host system, any file created inside the container won't be accessible by the user because of the files' permissions. To avoid this, you should create a replica of the host's user, with the same user ID and group ID.

Take a look at the [example entrypoint][entrypoint_file] provided with this repository to find the implementation for this.

<!-- External links -->
[compose]: https://docs.docker.com/reference/compose-file/

<!-- Internal links -->
[entrypoint_file]: /docker/entrypoint.sh
[docker_folder]: /docker/
