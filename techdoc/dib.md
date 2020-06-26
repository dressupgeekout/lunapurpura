# DIB format

This is kinda interesting...

```sh
./Choice1.PRX  	1: DIB -> [1000]	[1082, 4, 0, 3270]
./Choice2.PRX  	1: DIB -> [2000]	[1082, 4, 0, 3270]
./Choice3.PRX  	1: DIB -> [3000]	[1082, 4, 0, 3270]
./Choice4.PRX  	1: DIB -> [4000]	[1082, 4, 0, 3270]
./Choice5a.PRX  	1: DIB -> [5000]	[1082, 4, 0, 3270]
./Choice5b.PRX  	1: DIB -> [6000]	[1082, 4, 0, 3270]
./Choice5c.PRX  	1: DIB -> [7000]	[1082, 4, 0, 3270]
./Choice6a.PRX  	1: DIB -> [8000]	[1082, 4, 0, 3270]
./Choice6b.PRX  	1: DIB -> [9000]	[1082, 4, 0, 3270]
./Credits.PRX  	1: DIB -> [64000]	[1082, 0, 0, 3270]
./Global.PRX  	30: DIB -> [63000]	[1082, 33, 0, 209924]
./IDBackPk.PRX  ./IDGlobal.PRX
1: DIB -> [44000]	[1082, 4, 0, 3270]
./IDL/Arno.PRX  ./IDL/Arro.PRX  ./IDL/Bo.PRX  ./IDL/Chaz.PRX  ./IDL/Clev.PRX  ./IDL/Dana.PRX  ./IDL/Gen.PRX  ./IDL/Ging.PRX  ./IDL/Jess.PRX  ./IDL/Mavi.PRX  ./IDL/Max.PRX  ./IDL/Miko.PRX  ./IDL/Naki.PRX  ./IDL/Nico.PRX  ./IDL/Rock.PRX  ./IDL/Rube.PRX  ./IDL/Shar.PRX  ./IDL/Step.PRX  ./IDL/Teac.PRX  ./IDL/Viva.PRX  ./IDL/Whit.PRX  ./IDL/Wolf.PRX  ./IDLounge.PRX  ./IDNav.PRX  ./IDYear.PRX
./Replay.PRX  	1: DIB -> [700]	[1082, 4, 0, 3270]
./Scene1.PRX  	1: DIB -> [1000]	[1082, 4, 0, 3270]
./Scene2.PRX  	1: DIB -> [2000]	[1082, 4, 0, 3270]
./Scene3.PRX  	1: DIB -> [3000]	[1082, 4, 0, 3270]
./Scene4.PRX  	1: DIB -> [4000]	[1082, 4, 0, 3270]
./Scene5a.PRX  	1: DIB -> [5000]	[1082, 4, 0, 3270]
./Scene5b.PRX  	1: DIB -> [6000]	[1082, 4, 0, 3270]
./Scene6a.PRX  	1: DIB -> [8000]	[1082, 4, 0, 3270]
./Scene6b.PRX  	1: DIB -> [9000]	[1082, 4, 0, 3270]
./Shared2.PRX
./Splash.PRX  	1: DIB -> [500]	[1082, 0, 0, 3270]
./Terminal.PRX  	1: DIB -> [10000]	[1082, 4, 0, 3270]
	30: DIB -> [10100]	[1082, 33, 0, 1307307]
	33: DIB -> [10200]	[1082, 0, 0, 7873409]
./Title.PRX  	1: DIB -> [600]	[1082, 0, 0, 3270]
```

* Every DIB file is exactly 1082 bytes
* They are almost exclusively the topmost entry in the XPK file they appear in
* None of them appear in "character IDLs"
* they consist of a 2-byte magic (66,67 == "BM") and exactly 270 instances of 3 numbers + nul

* actually it's probably related to this https://en.wikipedia.org/wiki/BMP_file_format ... but why would there only be one per choice and one per scene

* Does "BM" stand for "bitmap" ?
* https://www-user.tu-chemnitz.de/~heha/petzold/ch15b.htm

* https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/ns-wingdi-tagbitmapfileheader
* https://msdn.microsoft.com/en-us/library/cc230318.aspx
