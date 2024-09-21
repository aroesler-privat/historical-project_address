# 1996: My application as a part-time programmer

Like probably every other student, I wanted to earn my own money alongside school. And because I spent a lot of time programming anyway and was also allowed to work on a small project at university, a job as a programmer was an obvious choice. Spoiler: Nothing came of it :-).

At university I programmed in C under Linux. That seemed more exciting to me than the [Pascal stuff](https://github.com/aroesler-privat/historical-project_chemistry) I had been working on for the last few months. So I read up on programming in C under Windows on the side. Just to mention it: YouTube hadn't been invented yet, I had to take the bus to university for the internet and the most useful information was available in books I borrowed from the library. Sounds almost prehistoric from today's perspective. 

> [!NOTE]
> Untouched since 1996-04-11. Pay attention to the date of the last write access ("letzter Schreibzugriff")!

![Screenshot of the Code in Action](screenshots/db_about.jpg)

<img src="screenshots/main.jpg" width="18%"></img> <img src="screenshots/search.jpg" width="18%"></img> <img src="screenshots/db_customize.jpg" width="18%"></img> <img src="screenshots/bc45_dialogs.jpg" width="18%"></img> <img src="screenshots/bc45_directories.jpg" width="18%"></img> 

# Run it

Obviously you need a compatible version of Windows to run this great address manager. In this case, the limit is probably a version that can handle 16-bit. It still worked in Windows 7 :-). However, Windows 3.1x is the most recommended version for an appropriate retro feeling.
- install Windows 3.1x as described [here](doc/INSTALL-win31.md)
- download both files from [/bin](bin/) to your PC
- start ADRESSE.EXE from File Manager
- create a new database

There are a few hiccups you should know about: I never implemented database encryption. Printing works, but somehow not really. And the search is still just behind the usability standards of 1996 :-).

# Compile the source code

To compile the source code you need Borland C++ 4.5. I have built the dialogs in the [Resource Workshop](screenshots/bc45_dialogs.jpg). The project file ([address.ide](src/ADRESSE.IDE)) must be opened in the IDE (graphical user interface of the compiler). The source code cannot be compiled in Win32. Please make sure to [set the directories](screenshots/bc45_directories.jpg) according to your setup. 