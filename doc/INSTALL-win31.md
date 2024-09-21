# Windows 3.1x
You could be hardcore enough (I did it :-)) to install MS DOS 6.22 and Windows 3.11 in a virtual machine. This would require some extra attention to slow down your system, install a suitable mouse driver or setup a CD-ROM. In the very end I am adding some links.



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
```config.sys
DEVICE=C:\DOS\EMM386.EXE NOEMS
LASTDRIVE=Z
DEVICE=C:\DOS\OAKCDROM.SYS /d:idecd000
```

```autoexec.bat
C:\DOS\MSCDEX.EXE /d:idecd000
C:\VIRTBOX\HIDE87.COM
C:\VIRTBOX\DOSIDLE.EXE
C:\VIRTBOX\VBMOUSE.EXE install
C:\VIRTBOX\VBMOUSE.EXE wheelkey updn
C:\VIRTBOX\VBSF.EXE
```