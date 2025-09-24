#pragma warning(disable : 4996)  

#include "Console.h"

//dbuf func
static void gotoxy(HANDLE hbuf, COORD pt) { SetConsoleCursorPosition(hbuf, pt); }

static HANDLE get_back_buffer(DoubleBuffer* p_dbuf) { return p_dbuf->hbuf[p_dbuf->idx]; }

static void clear_back_buffer(DoubleBuffer* p_dbuf) {
	DWORD n;
	COORD org = { 0, 0 };
	HANDLE buf = get_back_buffer(p_dbuf);

	FillConsoleOutputCharacterW(buf, ' ',
		(DWORD)p_dbuf->size.X * p_dbuf->size.Y, org, &n);
}

void init_dbuffer(DoubleBuffer* p_dbuf, short width, short height)
{
	p_dbuf->idx = 0;
	p_dbuf->size.X = width;
	p_dbuf->size.Y = height;

	HWND hwnd = GetConsoleWindow();
	CONSOLE_CURSOR_INFO con_info = { .bVisible = FALSE, .dwSize = 1 };
	SMALL_RECT window_rect = { 0, 0, WINDOW_W - 1, WINDOW_H - 1 };
	SMALL_RECT tiny = { 0,0,0,0 };

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);	
	wcscpy(cfi.FaceName, L"D2Coding"); // 글꼴 이름 (Wide string)

	for (int i = 0; i < 2; i++)
	{
		p_dbuf->hbuf[i] = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
		);

		GetCurrentConsoleFontEx(p_dbuf->hbuf[i], FALSE, &cfi);
		cfi.dwFontSize.X = cfi.dwFontSize.Y = WINDOW_FONTSIZE;
		SetCurrentConsoleFontEx(p_dbuf->hbuf[i], FALSE, &cfi);

		SetConsoleCursorInfo(p_dbuf->hbuf[i], &con_info);

		SetConsoleWindowInfo(p_dbuf->hbuf[i], TRUE, &tiny);               
		SetConsoleScreenBufferSize(p_dbuf->hbuf[i], p_dbuf->size);        
		SetConsoleWindowInfo(p_dbuf->hbuf[i], TRUE, &window_rect);
	}
	
	SetConsoleActiveScreenBuffer(p_dbuf->hbuf[0]);
	p_dbuf->idx = 1;
	clear_back_buffer(p_dbuf);

	RECT desk, r; GetWindowRect(GetDesktopWindow(), &desk);
	GetWindowRect(hwnd, &r);
	int winW = r.right - r.left, winH = r.bottom - r.top;
	int x = (desk.right - winW) / 2, y = (desk.bottom - winH) / 2;
	SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	SendMessage(hwnd, WM_SETREDRAW, TRUE, 0);    
	InvalidateRect(hwnd, NULL, TRUE);
}

void free_dbuffer(DoubleBuffer* p_dbuf) {
	if (!p_dbuf) return;
	for (int i = 0; i < 2; ++i) if (p_dbuf->hbuf[i] && p_dbuf->hbuf[i] != INVALID_HANDLE_VALUE)
		CloseHandle(p_dbuf->hbuf[i]);
	// 기본 표준 버퍼로 복귀
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

void draw_back_buffer(DoubleBuffer* p_dbuf) {
	SetConsoleActiveScreenBuffer(get_back_buffer(p_dbuf));
	p_dbuf->idx ^= 1;
	clear_back_buffer(p_dbuf);
}

void draw_fstring_at(DoubleBuffer* p_dbuf, short x, short y, const char* fmt, ...)
{
	DWORD wr;
	COORD pos = { x, y };
	HANDLE hbuf = get_back_buffer(p_dbuf);
	static char buf[512];

	va_list ap; va_start(ap, fmt);
	int n = _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
	va_end(ap);

	gotoxy(hbuf, pos);

	if (n < 0) n = (int)strlen(buf);           // 안전한 길이 보정
	WriteConsoleA(hbuf, buf, (DWORD)n, &wr, NULL);
}

void draw_fstring_center(DoubleBuffer* p_dbuf, short y_offset, const char* fmt, ...)
{
	DWORD wr;
	COORD pos = { 0,   WINDOW_H / 2 + y_offset };
	HANDLE hbuf = get_back_buffer(p_dbuf);
	static char buf[512];

	va_list ap; va_start(ap, fmt);
	int n = _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
	va_end(ap);

	if (n < 0) n = (int)strlen(buf);           // 안전한 길이 보정
	pos.X = (WINDOW_W - n - 1) / 2;
	gotoxy(hbuf, pos);

	WriteConsoleA(hbuf, buf, (DWORD)n, &wr, NULL);
}

void draw_2d(DoubleBuffer* p_dbuf, char* data, short W, short H, short frame, 
	short draw_x, short draw_y, short start_x, short start_y, short end_x, short end_y
)
{
	HANDLE buf;
	DWORD wr;
	COORD pos = { draw_x , draw_y };
	const char* base = data + frame * (W * H);

	buf = get_back_buffer(p_dbuf);

	for (int y = start_y; y < end_y; y++)
	{
		gotoxy(buf, pos);
		const char* row = base + y * W + start_x;
		WriteConsoleA(buf, row, end_x - start_x, &wr, NULL);
		pos.Y++;
	}
}


//tick function
void init_tick(TickCounter* p_counter, short player_period, short enemy_period, short shot_period)
{
	p_counter->player_tick = p_counter->tick;
	p_counter->enemy_tick = p_counter->tick;
	p_counter->shot_tick = p_counter->tick;

	p_counter->player_period = player_period;
	p_counter->enemy_period = enemy_period;
	p_counter->shot_period = shot_period;

}

void tick(TickCounter* p_counter)
{
	p_counter->tick = GetTickCount64();

}

int is_player_tick(TickCounter* p_counter)
{
	if (p_counter->tick - p_counter->player_tick > p_counter->player_period)
	{
		p_counter->player_tick = p_counter->tick;
		return 1;
	}
	return 0;
}

int is_enemy_tick(TickCounter* p_counter)
{
	if (p_counter->tick - p_counter->enemy_tick > p_counter->enemy_period)
	{
		p_counter->enemy_tick = p_counter->tick;
		return 1;
	}
	return 0;
}

int is_shot_tick(TickCounter* p_counter)
{
	if (p_counter->tick - p_counter->shot_tick > p_counter->shot_period)
		return 1;
	return 0;
}

int reset_tick(TickCounter* p_counter, ULONGLONG* tick) { *tick = GetTickCount64(); }


//kps func
void init_kps(KeyProcess* p_kps)
{
	p_kps->loop	 = -1;
	p_kps->dx	 =  0;
	p_kps->dy	 =  0;
	p_kps->shotx =  0;
	p_kps->shoty =  0;
}

void update_kps(KeyProcess* p_kps)
{
	// arrow kps update
	p_kps->dy += (GetAsyncKeyState(KEY_MOVE_DOWN)  & 0x8000 ? 1 : 0) 
			   - (GetAsyncKeyState(KEY_MOVE_UP)    & 0x8000 ? 1 : 0);
	p_kps->dx += (GetAsyncKeyState(KEY_MOVE_RIGHT) & 0x8000 ? 1 : 0) 
		       - (GetAsyncKeyState(KEY_MOVE_LEFT)  & 0x8000 ? 1 : 0);
	
	//attack kps update
	if		(GetAsyncKeyState(KEY_SHOT_LEFT)	& 0x8000)	p_kps->shotx = -1;
	else if (GetAsyncKeyState(KEY_SHOT_RIGHT)	& 0x8000)	p_kps->shotx =  1;
	else													p_kps->shotx =  0;

	if		(GetAsyncKeyState(KEY_SHOT_UP)	& 0x8000)	p_kps->shoty = -1;
	else if (GetAsyncKeyState(KEY_SHOT_DOWN)& 0x8000)	p_kps->shoty =  1;
	else												p_kps->shoty =  0;


	//loop signal
	if		(GetAsyncKeyState(KEY_RESTART)	& 0x8000) { p_kps->loop = LOOP_RESTART; } 
	else if	(GetAsyncKeyState(KEY_EXIT)		& 0x8000) { p_kps->loop = LOOP_END; } 
	else												p_kps->loop = LOOP_CONTINUE; 
}

int wait_kps(KeyProcess* p_kps)
{
	while (1)
	{
		update_kps(p_kps);
		if (p_kps->loop == LOOP_CONTINUE) continue;
		else return p_kps->loop;
	}
}


//etc func
void sleep_ms(int ms){ Sleep(ms); }
