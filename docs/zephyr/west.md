# West

As described by the Zephyr project, [West][west] is a swiss-army knife command line tool (CLI).  It is a Python package that acts as a wrapper for multiple repository management (`git submodule`), compilation (`cmake`, `dtc`), flashing (`avrdude`, `STM32CubeProgrammer`) and debugging (`gdb`).

Similar to Docker, the general command line syntax is to call `west` with its options, the `command` with its options, and the `arguments`, as such:

```bash
west [common-opts] <command> [opts] <args>
```

## West basics

A west workspace is composed of a top directory (`topdir`) which holds a `manifest repository`, i.e., the main application repository, which must hold a `manifest file` called `west.yml` from where the configuration is extracted, the workspace configuration under the `.west` path, and all submodules.

```bash
TODO tree
zephyrproject/                 # west topdir
├── .west/                     # marks the location of the topdir
│   └── config                 # per-workspace local configuration file
│
│   # The manifest repository, never modified by west after creation:
├── zephyr/                    # .git/ repo
│   ├── west.yml               # manifest file
│   └── [... other files ...]
│
│   # Projects managed by west:
├── modules/
│   └── lib/
│       └── zcbor/             # .git/ project
├── tools/
│   └── net-tools/             # .git/ project
└── [ ... other projects ...]
```

The first action to be taken is to initialize the workspace. This is done by calling `west init` and pointing to your desired repo, which must include a `west.yml` manifest file, and branch or tag (by default being the main or master branch).

```bash
mkdir -p west_ws && cd west_ws
west init -m <manifest_repo_url> [--mr <branch | tag>]
```

After this operation, the workspace configuration file `.west/config` should have been created with only two variables: the manifest repo and the manifest filename.

Like this:

```toml
[manifest]
path = example-application
file = west.yml
```

Then, you should update your workspace, i.e., read the contents of the manifest file.

```bash
west update
```

## Manifest file

The `west.yml` file in the manifest repo is the one that controls the `west update` command. Each that it is issued, it will read that file and get everything set-up and working, preserving local changes.

<https://docs.zephyrproject.org/latest/develop/west/manifest.html>

The `projects` subsections are equivalent to Git submodules, these are the sub-projects that are fetched. You need to define the `remote` from where it will be fetched, the path inside the west workspace and the `revision`, i.e., the branch, tag or commit hast to use.

The manifest repo is a project itself, but it is referenced in the `self` subsection. The only two important options are the path where it will be installed and to point to  any additional `west-commands` the manifest repo may have.

```yml
manifest:

    projects:
        - name: <project_name>
        description: <some text>
        url: <url_to_fetch_from>
        revision: <branch | tag | commit_hash>
        path: <path, relative to west's workspace topdir>
        submodules: <true | false> # Handle submodules for this project

    self:
        path: <path>
        west-commands: <yml_file>
```

The `import` property is mainly used when you want to import `zephyr` as a project. Check out [Zephyr's west.yml][zephyr_west_yml] and see all the projects it adds. You can simply type `import: true`, and import all projects defined inside the project; or you can selectively import only the ones you need. To do so use the `import.name-allowlist:` property, which is a list of the project's name you want to import.

```yml
import:
    name-allowlist:
        - proj-name1
        - proj-name2
```

TODO check what happens with local changes

## External modules

<https://docs.zephyrproject.org/latest/develop/modules.html>

An external module is a repository that extends Zephyr functionality and may or may not be a west project.

To be an external module of Zephyr, only the existence of the file `zephyr/module.yaml` is required.

Zephyr will then include your `CMakeLists.txt` and `KConfig` files and include them for building and configuration. (if not used, create them empty).

The `zephyr/module.yml` file may have the following syntax:

```yaml
name: <module_name>

build:
    cmake: .    # Cmake directory, relative to the root of the repo
    kconfig: Kconfig    # Kconfig file name, relative to the root of the repo

settings:
    # Additional roots for boards and DTS files. Zephyr will use the
    # `<board_root>/boards` for additional boards. The `.` is the root of this
    # repository.
    board_root: .

    # Zephyr will use the `<dts_root>/dts` for additional dts files and
    # `<dts_root>/dts/bindings` for additional dts binding files. The `.` is
    # the root of this repository.
    dts_root: .
```

Configuration options will appear in the menuconfig under "Modules -> \*\*\* Available modules. \*\*\* -> <repo_name>"

<!--External tools-->
[west]: https://docs.zephyrproject.org/latest/develop/west/index.html
[zephyr_west_yml]: https://github.com/zephyrproject-rtos/zephyr/blob/main/west.yml
