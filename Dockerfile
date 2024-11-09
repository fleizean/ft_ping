# Dockerfile
FROM ubuntu:latest

# Update and install necessary packages
RUN apt-get update && apt-get install -y \
    openssh-server \
    sudo \
    gcc \
    make

# Create a user and set password
RUN useradd -ms /bin/bash dockeruser
RUN echo 'dockeruser:dockerpassword' | chpasswd

# Add user to sudoers
RUN usermod -aG sudo dockeruser

# Configure SSH
RUN mkdir /var/run/sshd
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config
RUN echo 'PasswordAuthentication yes' >> /etc/ssh/sshd_config

# Copy parent directory contents into the container
COPY . /home/dockeruser/

# Set the working directory
WORKDIR /home/dockeruser/

# Expose SSH port
EXPOSE 22

# Start SSH service
CMD ["/usr/sbin/sshd", "-D"]


# How to build the Docker image
# docker build -t ssh-server .
# How to run the Docker container
# docker run -d -p 2222:22 --name ssh-server ssh-server
# How to SSH into the Docker container
# ssh -p 2222 dockeruser@localhost
# Password: dockerpassword
# How to known host key
# ssh-keygen -f "/home/username/.ssh/known_hosts" -R "[localhost]:2222"

# How to stop the Docker container
# docker stop ssh-server

# How to remove the Docker container
# docker rm ssh-server