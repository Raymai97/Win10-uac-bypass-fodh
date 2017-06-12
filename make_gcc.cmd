@echo off
md bin 2>nul
gcc -s -o bin\w10-uac-bypass-fodh.exe ^
	src\w10-uac-bypass-fodh.c ^
	src\CurrProcUserIsAdmin.c ^
	-ladvapi32 ^
	-lole32 || pause
exit
