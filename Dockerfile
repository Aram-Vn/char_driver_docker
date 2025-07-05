FROM archlinux:latest

RUN pacman -Syu --noconfirm \
    base-devel \
    linux \
    linux-headers \
    git \
    nano \
    bash \
    sudo \
    vim

# Add non-root user for safer work
RUN useradd -m dev && echo "dev ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
USER dev
WORKDIR /home/dev

