# Live2d_For_WPF

已过时，请转移到https://github.com/Coloryr/Live2DCSharpSDK

一个渲染Live2d的WPF组件

需要依赖OpenTK、OpenTK.WinForms

## 使用
1. 在xaml中添加
```xaml
<WindowsFormsHost Name="Host" Margin="0,0,400,0" />
```
2. 添加opengl渲染层
```C#
private DateTime beginTime = DateTime.Now;            //获取开始时间  
private GLControl glControl;
private Live2dApp live2d;
private string[] Expressions;
private Motion[] Motions;
private Part[] Parts;
private Parameter[] Parameters;
private Random random = new();

public MainWindow()
{
    InitializeComponent();

    live2d = new(LoadFile, LoadDone, Update);
}

 private void Update() 
{
    live2d.AddParameterValue(Parameters[0].Id, 30);
}
private void LoadDone(string name)
{
    live2d.SetRandomMotion(false);
    live2d.SetCustomValue(true);
    Expressions = live2d.GetExpressions();
    Motions = live2d.GetMotions();
    Parts = live2d.GetParts();
    Parameters = live2d.GetParameters();
}
private IntPtr LoadFile(string path, ref uint size)
{
    var temp = File.ReadAllBytes(path);
    IntPtr inputBuffer = Marshal.AllocHGlobal(temp.Length * sizeof(byte));
    Marshal.Copy(temp, 0, inputBuffer, temp.Length);
    size = (uint)temp.Length;
    return inputBuffer;
}

private void Window_Loaded(object sender, RoutedEventArgs e)
{
    var set = GLControlSettings.Default.Clone();
    set.API = ContextAPI.OpenGL;
    set.Flags = ContextFlags.Default;
    set.IsEventDriven = true;
    set.Profile = ContextProfile.Compatability;
    set.APIVersion = new Version(3, 3, 0, 0);
    glControl = new GLControl(set);

    glControl.Load += GlControl_Load;
    Host.Child = glControl;
}

private void GlControl_Load(object? sender, EventArgs e)
{
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
private void glControl_Paint(object sender, PaintEventArgs e)
{
    Render();
}

private void Render()
{
    glControl.MakeCurrent();

    live2d.Tick(glControl.Width, glControl.Height, (DateTime.Now - beginTime).TotalMilliseconds / 1000);

    glControl.SwapBuffers();
}
```
