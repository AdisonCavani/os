# os
Operating system

### Make

```
make build-x86_64
```

### Docker

```
docker run --rm -it -v $(pwd):/root/env myos-buildenv
```

### Qemu

```
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
```