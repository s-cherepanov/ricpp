// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*! \file ricpptest.cpp
 *  \brief This is a test programm to call some RenderMan interface routines
 *         RICPP - RenderMan Interface CPP Language Binding
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"

#include "rendererloader/rendererloader.h"
#include "tools/gettemp.h"

#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>

// #define FULLSCREEN

void spinDisplay(void);            // Setting of some variables for animation
void resetFPS(HWND hwnd);          // Resets counter for frames per second calculation
void setCamera(TRi &ri, RECT &r);  // Places the camera, setting of options
void display(TRi &ri, RtObjectHandle handle, bool showLamp);  // Displays some geometric primitives
int  idle(HWND hwnd, int cnt);     // Idle loop for animation

RtObjectHandle PinObject(TRi &ri); // RiObject of a geometry (pin)
RtObjectHandle pinHandle = (RtObjectHandle)-1;

// some 'running' variables for animation (processed in spinDisplay)
static RtFloat even = 1.0; // 1.0 or -1.0 switches after spin made a full 'turn'
static RtFloat spin = 0.0; // runs from 0.0 to 359.0

// Counter to calculate frames per second
static DWORD startTime = 0L;
static DWORD prevSystemTime = 0L;
static DWORD curSystemTime = 0L;
static DWORD frameCnt = 0L;

// Window is closed
static bool window_closed = true;

// Displayed primitive
static int primitiveNO = 16;  // 0..nPrimitives-1, changes with WM_LBUTTONDOWN
static int nPrimitives = 21;

// Windowname etc.
static char szAppName[] = "RiCpp-Test";
// static char *szProcName = "Quadrics";
static char *szProcName = "Ribarchive";
static char modulepath[512];

// perspective view or not
static bool perspective = true;

// Message processing
LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);

// Object to manage loaded renderers
static TRendererLoader *rendererLoader = NULL;

// Used renderes
static TRi *rc = NULL;        // OpenGL Context
static TRi *ribout = NULL;    // RIB context
static TRi *mesh = NULL;      // Mesh context

RtContextHandle displayContext;

///////////////////////////////////////////////////////////////////////////////
// WinMain

int WINAPI WinMain(
	HINSTANCE  hInstance,       // handle of current instance
	HINSTANCE  hPrevInstance,   // handle of previous instance
	LPSTR  lpszCmdLine,         // address of command line
	int  nCmdShow               // show state of window
) {
    // parameters not used

	hPrevInstance = hPrevInstance;  // no warnings
	lpszCmdLine = lpszCmdLine;      // no warnings

	setlocale( LC_ALL, "C" );

	// Get the path of the program called

	modulepath[0] = (char)0;
	DWORD fsize = GetModuleFileName(NULL, modulepath, sizeof(modulepath));
	while ( fsize ) {
		fsize--;
		if ( modulepath[fsize] == '\\' ) {
		    modulepath[fsize] = (char)0;
		    break;
		}
	}

	// Create a console window for output if there is none
	// s.a. INFO: Calling CRT Output Routines from a GUI Application [Q105305]
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if ( hStdOutput == NULL ) {
		int hCrt;
		FILE *hf;

		AllocConsole();
		SetConsoleTitle("RiCPP Console Window - Do not close!");
		hCrt = _open_osfhandle(
					(long) GetStdHandle(STD_OUTPUT_HANDLE),
					_O_TEXT
				);
		hf = _fdopen( hCrt, "w" );
		*stdout = *hf;
		int i = setvbuf( stdout, NULL, _IONBF, 0 );
		i = i;
	}

	// Initializes rendererloader for subdirectory "contexts"

	try {
		rendererLoader = new TRendererLoader("contexts");
		if ( !rendererLoader )
		    return 0;
	} catch (TSystemException &t) {
		t.displayMessage();
		if ( rendererLoader )
		    delete rendererLoader;
		return 0;
	} catch (...) {
		MessageBox(NULL, "Unexpected Error in WinMain() while establishing renderer loader.", NULL, MB_OK);
		return 0;
	}

	// Set up a window (need not to have an own DC)

	WNDCLASSEX wndClassEx;

	wndClassEx.cbSize = sizeof(wndClassEx);
	// wndClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = MyWndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = szAppName;
	wndClassEx.hIconSm = LoadIcon(hInstance, "IDR_MAINFRAME");

	RegisterClassEx(&wndClassEx);

	// Get the right window size for client size 512x384

#   ifdef FULLSCREEN
		RECT rectWin = {
			0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)
		};
		DWORD dwStyle = WS_POPUP | WS_VISIBLE;
#   ifdef _DEBUG
		DWORD dwExStyle = 0;
#   else
		DWORD dwExStyle = WS_EX_TOPMOST;
#   endif
#   else
	    RECT rectWin = {0, 0, 512, 384};
	    DWORD dwStyle =   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN
		                | WS_CLIPSIBLINGS | WS_VISIBLE;
	    DWORD dwExStyle = 0;
	    AdjustWindowRectEx(&rectWin, dwStyle, FALSE, dwExStyle);
#   endif

	// Create a new window

	HWND hwnd = CreateWindowEx(
		dwExStyle,
		szAppName,
		szAppName,
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rectWin.right - rectWin.left,
		rectWin.bottom - rectWin.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// Show the window

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Message-Loop for idle-processing

	MSG msg;    // Windows Message
	int g;      // Return code for GetMessage, != 0 -> program ends
	int cnt;    // Counts Idle calls

	do {
		cnt = 0;
		while ( PeekMessage(&msg, NULL, (unsigned int)0, (unsigned int)-1, (unsigned int)PM_NOREMOVE) == 0 && idle(hwnd, cnt++) );

		if ( (g = GetMessage(&msg, NULL, 0, 0)) != 0 ) {
		    TranslateMessage(&msg);
		    DispatchMessage(&msg);
		}
	} while ( g );

	// Delete the renderer loader (also deletes renderers and removes all renderer Dlls)
	try {
		delete rendererLoader;
	} catch (TSystemException &e) {
		e.displayMessage();
	} catch (...) {
		// Do nothing
		MessageBox(NULL, "Unexpected Error in WinMain() while discarding renderer loader", NULL, MB_OK);
	}

	// The end
	return msg.wParam;
}


///////////////////////////////////////////////////////////////////////////////
// idle() - Idle processing, spins display, calcs FPS and sets window title

int idle(HWND hwnd, int cnt) {
    // cnt (number of idle calls without interrupted by a message) is not used
    cnt = cnt;

    // Nothing done if window is closed
    if ( window_closed )
        return 1;

    // Change some parameters to make the image spin
    spinDisplay();

    // Count the frames
    ++frameCnt;
    if ( frameCnt*1000 <= frameCnt ) {
        // frameCnt overflow, begin new count
        resetFPS(hwnd);
    }

    // current time
    curSystemTime = timeGetTime();
    if ( curSystemTime <= startTime ) {
        // time overflow, begin new count
        resetFPS(hwnd);
    }

    // The start time is set by resetFPS()
    if ( startTime ) {
        // Display the FPS after a while
        if ( (curSystemTime - prevSystemTime) > 2000 ) {
            prevSystemTime = curSystemTime;
            DWORD fps = (frameCnt*1000) / (curSystemTime - startTime);
            char buffer[128];
            RECT r = {0, 0, 0, 0};
            GetClientRect(hwnd, &r);

            // Set the window title
            if ( szProcName && *szProcName ) {
                sprintf(buffer, "%ld fps / w:%d, h:%d / %s - %s", (unsigned long)fps, (int)r.right, (int)r.bottom, szAppName, szProcName);
            } else {
                sprintf(buffer, "%ld fps / w:%d, h:%d / %s", (unsigned long)fps, (int)r.right, (int)r.bottom, szAppName);
            }
            SetWindowText(hwnd, buffer);
        }
    } else {
        // Reinitialising
        startTime = prevSystemTime = curSystemTime;
        frameCnt = 1;
    }

    // Redraw the window
    InvalidateRect(hwnd, NULL, FALSE);
    UpdateWindow(hwnd);
    return 1;
}


///////////////////////////////////////////////////////////////////////////////
// MyWndProc() - Event handling

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch ( iMsg ) {
        case WM_CREATE: {
            window_closed = false; // Window is open now

            // Load the interface for OpenGL output
            rc = NULL;
            try {
                rc = rendererLoader->loadRenderer("glrenderer");
				if ( rc ) {
                    rc->errorHandler(rc->errorPrint);
					rc->begin();
					displayContext = rc->getContext();
				} else {
					displayContext = rc->RI_NULL;
				}
            } catch (TSystemException &t) {
                t.displayMessage();
            } catch (TRendererError &t) {
                t.displayMessage();
            } catch (...) {
                MessageBox(NULL, "Unexpected Error in MyWndProc() while loading glrenderer.", NULL, MB_OK);
            }

            // Load the interface for RIB output
            ribout = NULL;
            try {
                ribout = rendererLoader->loadRenderer("ribout");
                if ( ribout )
                    ribout->errorHandler(ribout->errorPrint);
            } catch (TSystemException &t) {
                t.displayMessage();
            } catch (TRendererError &t) {
                t.displayMessage();
            } catch (...) {
                MessageBox(NULL, "Unexpected Error in MyWndProc() while loading ribout.", NULL, MB_OK);
            }

            // Load the interface for mesh output
            mesh = NULL;
            try {
                mesh = rendererLoader->loadRenderer("mesh");
                if ( mesh )
                    mesh->errorHandler(mesh->errorPrint);
            } catch (TSystemException &t) {
                t.displayMessage();
            } catch (TRendererError &t) {
                t.displayMessage();
            } catch (...) {
                MessageBox(NULL, "Unexpected Error in MyWndProc() while loading ribout.", NULL, MB_OK);
            }

			// If there is no OpenGL renderer, treat as failure
            if ( !rc )
                return 1;

            // Set the start time
            resetFPS(hwnd);

#           ifdef FULLSCREEN
				ShowCursor(FALSE);
#           endif

            // OK
            return 0;
        }

        case WM_LBUTTONDOWN: {
            // Display the next screen (circular)
            ++primitiveNO;
            if ( primitiveNO >= nPrimitives )
                primitiveNO = 0;

            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
            resetFPS(hwnd);
            break;
        }

        case WM_RBUTTONDOWN: {
            // Try the RIB output

            // No interface for RIB output
            if ( !ribout )
                return 0;

            // Fix output path
			std::string filename;
			filename = getTemp();
            filename += "\\ribout.rib";
            int i, j;
			static char t[512];
			static char t2[512];

            try {
                RECT r;
                if ( !GetClientRect(hwnd, &r) )
                    throw TSystemException(__FILE__, __LINE__);

                ribout->begin(filename.c_str());
                    RtObjectHandle handle = (RtObjectHandle)-1;
                    ribout->archiveRecord(ribout->RI_STRUCTURE, "RenderMan RIB-Structure 1.1");
                    ribout->archiveRecord(ribout->RI_VERBATIM, "version 3.03\n");

                    // handle = PinObject(*ribout);
                    setCamera(*ribout, r);

                    ribout->display("ribout.tiff", ribout->RI_FILE, ribout->RI_RGBA);
                    ribout->display("+ribout.tiff", ribout->RI_FRAMEBUFFER, ribout->RI_RGBA);

                    ribout->frameBegin(1);
                        ribout->worldBegin();
                            display(*ribout, handle, false);
                        ribout->worldEnd();
                    ribout->frameEnd();

#if 0
                    for ( int i=1; i<361; ++i ) {
                        sprintf(t, "ribout_%03d.tiff", i);
                        ribout->display(t, ribout->RI_FILE, ribout->RI_RGBA);

                        ribout->frameBegin(i);
                            ribout->worldBegin();
                                display(*ribout, handle, false);
                            ribout->worldEnd();
                        ribout->frameEnd();
                        spinDisplay();
                    }
#endif

                    ribout->end();
            } catch (TSystemException &t) {
                t.displayMessage();
                ribout->synchronize(ribout->RI_ABORT);
            } catch (TRendererError &t) {
                t.displayMessage();
                ribout->synchronize(ribout->RI_ABORT);
            } catch (...) {
                MessageBox(NULL, "Unexpected Error in MyWndProc() while writing RIB code.", NULL, MB_OK);
                ribout->synchronize(ribout->RI_ABORT);
            }

            // Mask blank
			t2[0] = '\0';
            for ( i=0, j=0; modulepath[i]; /**/ ) {
                if ( modulepath[i] == ' ' ) {
                    t2[j++] = '^';
                }
                t2[j++] = modulepath[i++];
            }
            t2[j] = (char)0;
            strcat(t2, "\\Batches\\render.bat");
			t[0] = (char)0;
            sprintf(t, "%s \"%s\" \"%s\"", t2, modulepath, filename.c_str());

            // Call render.bat to start the renderer
            system(t);

            // Begin new Framecount
            resetFPS(hwnd);
            break;
        }

        case WM_MOVE: {
            resetFPS(hwnd);
            break;
        }

        case WM_SIZE: {
            resetFPS(hwnd);
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            PAINTSTRUCT *pps;
            HDC hdc = NULL;
            RECT rect;

            // Try the interface for OpenGL output

            if ( !rc || window_closed )
                return 0;
            if ( !GetUpdateRect(hwnd, &rect, FALSE) )
                return 0;

            try {
                pps = &ps;
                hdc = BeginPaint(hwnd, pps);
                if ( !hdc )
                    throw TSystemException(__FILE__, __LINE__);
            } catch (TSystemException &t) {
                t.displayMessage();
            } catch ( ... ) {
                MessageBox(NULL, "Unexpected Error in MyWndProc() while starting to paint.", NULL, MB_OK);
            }

            if ( !hdc )
                return 0;

            try {
				// rc->begin()
                rc->context(displayContext);
				rc->synchronize("reset");
                    RECT r;
                    RtInt origin[2] = {0, 0};
                    if ( !GetClientRect(hwnd, &r) )
                        throw TSystemException(__FILE__, __LINE__);
					rc->display(szAppName, rc->RI_FRAMEBUFFER, rc->RI_RGB, "devicecontext", &hdc, "windowhandle", &hwnd, rc->RI_ORIGIN, &origin, rc->RI_NULL);
                    rc->frameBegin(1);
						setCamera(*rc, r);
                        rc->worldBegin();
                            rc->geometricApproximation("tesselation", 32.0);
                            rc->geometricRepresentation(rc->RI_SMOOTH);
                            display(*rc, pinHandle, true);
                        rc->worldEnd();
                    rc->frameEnd();
                // rc->end();
            } catch (TSystemException &t) {
                t.displayMessage();
                rc->synchronize(rc->RI_ABORT);
            } catch (TRendererError &t) {
                t.displayMessage();
                rc->synchronize(rc->RI_ABORT);
            } catch (...) {
                MessageBox(NULL, "Unexpected Error in MyWndProc() while drawing to window.", NULL, MB_OK);
                rc->synchronize(rc->RI_ABORT);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_CLOSE:
            if ( rc ) {
                try {
                    if ( rc && !window_closed )
                        rc->end();
                } catch (TSystemException &t) {
                    t.displayMessage();
                } catch (TRendererError &t) {
                    t.displayMessage();
                } catch (...) {
                    MessageBox(NULL, "Unexpected Error in MyWndProc() while calling end of glrenderer.", NULL, MB_OK);
                }
            }

            window_closed = true; // window is closed now
            break;

        case WM_DESTROY:
			// Test
			if ( mesh ) {
				try {
					mesh->begin("Test");
						mesh->worldBegin();
						mesh->worldEnd();
					mesh->end();
				} catch (...) {
				}
			}

#           ifdef FULLSCREEN
				ShowCursor(TRUE);
#           endif

			PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

/********************/

///////////////////////////////////////////////////////////////////////////////
// resetFPS() - Resets counter 'startTime' for the 'frames per second'
//              calculation
void resetFPS(HWND hwnd) {
    startTime = 0L;

    char buffer[128];
    RECT r = {0, 0, 0, 0};
    GetClientRect(hwnd, &r);
    if ( szProcName && *szProcName ) {
        sprintf(buffer, "* / w:%d, h:%d / %s - %s", (int)r.right, (int)r.bottom, szAppName, szProcName);
    } else {
        sprintf(buffer, "* / w:%d, h:%d / %s", (int)r.right, (int)r.bottom, szAppName);
    }
    SetWindowText(hwnd, buffer);
}


///////////////////////////////////////////////////////////////////////////////
// spinDisplay() - sets variables 'spin' and 'even' for animation
void spinDisplay(void)
{
    spin += 1.0F;
    if ( spin >= 360.0F ) {
        spin -= 360.0F;
        even *= -1.0;
    }
}

///////////////////////////////////////////////////////////////////////////////
// setCamera() - camera placement, options
void setCamera(TRi &ri, RECT &r) {
    // Camera Orientation
    static RtFloat spin_x = -30.0;
    static RtFloat spin_y = 60.0;
    static RtFloat spin_z = 0.0;

    RtFloat d = 39.6F; // normal objektive 50mm on 35mm film
//  RtFloat d = 90.0F; // default

//  ri.pixelSamples(2.0F, 2.0F);

    RtFloat p_ratio = 1.0F;
    ri.format(r.right, r.bottom, p_ratio);
    RtFloat f_ratio = (RtFloat)(r.right * p_ratio)/(RtFloat)r.bottom;
    f_ratio = f_ratio;
//  ri.format(400, 400, 1.0F);
//  ri.frameAspectRatio((RtFloat)r.right/(RtFloat)r.bottom);
//  ri.frameAspectRatio(16.0F/9.0F);
//  ri.frameAspectRatio(4.0F/3.0F);
//  ri.frameAspectRatio(1.0F);
//  ri.screenWindow(-1.0F, 1.0F, -1.0F, 1.0F);

    /*
    if ( f_ratio > 1.0 )
        ri.screenWindow(-f_ratio, f_ratio, -1.0F, 1.0F);
    else
        ri.screenWindow(-1.0F, 1.0F, -1.0F/f_ratio, 1.0F/f_ratio);
    */

//  ri.cropWindow(0.1F, 0.5F, 0.3F, 0.6F);
    ri.clipping(.5F, 15.0F);

    ri.exposure(1.0F, 1.2F);

    // Pre projection
    // ri.translate(-0.2F, 0.0F, 0.0F);

    if ( perspective ) {
//      ri.projection(ri.RI_PERSPECTIVE, ri.RI_NULL);
        ri.projection(ri.RI_PERSPECTIVE, ri.RI_FOV, &d, ri.RI_NULL);
    } else {
        ri.projection(ri.RI_ORTHOGRAPHIC, ri.RI_NULL);
//      ri.projection(ri.RI_NULL, ri.RI_NULL);
    }
    ri.translate(0.0F, 0.0F, 5.0F);

    if ( spin_x != 0.0 )
        ri.rotate(spin_x, 1.0F, 0.0F, 0.0F);
    if ( spin_y != 0.0 )
        ri.rotate(spin_y, 0.0F, 1.0F, 0.0F);
    if ( spin_z != 0.0 )
        ri.rotate(spin_z, 0.0F, 0.0F, 1.0F);
}

///////////////////////////////////////////////////////////////////////////////
// displays the 'Pixar' pin
static RtFloat pinMesh[] = {
    0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F,
    0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F,
    0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F,
    0.0703F, 0.0F, 1.5F, 0.0703F, 0.038826F, 1.5F, 0.038826F, 0.0703F, 1.5F, 0.0F, 0.0703F, 1.5F,
    -0.038826F, 0.0703F, 1.5F, -0.0703F, 0.038826F, 1.5F, -0.0703F, 0.0F, 1.5F, -0.0703F, -0.038826F, 1.5F,
    -0.038826F, -0.0703F, 1.5F, 0.0F, -0.0703F, 1.5F, 0.038826F, -0.0703F, 1.5F, 0.0703F, -0.038826F, 1.5F,
    0.1273F, 0.0F, 1.4293F, 0.1273F, 0.070306F, 1.4293F, 0.070306F, 0.1273F, 1.4293F, 0.0F, 0.1273F, 1.4293F,
    -0.070306F, 0.1273F, 1.4293F, -0.1273F, 0.070306F, 1.4293F, -0.1273F, 0.0F, 1.4293F, -0.1273F, -0.070306F, 1.4293F,
    -0.070306F, -0.1273F, 1.4293F, 0.0F, -0.1273F, 1.4293F, 0.070306F, -0.1273F, 1.4293F, 0.1273F, -0.070306F, 1.4293F,
    0.1273F, 0.0F, 1.3727F, 0.1273F, 0.070306F, 1.3727F, 0.070306F, 0.1273F, 1.3727F, 0.0F, 0.1273F, 1.3727F,
    -0.070306F, 0.1273F, 1.3727F, -0.1273F, 0.070306F, 1.3727F, -0.1273F, 0.0F, 1.3727F, -0.1273F, -0.070306F, 1.3727F,
    -0.070306F, -0.1273F, 1.3727F, 0.0F, -0.1273F, 1.3727F, 0.070306F, -0.1273F, 1.3727F, 0.1273F, -0.070306F, 1.3727F,
    0.1273F, 0.0F, 1.23F, 0.1273F, 0.070306F, 1.23F, 0.070306F, 0.1273F, 1.23F, 0.0F, 0.1273F, 1.23F,
    -0.070306F, 0.1273F, 1.23F, -0.1273F, 0.070306F, 1.23F, -0.1273F, 0.0F, 1.23F, -0.1273F, -0.070306F, 1.23F,
    -0.070306F, -0.1273F, 1.23F, 0.0F, -0.1273F, 1.23F, 0.070306F, -0.1273F, 1.23F, 0.1273F, -0.070306F, 1.23F,
    0.0899F, 0.0F, 1.16F, 0.0899F, 0.04965F, 1.16F, 0.04965F, 0.0899F, 1.16F, 0.0F, 0.0899F, 1.16F,
    -0.04965F, 0.0899F, 1.16F, -0.0899F, 0.04965F, 1.16F, -0.0899F, 0.0F, 1.16F, -0.0899F, -0.04965F, 1.16F,
    -0.04965F, -0.0899F, 1.16F, 0.0F, -0.0899F, 1.16F, 0.04965F, -0.0899F, 1.16F, 0.0899F, -0.04965F, 1.16F,
    0.0899F, 0.0F, 1.0F, 0.0899F, 0.04965F, 1.0F, 0.04965F, 0.0899F, 1.0F, 0.0F, 0.0899F, 1.0F,
    -0.04965F, 0.0899F, 1.0F, -0.0899F, 0.04965F, 1.0F, -0.0899F, 0.0F, 1.0F, -0.0899F, -0.04965F, 1.0F,
    -0.04965F, -0.0899F, 1.0F, 0.0F, -0.0899F, 1.0F, 0.04965F, -0.0899F, 1.0F, 0.0899F, -0.04965F, 1.0F,
    0.0899F, 0.0F, 0.75F, 0.0899F, 0.04965F, 0.75F, 0.04965F, 0.0899F, 0.75F, 0.0F, 0.0899F, 0.75F,
    -0.04965F, 0.0899F, 0.75F, -0.0899F, 0.04965F, 0.75F, -0.0899F, 0.0F, 0.75F, -0.0899F, -0.04965F, 0.75F,
    -0.04965F, -0.0899F, 0.75F, 0.0F, -0.0899F, 0.75F, 0.04965F, -0.0899F, 0.75F, 0.0899F, -0.04965F, 0.75F,
    0.41F, 0.0F, 0.678F, 0.41F, 0.226437F, 0.678F, 0.226437F, 0.41F, 0.678F, 0.0F, 0.41F, 0.678F,
    -0.226437F, 0.41F, 0.678F, -0.41F, 0.226437F, 0.678F, -0.41F, 0.0F, 0.678F, -0.41F, -0.226437F, 0.678F,
    -0.226437F, -0.41F, 0.678F, 0.0F, -0.41F, 0.678F, 0.226437F, -0.41F, 0.678F, 0.41F, -0.226437F, 0.678F,
    0.125F, 0.0F, 0.0F, 0.125F, 0.069036F, 0.0F, 0.069036F, 0.125F, 0.0F, 0.0F, 0.125F, 0.0F,
    -0.069036F, 0.125F, 0.0F, -0.125F, 0.069036F, 0.0F, -0.125F, 0.0F, 0.0F, -0.125F, -0.069036F, 0.0F,
    -0.069036F, -0.125F, 0.0F, 0.0F, -0.125F, 0.0F, 0.069036F, -0.125F, 0.0F, 0.125F, -0.069036F, 0.0F
};

void displayPin(TRi &ri, RtObjectHandle handle) {
    if ( handle == (RtObjectHandle)-1 ) {
        ri.attributeBegin();
            ri.basis(ri.bezierBasis, ri.RI_BEZIERSTEP, ri.bezierBasis, ri.RI_BEZIERSTEP);
            ri.patchMesh(ri.RI_BICUBIC, 12, ri.RI_PERIODIC, 10, ri.RI_NONPERIODIC, ri.RI_P, pinMesh, ri.RI_NULL);
        ri.attributeEnd();
        szProcName = "PatchMesh, Bicubic";
    } else {
        ri.attributeBegin();
            ri.objectInstance(handle);
        ri.attributeEnd();
        szProcName = "Object, PatchMesh, Bicubic";
    }
}

RtObjectHandle PinObject(TRi &ri) {
    RtObjectHandle handle = ri.objectBegin();
        ri.basis(ri.bezierBasis, ri.RI_BEZIERSTEP, ri.bezierBasis, ri.RI_BEZIERSTEP);
        ri.patchMesh(ri.RI_BICUBIC, 12, ri.RI_PERIODIC, 10, ri.RI_NONPERIODIC, ri.RI_P, pinMesh, ri.RI_NULL);
    ri.objectEnd();
    return handle;
}

///////////////////////////////////////////////////////////////////////////////
// displays the 'BMRT' vase
void displayVase(TRi &ri) {
    static RtFloat vaseMesh[] = {
        0.0F, 0.0F, 0.0F, 0.869825F, 0.0F, 0.0F, 0.0F, 0.902369F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F,
        0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.804738F, 0.0F, 0.0F, 0.0F, 0.869825F,
        2.1269F, 3.79357F, 0.0F, 0.869825F, 1.17851F, 4.51184F, 0.0F, 0.902369F, 0.0F, 5.0F, 0.0F, 1.0F, -3.53553F, 3.53553F, 0.0F, 0.707107F, -5.0F, 3.06152e-016F, 0.0F, 1.0F, -3.53553F, -3.53553F, 0.0F,
        0.707107F, -6.12303e-016F, -5.0F, 0.0F, 1.0F, 3.53553F, -3.53553F, 0.0F, 0.707107F, 5.0F, -9.18455e-016F, 0.0F, 1.0F, 4.02369F, 2.35702F, 0.0F, 0.804738F, 2.1269F, 3.79357F, 0.0F, 0.869825F,
        12.1872F, 21.7372F, 0.0F, 4.9841F, 6.75287F, 25.8529F, 0.0F, 5.17057F, 0.0F, 28.65F, 0.0F, 5.73F, -20.2586F, 20.2586F, 0.0F, 4.05172F, -28.65F, 1.75425e-015F, 0.0F, 5.73F, -20.2586F,
        -20.2586F, 0.0F, 4.05172F, -3.5085e-015F, -28.65F, 0.0F, 5.73F, 20.2586F, -20.2586F, 0.0F, 4.05172F, 28.65F, -5.26275e-015F, 0.0F, 5.73F, 23.0557F, 13.5057F, 0.0F, 4.61115F, 12.1872F,
        21.7372F, 0.0F, 4.9841F, 2.1269F, 3.79357F, 0.260948F, 0.869825F, 1.17851F, 4.51184F, 0.270711F, 0.902369F, 0.0F, 5.0F, 0.3F, 1.0F, -3.53553F, 3.53553F, 0.212132F, 0.707107F, -5.0F,
        3.06152e-016F, 0.3F, 1.0F, -3.53553F, -3.53553F, 0.212132F, 0.707107F, -6.12303e-016F, -5.0F, 0.3F, 1.0F, 3.53553F, -3.53553F, 0.212132F, 0.707107F, 5.0F, -9.18455e-016F, 0.3F, 1.0F,
        4.02369F, 2.35702F, 0.241421F, 0.804738F, 2.1269F, 3.79357F, 0.260948F, 0.869825F, 6.38071F, 11.3807F, 0.782843F, 2.60948F, 3.53553F, 13.5355F, 0.812132F, 2.70711F, 0.0F,
        15.0F, 0.9F, 3.0F, -10.6066F, 10.6066F, 0.636396F, 2.12132F, -15.0F, 9.18455e-016F, 0.9F, 3.0F, -10.6066F, -10.6066F, 0.636396F, 2.12132F, -1.83691e-015F, -15.0F, 0.9F, 3.0F, 10.6066F,
        -10.6066F, 0.636396F, 2.12132F, 15.0F, -2.75536e-015F, 0.9F, 3.0F, 12.0711F, 7.07107F, 0.724264F, 2.41421F, 6.38071F, 11.3807F, 0.782843F, 2.60948F, 2.72244F, 4.85577F,
        2.0006F, 0.869825F, 1.50849F, 5.77516F, 2.07545F, 0.902369F, 0.0F, 6.4F, 2.3F, 1.0F, -4.52548F, 4.52548F, 1.62635F, 0.707107F, -6.4F, 3.91874e-016F, 2.3F, 1.0F, -4.52548F, -4.52548F,
        1.62635F, 0.707107F, -7.83748e-016F, -6.4F, 2.3F, 1.0F, 4.52548F, -4.52548F, 1.62635F, 0.707107F, 6.4F, -1.17562e-015F, 2.3F, 1.0F, 5.15032F, 3.01699F, 1.8509F, 0.804738F,
        2.72244F, 4.85577F, 2.0006F, 0.869825F, 2.97767F, 5.311F, 4.34913F, 0.869825F, 1.64992F, 6.31658F, 4.51184F, 0.902369F, 0.0F, 7.0F, 5.0F, 1.0F, -4.94975F, 4.94975F, 3.53553F,
        0.707107F, -7.0F, 4.28612e-016F, 5.0F, 1.0F, -4.94975F, -4.94975F, 3.53553F, 0.707107F, -8.57224e-016F, -7.0F, 5.0F, 1.0F, 4.94975F, -4.94975F, 3.53553F, 0.707107F, 7.0F, -1.28584e-015F, 5.0F,
        1.0F, 5.63316F, 3.29983F, 4.02369F, 0.804738F, 2.97767F, 5.311F, 4.34913F, 0.869825F, 2.55228F, 4.55228F, 6.9586F, 0.869825F, 1.41421F, 5.41421F, 7.21895F, 0.902369F, 0.0F, 6.0F, 8.0F,
        1.0F, -4.24264F, 4.24264F, 5.65685F, 0.707107F, -6.0F, 3.67382e-016F, 8.0F, 1.0F, -4.24264F, -4.24264F, 5.65685F, 0.707107F, -7.34764e-016F, -6.0F, 8.0F, 1.0F, 4.24264F, -4.24264F, 5.65685F,
        0.707107F, 6.0F, -1.10215e-015F, 8.0F, 1.0F, 4.82843F, 2.82843F, 6.4379F, 0.804738F, 2.55228F, 4.55228F, 6.9586F, 0.869825F, 1.27614F, 2.27614F, 8.69825F, 0.869825F, 0.707107F,
        2.70711F, 9.02369F, 0.902369F, 0.0F, 3.0F, 10.0F, 1.0F, -2.12132F, 2.12132F, 7.07107F, 0.707107F, -3.0F, 1.83691e-016F, 10.0F, 1.0F, -2.12132F, -2.12132F, 7.07107F, 0.707107F,
        -3.67382e-016F, -3.0F, 10.0F, 1.0F, 2.12132F, -2.12132F, 7.07107F, 0.707107F, 3.0F, -5.51073e-016F, 10.0F, 1.0F, 2.41421F, 1.41421F, 8.04738F, 0.804738F, 1.27614F, 2.27614F, 8.69825F,
        0.869825F, 1.27614F, 2.27614F, 10.4379F, 0.869825F, 0.707107F, 2.70711F, 10.8284F, 0.902369F, 0.0F, 3.0F, 12.0F, 1.0F, -2.12132F, 2.12132F, 8.48528F, 0.707107F, -3.0F, 1.83691e-016F,
        12.0F, 1.0F, -2.12132F, -2.12132F, 8.48528F, 0.707107F, -3.67382e-016F, -3.0F, 12.0F, 1.0F, 2.12132F, -2.12132F, 8.48528F, 0.707107F, 3.0F, -5.51073e-016F, 12.0F, 1.0F, 2.41421F, 1.41421F,
        9.65685F, 0.804738F, 1.27614F, 2.27614F, 10.4379F, 0.869825F, 1.44629F, 2.57963F, 10.4379F, 0.869825F, 0.801388F, 3.06805F, 10.8284F, 0.902369F, 0.0F, 3.4F, 12.0F, 1.0F, -2.40416F,
        2.40416F, 8.48528F, 0.707107F, -3.4F, 2.08183e-016F, 12.0F, 1.0F, -2.40416F, -2.40416F, 8.48528F, 0.707107F, -4.16366e-016F, -3.4F, 12.0F, 1.0F, 2.40416F, -2.40416F, 8.48528F,
        0.707107F, 3.4F, -6.24549e-016F, 12.0F, 1.0F, 2.73611F, 1.60278F, 9.65685F, 0.804738F, 1.44629F, 2.57963F, 10.4379F, 0.869825F, 1.44629F, 2.57963F, 11.3077F, 0.869825F,
        0.801388F, 3.06805F, 11.7308F, 0.902369F, 0.0F, 3.4F, 13.0F, 1.0F, -2.40416F, 2.40416F, 9.19239F, 0.707107F, -3.4F, 2.08183e-016F, 13.0F, 1.0F, -2.40416F, -2.40416F, 9.19239F, 0.707107F,
        -4.16366e-016F, -3.4F, 13.0F, 1.0F, 2.40416F, -2.40416F, 9.19239F, 0.707107F, 3.4F, -6.24549e-016F, 13.0F, 1.0F, 2.73611F, 1.60278F, 10.4616F, 0.804738F, 1.44629F, 2.57963F,
        11.3077F, 0.869825F, 1.19107F, 2.1244F, 11.3077F, 0.869825F, 0.659966F, 2.52663F, 11.7308F, 0.902369F, 0.0F, 2.8F, 13.0F, 1.0F, -1.9799F, 1.9799F, 9.19239F, 0.707107F, -2.8F,
        1.71445e-016F, 13.0F, 1.0F, -1.9799F, -1.9799F, 9.19239F, 0.707107F, -3.4289e-016F, -2.8F, 13.0F, 1.0F, 1.9799F, -1.9799F, 9.19239F, 0.707107F, 2.8F, -5.14335e-016F, 13.0F, 1.0F, 2.25327F,
        1.31993F, 10.4616F, 0.804738F, 1.19107F, 2.1244F, 11.3077F, 0.869825F, 1.19107F, 2.1244F, 10.4379F, 0.869825F, 0.659966F, 2.52663F, 10.8284F, 0.902369F, 0.0F, 2.8F, 12.0F, 1.0F,
        -1.9799F, 1.9799F, 8.48528F, 0.707107F, -2.8F, 1.71445e-016F, 12.0F, 1.0F, -1.9799F, -1.9799F, 8.48528F, 0.707107F, -3.4289e-016F, -2.8F, 12.0F, 1.0F, 1.9799F, -1.9799F, 8.48528F,
        0.707107F, 2.8F, -5.14335e-016F, 12.0F, 1.0F, 2.25327F, 1.31993F, 9.65685F, 0.804738F, 1.19107F, 2.1244F, 10.4379F, 0.869825F, 1.14853F, 2.04853F, 8.78523F, 0.869825F,
        0.636396F, 2.4364F, 9.11393F, 0.902369F, 0.0F, 2.7F, 10.1F, 1.0F, -1.90919F, 1.90919F, 7.14178F, 0.707107F, -2.7F, 1.65322e-016F, 10.1F, 1.0F, -1.90919F, -1.90919F, 7.14178F,
        0.707107F, -3.30644e-016F, -2.7F, 10.1F, 1.0F, 1.90919F, -1.90919F, 7.14178F, 0.707107F, 2.7F, -4.95966e-016F, 10.1F, 1.0F, 2.17279F, 1.27279F, 8.12785F, 0.804738F, 1.14853F,
        2.04853F, 8.78523F, 0.869825F, 2.38213F, 4.2488F, 6.9586F, 0.869825F, 1.31993F, 5.05327F, 7.21895F, 0.902369F, 0.0F, 5.6F, 8.0F, 1.0F, -3.9598F, 3.9598F, 5.65685F, 0.707107F, -5.6F,
        3.4289e-016F, 8.0F, 1.0F, -3.9598F, -3.9598F, 5.65685F, 0.707107F, -6.8578e-016F, -5.6F, 8.0F, 1.0F, 3.9598F, -3.9598F, 5.65685F, 0.707107F, 5.6F, -1.02867e-015F, 8.0F, 1.0F, 4.50653F,
        2.63987F, 6.4379F, 0.804738F, 2.38213F, 4.2488F, 6.9586F, 0.869825F, 2.80751F, 5.00751F, 4.34913F, 0.869825F, 1.55563F, 5.95564F, 4.51184F, 0.902369F, 0.0F, 6.6F, 5.0F, 1.0F,
        -4.6669F, 4.6669F, 3.53553F, 0.707107F, -6.6F, 4.0412e-016F, 5.0F, 1.0F, -4.6669F, -4.6669F, 3.53553F, 0.707107F, -8.0824e-016F, -6.6F, 5.0F, 1.0F, 4.6669F, -4.6669F, 3.53553F,
        0.707107F, 6.6F, -1.21236e-015F, 5.0F, 1.0F, 5.31127F, 3.11127F, 4.02369F, 0.804738F, 2.80751F, 5.00751F, 4.34913F, 0.869825F, 2.59482F, 4.62816F, 2.43551F, 0.869825F,
        1.43778F, 5.50445F, 2.52663F, 0.902369F, 0.0F, 6.1F, 2.8F, 1.0F, -4.31335F, 4.31335F, 1.9799F, 0.707107F, -6.1F, 3.73505e-016F, 2.8F, 1.0F, -4.31335F, -4.31335F, 1.9799F, 0.707107F,
        -7.4701e-016F, -6.1F, 2.8F, 1.0F, 4.31335F, -4.31335F, 1.9799F, 0.707107F, 6.1F, -1.12051e-015F, 2.8F, 1.0F, 4.9089F, 2.87557F, 2.25327F, 0.804738F, 2.59482F, 4.62816F, 2.43551F,
        0.869825F, 1.99929F, 3.56596F, 0.34793F, 0.869825F, 1.1078F, 4.24113F, 0.360948F, 0.902369F, 0.0F, 4.7F, 0.4F, 1.0F, -3.3234F, 3.3234F, 0.282843F, 0.707107F, -4.7F,
        2.87782e-016F, 0.4F, 1.0F, -3.3234F, -3.3234F, 0.282843F, 0.707107F, -5.75565e-016F, -4.7F, 0.4F, 1.0F, 3.3234F, -3.3234F, 0.282843F, 0.707107F, 4.7F, -8.63347e-016F, 0.4F, 1.0F,
        3.78227F, 2.2156F, 0.321895F, 0.804738F, 1.99929F, 3.56596F, 0.34793F, 0.869825F, 0.0F, 0.0F, 0.173965F, 0.869825F, 0.0F, 0.0F, 0.180474F, 0.902369F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F,
        0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F, 0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F, 0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.160948F, 0.804738F, 0.0F, 0.0F, 0.173965F, 0.869825F
    };

    RtFloat uknot[] = {0.0F, 0.0F, 0.0F, 0.0833333F, 0.0833333F, 0.333333F, 0.333333F, 0.583333F, 0.583333F, 0.833333F, 0.833333F, 1.0F, 1.0F, 1.0F};
    RtFloat vknot[] = {0.0F, 0.0F, 0.0F, 0.0555556F, 0.111111F, 0.166667F, 0.222222F, 0.277778F, 0.333333F, 0.388889F, 0.444444F, 0.5F, 0.555556F, 0.611111F, 0.666667F, 0.722222F, 0.777778F, 0.833333F, 0.888889F, 0.944444F, 1.0F, 1.0F, 1.0F};

    ri.attributeBegin();
		ri.geometricApproximation("tesselation", 8.0);
        ri.scale(.1F, .1F, .1F);
        // ri.surface("DPBlueMarble", ri.RI_NULL);
		ri.surface(ri.RI_MATTE, ri.RI_NULL);
        ri.nuPatch(11, 3, uknot, 0.0F, 1.0F, 20, 3, vknot, 0.0F, 1.0F, ri.RI_PW, vaseMesh, ri.RI_NULL);
    ri.attributeEnd();
    szProcName = "NuPatch";
}

///////////////////////////////////////////////////////////////////////////////
// display small caps 'a' as plane nurbs with trim curves
void displayLetter(TRi &ri) {
    RtInt nloops = 2;

    RtFloat uknot[] = {0.0F, 0.0F, 1.0F, 1.0F};
    RtFloat vknot[] = {0.0F, 0.0F, 1.0F, 1.0F};
    RtFloat patch[] = {-.123497F, .138252F, 0.0F, .123497F, .138252F, 0.0F, -.123497F, -.138252F, 0.0F, .123497F, -.138252F, 0.0F};

    RtInt ncurves[] = {1, 1};
    RtInt order[] = {4, 4};
    RtFloat knot[] = {
     0.0F,  0.0F,  0.0F,  0.0F,
     1.0F,  1.0F,  1.0F,  2.0F,  2.0F,  2.0F,  3.0F,  3.0F,  3.0F,  4.0F,  4.0F,  4.0F,
     5.0F,  5.0F,  5.0F,  6.0F,  6.0F,  6.0F,  7.0F,  7.0F,  7.0F,  8.0F,  8.0F,  8.0F,
     9.0F,  9.0F,  9.0F, 10.0F, 10.0F, 10.0F, 11.0F, 11.0F, 11.0F, 12.0F, 12.0F, 12.0F,
    13.0F, 13.0F, 13.0F, 14.0F, 14.0F, 14.0F, 15.0F, 15.0F, 15.0F,
    16.0F, 16.0F, 16.0F, 16.0F,

     0.0F,  0.0F,  0.0F,  0.0F,
     1.0F,  1.0F,  1.0F,  2.0F,  2.0F,  2.0F,  3.0F,  3.0F,  3.0F,  4.0F,  4.0F,  4.0F,
     5.0F,  5.0F,  5.0F,  5.0F
    };
    RtFloat amin[] = {0.0F, 0.0F};
    RtFloat amax[] = {16.0F, 5.0F};
    RtInt n[] = {49, 16};

    RtFloat u[] = {
        0.216594F, 0.228702F, 0.257058F, 0.459505F, 0.627541F,
        0.70847F,  0.70847F,  0.70847F,  0.657874F, 0.61537F,
        0.517633F, 0.419601F, 0.321863F, 0.026317F, 0.0F,
        0.0F,      0.0F,      0.137641F, 0.331995F, 0.538457F,
        0.645765F, 0.714586F, 0.72064F,  0.744919F, 0.902823F,
        0.94335F,  0.969667F, 1.0F,      1.0F,      1.0F,
        1.0F,      0.979737F, 0.957497F, 0.941311F, 0.904862F,
        0.88256F,  0.88256F,  0.882561F, 0.88256F,  0.88256F,
        0.88256F,  0.574906F, 0.489899F, 0.228702F, 0.060728F,
        0.052635F, 0.107234F, 0.161995F, 0.216594F,

        0.704454F, 0.704454F, 0.542472F, 0.376475F, 0.242911F,
        0.184222F, 0.184222F, 0.184222F, 0.323902F, 0.410947F,
        0.631556F, 0.676098F, 0.704454F, 0.704455F, 0.704454F,
        0.704454F
    };
    RtFloat v[] = {
        0.314607F, 0.242316F, 0.132002F, 0.132002F, 0.132002F,
        0.186248F, 0.285691F, 0.379725F, 0.394184F, 0.397826F,
        0.408669F, 0.408668F, 0.430385F, 0.462888F, 0.64737F,
        0.726947F, 0.889686F, 1.0F,      1.0F,      1.0F,
        0.913195F, 0.846256F, 0.918603F, 0.99095F,  0.99095F,
        0.99095F,  0.980078F, 0.972849F, 0.934315F, 0.934314F,
        0.857127F, 0.860714F, 0.864356F, 0.864356F, 0.864356F,
        0.848077F, 0.808289F, 0.627043F, 0.627042F, 0.264003F,
        0.021688F, 0.0F,      0.0F,      0.0F,      0.088571F,
        0.314607F, 0.314608F, 0.314607F, 0.314607F,

        0.645549F, 0.772143F, 0.867943F, 0.867943F, 0.867943F,
        0.806468F, 0.703438F, 0.584074F, 0.560565F, 0.549694F,
        0.524419F, 0.509906F, 0.490039F, 0.541825F, 0.541824F,
        0.645549F
    };
    RtFloat w[] = {
        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,

        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
        1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
    };

    ri.attributeBegin();
        ri.scale(5.0F, 5.0F, 5.0F);
        ri.trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
        ri.nuPatch(2, 2, uknot, 0.0F, 1.0F, 2, 2, vknot, 0.0F, 1.0F, ri.RI_P, patch, ri.RI_NULL);
    ri.attributeEnd();
    szProcName = "NuPatch, TrimCurve";
}


///////////////////////////////////////////////////////////////////////////////
// display some quadrics as spinning coordinate-system
void displayQuadrics(TRi &ri) {
    RtFloat material[4]  = {0.1F, 0.5F, 0.8F, 1.0F};
    // RtFloat materialL[4] = {1.0F, 1.0F, 1.0F, 1.0F};
    RtFloat materialX[4] = {1.0F, 0.0F, 0.0F, 1.0F};
    RtFloat materialY[4] = {0.0F, 1.0F, 0.0F, 1.0F};
    RtFloat materialZ[4] = {0.0F, 0.0F, 1.0F, 1.0F};

    RtPoint p1 = {0.0F, 0.2F, -0.2F};
    RtPoint p2 = {(RtFloat)0.1, (RtFloat)0.1, 0.2F};
    RtFloat len1 = (RtFloat)sqrt(p1[0]*p1[0]+p1[1]*p1[1]);
    RtFloat len2 = (RtFloat)sqrt(p2[0]*p2[0]+p2[1]*p2[1]);

    ri.attributeBegin();
        ri.transformBegin();
            ri.color(material);
            ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.sphere((RtFloat)0.2, (RtFloat)-0.2, (RtFloat)0.2, (RtFloat)360.0);
        ri.transformEnd();

        ri.transformBegin();
            ri.translate(0.0F, 0.0F, 0.8F);
            ri.color(materialZ);
            ri.surface(ri.RI_PLASTIC, ri.RI_NULL);
            ri.cone((RtFloat)0.4, (RtFloat)0.3, (RtFloat)360.0);
            ri.cylinder((RtFloat)0.15, (RtFloat)-0.8, (RtFloat)0.0, (RtFloat)360.0);
            ri.rotate(180.0F, 1.0F, 0.0F, 0.0F);
            ri.disk((RtFloat)0.0, (RtFloat)0.3, (RtFloat)360.0);
        ri.transformEnd();

        ri.transformBegin();
            ri.rotate(270.0F, 1.0F, 0.0F, 0.0F);
            ri.translate(0.0F, 0.0F, 0.8F);
            ri.color(materialY);
            ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.cone((RtFloat)0.4, (RtFloat)0.3, (RtFloat)360.0);
            // ri.matte(ri.RI_TRUE);
            ri.cylinder((RtFloat)0.15, (RtFloat)-0.8, (RtFloat)0.0, (RtFloat)360.0);
            // ri.matte(ri.RI_FALSE);
            ri.rotate(180.0F, 1.0F, 0.0F, 0.0F);
            ri.disk((RtFloat)0.0, (RtFloat)0.3, (RtFloat)360.0);
        ri.transformEnd();

        ri.transformBegin();
            ri.rotate(-90.0F, 1.0F, 0.0F, 0.0F);
            ri.rotate(90.0F, 0.0F, 1.0F, 0.0F);
            ri.translate(0.0F, 0.0F, 0.8F);

            ri.color(materialX);
            ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.cone((RtFloat)0.4, (RtFloat)0.3, (RtFloat)360.0);
            ri.cylinder((RtFloat)0.15, (RtFloat)-0.8, (RtFloat)0.0, 360.0F);
            ri.rotate(180.0F, 1.0F, 0.0F, 0.0F);
            ri.disk((RtFloat)0.0, (RtFloat)0.3, (RtFloat)360.0);
        ri.transformEnd();

        ri.transformBegin();
            ri.color(material);
            ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.torus(0.5F, 0.1F, 0.0F, 360.0F, 360.0F);
            ri.rotate(90.0F, 0.0F, 1.0F, 0.0F);
            ri.torus(0.5F, 0.1F, 0.0F, 360.0F, 360.0F);
            ri.rotate(-90.0F, 0.0F, 1.0F, 0.0F);
            ri.rotate(90.0F, 1.0F, 0.0F, 0.0F);
            ri.torus(0.5F, 0.1F, 0.0F, 360.0F, 360.0F);
        ri.transformEnd();

        // X
        ri.transformBegin();
            ri.rotate(spin, 1.0F, 0.0F, 0.0F);
            ri.translate(0.9F, 0.0F, 0.9F);
            ri.rotate(270.0f, 1.0F, 0.0F, 0.0F);

            ri.color(materialX);
            ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.paraboloid(.16F, 0.0F, .4F, 360.0F, ri.RI_NULL);
            ri.disk(0.4F, 0.16F, 360.0F);
        ri.transformEnd();

        // Y
        ri.transformBegin();
            ri.rotate(-spin, 0.0F, 1.0F, 0.0F);
            ri.translate(0.7F, 0.7F, 0.0F);

            ri.color(materialY);
            ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.sphere(0.2F, -0.2F, 0.2F, 360.0, ri.RI_NULL);
        ri.transformEnd();

        // Z
        ri.transformBegin();
            ri.rotate(spin, 0.0F, 0.0F, 1.0F);
            ri.translate(0.5F, 0.0F, 0.5F);
            ri.rotate(90.0f, 1.0F, 0.0F, 0.0F);

            ri.color(materialZ);
            ri.surface(ri.RI_METAL, ri.RI_NULL);
            ri.hyperboloid(p1, p2, 360.0F);
            ri.disk(p2[2], len2, 360.0F);
            ri.transformBegin();
                ri.translate(0.0, 0.0, p1[2]);
                ri.rotate(180.0F, 1.0F, 0.0F, 0.0F);
                ri.disk(0.0, len1, 360.0F);
            ri.transformEnd();
        ri.transformEnd();
    ri.attributeEnd();
    szProcName = "Quadrics";
}


///////////////////////////////////////////////////////////////////////////////
// The teapot geometry
void displayTeapot(TRi &ri) {
    RtFloat material[4]  = {0.1F, 0.5F, 0.8F, 1.0F};

    ri.rotate(-90.0, 1.0F, 0.0F, 0.0F);
    ri.scale(.5F, .5F, .5F);
    ri.translate(0.0, 0.0, -1.0);
    ri.color(material);
    ri.surface(ri.RI_MATTE, ri.RI_NULL);
    ri.geometry("teapot", ri.RI_NULL);
    szProcName = "Geometry Teapot";
}


///////////////////////////////////////////////////////////////////////////////
// the lights for the scene
void displayGlobalLights(TRi &ri, bool showLamp) {
    RtFloat lampColor[3] = {1.0F, 1.0F, 1.0F};
    // RtFloat intensity = 0.8F;
    RtFloat ambientIntensity = 0.1F;
    RtFloat distantIntensity = 0.5F;
	RtFloat illuIntensity = 1.0F - distantIntensity - ambientIntensity;

    RtPoint p1 = {0.0F, 0.2F, -0.2F};
    RtPoint p2 = {(RtFloat)0.1, (RtFloat)0.1, 0.2F};
//  RtFloat len1 = (RtFloat)sqrt(p1[0]*p1[0]+p1[1]*p1[1]);
    RtFloat len2 = (RtFloat)sqrt(p2[0]*p2[0]+p2[1]*p2[1]);

    RtFloat distant_from[3]  = {1.0F, 1.0F, -1.0F};

    RtFloat position[3]  = {1.5F, 1.5F, 1.5F};
    RtFloat position0[3] = {0.0F, 0.0F, 0.0F};

    ri.lightSource(ri.RI_AMBIENTLIGHT, ri.RI_INTENSITY, &ambientIntensity, ri.RI_NULL);

//  RtFloat ambientIntensity2 = .1F;
//  RtFloat ambientColor2[3] = {1.0F, 1.0F, 0.0F};
//  ri.lightSource(ri.RI_AMBIENTLIGHT, ri.RI_INTENSITY, &ambientIntensity2, ri.RI_LIGHTCOLOR, ambientColor2, ri.RI_NULL);


    ri.transformBegin();
        RtLightHandle handle = ri.lightSource(ri.RI_DISTANTLIGHT, ri.RI_FROM, distant_from, ri.RI_TO, position0, ri.RI_INTENSITY, &distantIntensity, ri.RI_NULL);
        handle = handle;
        ri.rotate(spin, 0.0F, 1.0F, 0.0F);
        ri.transformBegin();
            ri.color(lampColor);
//          ri.lightSource(ri.RI_SPOTLIGHT, ri.RI_FROM, position, ri.RI_TO, position0, ri.RI_NULL);
//          ri.translate(position[0], position[1], position[2]);
//            ri.lightSource(ri.RI_POINTLIGHT, ri.RI_FROM, position, (RtToken)ri.RI_NULL);
            ri.lightSource(ri.RI_DISTANTLIGHT, ri.RI_FROM, position, ri.RI_TO, position0, ri.RI_INTENSITY, &illuIntensity, (RtToken)ri.RI_NULL);
        ri.transformEnd();
    ri.transformEnd();


    if ( showLamp ) {
        ri.attributeBegin();
            ri.rotate(spin, 0.0F, 1.0F, 0.0F);
            ri.transformBegin();
                ri.translate(position[0], position[1], position[2]);
                ri.rotate(45.0F, 0.0F, 1.0F, 0.0F);
                ri.rotate(-45.0F, 1.0F, 0.0F, 0.0F);
                ri.color(lampColor);
                ri.surface(ri.RI_MATTE, ri.RI_NULL);
                ri.hyperboloid(p1, p2, 360.0F);
                ri.transformBegin();
                    ri.disk(p2[2], len2, 360.0F);
                ri.transformEnd();
                ri.translate(0.0F, 0.0F, -0.1F);
                ri.scale(1.0F, 1.0F, 2.0F);
                ri.surface(ri.RI_CONSTANT, ri.RI_NULL);
                ri.sphere((RtFloat)0.1, (RtFloat)-0.1, (RtFloat)0.1, (RtFloat)360.0);
                ri.surface(ri.RI_MATTE, ri.RI_NULL);
            ri.transformEnd();
        ri.attributeEnd();
    }

}

///////////////////////////////////////////////////////////////////////////////
// polygon primitives
void displayPolygon(TRi &ri, int type) {
    int nVerts[2] = {4, 8};
    RtFloat points[36] = {
        -1.0,  1.0, 0.0,
         1.0,  1.0, 0.0,
         1.0, -1.0, 0.0,
        -1.0, -1.0, 0.0,

         0.5,   0.5,  0.0,
         0.25,  0.0,  0.0,
         0.5,  -0.5,  0.0,
         0.0,  -0.25, 0.0,
        -0.5,  -0.5,  0.0,
        -0.25,  0.0,  0.0,
        -0.5,   0.5,  0.0,
         0.0,   0.25, 0.0
    };

    int npolys_cube = 6;
    int nloops_cube[6] = {1, 1, 1, 1, 1, 1};
    int nverts_cube[6] = {4, 4, 4, 4, 4, 4};
    int verts_cube[24] = {
        0, 1, 2, 3, // Front
        4, 0, 3, 7, // Left
        5, 4, 7, 6, // Rear
        1, 5, 6, 2, // Right
        4, 5, 1, 0, // Top
        3, 2, 6, 7  // Bottom
    };
    RtFloat points_cube[24] = {
        -1.0,  1.0,  1.0, // 0 Front face
         1.0,  1.0,  1.0, // 1
         1.0, -1.0,  1.0, // 2
        -1.0, -1.0,  1.0, // 3

        -1.0,  1.0,  3.0, // 4 Rear face
         1.0,  1.0,  3.0, // 5
         1.0, -1.0,  3.0, // 6
        -1.0, -1.0,  3.0, // 7
    };

    switch ( type ) {
    case 1:
        ri.polygon(4, ri.RI_P, points, ri.RI_NULL);
        szProcName = "Polygon";
        break;
    case 2:
        ri.generalPolygon(2, nVerts, ri.RI_P, points, ri.RI_NULL);
        szProcName = "GeneralPolygon";
        break;
    case 3:
        ri.pointsPolygons(npolys_cube, nverts_cube, verts_cube, ri.RI_P, points_cube, ri.RI_NULL);
        szProcName = "PointsPolygon";
        break;
    default:
        ri.pointsGeneralPolygons(npolys_cube, nloops_cube, nverts_cube, verts_cube, ri.RI_P, points_cube, ri.RI_NULL);
        szProcName = "PointsGeneralPolygon";
        break;

    }
}


///////////////////////////////////////////////////////////////////////////////
// linear, bicubic patch primitives
void displayPatch(TRi &ri, int type) {
    RtFloat mvpt = (RtFloat)(((even > 0.0) ? spin/360.0 : (360.0-spin)/360.0) * -1.0);

    RtFloat patch[12] = {
        -1.0,  1.0,  -0.5F,
         1.0,  1.0,  0.5F + mvpt,
        -1.0, -1.0,  0.8F,
         1.0, -1.0,  -0.2F
    };

    RtFloat bicubic[4*4*3] = {
        -0.9F,  0.9F, -0.5F - mvpt,
        -0.3F,  0.9F,  0.0F,
         0.3F,  0.9F,  0.0F,
         0.9F,  0.9F, -0.5F + mvpt,

        -0.9F,  0.3F,  0.0F,
        -0.3F,  0.3F,  0.5F + mvpt,
         0.3F,  0.3F,  0.5F + mvpt,
         0.9F,  0.3F,  0.0F,

        -0.9F, -0.3F,  0.0F,
        -0.3F, -0.3F,  0.5F + mvpt,
         0.3F, -0.3F,  0.5F + mvpt,
         0.9F, -0.3F,  0.0F,

        -0.9F, -0.9F, -0.5F + mvpt,
        -0.3F, -0.9F,  0.0F,
         0.3F, -0.9F,  0.0F,
         0.9F, -0.9F, -0.5F - mvpt
    };

    RtFloat patchMesh [5*5*3] = {
        -1.0F,  1.0F, 0.0F,  -0.5F,  1.0F, 0.0F,   0.0F,  1.0F, 0.8F,  0.5F,  1.0F, 0.0F,  1.0F,  1.0F, 0.0F,
        -1.0F,  0.5F, 0.0F,  -0.5F,  0.5F, 0.1F,   0.0F,  0.5F, 0.4F,  0.5F,  0.5F, 0.1F,  1.0F,  0.5F, 0.0F,
        -1.0F,  0.0F, 0.0F,  -0.5F,  0.0F, 0.2F,   0.0F,  0.0F, 0.2F,  0.5F,  0.0F, 0.2F,  1.0F,  0.0F, 0.0F,
        -1.0F, -0.5F, 0.0F,  -0.5F, -0.5F, 0.4F,   0.0F, -0.5F, 0.1F,  0.5F, -0.5F, 0.4F,  1.0F, -0.5F, 0.0F,
        -1.0F, -1.0F, 0.0F,  -0.5F, -1.0F, 0.8F,   0.0F, -1.0F, 0.0F,  0.5F, -1.0F, 0.8F,  1.0F, -1.0F, 0.0F
    };

	RtFloat colors [3*16] = {
		0.0F, 0.0F, 1.0F,
		0.0F, 0.2F, 0.8F,
		0.0F, 0.4F, 0.4F,
		0.0F, 0.8F, 0.2F,

		0.0F, 1.0F, 0.0F,
		0.2F, 0.8F, 0.0F,
		0.4F, 0.4F, 0.0F,
		0.8F, 0.2F, 0.0F,

		1.0F, 0.0F, 0.0F,
		0.8F, 0.0F, 0.2F,
		0.4F, 0.0F, 0.4F,
		0.2F, 0.0F, 0.8F,

		1.0F, 1.0F, 1.0F,
		0.8F, 0.8F, 0.8F,
		0.4F, 0.4F, 0.4F,
		0.2F, 0.2F, 0.2F
	};

    switch ( type ) {
    case 1:
        ri.patch(ri.RI_BILINEAR, ri.RI_P, patch, "Cs", colors, ri.RI_NULL);
        szProcName = "Patch, Bilinear (Cs)";
        break;
    case 2:
        ri.patch(ri.RI_BICUBIC, ri.RI_P, bicubic, "vertex color Cs", colors, ri.RI_NULL);
        szProcName = "Patch, Bicubic (vertex color Cs)";
        break;
    default:
        ri.patchMesh(ri.RI_BILINEAR, 5, ri.RI_NONPERIODIC, 5, ri.RI_NONPERIODIC, ri.RI_P, patchMesh, "uniform color Cs", colors, ri.RI_NULL);
        szProcName = "PatchMesh, Bilinear (uniform color Cs)";
        break;

    }
}


///////////////////////////////////////////////////////////////////////////////
// subdivision surface
void displaySubdivisionSurface(TRi &ri) {


/*
	RtInt nfaces = 9;
    RtInt nvertices[9] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
    RtInt vertices[9*4] = {
        0, 4,   5, 1, 1, 5,   6,  2,  2,  6,  7,  3,
        4, 8,   9, 5, 5, 9,  10,  6,  6, 10, 11,  7,
        8, 12, 13, 9, 9, 13, 14, 10, 10, 14, 15, 11
    };
    RtInt ntags = 1;
	RtToken tags[1] = { ri.RI_INTERPOLATEBOUNDARY };
    RtInt nargs[2] = { 0, 0 };
    RtFloat controlMesh[16*4] = {
        -0.6F, 0.6F, 0.0F, -0.6F, 0.2F, 0.0F,  -0.6F, -0.2F, 0.0F,  -0.6F, -0.6F, 0.0F,
        -0.2F, 0.6F, 0.0F, -0.2F, 0.2F, 0.45F, -0.2F, -0.2F, 0.45F, -0.2F, -0.6F, 0.0F,
         0.2F, 0.6F, 0.0F,  0.2F, 0.2F, 0.45F,  0.2F, -0.2F, 0.45F,  0.2F, -0.6F, 0.0F,
         0.6F, 0.6F, 0.0F,  0.6F, 0.2F, 0.0F,   0.6F, -0.2F, 0.0F,   0.6F, -0.6F, 0.0F
    };
    ri.subdivisionMesh(ri.RI_CATMULLCLARK, nfaces, nvertices, vertices, ntags, tags, nargs, NULL, NULL, ri.RI_P, controlMesh, ri.RI_NULL);
//	ri.subdivisionMesh(ri.RI_CATMULLCLARK, nfaces, nvertices, vertices, 0, NULL, NULL, NULL, NULL, ri.RI_P, controlMesh, ri.RI_NULL);
*/

	if ( 1 ) {

		RtInt nfaces = 9;
		RtInt nvertices[9] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
		RtInt vertices[9*4] = {
			0, 4,   5, 1, 1, 5,   6,  2,  2,  6,  7,  3,
			4, 8,   9, 5, 5, 9,  10,  6,  6, 10, 11,  7,
			8, 12, 13, 9, 9, 13, 14, 10, 10, 14, 15, 11
		};
		RtFloat controlMesh[16*3] = {
			-0.6F, 0.6F, 0.0F, -0.6F, 0.2F, 0.0F,  -0.6F, -0.2F, 0.0F,  -0.6F, -0.6F, 0.0F,
			-0.2F, 0.6F, 0.0F, -0.2F, 0.2F, 0.45F, -0.2F, -0.2F, 0.45F, -0.2F, -0.6F, 0.0F,
			0.2F, 0.6F, 0.0F,  0.2F, 0.2F, 0.45F,  0.2F, -0.2F, 0.45F,  0.2F, -0.6F, 0.0F,
			0.6F, 0.6F, 0.0F,  0.6F, 0.2F, 0.0F,   0.6F, -0.2F, 0.0F,   0.6F, -0.6F, 0.0F
		};
		// RtInt sumVerts = 16*3;
		RtInt ntags = 1;
		RtToken tags[1] = { TRi::RI_INTERPOLATEBOUNDARY };
		RtInt nargs[2] = { 0, 0 };

		ri.subdivisionMesh(TRi::RI_CATMULLCLARK, nfaces, nvertices, vertices, ntags, tags, nargs, NULL, NULL, TRi::RI_P, controlMesh, TRi::RI_NULL);

	} else {

		RtInt nfaces = 2;
		RtInt nvertices[2] = {4, 4};
		RtInt vertices[2*4] = {
			0, 1, 2, 3,
			0, 4, 5, 1
		};
		RtFloat controlMesh[6*3] = {
			1.0,  1.0, 0.0,
			-1.0,  1.0, 0.0,
			-1.0, -1.0, 0.0,
			1.0, -1.0, 0.0,
			1.0,  3.0, 0.0,
			-1.0,  3.0, 0.0
		};
		// RtInt sumVerts = 6*3;
		RtInt ntags = 1;
		RtToken tags[1] = { TRi::RI_INTERPOLATEBOUNDARY };
		RtInt nargs[2] = { 0, 0 };

		ri.subdivisionMesh(TRi::RI_CATMULLCLARK, nfaces, nvertices, vertices, ntags, tags, nargs, NULL, NULL, TRi::RI_P, controlMesh, TRi::RI_NULL);

	}


	szProcName = "SubdivisionMesh";
}


///////////////////////////////////////////////////////////////////////////////
// Archive Ed Catmull's Gumbo
void displayRibElephant(TRi &ri) {
    static char t[512];
	t[0] = '\0';
    sprintf(t, "%s\\ribs\\%s", modulepath, "Elephant.rib");
    int i;
    for (i=0; t[i] != (char)0; ++i ) {
        if ( t[i] == '\\' ) t[i] = '/';
    }
    ri.scale(0.09F, 0.09F, 0.09F);
    ri.translate(-10.0, -5.0, 0.0);
    ri.rotate(-90.0, 1.0, 0.0, 0.0);
    float Km[1] = { -1.0 };
    float roughness[1] = { 1.5 };
    ri.displacement("dented", "float Km", Km, ri.RI_NULL);
    RtFloat Cs[3] = {0.7F, 0.7F, 0.7F};
    // ri.surface(ri.RI_METAL, "float roughness", roughness, ri.RI_NULL);
    ri.surface(ri.RI_METAL, ri.RI_ROUGHNESS, roughness, ri.RI_NULL);
    // ri.surface(ri.RI_MATTE, ri.RI_NULL);
    ri.color(Cs);
    ri.readArchive(t, NULL, ri.RI_NULL);
    szProcName = "Ribarchive Elephant";
}


///////////////////////////////////////////////////////////////////////////////
// Archive Enterprise
void displayRibEnterprise(TRi &ri) {
    static char t[512];
	t[0] = '\0';
    sprintf(t, "%s\\ribs\\%s", modulepath, "Enterprise.rib");
    int i;
    for (i=0; t[i] != (char)0; ++i ) {
        if ( t[i] == '\\' ) t[i] = '/';
    }
    ri.scale(0.5F, 0.5F, 0.5F);
    ri.rotate(-90.0, 1.0, 0.0, 0.0);
    ri.readArchive(t, NULL, ri.RI_NULL);
    szProcName = "Ribarchive Enterprise";
}


///////////////////////////////////////////////////////////////////////////////
// Archive WarBird
void displayRibWarbird(TRi &ri) {
    static char t[512];
	t[0] = '\0';
    sprintf(t, "%s\\ribs\\%s", modulepath, "WarBird.rib");
    int i;
    for (i=0; t[i] != (char)0; ++i ) {
        if ( t[i] == '\\' ) t[i] = '/';
    }
    ri.scale(0.002F, 0.002F, 0.002F);
    ri.rotate(-90.0, 1.0, 0.0, 0.0);
    // char *data[2] = {t, ""};
    ri.readArchive(t, NULL, ri.RI_NULL);
    szProcName = "Ribarchive WarBird";
}


///////////////////////////////////////////////////////////////////////////////
// Archive AirPlane
void displayRibAirplane(TRi &ri) {
    static char t[512];
	t[0] = '\0';
    sprintf(t, "%s\\ribs\\%s", modulepath, "AirPlane.rib");
    int i;
    for (i=0; t[i] != (char)0; ++i ) {
        if ( t[i] == '\\' ) t[i] = '/';
    }
    ri.scale(0.3F, 0.3F, 0.3F);
    ri.translate(0.0, -3.0, 0.0);
    ri.rotate(-90.0, 1.0, 0.0, 0.0);
    ri.readArchive(t, NULL, ri.RI_NULL);
    szProcName = "Ribarchive Airplane";
}


///////////////////////////////////////////////////////////////////////////////
// RIB Archive
void displayRibArchive(TRi &ri, const char *filename) {
    static char t[512];
	t[0] = '\0';
    sprintf(t, "%s\\ribs\\%s", modulepath, filename);
    int i;
    for (i=0; t[i] != (char)0; ++i ) {
        if ( t[i] == '\\' ) t[i] = '/';
    }
    ri.readArchive(t, NULL, ri.RI_NULL);
    szProcName = "Ribarchive";
}


///////////////////////////////////////////////////////////////////////////////
// Solids (dice)
void displayDots(TRi &ri, int val, RtFloat mag=1.0) {
    if ( val < 1 || val > 6 )
        return;

    RtFloat offs[6*3] = {
         0.0F*mag, -1.0F*mag,  0.0F*mag,
         1.0F*mag,  0.0F*mag,  0.0F*mag,
         0.0F*mag,  0.0F*mag,  1.0F*mag,
         0.0F*mag,  0.0F*mag, -1.0F*mag,
        -1.0F*mag,  0.0F*mag,  0.0F*mag,
         0.0F*mag,  1.0F*mag,  0.0F*mag
    };

    RtFloat pos[7*3] = {
        -0.5F*mag,  0.5F*mag,
         0.0F*mag,  0.5F*mag,
         0.5F*mag,  0.5F*mag,
         0.0F*mag,  0.0F*mag,
        -0.5F*mag, -0.5F*mag,
         0.0F*mag, -0.5F*mag,
         0.5F*mag, -0.5F*mag
    };

    RtFloat dotSize = 0.15F*mag;

    ri.solidBegin(ri.RI_PRIMITIVE);
        ri.transformBegin();
            ri.translate(offs[(val-1)*3+0], offs[(val-1)*3+1], offs[(val-1)*3+2]);
            switch ( val ) {
            case 1:
                ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                break;
            case 2:
                ri.transformBegin();
                    ri.translate(0.0, pos[0*2+0], pos[0*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(0.0, pos[6*2+0], pos[6*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                break;
            case 3:
                ri.transformBegin();
                    ri.translate(pos[0*2+0], pos[0*2+1], 0.0);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformBegin();
                    ri.translate(pos[6*2+0], pos[6*2+1], 0.0);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                break;
            case 4:
                ri.transformBegin();
                    ri.translate(pos[0*2+0], pos[0*2+1], 0.0);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[2*2+0], pos[2*2+1], 0.0);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[4*2+0], pos[4*2+1], 0.0);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[6*2+0], pos[6*2+1], 0.0);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                break;
            case 5:
                ri.transformBegin();
                    ri.translate(0.0, pos[0*2+0], pos[0*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(0.0, pos[2*2+0], pos[2*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformBegin();
                    ri.translate(0.0, pos[4*2+0], pos[4*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(0.0, pos[6*2+0], pos[6*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                break;
            default:
                ri.transformBegin();
                    ri.translate(pos[0*2+0], 0.0, pos[0*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[1*2+0], 0.0, pos[1*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[2*2+0], 0.0, pos[2*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[4*2+0], 0.0, pos[4*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[5*2+0], 0.0, pos[5*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                ri.transformBegin();
                    ri.translate(pos[6*2+0], 0.0, pos[6*2+1]);
                    ri.sphere(dotSize, -dotSize, dotSize, 360.0F, ri.RI_NULL);
                ri.transformEnd();
                break;
            }
        ri.transformEnd();
    ri.solidEnd();
}

void displaySolids(TRi &ri) {
    RtFloat lightwood[3] = {0.69F, 0.44F, 0.25F};
    RtFloat darkwood[3]  = {0.35F, 0.22F, 0.08F};
    RtFloat lightwood2[3] = {0.69F*0.2F, 0.44F*0.2F, 0.25F*0.2F};
    RtFloat darkwood2[3]  = {0.35F*0.2F, 0.22F*0.2F, 0.08F*0.2F};

    ri.attributeBegin();
        ri.surface("wood2", "color darkwood", &darkwood[0], "color lightwood", &lightwood[0], ri.RI_NULL);
        ri.solidBegin(ri.RI_DIFFERENCE);
            ri.solidBegin(ri.RI_INTERSECTION);
                ri.solidBegin(ri.RI_PRIMITIVE);
                    ri.transformBegin();
                        ri.translate(0.0, 0.0, -2.0);
                        displayPolygon(ri, 3);
                    ri.transformEnd();
                ri.solidEnd();
                ri.solidBegin(ri.RI_PRIMITIVE);
                    ri.sphere(1.4F, -1.4F, +1.4F, 360.0F, ri.RI_NULL);
                ri.solidEnd();
            ri.solidEnd();
            ri.surface("wood2", "color darkwood", &darkwood2[0], "color lightwood", &lightwood2[0], ri.RI_NULL);
            displayDots(ri, 1);
            displayDots(ri, 2);
            displayDots(ri, 3);
            displayDots(ri, 4);
            displayDots(ri, 5);
            displayDots(ri, 6);
        ri.solidEnd();
    ri.attributeEnd();
    szProcName = "Solids (CSG)";
}


///////////////////////////////////////////////////////////////////////////
// display primitives
void display(TRi &ri, RtObjectHandle handle, bool showLamp) {
    RtFloat material[4]  = {0.1F, 0.5F, 0.8F, 1.0F};

    ri.declare("identifier", "string");
    ri.attributeBegin();
        displayGlobalLights(ri, showLamp);

//      ri.sides(1);
        ri.rotate(-spin, 0.0, 1.0, 0.0);

        ri.color(material);
        ri.surface(ri.RI_MATTE, ri.RI_NULL);

//      RtFloat lightwood[3] = {0.69F, 0.44F, 0.25F};
//      RtFloat darkwood[3]  = {0.35F, 0.22F, 0.08F};
//      ri.surface("wood2", "color darkwood", &darkwood[0], "color lightwood", &lightwood[0], ri.RI_NULL);

        switch ( primitiveNO ) {
        case 0:
            displayQuadrics(ri);
            break;
        case 1:
            displayPolygon(ri, 1);
            break;
        case 2:
            displayPolygon(ri, 2);
            break;
        case 3:
            ri.translate(0.0, 0.0, -0.25);
            ri.perspective(90.0);
            displayPolygon(ri, 3);
            break;
        case 4:
            ri.translate(0.0, 0.0, -2.0);
            displayPolygon(ri, 4);
            break;
        case 5:
            displayPatch(ri, 1);
            break;
        case 6:
            displayPatch(ri, 2);
            break;
        case 7:
            displayPatch(ri, 3);
            break;
        case 8:
            displayPin(ri, handle);
            break;
        case 9:
            displayLetter(ri);
            break;
        case 10:
            displaySubdivisionSurface(ri);
            break;
        case 11:
            displayTeapot(ri);
            break;
        case 12:
            displaySolids(ri);
            break;
        case 13:
            // ri.scale(1.5, 1.5, 1.5);
            ri.translate(0.0, -0.4F, 0.0);
            displayRibArchive(ri, "Tetra.rib");
            break;
        case 14: {
            char name[64];
            static int ispin = 0;
            ispin %= 36;
            ri.scale(.35F,.35F,.35F);
            // ri.declare("u", "float");
            // ri.declare("v", "float");
            sprintf(name, "mechtoy.%d.rib", ispin+1);
            ispin += 1;
            displayRibArchive(ri, name);
            break;
        }
        case 15: {
            char name[64];
            static int ispin = 0;
            ispin %= 5;
            ri.scale(.085F,.085F,.085F);
            sprintf(name, "gears.%d.rib", ispin+1);
            ispin += 1;
            displayRibArchive(ri, name);
            break;
        }
        case 16:
            displayVase(ri);
            break;
        case 17:
            displayRibElephant(ri);
            break;
        case 18:
            displayRibEnterprise(ri);
            break;
        case 19:
            displayRibWarbird(ri);
            break;
        case 20:
            displayRibAirplane(ri);
            break;
        }

    ri.attributeEnd();
}
