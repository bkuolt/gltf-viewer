sudo -i

# Script to install essential tools on a Debian-based system
apt install -y snap
snap install task --

# Install Conan using pipx
apt install -y pipx
pipx install conan  --force
pipx ensurepath
