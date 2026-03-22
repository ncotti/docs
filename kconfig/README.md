# Example KConfig project

Install the KConfig frontend:

```bash
sudo apt install kconfig-frontends
```

Execute the menuconfig TUI:

```bash
make menuconfig
make nconfig
make qconfig
```

Compile and run. You should see the `gcc` compilation steps with all the custom flags, and the program output reflecting the usage of those flags.

```bash
make
```
