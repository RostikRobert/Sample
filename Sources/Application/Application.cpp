#include "pch.h"

#include "Application.h"
#include "Common/Globals.h"
#include "Common/Base_Funcs.h"
#include <iostream>
#include <chrono>

namespace
{
	// static pointer for class application to get rid of complexity
	static Application* kpApp = nullptr;

	enum class ELayer : int
	{
		kLayerBackground = 0,
		kLayerUnit,
		kLayaerParticle
	};

	std::unordered_map< ELayer, float > kLayers =
	{
		{ELayer::kLayerBackground, 0.0f},
		{ELayer::kLayerUnit, 1.0f},
		{ELayer::kLayaerParticle, 2.0f}
	};

}
//fwd
LRESULT WINAPI ESWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

Application::Application()
	: eglContext(nullptr)
	, eglDisplay(nullptr)
	, eglSurface(nullptr)
	//, _uiSprites(0)
{
	kpApp = this;
}

Application::~Application()
{
}

void Application::Init()
{
	// initing window and open gl
	this->_MakeWindow( "Drawing Sample", Globals::screenWidth, Globals::screenHeight );
	this->_InitOGL();

	// initing drawable for all sprites
	this->_drawable.Init();
}

void Application::Mouse( float x, float y )
{
	//SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}

void Application::MouseClick( bool isLeft )
{
}



void Application::Key(unsigned char key, bool bIsPressed)
{
	if (!bIsPressed)
		return;

	// move cam by X axis
	//if( key == 'N' )
	//{
	//}

	if (key == VK_SPACE)
	{
		Sprite spr;

		// Random color changing implementation
		spr.setTint({ Base::RandF(), Base::RandF(), Base::RandF(), 1.0f });

		spr.setTexture(Globals::Texures["star.tga"]);
		spr.setDepth(kLayers[ELayer::kLayaerParticle]);
		spr.setScale({ 50, 50 });
		Vector2 pos =
		{
			Base::RandF() * (float)Globals::screenWidth,
			Base::RandF() * (float)Globals::screenHeight
		};
		spr.setPosition(pos);
		spr.MakeObjectTM();
		
		_animation.SetAnim(spr, 0.0f, 2.f);

	}
}

void Application::Frame()
{
	this->_curTime = GetTickCount64();
	ULONGLONG diff = this->_curTime - this->_lastTime;

	// Intel GPU is trash! 
	// it's just working regardless to sync primitive.
	// as result, there are frames with ZERO 
	if( diff < 1 )
	{
		// skip this frame
		return;
	}

	float deltaTime = ( float )( diff ) / 1000.0f;
	this->_lastTime = this->_curTime;

	if( deltaTime > ( 1.0f / 30.0f ) )
	{
		// avoid debug values
		deltaTime = 1.0f / 30.0f;
	}
	else if( deltaTime < 0.0001f )
	{
		// insane! 
		deltaTime = 1.0f / 60.0f;
	}

	_animation.Update(deltaTime);
	//this->Update(deltaTime);

	if( this->eglContext )
	{
		eglMakeCurrent( this->eglDisplay, this->eglSurface, this->eglSurface, this->eglContext );

		this->Draw();

		// present backbuffer
		SwapBuffers(GetDC(this->_hwnd));
	}
}


void Application::Update( float deltaTime )
{	
	//Dummy update
	/*	int i;
	for ( i = 0; i < _sprite.size(); i++)
	{

		this->_sprite.at(i).setPosition({
			this->_sprite.at(i).getPosition().x + 1,
			this->_sprite.at(i).getPosition().y + 2
			});

		this->_sprite.at(i).setRotation(
			this->_sprite.at(i).getRotation() + 3
			);

		this->_sprite.at(i).MakeObjectTM();
	}*/

	//Dummy and hardcode update -
	/*for (int i = 0; i < _sprite.size(); i++)
	{
		if ((_uiSprites > 0) && (this->_lifeTimes[i] >= totalTime))
		{
			this->_sprite.erase(_sprite.begin() + i );
			this->_lifeTimes.erase(_lifeTimes.begin() + i );
			_uiSprites--;
		}
		else if (_uiSprites > 0)
		{
			this->_lifeTimes[i] += deltaTime;

			this->_sprite.at(i).setPosition({
				this->_sprite.at(i).getPosition().x + 1,
				this->_sprite.at(i).getPosition().y + 2
				});

			this->_sprite.at(i).setRotation(
				this->_sprite.at(i).getRotation() + 3
			);

			//this->_sprite.at(i).setTint({Base::RandF(),Base::RandF(),Base::RandF(), 1.f});

			this->_sprite.at(i).MakeObjectTM();
		}
	}
	*/

} 

void Application::Draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// allow alpha blending

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_DEPTH_TEST );

	// setup assembly
	this->_drawable.PreDraw();

	// open GL does NOT allow to draw in threads, so all this task should be in main thread
	// poor OpenGL.....


	// order of drawing object with depth test -> doens't matter
	for( unsigned int i = 0; i < _animation.getSize(); i++) //this->_uiSprites
	{
		this->_drawable.Draw(_animation.getSprite(i)); //this->_sprite[i]
	}
	
	// clean up binding
	this->_drawable.PostDraw();
}

void Application::clean()
{

}

void Application::_MakeWindow( std::string sTitle, unsigned int uiWidth, unsigned int uiHeight )
{
	WNDCLASS wndclass = { 0 };
	DWORD    wStyle = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle( NULL );


	wndclass.style = CS_CLASSDC;
	wndclass.lpfnWndProc = ( WNDPROC )ESWindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wndclass.lpszClassName = "ogl";

	if( !RegisterClass( &wndclass ) )
	{
		//   return FALSE;
	}

	wStyle = WS_OVERLAPPEDWINDOW;// WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

								 // Adjust the window rectangle so that the client area has
								 // the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = uiWidth;
	windowRect.bottom = uiHeight;

	AdjustWindowRect( &windowRect, wStyle, FALSE );

	this->_hwnd = CreateWindowA(
		"ogl",
		sTitle.c_str(),
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		nullptr );

	ShowWindow( this->_hwnd, TRUE );
	UpdateWindow( this->_hwnd );
}

LRESULT Application::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	case WM_MOUSEMOVE:
	{
		float x = ( float )( LOWORD( lParam ) );
		float y = ( float )( HIWORD( lParam ) );

		this->Mouse( x, y );

	}
	break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		this->Key( ( unsigned char )wParam, uMsg == WM_KEYDOWN );
	}
	break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		this->MouseClick( uMsg == WM_LBUTTONDOWN );
		break;

	}

	return -1;
}

void Application::_InitOGL()
{
	GLuint flags = ES_WINDOW_RGB | ES_WINDOW_DEPTH;

	EGLint attribList[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, ( flags & ES_WINDOW_ALPHA ) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE, ( flags & ES_WINDOW_DEPTH ) ? 24 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE, ( flags & ES_WINDOW_STENCIL ) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, ( flags & ES_WINDOW_MULTISAMPLE ) ? 1 : 0,
		EGL_NONE
	};



	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	this->eglDisplay = eglGetDisplay( GetDC( this->_hwnd ) );


	if( this->eglDisplay == EGL_NO_DISPLAY )
	{
		return;
	}


	// Initialize EGL
	if( !eglInitialize( this->eglDisplay, &majorVersion, &minorVersion ) )
	{
		return;
	}

	// Get configs
	if( !eglGetConfigs( this->eglDisplay, NULL, 0, &numConfigs ) )
	{
		return;
	}

	// Choose config
	if( !eglChooseConfig( this->eglDisplay, attribList, &config, 1, &numConfigs ) )
	{
		return;
	}



	// Create a surface
	this->eglSurface = eglCreateWindowSurface( this->eglDisplay, config, this->_hwnd, NULL );
	if( this->eglSurface == EGL_NO_SURFACE )
	{
		return;
	}


	// try to share context
	EGLContext contextother = eglGetCurrentContext();

	// Create a GL context
	this->eglContext = eglCreateContext( this->eglDisplay, config, contextother, contextAttribs );
	if( this->eglContext == EGL_NO_CONTEXT )
	{
		return;
	}

	// Make the context current
	if( !eglMakeCurrent( this->eglDisplay, this->eglSurface, this->eglSurface, this->eglContext ) )
	{
		return;
	}
}

void Application::EntryPoint()
{
	MSG msg = { 0 };
	this->_lastTime = GetTickCount64();
	this->_curTime = GetTickCount64();

	while( true )
	{
		int gotMsg = ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) != 0 );
		if( gotMsg )
		{
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			continue;
		}

		// we don't have messages for window, let's draw smth
		this->Frame();

	}
}

LRESULT WINAPI ESWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = -1;
	if( kpApp )
	{
		lr = kpApp->WindowProc( hWnd, uMsg, wParam, lParam );
	}

	if( lr == -1 )
	{
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	// return value from app
	return lr;
}