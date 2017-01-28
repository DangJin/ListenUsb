#include <boost/filesystem.hpp>
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <dbt.h>
#include <math.h>
#include <string.h>

using namespace boost::filesystem;
using namespace std;

void print_files(const path &full_path) {
    if (exists(full_path)) {
        directory_iterator item_begin(full_path);
        directory_iterator item_end;

        for (; item_begin != item_end; ++item_begin) {
            if (is_directory(*item_begin)) {
                cout << item_begin->path().native() << "\t[dir]" << endl;
                print_files(item_begin->path());
            } else {
                cout << item_begin->path().native() << endl;
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_DEVICECHANGE) {
        if ((DWORD) wp == DBT_DEVICEARRIVAL) {
            DEV_BROADCAST_VOLUME *p = (DEV_BROADCAST_VOLUME *) lp;
            if (p->dbcv_devicetype == DBT_DEVTYP_VOLUME) {
                int l = (int) (log(double(p->dbcv_unitmask)) / log(double(2)));
                printf("%c Inserted  \n", 'A' + l);
                string drive;
                drive = (char) ('A' + l);
                string dir = drive + ":\\";
                path full_path(dir);
                print_files(full_path);

            }
        } else if ((DWORD) wp == DBT_DEVICEREMOVECOMPLETE) {
            DEV_BROADCAST_VOLUME *p = (DEV_BROADCAST_VOLUME *) lp;
            if (p->dbcv_devicetype == DBT_DEVTYP_VOLUME) {
                int l = (int) (log(double(p->dbcv_unitmask)) / log(double(2)));
                printf("%c Removed  \n", 'A' + l);
            }
        }
        return TRUE;
    } else
        return DefWindowProc(h, msg, wp, lp);
}


int main(int argc, const char **argv) {
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpszClassName = TEXT("USB_MSG");
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);
    HWND h = CreateWindow(TEXT("USB_MSG"), TEXT(""), 0, 0, 0, 0, 0,
                          0, 0, GetModuleHandle(0), 0);
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}
