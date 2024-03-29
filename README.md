# dwm - dynamic window manager
dwm is an extremely fast, small, and dynamic window manager for X.
![alt text](./dwm-screen.png)

### Install packages from pkglist.txt by running<br>
`sudo pacman -S - < pkglist.txt`<br>
In case, the backup list includes foreign packages, such as AUR packages, remove them first, and then install the rest of packages:<br>
`sudo pacman -S $(comm -12 <(pacman -Slq | sort) <(sort pkglist.txt))`

### Customizations
Applied systray, pertag patches and configured statusbar to show
on all monitors.
Added keybindings for audio, backlight, screen locker, screenshots
and some programs.


### Requirements
In order to build dwm you need the Xlib header files.


### Installation
Edit config.mk to match your local setup (dwm is installed into
the `/usr/local` namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):<br>
`make clean install`

### Running dwm
Add the following line to your .xinitrc to start dwm using startx:<br>
`exec dwm`

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:<br>
`DISPLAY=foo.bar:1 exec dwm`

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:
`
    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm
`

### Configuration
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.
