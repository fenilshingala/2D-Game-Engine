* Place 'SDL2.dll' and 'glew32.dll' in:
      C:\WINDOWS
  or other common location for DDLs.

* Place 'SDL2.lib', 'SDL2main.lib', and 'glew32.lib' in an appropriate
  location for library files

  ** For Visual Studio 2015, you can use
         C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib

  ** For Visual Studio 2017, you will need to locate this on your machine.
     From the Visual Studio 2017 command prompt, type
         echo %LIB%
     This will produce a semi-colon separated list of library directories.
     Choose one.  On my machine, I used
         C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.13.26128\lib\x86

* Place the folders (and their contents) 'SDL2', 'GL', and 'glm' in an appropriate
  location for include files

  ** For Visual Studio 2015, you can use
         C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include

  ** For Visual Studio 2017, again you will need to find a location.
     You can do this by tying
         echo %INCLUDE%
     from the Visual Studio 2017 command prompt.  Choose one of the
     directories.  On my machine, I used
         C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.13.26128\include


If you are working on a DigiPen machine (and don't have administrative
privileges), you can do the following.  In the folder that you are working in:

* Place copies of 'SDL2.lib', 'SDL2main.lib', 'SDL2.dll', 'glew32.lib', and
  'glew32.dll' in the current folder.

* Place copies of the folders (and their contents) 'SDL2', 'GL', and 'glm' in the
  current folder.

* Compile your code from the Visual Studio 2015/2017 Command Prompt using
      cl /EHsc /I. <file(s)> SDL2.lib SDL2main.lib opengl32.lib glew32.lib /link /subsystem:console

