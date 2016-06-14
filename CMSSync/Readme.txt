
1. To Install Service
%Windir%\Microsoft.NET\Framework\v4.0.30319\installutil /LogFile=inst.log /LogToConsole=true CMSSync.exe

2. To UnInstall Service
%Windir%\Microsoft.NET\Framework\v4.0.30319\installutil /u CMSSync.exe

3. Structure of folder:
CMSSync.exe
ccmapi.dll
CPR 2.1.8.sample.signed.xml
CMSSync.exe.config
SDK dlls.

4. Command line paramaters for AdPoolService.exe:
/setcredential - input configurations for AD servers.
/update 	- update password for AD servers.
/d		- start as console app for debug.

5. Bulk copy accounts.
5.1. Start service first time. Wait till file Invocation_*.txt created.
5.2. Stop service.
5.3. Edit Invocation_*.txt and set number to 0.
5.4. Start service. All accounts since USN = 0 will be processed.
