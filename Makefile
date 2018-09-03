# Digital Logic
# www.d-logic.net
#
# Helper Makefile
#
$(VERBOSE).SILENT:

PRJNAME = ulc_example

all : help

help info : 
	echo "----------------------------------------------"
	echo "Making example to test Reader opening function"
	echo "Project output name: $(PRJNAME)"
	echo "----------------------------------------------"
	echo "For 32 bit Windows - type:        'make win32'"
	echo "For 64 bit Windows - type:        'make win64'"
	echo "For 32 bit Linux - type:          'make lin32'"
	echo "For 64 bit Linux - type:          'make lin64'"
	echo "----------------------------------------------"

lin32 linux32 :
	cd linux32_release && gcc -m32 -o $(PRJNAME) ../src/*.c -I../lib/include -L../lib/linux/x86 -luFCoder-x86 -Wl,-R../lib/linux/x86
	echo "Making done, without errors."
	echo "To run the example - type:"
	echo "   'cd linux32_release && ./$(PRJNAME)'"

lin64 linux64 :
	cd linux64_release && gcc -m64 -o $(PRJNAME) ../src/*.c -I../lib/include -L../lib/linux/x86_64 -luFCoder-x86_64 -Wl,-R../lib/linux/x86_64
	echo "Making done, without errors."
	echo "To run the example - type:"
	echo "   'cd linux64_release && ./$(PRJNAME)'"

win32 :
	echo "If You see error like: 'skipping incompatible lib/windows/x86/uFCoder-x86.dll when searching for...'"
	echo "  This means You have the gcc compiler for 64 bit"
	echo "  type 'make win64' instead of 'make win32'"
	echo ""
	cd win32_release && gcc -o $(PRJNAME) ../src/*.c -I../lib/include -L../lib/windows/x86 -luFCoder-x86 -Wl,-Rlib/windows/x86 -Wl,--enable-stdcall-fixup
	echo "Making done, without errors."
	echo "To run the example - type:"
	echo "   'cd win32_release'"
	echo "   'run_me'"
	# generate run_me
	echo "@echo off" > win32_release\\run_me.cmd
	echo "set path=..\lib\windows\x86;%path%" >> win32_release\\run_me.cmd
	echo "$(PRJNAME).exe" >> win32_release\\run_me.cmd

win64 :
	echo "If You see error like: 'lib/windows/x86_64/uFCoder-x86_64.dll: file not recognized: File format not recognized'"
	echo "  This means You have the gcc compiler for 32 bit"
	echo "  type 'make win32' instead of 'make win64'"
	echo ""
	cd win64_release && gcc -o $(PRJNAME) ../src/*.c -I../lib/include -L../lib/windows/x86_64 -luFCoder-x86_64 -Wl,-Rlib/windows/x86_64
	echo "Making done, without errors."
	echo "You must use library from the 'lib\windows\x86_64\'"
	echo ""
	echo "To run the example - type:"
	echo "   'cd win64_release'"
	echo "   'run_me'"
	# generate run_me
	echo "@echo off" > win64_release\\run_me.cmd
	echo "set path=..\lib\windows\x86_64;%path%" >> win64_release\\run_me.cmd
	echo "$(PRJNAME)" >> win64_release\\run_me.cmd
