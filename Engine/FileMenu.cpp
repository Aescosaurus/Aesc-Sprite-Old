#include "FileMenu.h"
#include "FileOpener.h"
#include "WriteToBitmap.h"

FileMenu::FileMenu( const RectI& screenArea,Surface& art,
	MainWindow& wnd )
	:
	screenArea( screenArea ),
	art( art ),
	wnd( wnd )
{}

bool FileMenu::Update( const Mouse& mouse,const Keyboard& kbd )
{
	if( open.Update( mouse ) || ( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'O' ) ) )
	{
		wnd.HideCursor( false );
		const auto path = FileOpener::OpenFile();
		if( path.length() > 0 )
		{
			if( path.substr( path.length() - 4 ) == ".bmp" )
			{
				Surface temp = path;
				art.Resize( Vei2{ temp.GetWidth(),temp.GetHeight() } );
				art.CopyInto( temp );
			}
		}
		wnd.HideCursor( true );

		return( true );
	}
	if( save.Update( mouse ) || ( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'S' ) ) )
	{
		wnd.HideCursor( false );
		auto path = FileOpener::SaveFile();
		if( path.length() > 0 )
		{
			if( path.substr( path.length() - 4 ) != ".bmp" )
			{
				path += ".bmp";
			}
			WriteToBitmap::Write( art,path );
		}
		wnd.HideCursor( true );

		return( true );
	}

	return( false );
}

void FileMenu::Draw( Graphics& gfx ) const
{
	open.Draw( gfx );
	save.Draw( gfx );
}
