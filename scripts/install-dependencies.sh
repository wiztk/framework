#!/usr/bin/env sh

which lsb_release 1>/dev/null 2>&1

if [ $? = 0 ]; then
    OS=$(lsb_release -si)
elif [ -f /etc/os-release ]; then
    source /etc/os-release
    OS=${NAME}
else
    echo "Unknown OS"
fi

ARCH=$(uname -m | sed 's/x86_//;s/i[3-6]86/32/')
# VER=$(lsb_release -sr)
PACKAGES=""

if [ "$OS" = "Ubuntu" ]; then
	PACKAGES="cmake \
		libwayland-dev \
		libxkbcommon-dev \
		libegl1-mesa-dev \
		libgl1-mesa-dev \
		libgles2-mesa-dev \
		libvulkan-dev \
		libjpeg-dev \
		libgif-dev \
		libfreetype6-dev \
		libfontconfig1-dev \
		libexpat1-dev \
		zlib1g-dev \
		libwebp-dev \
		liblua5.3-dev \
		libinput-dev \
		libavcodec-dev \
		libavformat-dev \
		libavdevice-dev \
		libavutil-dev \
		libavfilter-dev \
		libswscale-dev \
		libpostproc-dev \
		libswresample-dev \
		doxygen \
		graphviz"
	sudo apt install ${PACKAGES}
elif [ "$OS" = "Fedora" ]; then
    echo "Please make sure that rpmfusion repos were loaded."
	PACKAGES="gcc-c++ \
		cmake \
		wayland-devel \
		libxkbcommon-devel \
		mesa-libwayland-egl-devel \
		mesa-libGLES-devel \
		mesa-libGL-devel \
		mesa-libEGL-devel \
		vulkan-devel \
		libwebp-devel \
		libjpeg-turbo-devel \
		giflib-devel \
		expat-devel \
		zlib-devel \
		lua-devel \
		libinput-devel \
		freetype-devel \
		fontconfig-devel \
		ffmpeg-devel \
		OpenImageIO-devel \
		libicu-devel \
		OpenEXR-devel \
		doxygen \
		graphviz"
	# sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm
	sudo dnf install ${PACKAGES}
else
	echo "Unsupported Linux distribution!"
fi

# Recommended to install ccache.
