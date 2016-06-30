
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

4. Command line paramaters for CMSSync.exe:
/setcredential - input configurations for AD servers.
/update 	- update password for AD servers.
/d		- start as console app for debug.
/?		- help

todo: 
If Ccm submit wasn't successful delete the account if we were doing a create transaction.

ReleseNotes:

5. if AD SSL connection failed but ClearLDAP works fine then look at System Log "Event ID: 36881 Source: Schannel".
4. QualityCheck EventID=15 (Error)
3. "ADHint is not found ... " EventID=150 (Warning)
2. "ADHint is not found ... " prints if this settings is True:
      <setting name="DataMismatchLogging" serializeAs="String">
        <value>True</value>
      </setting>

1. Added <QualityCheck>:
       <ADHint>
		<HintAttributes>
		....
        </HintAttributes>

        <QualityCheck>
          <CheckAttribute Name="mail">
          <CheckAttributeValues>
            <add Value="*asd"/>
            <add Value="qwe*"/>
            <add Value="*dfg*"/>
          </CheckAttributeValues>
          </CheckAttribute>
          <CheckAttribute Name="userPrincipalName">
            <CheckAttributeValues>
              <add Value="*"/>
            </CheckAttributeValues>
          </CheckAttribute>
        </QualityCheck>
        
      </ADHint>
