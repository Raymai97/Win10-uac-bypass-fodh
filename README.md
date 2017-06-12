# Win10 UAC bypass - fodhelper

[Details about the auto-elevation bug](https://winscripting.blog/2017/05/12/first-entry-welcome-and-uac-bypass/)

In short, the culprit is `fodhelper.exe` which is authorized to be auto-elevated **yet** its execution can be affected by current-user registry key. The good news is, if you set the UAC setting to the highest aka strictest, this trick won't work. However using the strictest setting mean the UAC will be very annoying.

To use this exploit in coding, you must disable the WOW64 redirection because there is no 32-bit `fodhelper.exe` in 64-bit Windows 10. If you `CreateProcess()` the `fodhelper.exe` directly, it will return `FALSE`, `GetLastError()` will return 740 which is `The requested operation requires elevation`. The workaround is to `CreateProcess()` the `cmd.exe` instead, with appropriate command-line so the CMD will start the `fodhelper.exe`.

I wonder how would Microsoft fix this issue. If you had used Vista, you would know why UAC was very annoying especially when you were doing file operation. To do something to a protected directory, Explorer would ask for your permission, then the UAC would ask again. WTF? Why don't just let UAC ask? Why ask twice? Then everyone know how Microsoft solved it. Instead of 'just let UAC ask', it shuts the UAC up by granting Explorer the power of auto-elevation. Now Microsoft has to make sure every `auto-elevation` EXE files would never get compromised.
