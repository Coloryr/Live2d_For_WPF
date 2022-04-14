using OpenTK;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using OpenTK.Windowing.Common;
using OpenTK.WinForms;
using System;
using System.Collections.Generic;
using System.Linq;
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
using System.Windows.Shapes;

namespace Live2d_For_WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Timer _timer = null!;
        private float _angle = 0.0f;
        private GLControl glControl;
        private Live2dApp live2d;
        public MainWindow()
        {
            InitializeComponent();

            live2d = new();
        }

        private void WindowsFormsHost_Initialized(object sender, EventArgs e)
        {
            
        }

        private void GlControl_Load(object? sender, EventArgs e)
        {
            glControl_Resize(glControl, EventArgs.Empty);

            glControl.Resize += glControl_Resize;
            glControl.Paint += glControl_Paint;

            _timer = new Timer();
            _timer.Tick += (sender, e) =>
            {
                _angle += 0.5f;
                Render();
            };
            _timer.Interval = 50;   // 1000 ms per sec / 50 ms per frame = 20 FPS
            _timer.Start();
        }

        private void glControl_Resize(object? sender, EventArgs e)
        {
            glControl.MakeCurrent();

            if (glControl.ClientSize.Height == 0)
                glControl.ClientSize = new System.Drawing.Size(glControl.ClientSize.Width, 1);

            GL.Viewport(0, 0, glControl.ClientSize.Width, glControl.ClientSize.Height);
            if (glControl.Width == 0)
                return;
            live2d.Start(glControl.Width, glControl.Height);
        }

        private void glControl_Paint(object sender, PaintEventArgs e)
        {
            Render();
        }

        private int a =0;

        private void Render()
        {
            a++;
            glControl.MakeCurrent();

            live2d.Tick(glControl.Width, glControl.Height, a);

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
    }
}
