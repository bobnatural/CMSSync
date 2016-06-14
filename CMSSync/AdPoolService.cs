using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace AdPoolService
{
    public partial class AdPool : ServiceBase
    {
        public AdPool()
        {
            // the Windows Service Name 
            ServiceName = "Active Directory Pool Service";

            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            BackgroundPool.Run();
        }

        protected override void OnStop()
        {
            BackgroundPool.Stop();
        }
    }
}
