# Windows 3.1x
You could be hardcore enough (I did it :-)) to install MS DOS 6.22 and Windows 3.11 in a virtual machine. This would require some extra attention to slow down your system, install a suitable mouse driver or setup a CD-ROM. In the very end I am adding some links.

## Recommended way: Use DOSbox-X
[DOSBox-X](https://dosbox-x.com/) is a DOS emulator being available for Linux, Windows and macOS. There are many manuals on how to install Windows 3.1x in DOSbox-X. This paragraph quickly summarizes my experience. I followed the [DOSbox-X Wiki](https://dosbox-x.com/wiki/Guide:Installing-Windows-3.1x):
- In all versions but the Snap-version I had troubles in Ubuntu when moving the DOSbox-X window on my second monitor. That's the main reason why I went with the [Snap-Version](https://snapcraft.io/dosbox-x).
- There seems to be no real cool way of storing the config file. It has to be copied (not just linked!) into the folder `~/snap/dosbox-x/current/.config/dosbox-x/`. I created a config named `win31.conf` as shown below.
- As I am German I preferred a German keyboard and had to call `KEYB DE` from the command line.
- To install Windows I copied all installation files into one folder and mounted it as drive D: `MOUNT D <path to installation files>`. As an installation target is required as well I created an empty directory and mounted this as drive C: `MOUNT C <path to directory>`.
- I used the [S3 Trio64](https://vogonsdrivers.com/getfile.php?fileid=275&menustate=0) graphic card, as it was named as the default selection.

My win31.conf looks as follows:
```
[sdl]
autolock=true

[dosbox]
title=Windows 3.1x
memsize=256
machine=svga_s3trio64

[dos]
hard drive data rate limit=0
floppy drive data rate limit=0

[cpu]
cputype=pentium
core=normal

[pci]
voodoo=false

[ide, primary]
int13fakeio=true
int13fakev86io=false

[render]
scaler=none

[autoexec]
KEYB DE
MOUNT C <full path to directory containing my drive C>
SET PATH=%PATH%;C:\WINDOWS;
SET TEMP=C:\WINDOWS\TEMP
C:\WINDOWS\WIN.COM
EXIT
```

## Hardcore way: Run MS DOS 6.22 & Windows 3.11 in a VirtualBox
Ingredients:
- DJ-feeling of having fun when changing floppy disks 
- MS DOS 6.22
- MS Windows 3.1x
- [DOSidle](http://www.win16.info/files/patch/dosidle.img) in version 2.10, as newer versions seem to break Windows
- [WinFloat](http://www.win16.info/files/winfloat.img) to solve 16-bit troubles with by using hide87.com
- A [suitable mouse driver](https://git.javispedro.com/cgit/vbados.git/about/) and more
- maybe [WQGHLT.386](http://www.win16.info/files/patch/wqghlt.img) to solve some other troubles

As DOS and Windows do not deal with shared folders out of the box, you may need to "transport" files and programs via virtual floppy disks into the system. To do so, I used this way of creating disk images in Linux:
```bash
dd if=/dev/zero of=disk01.img bs=512 count=2880
mkfs.msdos -F 12 -n "name" disk01.img
# XX = next free /dev/loop-device
sudo losetup /dev/loopXX disk01.img
sudo mount /dev/loopXX /mnt
# copy required files (be aware: 1.44MB only!)
sudo umount /mnt
sudo losetup -d /dev/loopXX
```

Finally, my config.sys and autoexec.bat looked as follows:
```
DEVICE=C:\DOS\EMM386.EXE NOEMS
LASTDRIVE=Z
DEVICE=C:\DOS\OAKCDROM.SYS /d:idecd000
```

```
C:\DOS\MSCDEX.EXE /d:idecd000
C:\VIRTBOX\HIDE87.COM
C:\VIRTBOX\DOSIDLE.EXE
C:\VIRTBOX\VBMOUSE.EXE install
C:\VIRTBOX\VBMOUSE.EXE wheelkey updn
C:\VIRTBOX\VBSF.EXE
```