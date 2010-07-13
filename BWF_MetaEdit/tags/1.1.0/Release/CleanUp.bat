@rem echo off
@rem --- General ---
cd ..
del *.~* *.obj *.o *.tds *.dcu *.ddp *.opt *.ncb *.suo *.ilk *.idb *.pdb *.pch *.plg *.aps *.user *.win *.layout *.local *.depend *.identcache *.tgs *.tgw /Q /S
del *.~* *.obj *.o *.tds *.dcu *.ddp *.opt *.ncb *.suo *.ilk *.idb *.pdb *.pch *.plg *.aps *.user *.win *.layout *.local *.depend  *.identcache *.tgs *.tgw /AH /Q /S
del *.ogg *.ogm *.avi *.mkv /Q /S
del *.ogg *.ogm *.avi *.mkv /AH /Q /S
cd Release

@rem --- In Project files ---
cd ..\Project
del *.exe *.dll *.a *.lib *.exp *.class *.zip *.7z /Q /S
del *.exe *.dll *.a *.lib *.exp *.class *.zip *.7z /AH /Q /S
cd ..\Release

@rem --- MS Visual Studio specific ---
cd ..\Project
rmdir MSVC\CLI\Release /Q /S
rmdir MSVC\CLI\Release_Ansi /Q /S
rmdir MSVC\ShellExtension\Release /Q /S
cd ..\Release

@rem Borland Developper Studio specific ---
cd ..\Source
rmdir CLI\__history /Q /S
rmdir Common\__history /Q /S
rmdir GUI\__history /Q /S
rmdir GUI\Qt\__history /Q /S
rmdir GUI\Qt\_Automated\__history /Q /S
rmdir Install\__history /Q /S
cd ..\Release
cd ..\Project
rmdir BCB\__history /Q /S
rmdir BCB\CLI\__history /Q /S
rmdir BCB\CLI\Debug_Build /Q /S
rmdir BCB\CLI\Release_Build /Q /S
rmdir BCB\GUI\__history /Q /S
rmdir BCB\GUI\Debug_Build /Q /S
rmdir BCB\GUI\Release_Build /Q /S
rmdir BCB\PreRelease\__history /Q /S
rmdir BCB\PreRelease\Debug_Build /Q /S
rmdir BCB\PreRelease\Release_Build /Q /S
rmdir MSVC\CLI\Win32 /Q /S
rmdir MSVC\CLI\x64 /Q /S
rmdir MSVC\GUI\Win32 /Q /S
rmdir MSVC\GUI\x64 /Q /S
cd ..\Release

@rem Code::Blocks specific ---
cd ..\Project
rmdir CodeBlocks\CLI\.objs /Q /S
rmdir CodeBlocks\GUI\.objs /Q /S
rmdir CodeBlocks\PreRelease\.objs /Q /S
cd ..\Release

@rem GCC specific ---
cd ..\Project
rmdir GCC\CLI\.deps /Q /S
rmdir GCC\CLI\.objs /Q /S
rmdir GCC\CLI_WithoutLib\.deps /Q /S
rmdir GCC\CLI_WithoutLib\.objs /Q /S
rmdir GCC\GUI\.deps /Q /S
rmdir GCC\GUI\.objs /Q /S
cd ..\Release

@rem --- Release specific ---
del *.zip *.gz *.bz2 *.lib *.dll *.exp *.a *.so *.7z /Q /S
del AVWG*.* Debug*.* /Q /S
rmdir BCB\GUI\Plugins /Q /S