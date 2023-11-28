using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace WindowsService1
{
    public partial class Service1 : ServiceBase
    {
        Timer timer = new Timer(); // name space(using System.Timers;)
        public Service1()
        {
            InitializeComponent();
        }

        // Bắt đầu Service
        protected override void OnStart(string[] args)
        {
            WriteToFile("Service is started at " + DateTime.Now);
            timer.Elapsed += new ElapsedEventHandler(OnElapsedTime);
            timer.Interval = 5000; //number in milisecinds 
            timer.Enabled = true;
        }
        // Dừng Service
        protected override void OnStop()
        {
            WriteToFile("Service is stopped at " + DateTime.Now);
        }

        //Mỗi 5s hàm này sẽ thực hiện 1 lần
        private void OnElapsedTime(object source, ElapsedEventArgs e)
        {
            WriteToFile("Service is recall at " + DateTime.Now);
            // List các process
            Process[] pname = Process.GetProcessesByName("notepad");
            // Kiểm tra status của process
            
            // k có process
            if(pname.Length == 0)
            {
                //Ghi vào logs
                WriteToFile("Notepad  | Status : Hiding for sure");
                WriteToFile("And ready to SHOW UP in 5 seconds");
                // k có thì mở lên
                Process.Start(@"notepad.exe");
            }
            else // có
            {
                //Ghi vào logs
                WriteToFile("Notepad  | Status : Here I am (Alive)");
                WriteToFile("5 seconds left till i disapear !");
                //Xóa process
                foreach(var process in Process.GetProcessesByName("notepad"))
                {
                    process.Kill();
                }
            }
        }

        //Hàm để ghi vào logs
        public void WriteToFile(string Message)
        {
            string path = AppDomain.CurrentDomain.BaseDirectory + "\\Logs";
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            string filepath = AppDomain.CurrentDomain.BaseDirectory + "\\Logs\\ServiceLog_" +
                DateTime.Now.Date.ToShortDateString().Replace('/', '_') + ".txt";
            if (!File.Exists(filepath))
            {
                using (StreamWriter sw = File.CreateText(filepath))
                {
                    sw.WriteLine(Message);
                }
            }
            else { 
                     using(StreamWriter sw = File.AppendText(filepath)) 
                     { 
                     sw.WriteLine(Message); 
                     }
                 } 
        }

            

    }
}
