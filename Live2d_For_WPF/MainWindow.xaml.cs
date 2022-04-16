using OpenTK;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using OpenTK.Windowing.Common;
using OpenTK.WinForms;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Resources;
using System.Windows.Shapes;
using Application = System.Windows.Application;

namespace Live2d_For_WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Timer _timer = null!;
        private GLControl glControl;
        private Live2dApp live2d;
        public MainWindow()
        {
            InitializeComponent();

            live2d = new();
            live2d.SetCall(LoadFile);
        }

        private IntPtr LoadFile(string path, ref uint size) 
        {
            if (path.StartsWith("/Data"))
            {
                Uri uri = new(path, UriKind.Relative);
                StreamResourceInfo info = Application.GetResourceStream(uri);
                var temp = new byte[info.Stream.Length];
                info.Stream.Read(temp, 0, temp.Length);
                IntPtr inputBuffer = Marshal.AllocHGlobal(temp.Length * sizeof(byte));
                Marshal.Copy(temp, 0, inputBuffer, temp.Length);
                size = (uint)temp.Length;
                return inputBuffer;
            }
            else
            {
                var temp = File.ReadAllBytes(path);
                IntPtr inputBuffer = Marshal.AllocHGlobal(temp.Length * sizeof(byte));
                Marshal.Copy(temp, 0, inputBuffer, temp.Length);
                size = (uint)temp.Length;
                return inputBuffer;
            }
        }

        private void WindowsFormsHost_Initialized(object sender, EventArgs e)
        {
            
        }

        private void GlControl_Load(object? sender, EventArgs e)
        {
            glControl_Resize(glControl, EventArgs.Empty);

            glControl.Resize += glControl_Resize;
            glControl.Paint += glControl_Paint;
            glControl.MouseDown += GlControl_MouseDown;
            glControl.MouseUp += GlControl_MouseUp;
            glControl.MouseMove += GlControl_MouseMove;

            _timer = new Timer();
            _timer.Tick += (sender, e) =>
            {
                Render();
            };
            _timer.Interval = 10;   // 1000 ms per sec / 10 ms per frame = 100 FPS
            _timer.Start();

            live2d.Start(glControl.ClientSize.Width, glControl.ClientSize.Height);
            live2d.LoadModel("/Data/", "Haru");
        }

        private void GlControl_MouseMove(object? sender, System.Windows.Forms.MouseEventArgs e)
        {
            live2d.MouseMove(e.X, e.Y);
        }

        private void GlControl_MouseUp(object? sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;
            live2d.MouseEvent(1);
        }

        private void GlControl_MouseDown(object? sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;
            live2d.MouseEvent(0);
        }

        private void glControl_Resize(object? sender, EventArgs e)
        {
            
        }

        private void glControl_Paint(object sender, PaintEventArgs e)
        {
            Render();
        }

        private DateTime beginTime = DateTime.Now;            //获取开始时间  

        private void Render()
        {
            glControl.MakeCurrent();

            live2d.Tick(glControl.Width, glControl.Height, (DateTime.Now - beginTime).TotalMilliseconds / 1000);

            glControl.SwapBuffers();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var set = GLControlSettings.Default.Clone();
            set.API = ContextAPI.OpenGL;
            set.Flags = ContextFlags.Default;
            set.IsEventDriven = true;
            set.Profile = ContextProfile.Compatability;
            set.APIVersion = new Version(3, 3, 0, 0);
            glControl = new GLControl(set);//创建GLControl控件

            glControl.Load += GlControl_Load;
            Host.Child = glControl;//将控件放在WindowsFormsHost控件中
            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            live2d.SetScale(0.5f);
            live2d.SetPosX(0.3f);
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            live2d.SetScale(1f);
            live2d.SetPosX(0f);
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            OpenFileDialog file = new()
            {
                Filter = "模型文件|*.model3.json",
                RestoreDirectory = true,
                FilterIndex = 1
            };

            if (file.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string filename = file.FileName;
                FileInfo info = new(filename);

                live2d.LoadModel(info.DirectoryName + "/", info.Name.Replace(".model3.json", ""));
            }
        }
    }
}
