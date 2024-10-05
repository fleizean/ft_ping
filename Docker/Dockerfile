# Dockerfile
FROM ubuntu:latest

# Update and install necessary packages
RUN apt-get update && apt-get install -y \
    openssh-server \
    sudo

# Create a user and set password
RUN useradd -ms /bin/bash dockeruser
RUN echo 'dockeruser:dockerpassword' | chpasswd

# Add user to sudoers
RUN usermod -aG sudo dockeruser

# Configure SSH
RUN mkdir /var/run/sshd
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config
RUN echo 'PasswordAuthentication yes' >> /etc/ssh/sshd_config

# Expose SSH port
EXPOSE 22

# Start SSH service
CMD ["/usr/sbin/sshd", "-D"]