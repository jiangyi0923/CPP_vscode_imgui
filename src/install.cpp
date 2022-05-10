
#include "install.h"

#include <wininet.h>
#include <thread>
#pragma comment(lib, "wininet.lib")

install::install(/* args */)
{
    设置.addlog("开始安装");
}

install::~install()
{

}

void install::download(const char *Url, const char *save_as) /*将Url指向的地址的文件下载到save_as指向的本地文件*/
{
	BYTE Temp[1024];
	ULONG Number = 1;

	FILE *stream;
	HINTERNET hSession = InternetOpenA("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET handle2 = InternetOpenUrlA(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (handle2 != NULL)
		{

			if (fopen_s(&stream, save_as, "wb") != EINVAL)
			{
				while (Number > 0)
				{
					InternetReadFile(handle2, Temp, 1024 - 1, &Number);
					// printf_s(to_string(Number).c_str());
					//fewtext += to_string(Number + 1) + "\r\n";
					fwrite(Temp, sizeof(char), Number, stream);
				}
				fclose(stream);
			}

			InternetCloseHandle(handle2);
			handle2 = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}