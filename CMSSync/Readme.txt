
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

TODO: 
If changed Check Card Policy or DestOU then do terminate-create? I can get list of CardPolicy by method findApplicationSets and check if it is changed. 
Instead of ADOnly transaction type it is better add list of CcmAttributes in same level as HintAttributes. If one of CcmAttributes changed then submitCPR.

ReleseNotes:
Jul 13 2016
18. Call CreateCPR only if GetLifecycleStatus returns empty string. I.e. no card is issued.
17. AIMS_NO_SUCH_USER special EventID 12.
16. Print Null-ed attributes when Hint not found. 
15. Accountdisabled and Smartcardrequired allowed in transition rules.
14. Filter OudnTomonitor is matched by contains.
13. In HealthCheck print all unavailable AD(s) in one message 'the following health check failed ...'  with special EventID=120.
12. In Polling don't print if AD is unavalable. Only if all AD(s) is unavalable?

Jul 07 2016
11. On startup, if all of Dest AD is down then don't start.
10. Check that the account exists in all secondary destination AD. Only after the account exists in all others Dest AD then send in the CPR to CMS.
9. Adhint SMARTCARD_REQUIRED (True / False)
8. Adhint ACCOUNTDISABLE (True / False)
7. * in hintAttribute

6. If CCM submit fails then Rollback Destination AD action (delete user or rollback attributes). Print warning to log with EventID=151. If this occurred in startup then accounts stored in list and will be processed again during Polling.
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
