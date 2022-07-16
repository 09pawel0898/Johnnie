echo [97m
if not exist ../Build  (
	cd ..
	mkdir Build
	cd Build
) else (
	cd ../Build
)
echo [36m
cmake .. -G "Visual Studio 16 2019"
echo [97m
pause