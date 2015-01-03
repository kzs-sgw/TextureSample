// timeGetTime() を使った時間計測タイマー クラス

//#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")
class Timer
{
public:
    Timer() { restart(); }
public:
    void  restart()
    {
        m_start = timeGetTime();        // 計測開始時間を保存
    }
    double  elapsed()    // リスタートからの秒数を返す
    {
        DWORD end = timeGetTime();
        return (double)(end - m_start) / 1000;
    }
private:
    DWORD    m_start;    //  計測開始時間
};