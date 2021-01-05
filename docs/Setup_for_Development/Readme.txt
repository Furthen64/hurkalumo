WINDOWS INSTALLATION
=========================================================


	Code::Blocks		
	==============	
	Download and install Code::Blocks, with MinGW, 32-bit.
	
	Put the MinGW bin path in environment path,
	I added "C:\mingw\bin\" to %PATH%. 

	Restart computer.



	



	SFML Library - Source package
	================================
	Download the source and build the SFML library yourself. 
	Unzip to C:\SFML_src_2.5.0 for instance.

	
	Follow the instructions on their website and you'll be fine:
 	   https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php 

	I used the CMAKE Gui to configure and generate the library.

	First make the Release DLLs:
 

			BUILD_SHARED_LIBS 	[X]
			CMAKE_BUILD_TYPE	"Release" 

	And then after you have Generated the release, do another configure with:

			BUILD_SHARED_LIBS 	[X]
			CMAKE_BUILD_TYPE  	"Debug"
						



	After it's done, move the output of the compiled SFML library source from:
	   C:\SFML_build
	To:
	   C:\fat64\LIBS\SFML_2.5.0_FAT64

	
		(or something without the fat64 stuff)


	

	Setup Code::Blocks to use SFML
	====================================
	Now that we have compiled and placed the library files somewhere sensibly,
	we can setup Code::Blocks to make use of it.







	Setup the global vars to point to where the SFML library files are, C:\fat64\LIBS\SFML-2.4.0fat\
	See:	codeblocks_global_vars.png


	Great!
	Now follow the guides in these:
		Linker Settings Debug.png
		Linker Settings Release.png
		

	OK!
	Now go to the folder where the SFML dlls are. They should be in C:\fat64\LIBS\SFML-2.4.0fat\bin\
	and do the following:
 

		Copy these dlls to PATH-TO-src\hurka\bin\release\
		==========================================	
		sfml-audio-2.dll
		sfml-graphics-2.dll
		sfml-network-2.dll
		sfml-system-2.dll
		sfml-window-2.dll



		Copy these dlls to src\hurka\bin\debug\
		==========================================	
		sfml-audio-d-2.dll		<-- notice these have "d" in their names
		sfml-graphics-d-2.dll	
		sfml-network-d-2.dll
		sfml-system-d-2.dll
		sfml-window-d-2.dll



	And you should be able to compile and run HurkaLumo, at least as of 2018-07-11!


Troubleshooting
=====================================


	Look at the docs\Setup for Development\ pictures and see if 
	you find something that relates to your issue.
	
	For instance, LInker issues 1.png
	If you look at Linker Issues 1 - Resolution.png you should find out what to do.
