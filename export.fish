# the purpose of this script is to gather all local changes into repo destination

set destinations .config/fish .config/alacritty .config/libreoffice .config/nvim .xinitrc

cd ~/

for destination in $destinations
    cp -rf --parents $destination ~/linux-preset/
end

cp /etc/xdg/picom.conf ~/linux-preset/

