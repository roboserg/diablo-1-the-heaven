The Hell 2  readme
Project development started 2017-12-22


The Hell 2 is a next generation mod after The Hell. Having achieved the stage
of synchronization with The Hell, it now begins going its own way. It's in active development.
Lots of content is being added, new gameplay features implemented. A lot of changes happening
at this phase, but it's growing and shaping into a massive mod with tons of improvements.
For now, there is no stability from one release to another. Items will morph, other things - too.
Stability will be achieved when version 1.00 is released.


-------------------------------------------------------------------------------------------------------


Instructions:
-------------

Configure by altering settings in config.ini. There are some useful options:
-------------------------------------------------------------------------------

	Changes screen resolution. For non-windowed fullscreen you must set only hardware supported resolution.

		set screenwidth 640
		set screenheight 480

	Startup windows state. You can switch fullscreen mode by pressing Alt + Enter:
	     For windowed fullscreen mode set win[off/on] mode.

		set fullscreen off/on/win/winon/winoff

	Set maximum fps rendering and show it. You can switch fps on the fly by Alt + F

		set fps 20/40/60/80
		set showfps on/off

	Set v-sync (default on, for G-Sync monitor may require turning off)

		set vsync on/off

	Launch title video on start:

		set startvideo off/on

	Turn on easy autoclick mode. You can auto attack and auto cast spells without Shift key (off by default):
		
		set easyclick on/off

	Turn on auto pickup gold.  You can switch it on the fly by Alt + A :
		
		set autopickup on/off

	Save files are stored in \save_sp and \save_mp folders

		set savefolder on/off

	Turn on automatic backup for game saves
		
		set autobackup on/off

4. Parameters in config.ini for special TH 1-2 testing/comparing mode:
--------------------------------------------------

	When you run game in testing mode you see both TH playing at the same time on screen.
	You can just play game as usual, and then program noticed any difference in memory it stops
	and write two log files: th1.log and th2.log, which you have to send to developers to fix the error.
	You can send them to TheHell@support.assembla.com with description of gameplay situation just before the break,
	or create new support request on https://app.assembla.com/spaces/TheHell/support/tickets (anonymous posts with file attachment are allowed).
	
	You can use God Mode (Alt + G) and Free Spells (Alt + S) only in this testing mode.
	
	To turn on testing mode you can set or add this essential options in config.ini file:
		
		set exevalidation on
		set autocopysave on
		set loadonstart [testing_save_file_index]
		set difficulty [testing_difficulty]
		set newgame on/off	

	for example:

		set exevalidation on
		set autocopysave on
		set loadonstart m0 //multi player hellmp_0.drv testing
		set difficulty 2 //doom difficulty
		set newgame on //does not matter in multi player

	or:

		set exevalidation on
		set autocopysave on
		set loadonstart s0 //single player  hellsp_0.hsv testing
		set difficulty 0 //horror difficulty
		set newgame on //start new game in single player

	or:

		set exevalidation on
		set autocopysave on
		set loadonstart s1 //multiplayer hellsp_1.hsv testing
		set difficulty 1 //purgatory difficulty, but does not matter when loading single player game
		set newgame off //load saved game in single player
	
	Description for this options:

	4.1. Turns on simultaneously THs mode, TH 1 and TH 2 launch together in two windows at the same time:
		 (TH 1 in the top window, TH 2 in the bottom window)

		set exevalidating on/off

	4.2. Loads savefile specified by slot number on start (skips main menu):
		 For example: "set loadonstart s7" will load hellsp_7.hsv in TH 1 and hell2sp_7.hsv in TH 2.
		 This parameter is recommended to use in two-windows mode, because TH meta-menu doesn't support multithreading.
		 Also this parameter works in standalone mode when you just playing TH 2 or TH 1.

		set loadonstart off/s0-9/m0-9

	4.3. Set difficulty for multiplayer auto load.

		set difficulty 0-2

	4.4. Turn on automatic synchronization overwriting th2 save file with th1 on start (including stash file)
		
		set autocopysave on/off

	4.5. Load or renew single player game on start

		set newgame on/off	

	4.6. Dispose windows horizontaly

		set vertical off/on

