# LCDHost on OS/X #

As of alpha 15, LCDHost is available for OS/X. There are still features that are available on Windows that have not yet been implemented.

  * The entire input subsystem is missing. Detecting and reading input devices such as keyboard, mice and joysticks won't work. LCD buttons do work, as they are provided by the LCD drivers.
  * The Qt support libraries for Lua aren't provided. There's nothing stopping someone from building them though.
  * Network interface filtering isn't implemented; network data is always the aggregate.
  * ...and of course a lot of other things that has currently slipped my mind.
