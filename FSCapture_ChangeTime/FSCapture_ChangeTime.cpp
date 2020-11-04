﻿// FSCapture_ChangeTime.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <iostream>
#include <sysinfoapi.h>
#include <fileapi.h>

WORD getResult(WORD year, WORD month, WORD day);
void saveToFile(WORD result);
char* converToArray(WORD result);

using namespace std;
int main()
{
    PSYSTEMTIME nowTime = new SYSTEMTIME();
    GetLocalTime(nowTime);
    if (nowTime == NULL)
    {
        cout << "获取现在时间失败" << endl;
        system("pause");
        return 1;
    }
    WORD year = nowTime->wYear; 
    WORD month = nowTime->wMonth;
    WORD day = nowTime->wDay;
    cout << "年" << year << "月" << month << "日" << day <<endl;

    WORD result = getResult(year, month, day);
    saveToFile(result);
}

//获取结果
WORD getResult(WORD year, WORD month, WORD day) {
    WORD from1980 = year - 1980;  //现在到1980的差值
    WORD encodeYear = from1980 << 9;
    WORD encodeMonth = month << 5;
    WORD encodeDay = day;
    WORD result = encodeYear | encodeMonth | encodeDay;
    cout << "结果为:" << result << endl;
    return result;
}

char* converToArray(WORD result) {
    char array[2] = { 0 };
    array[1] = (result & 0xFF00) >> 8;
    array[0] = result & 0xFF;
    return array;
}

// 写回文件
void saveToFile(WORD result) {
    char path[MAX_PATH] = "F:\\Project\\DeskTop\\fsc.db";
    HANDLE hfile = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        cout << "找不到目标文件" << endl;
        system("pause");
        return;
    }
    SetFilePointer(hfile, 0x25, NULL, NULL);
    char nowTime[2] = { 0 };
    DWORD receiveSize = NULL;


    char* array = converToArray(result);
    char encodeArray[2] = { array[0], array[1] };

    BOOL writeResult = WriteFile(hfile, encodeArray, 2, &receiveSize, NULL);
    if (!writeResult)
    {
        cout << "写入失败" << GetLastError() << endl;
        CloseHandle(hfile);
        return;
    }
    CloseHandle(hfile);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
