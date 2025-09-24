#pragma warning(disable : 4996)  

#include "Console.h"


static void gotoxy(HANDLE hbuf, COORD pt) { SetConsoleCursorPosition(hbuf, pt); }

static HANDLE get_back_buffer(DoubleBuffer* p_dbuf) { return p_dbuf->hbuf[p_dbuf->idx]; }


void init_tick(TickManager* p_counter, short player_period, short enemy_period, short shot_period)
{
	p_counter->player_tick = p_counter->tick;
	p_counter->enemy_tick = p_counter->tick;
	p_counter->shot_tick = p_counter->tick;

	p_counter->player_period = player_period;
	p_counter->enemy_period = enemy_period;
	p_counter->shot_period = shot_period;

}

void tick(TickManager* p_counter)
{
	p_counter->tick = GetTickCount64();

}

int is_player_tick(TickManager* p_counter)
{
	if (p_counter->tick - p_counter->player_tick > p_counter->player_period)
	{
		p_counter->player_tick = p_counter->tick;
		return 1;
	}
	return 0;
}

int is_enemy_tick(TickManager* p_counter)
{
	if (p_counter->tick - p_counter->enemy_tick > p_counter->enemy_period)
	{
		p_counter->enemy_tick = p_counter->tick;
		return 1;
	}
	return 0;
}

int is_shot_tick(TickManager* p_counter)
{
	if (p_counter->tick - p_counter->shot_tick > p_counter->shot_period)
		return 1;
	return 0;
}

int reset_shot(TickManager* p_counter)
{
	p_counter->shot_tick = GetTickCount64();
}


void init_dbuffer(DoubleBuffer* p_dbuf, short width, short height)
{
	p_dbuf->idx = 0;
	p_dbuf->size.X = width;
	p_dbuf->size.Y = height;

	HWND hwnd = GetConsoleWindow();
	CONSOLE_CURSOR_INFO con_info = { .bVisible = FALSE, .dwSize = 1 };
	SMALL_RECT window_rect = { 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };
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

void clear_back_buffer(DoubleBuffer* p_dbuf) {
	DWORD n;
	COORD org = { 0, 0 };
	HANDLE buf = get_back_buffer(p_dbuf);

	FillConsoleOutputCharacterW(buf, ' ',
		(DWORD)p_dbuf->size.X * p_dbuf->size.Y, org, &n);
}

void draw_back_buffer(DoubleBuffer* p_dbuf) {
	SetConsoleActiveScreenBuffer(get_back_buffer(p_dbuf));
	p_dbuf->idx ^= 1;
	clear_back_buffer(p_dbuf);
}


void draw_2d(DoubleBuffer* p_dbuf, Object2D* obj)
{
	HANDLE buf;
	
	DWORD wr;
	COORD pos;

	const size_t W = (size_t)obj->sprite->w;
	const size_t H = (size_t)obj->sprite->h;
	const size_t frame = (size_t)obj->current_frame;
	const char* base = obj->sprite->data + frame * (W * H);

	buf = get_back_buffer(p_dbuf);
	pos = obj->pos;

	int start_y = 0, end_y = H;
	int start_x = 0, end_x = W;
	int draw_x = obj->pos.X;
	int draw_y = obj->pos.Y;

	// X 클리핑
	if (draw_x < 0) { start_x = -draw_x; draw_x = 0; }
	if (draw_x + (end_x - start_x) > WINDOW_WIDTH)
		end_x = start_x + (WINDOW_WIDTH - draw_x);

	// Y 클리핑
	if (draw_y < 0) { start_y = -draw_y; draw_y = 0; }
	if (draw_y + (end_y - start_y) > WINDOW_HEIGHT-1)
		end_y = start_y + (WINDOW_HEIGHT-1 - draw_y);

	pos.X = draw_x;
	pos.Y = draw_y;
	for (int y = start_y; y < end_y; y++) 
	{
		gotoxy(buf, pos);
		const char* row = base + y * W + start_x;
		WriteConsoleA(buf, row, end_x - start_x, &wr, NULL);
		pos.Y++;
	}
}

void draw_fstring_at(DoubleBuffer* p_dbuf, short x, short y, const char* fmt, ...)
{
	DWORD wr;
	COORD pos = { x, y };
	HANDLE hbuf = get_back_buffer(p_dbuf);
	static char buf[512];

	va_list ap; va_start(ap, fmt);
	int n = _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap );
	va_end(ap);
	
	gotoxy(hbuf, pos);

	if (n < 0) n = (int)strlen(buf);           // 안전한 길이 보정
	WriteConsoleA(hbuf, buf, (DWORD)n, &wr, NULL);
}

void draw_fstring_center(DoubleBuffer* p_dbuf, short y, const char* fmt, ...)
{
	DWORD wr;
	COORD pos = { 0,  y };
	HANDLE hbuf = get_back_buffer(p_dbuf);
	static char buf[512];

	va_list ap; va_start(ap, fmt);
	int n = _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
	va_end(ap);

	if (n < 0) n = (int)strlen(buf);           // 안전한 길이 보정
	pos.X = (WINDOW_WIDTH - n-1) / 2;
	gotoxy(hbuf, pos);

	WriteConsoleA(hbuf, buf, (DWORD)n, &wr, NULL);
}


void update_key(KeyProcess* p_kps)
{
	// arrow key update
	p_kps->dy += (GetAsyncKeyState(KEY_MOVE_DOWN)  & 0x8000 ? PLAYER_VY : 0) - (GetAsyncKeyState(KEY_MOVE_UP)   & 0x8000 ? PLAYER_VY : 0);
	p_kps->dx += (GetAsyncKeyState(KEY_MOVE_RIGHT) & 0x8000 ? PLAYER_VX : 0) - (GetAsyncKeyState(KEY_MOVE_LEFT) & 0x8000 ? PLAYER_VX : 0);
	
	//attack key update
	if		(GetAsyncKeyState(KEY_SHOT_LEFT) & 0x8000) p_kps->shotx = -2;
	else if (GetAsyncKeyState(KEY_SHOT_RIGHT) & 0x8000) p_kps->shotx =  2;
	else p_kps->shotx = 0;

	if		(GetAsyncKeyState(KEY_SHOT_UP) & 0x8000) p_kps->shoty = -1;
	else if (GetAsyncKeyState(KEY_SHOT_DOWN) & 0x8000) p_kps->shoty =  1;
	else p_kps->shoty = 0;


	//loop signal
	if (GetAsyncKeyState(KEY_EXIT) & 0x8000) { p_kps->loop = 0; } //stop
	else if (GetAsyncKeyState(KEY_RESTART) & 0x8000) { p_kps->loop = 1; } //start
	else p_kps->loop = -1;	//none
}


void sleep(int ms)
{
	Sleep(ms);
}