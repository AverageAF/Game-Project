del %FIRSTPROJDIR%\x64\Debug\Assets\ /Q
xcopy %FIRSTPROJDIR%\Assets %FIRSTPROJDIR%\x64\Debug\Assets /i /y


del Assets.dat
del CopyAssets.log

REM  -bitmaps-

MyMiniz.exe Assets.dat + .\Assets\PixelFont(6x7).bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingDown0.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingDown1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingDown2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingLeft0.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingLeft1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingLeft2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingRight0.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingRight1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingRight2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingUp0.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingUp1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Suit0FacingUp2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Maps\Overworld01.bmpx >> CopyAssets.log

REM -music-

MyMiniz.exe Assets.dat + .\Assets\NewAdventures.ogg >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\MysteriousDeep.ogg >> CopyAssets.log

REM -sound-

MyMiniz.exe Assets.dat + .\Assets\item.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\itemshort.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\menu.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\ouch.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\ouchhurt.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\ouchlethal.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\SplashNoise.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\SplashNoise2.wav >> CopyAssets.log

REM -tilemaps-

MyMiniz.exe Assets.dat + .\Assets\Maps\Overworld01.tmx >> CopyAssets.log


REM copy Assets.dat into whatever directory it needs to be in