#!/bin/bash

## Creates a replica of the host's user and group inside the Docker
## environment, so that any file created by the Docker will be editable by
## the host machine (otherwise, files would only be accessible through
## the "root" user)
function change_to_host_user() {
    # Delete the user if already exists
    if id "${HOST_UID}" &>/dev/null; then
        userdel "$(id -un "${HOST_UID}")" &>/dev/null || true
    fi
    addgroup --force-badname --gid "${HOST_GID}" "${HOST_GROUP}" &>/dev/null || true
    adduser --force-badname --quiet --gecos "" --home "${HOME_PATH}" \
        --shell /bin/bash --uid "${HOST_UID}" --gid "${HOST_GID}" \
        --disabled-password "${HOST_USER}" &>/dev/null
    usermod -aG sudo "${HOST_USER}"
    passwd -d "${HOST_USER}" &>/dev/null

    su "${HOST_USER}" -P --shell="/bin/bash" -lc "cd ${PWD} && ${HOME_PATH}/entrypoint.sh $*"
    exit 0
}

## These commands are executed by the root user.
function root_commands() {
    # <Your commands>
    true
}

## These commands are executed by the host user.
function user_commands() {
    # <Your commands>
    true
}

## Root user
if [ "$(id -u)" -eq 0 ]; then
    set -e
    root_commands
    change_to_host_user "$@"
fi

## Host user
user_commands

## The value of "$@" equals the "command" specified in the docker-compose.yaml.
exec "$@"
