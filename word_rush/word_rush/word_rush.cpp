#include "stdafx.h"
#include <guid.h>
#include <cachedbuffer.h>
#include <set>
#include <time.h>
#include <strhelp.h>
#define BufPrintArgs(buf) ((buf).is_null() ? (int)(sizeof("<NULL>") - 1) : (int)(buf).length()), (buf).is_null() ? "<NULL>" : (buf).as<const char*>()

class timer {
private:
	uint32_t begTime;
public:
	void start() {
		begTime = clock();
	}

	uint32_t elapsedTime() {
		return ((uint32_t)clock() - begTime) / CLOCKS_PER_SEC;
	}
};

class cursor {
private:
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD p;
public:
	void set_position(short x, short y)
	{
		p.X = x;
		p.Y = y;
		SetConsoleCursorPosition(hStdout, p);
	}
	void set_color_red()
	{
		SetConsoleTextAttribute(hStdout, 12);
	}
	void reset_color()
	{
		SetConsoleTextAttribute(hStdout, 15);
	}
};

#define BUF_SIZE 64

int main()
{
	cursor cursor;
	int life = 9;
	int score = 0;

	printf("------------------------------------------------------------------------------");
	cursor.set_position(0, 23);
	printf("------------------------------------------------------------------------------");

	cursor.set_position(30, 2);
	printf("Loading dictionary...");

	bdp::buffer_cache<LIB_ALIGNMENT> cache;
	bdp::cached_buffer<LIB_ALIGNMENT> word = cache.reserve(4096);
	std::set<bdp::cached_buffer<LIB_ALIGNMENT>> words;

	FILE* __restrict word_list = fopen("word_list.txt", "r");
	char* str_buff = new char[BUF_SIZE];

	while (fgets(str_buff, BUF_SIZE, word_list) != nullptr)
	{
		words.insert(cache.copy(str_buff, strlen(str_buff) - 1));
	}

	printf("\r                                                                    \r");
	timer t;
	uint32_t time_limit = 20;
	char beginning_character;
	char* ca_input = new char[64];
	char* ca_final_input = new char[64];
	bdp::cached_buffer<LIB_ALIGNMENT> cb_input = cache.reserve(4096);
	std::set<bdp::cached_buffer<LIB_ALIGNMENT>> recent_input;
	
	time_t seed;
	srand((unsigned)time(&seed));

	cursor.set_position(0, 1);

	printf("                                             ____                         \n");
	printf("           \\            /                   |    |                       \n");
	printf("            \\    /\\    /  ___   __  ___|    |___/         ___  |___         \n"); 
	printf("             \\  /  \\  /  |   | |   |   |    |   \\  |   | |___  |   |          \n");
	printf("              \\/    \\/   |___| |   \\___|    |    \\ |___|  ___| |   |            \n\n");
	printf("------------------------------------------------------------------------------");


	while (life > 0)
	{

		cursor.set_position(34, 10);
		printf("life: %d", life);
		cursor.set_position(30, 11);
		printf("                         \r");
		cursor.set_position(30, 11);
		printf("time limit: %u s", time_limit);
		cursor.set_position(33, 20);
		printf("score: %d", score);

		beginning_character = (char)(rand() % 26) + 97;
		
		cursor.set_position(24, 15);
		printf("\r                                                                    \r");
		cursor.set_position(24, 15);

		cursor.set_position(24, 14);
		printf("---------------------------");
		cursor.set_position(23, 15);
		printf("|                           |");
		cursor.set_position(24, 16);
		printf("---------------------------");

		t.start();
		cursor.set_position(32, 15);
		printf("%c", beginning_character);
	
		ca_final_input[0] = beginning_character;

		fgets(ca_input, 20, stdin);

		if (t.elapsedTime() > time_limit)
		{
			cursor.set_position(24, 13);
			printf("\r                                                                    \r");
			cursor.set_position(24, 13);
			cursor.set_color_red();
			printf("You didn't finish in time!");
			cursor.reset_color();

			--life;
		}
		else
		{
			for (int i = 1; i < 20; ++i)
			{
				ca_final_input[i] = ca_input[i - 1];
			}

			cb_input.clear();
			cb_input = cache.copy(ca_final_input, strlen(ca_final_input) - 1);



			if (words.find(cb_input) == words.end())
			{
				cursor.set_position(31, 13);
				printf("\r                                                                    \r");
				cursor.set_position(31, 13);
				cursor.set_color_red();
				printf("Not a word!");
				cursor.reset_color();

				--life;
			}
			else
			{
				if (recent_input.find(cb_input) == recent_input.end())
				{
					recent_input.insert(cb_input);
					score = score + (strlen(ca_final_input) - 1) * 10;
					--time_limit;

					cursor.set_position(21, 13);
					printf("\r                                                                    \r");
					cursor.set_position(21, 13);
				}
				else
				{
					cursor.set_position(21, 13);
					printf("\r                                                                    \r");
					cursor.set_position(21, 13);
					cursor.set_color_red();
					printf("You already used that word!");
					cursor.reset_color();

					--life;
				}
			}
		}

	}

	getchar();
	delete[] ca_input;
	delete[] ca_final_input;
	delete[] str_buff;
	return 0;
}

