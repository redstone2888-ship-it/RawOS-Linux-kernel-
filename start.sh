# change this to /bin/bash if you're on linux
#!/usr/bin/env bash
cd "$(dirname "$0")"
ls

cd rootfs
find . | cpio -o -H newc | gzip > ../initramfs.img
cd ..

cp initramfs.img iso/boot
rm -r initramfs.img

qemu-system-x86_64 \
  -m 512M \
  -kernel iso/boot/vmlinuz-linux \
  -initrd iso/boot/initramfs.img \
  -append "console=ttyS0" \
  -nographic