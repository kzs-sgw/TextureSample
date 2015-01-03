// timeGetTime() ���g�������Ԍv���^�C�}�[ �N���X

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
        m_start = timeGetTime();        // �v���J�n���Ԃ�ۑ�
    }
    double  elapsed()    // ���X�^�[�g����̕b����Ԃ�
    {
        DWORD end = timeGetTime();
        return (double)(end - m_start) / 1000;
    }
private:
    DWORD    m_start;    //  �v���J�n����
};