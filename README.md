# char_driver_docker

# Simple Linux Character Device Driver (with Docker Environment)

This repository provides a minimal Linux **character device driver**, along with a **Docker-based build and test environment** that works safely on any Linux host.

---

## Content

```bash
.
├── Dockerfile                  # Docker image for kernel module development
├── .gitignore                  # Ignore build artifacts
└── dev/
    └── simple_char_driver/
        ├── Makefile           # Kernel module build rules
        └── simple_char_driver.c  # The char device driver source
```

## Build Docker image
```bash
docker build -t char_dev_env .

# If you get permission denied, use
sudo docker build -t char_dev_env .
```

## Run the Docker container

```bash
docker run --rm -it \
  --privileged \
  --name char_dev \
  -v /lib/modules:/lib/modules \
  -v /usr/src:/usr/src \
  -v $(pwd)/dev:/home/dev/dev \
  char_dev_env
```

## Build the driver

```bash
cd ~/dev/simple_char_driver
make
```

## Insert and test the driver

```bash
sudo insmod simple_char_driver.ko
dmesg | tail
```

You’ll see something like: `Simple char driver loaded: major=509`

Now create the device node (replace 509 with your actual major):

```bash
sudo mknod /dev/simple_char c 509 0
sudo chmod 666 /dev/simple_char
```

## Testing

```bash
echo "Hello Driver" > /dev/simple_char
cat /dev/simple_char
```

Expected output: `Hello Driver`

## cleanup

```bash
sudo rmmod simple_char_driver
```