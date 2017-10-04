using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class HMDWindowControl : MonoBehaviour {

	[SerializeField] bool hideTitleBar = true;
	[SerializeField] bool autoHideMouseCur = true;
	[SerializeField] KeyCode endAppKey = KeyCode.Escape;

	private float curTimer = 5.0f;	//5s
	private float curTimeNow;

	private float mouseVecPrev;

#if UNITY_STANDALONE_WIN
	[DllImport("user32.dll", EntryPoint = "SetWindowPos")]
	private static extern bool SetWindowPos(IntPtr hWnd, int hWndInsertAfter, int x, int Y, int cx, int cy, int wFlags);

	[DllImport("user32.dll", EntryPoint = "FindWindow")]
	public static extern IntPtr FindWindow(System.String className, System.String windowName);

	// Sets window attributes
	[DllImport("user32.dll")]
	public static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

	// Gets window attributes
	[DllImport("user32.dll")]
	public static extern int GetWindowLong(IntPtr hWnd, int nIndex);

	// Import function mouse_event() from WinApi
	[DllImport("user32.dll")] 
	private static extern void mouse_event(MouseFlags dwFlags, int dx, int dy, int dwData, System.UIntPtr dwExtraInfo);

	[System.Flags]
	private enum MouseFlags 
	{ 
		Move = 0x0001, 
		LeftDown = 0x0002, 
		LeftUp = 0x0004, 
		RightDown = 0x0008,
		RightUp = 0x0010,
		Absolute = 0x8000,
	}

	// assorted constants needed
	public static int GWL_STYLE = -16;
	public static int WS_CHILD = 0x40000000; //child window
	public static int WS_BORDER = 0x00800000; //window with border
	public static int WS_DLGFRAME = 0x00400000; //window with double border but no title
	public static int WS_CAPTION = WS_BORDER | WS_DLGFRAME; //window with a title bar
	public static int HWND_TOPMOST = -1; //window with a title bar

	void Start() {
		if (Application.platform != RuntimePlatform.WindowsEditor)
		{
			var window = FindWindow(null, Application.productName);
			if(hideTitleBar) {
				int style = GetWindowLong(window, GWL_STYLE);
				SetWindowLong(window, GWL_STYLE, (style & ~WS_CAPTION));
			}
			SetWindowPos(window, HWND_TOPMOST, 0, 0, Screen.currentResolution.width, Screen.currentResolution.height, 0);

			//Window Active
			MouseClick();
		}

		if(autoHideMouseCur) {
			curTimeNow = curTimer;
			Cursor.visible = true;
			mouseVecPrev = Input.mousePosition.x + Input.mousePosition.y + Input.mousePosition.z;
		}
	}

	void Update() {
		//Shutdown
		if (Input.GetKeyDown(endAppKey)){
			Debug.Log("Application was ended by KEY operation.");
			Application.Quit();
		}

		if(autoHideMouseCur) {
			float mouse_pos = Input.mousePosition.x + Input.mousePosition.y + Input.mousePosition.z;
			float mouse_delta = mouseVecPrev - mouse_pos;
			mouseVecPrev = mouse_pos;
			if (Mathf.Abs(mouse_delta) > 10.0f)
			{
				if (Cursor.visible == false)
				{
					Cursor.visible = true;
					curTimeNow = curTimer;
				}
			} else {
				
				if(Cursor.visible == true) {
					curTimeNow -= Time.deltaTime;
					if (curTimeNow < 0.0f)
						Cursor.visible = false;
				}
			}
		}
	}

	void OnDestroy() {
		if (autoHideMouseCur) {
			curTimeNow = curTimer;
			Cursor.visible = true;
		}
	}

	public static void MouseClick()
	{
		mouse_event(MouseFlags.Move | MouseFlags.Absolute, 32768, 32768, 0, System.UIntPtr.Zero);
		System.Threading.Thread.Sleep(20);
		mouse_event(MouseFlags.LeftDown, 0, 0, 0, System.UIntPtr.Zero);
		System.Threading.Thread.Sleep(10);
		mouse_event(MouseFlags.LeftUp, 0, 0, 0, System.UIntPtr.Zero);
	}

#endif
}

