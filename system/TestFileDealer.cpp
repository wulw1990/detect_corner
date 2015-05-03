#if 0


#include "FileDealer.h"

int main()
{
	FileDealer* dealer = new FileDealer();
	vector<string> list = dealer->ls("D:\\No_Backup_BBNC\\giga_video\\2014_11_24\\2014_11_24_out\\GYM_big\\pyramid\\0\\");
	dealer->sortFileList(list);
	dealer->print_list(list);

	system("pause");
}

#endif