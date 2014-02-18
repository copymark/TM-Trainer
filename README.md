TM-Trainer
==========

Trackmania Nations, United and Trackmania 2 Hack

Feel free to contribute new features, create issues or write a wiki.

You can share this site with your friends: [GitHub Tm-Trainer](http://copymark.github.io/TM-Trainer/)

If you want to support me with money or you are not able to compile this hack then you can still buy the hacks on my page: [Copymark Productions](http://copy-mark.de/trainer-tools.html)

***
##Installation
You may have to fix some linking errors before you are able to compile the trainer. Im sorry that the errors are in german but you should be able to see what they are about.

###Error 1
```
Fehler 1 error C1083: Datei (Include) kann nicht geöffnet werden: "d3dx9.h": No such file or directory c:\tm-trainer\d3dhook\dllmain.cpp 7 1
```
####Solution
Install the DirectX SDK (Download) or just unload the project "d3dhook". (Right click -> unload project)

###Error 2 (and all other include errors)
```
'Fehler 2 error C1083: Datei (Include) kann nicht geöffnet werden: "Error.h": No such file or directory c:\tm-trainer\tm_trainer\Globals.h 10 1 TMUF_Trainer'
````
####Solution
Add tlTrainer directory to your additional include directories. (in all three trainer projects)
..\tlTrainer

***
Best Regards 
**copymark**
