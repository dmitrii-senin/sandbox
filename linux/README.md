# Playing with Linux Kernel

## Prerequisits

```console
sudo apt install -y \
    git \
    curl \
    build-essential \
    libelf-dev \
    libssl-dev \
    libncurses5-dev \
    qemu-system-x86 \
    xz-utils \
    libxml2-utils \
    flex \
    bison \
    bc

SB="/tmp/sandbox/linux"
mkdir -p "$SB"
cd "$SB"

```

## Build Kernel

Get the latest kernel link:

```console
LINUX_URL="$(curl -s https://www.kernel.org | \
    xmllint --html --format - --xpath '//td[@id="latest_link"]/a/@href' 2> /dev/null | \
    cut -d\" -f2)"
curl "$LINUX_URL" | tar xJf -

cd linux-*
make defconfig
make menuconfig # to manually reconfigure some options
make -j "$(nproc)"
```

## Generate a Minimal initramfs

```console
cd "$SB"
cat > init.c << EOF
#include <stdio.h>
int main(int argc, char **argv) {
	printf("Hello world!\n");
	return 0;
}
EOF

gcc -static -o init init.c
echo init | cpio -ov --format=newc > initramfs.img
```

## Generate a initramfs with BusyBox

## Boot Kernel on QEMU with initramfs

```console
qemu-system-x86_64 \
    -kernel linux-*/arch/x86/boot/bzImage \
    -initrd initramfs.img \
    -append "console=ttyS0 root=/dev/ram init=/init" \
    -nographic

# Use ""Ctrl-A X" to kill the run
```
