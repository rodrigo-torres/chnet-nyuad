## ROOT installation steps

### Install the dependencies
```
sudo dnf install libXpm-devel libXft-devel
sudo dnf install python2-devel
sudo dnf install python3-devel python3-neovim
```

### Install ROOT
```
# Download the source code from the project page, and navigate to the download directory
# Uncompress the file
cd root-6.xx
mkdir obj
cd obj
cmake ../
make
# You can accelerate the installation by using more threads like this
make -j 8
```


