FROM ubuntu:24.04 AS build_cppcheck
    # Version of cppcheck to install
    ARG CPPCHECK_VERSION="2.7"

    # Required packages
    RUN apt-get update \
        && apt-get install -y --no-install-recommends \
            git make cmake g++ \
        && rm -rf /var/lib/apt/lists/*

    # Installation steps
    RUN export GIT_SSL_NO_VERIFY=1 \
        && git clone --depth 1 --branch ${CPPCHECK_VERSION} https://github.com/danmar/cppcheck.git \
        && mkdir -p cppcheck/build \
        && cd cppcheck/build \
        && cmake .. \
        && cmake --build . \
        && make install/strip \
        && cd ../.. \
        && rm -r cppcheck

FROM ubuntu:24.04 AS target
    # Docker's default user home path
    ARG HOME_PATH="/home/user"

    # Copy all built binaries from build stages
    COPY --from=build_cppcheck /usr/local/share/Cppcheck /usr/local/share/Cppcheck
    COPY --from=build_cppcheck /usr/local/bin/cppcheck /usr/local/bin/cppcheck

    # Install all needed packages
    RUN apt-get update \
        && apt-get install -y --no-install-recommends \
            # Needed to create local user
            adduser \
            # Quality of Life packages
            git bash-completion curl sudo nano ssh rsync python3 \
        && rm -rf /var/lib/apt/lists/*

    # Steps needed to set up tools and the environment
    RUN mkdir -p ${HOME_PATH} \
        # Copy root's ".bashrc" and ".profile" into the user folder
        && cp /root/.bashrc ${HOME_PATH}/.bashrc \
        && cp /root/.profile ${HOME_PATH}/.profile \
        # Adds "(docker)" prefix to shell prompt
        && echo "PS1='(docker) '\${PS1:-}" >> "${HOME_PATH}/.bashrc" \
        # Avoid warning messages with sudo
        && touch ${HOME_PATH}/.sudo_as_admin_successful \
        # Give permission to edit any file inside the user directory
        && chmod --recursive 777 ${HOME_PATH}

    ENTRYPOINT [ "${HOME_PATH}/entrypoint.sh" ]
    CMD [ "/bin/bash" ]