# Playing with Linux Kernel

## Prerequisits

```console
sudo apt install -y \
    curl \
    build-essential \
    libelf-dev \
    libssl-dev \
    libncurses5-dev \
    qemu-system-x86 \
    xz-utils \
    flex \
    bison \
    bc

PROJ_DIR="/tmp/sandbox/linux"
SRC_DIR="$PROJ_DIR/src"
BUILD_DIR="$PROJ_DIR/build"
mkdir -pv "$PROJ_DIR" "$SRC_DIR" "$BUILD_DIR"

```

## Build Kernel

Get the latest kernel link from https://www.kernel.org :

```console
KERNEL_URL="https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.9.6.tar.xz"
KERNEL_BUILD="$BUILD_DIR/linux"

cd "$SRC_DIR"
curl "$KERNEL_URL" | tar xJf -

cd linux-6.9.6
make distclean
make defconfig O="$KERNEL_BUILD"
make -j "$(nproc)" O="$KERNEL_BUILD"
```

## Generate a Minimal initramfs

```console
cat > "$SRC_DIR/init.c" << EOF
#include <stdio.h>
int main(int argc, char **argv) {
	printf("Hello world!\n");
	return 0;
}
EOF

gcc -static -o "$BUILD_DIR/init" "$SRC_DIR/init.c"
echo "$BUILD_DIR/init" | cpio -ov --format=newc > "$BUILD_DIR/initramfs.img"
```

## Generate a initramfs with BusyBox

### Build BusyBox

The latest busybox version could be checked on https://busybox.net/downloads/ .

```console
BUSYBOX_VER=1.36.1
BUSYBOX_SRC="$SRC_DIR/busybox-${BUSYBOX_VER}"
BUSYBOX_BUILD="$BUILD_DIR/busybox"
mkdir -pv "$BUSYBOX_BUILD"

cd "$SRC_DIR"
curl https://busybox.net/downloads/busybox-${BUSYBOX_VER}.tar.bz2 | tar xjf -
cd "$BUSYBOX_SRC"
make O="$BUSYBOX_BUILD" defconfig

cd "$BUSYBOX_BUILD"
make menuconfig
# To enable BusyBox building as a static binary:
# Busybox Settings ---> Build Options ---> Build BusyBox as a static binary (no shared libs) ---> yes

make -j "$(nproc)"
make install
```

### Generate a initramfs

```console
INITRAMFS_BUILD="$BUILD_DIR/initramfs"
mkdir -pv "$INITRAMFS_BUILD"
cd "$INITRAMFS_BUILD"
mkdir -p bin sbin etc proc sys usr/bin usr/sbin
cp -a "$BUSYBOX_BUILD"/_install/* .

cat > init << EOF
#!/bin/sh

mount -t proc none /proc
mount -t sysfs none /sys

echo Boot took "$(cut -d' ' -f1 /proc/uptime)" seconds
exec /bin/sh
EOF

chmod +x init
find . -print0 | cpio --null -ov --format=newc | gzip -9 > "$BUILD_DIR/initramfs.cpio.gz"
```

## Boot Kernel on QEMU with initramfs

```console
qemu-system-x86_64 \
    -kernel linux-*/arch/x86/boot/bzImage \
    -initrd initramfs.img \
    -append "console=ttyS0 root=/dev/ram init=/init" \
    -nographic
```

Use "Ctrl-A X" to kill the run.
