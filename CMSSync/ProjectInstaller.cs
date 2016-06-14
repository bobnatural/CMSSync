using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace AdPoolService
{
    // Provide the ProjectInstaller class which allows 
    // the service to be installed by the Installutil.exe tool
    // %Windir%\Microsoft.NET\Framework\v4.0.30319\installutil /LogFile=inst.log /LogToConsole=true Cmssync.exe
    // %Windir%\Microsoft.NET\Framework\v4.0.30319\installutil /u Cmssync.exe
    [RunInstaller(true)]
    public class ProjectInstaller : Installer
    {
        private ServiceProcessInstaller process;
        private ServiceInstaller service;

        public ProjectInstaller()
        {
            process = new ServiceProcessInstaller();
            process.Account = ServiceAccount.LocalSystem;
            service = new ServiceInstaller();
            service.ServiceName = "CMS Syncronization Service";
            service.Description = "Provides Active Directory account sync";
            service.DisplayName = "CMSSync";            

            Installers.Add(process);
            Installers.Add(service);
        }
    }
}
