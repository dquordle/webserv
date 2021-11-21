#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
	char buf[100];
	int num = read(0, buf, 99);
	buf[num] = 0;
	char res[num + 11];
	strcpy(res, "Fuck you, ");
	strcat(res, buf);
	write(1, "Status: 200 OK\r\n\r\n", 18);
	write(1, res, num + 10);
	return 0;
}