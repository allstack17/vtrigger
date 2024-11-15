# vtrigger
valorant triggerbot

built with LLVM clang && uses https://github.com/nlohmann/json.

tested on: ryzen 7 7700 4.5GHz 32gb 4800MHz
average reaction time: 3-8 ms

# how to use
in config file change value from 0x for yourself 
(take values from https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)

set the color of your opponents outlines to purple. If you want a different one, you'll have to rebuild the project (Im too lazy to make color parameters in the config)

download .exe file - https://disk.yandex.ru/d/tgt6ILA35nJoGw
----------------------------------------------------------------------------------------------------------
*  hotkey - the key at which the trigger will be active (by default it is left alt)
*  delay - delay between shots
*  second_key - in the game set the 2nd key for shooting on the keyboard (by default it is K).
*  always enable - the hotkey will be ignored and the trigger will work all the time
*  zone - trigger reaction zone (x, y)
*  tolerance - color tolerance (trigger sensitivity setting. default is 70)
